/* Copyright (C) 2004-2017 J.F.Dockes
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */
#include "autoconfig.h"

#include <math.h>

#include <map>
#include <unordered_map>
#include <deque>
#include <algorithm>

#include "log.h"
#include "rcldb.h"
#include "rcldb_p.h"
#include "rclquery.h"
#include "rclquery_p.h"
#include "textsplit.h"
#include "hldata.h"
#include "chrono.h"
#include "unacpp.h"
#include "zlibut.h"

using namespace std;

// We now let plaintorich do the highlight tags insertions which is
// wasteful because we have most of the information (but the perf hit
// is small because it's only called on the output fragments, not on
// the whole text). The highlight zone computation code has been left
// around just in case I change my mind.
#undef COMPUTE_HLZONES

namespace Rcl {

// Chars we turn to spaces in the Snippets
static const string cstr_nc("\n\r\x0c\\");

// Fragment descriptor. A fragment is a text area with one or several
// matched terms and some context. It is ranked according to the
// matched term weights and the near/phrase matches get a boost.
struct MatchFragment {
    // Start/End byte offsets of fragment in the document text
    int start;
    int stop;
    // Weight for this fragment (bigger better)
    double coef;
#ifdef COMPUTE_HLZONES
    // Highlight areas (each is one or several contiguous match
    // terms). Because a fragment extends around a match, there
    // can be several contiguous or separate matches in a given
    // fragment.
    vector<pair<int,int>> hlzones;
#endif
    // Position of the first matched term (for page number computations)
    unsigned int hitpos;
    // "best term" for this match (e.g. for use as ext app search term)
    string term;
        
    MatchFragment(int sta, int sto, double c,
#ifdef COMPUTE_HLZONES
                  vector<pair<int,int>>& hl,
#endif
                  unsigned int pos, string& trm) 
        : start(sta), stop(sto), coef(c), hitpos(pos) {
#ifdef COMPUTE_HLZONES
        hlzones.swap(hl);
#endif
        term.swap(trm);
    }
};


// Text splitter for finding the match areas in the document text.
class TextSplitABS : public TextSplit {
public:

    TextSplitABS(const vector<string>& matchTerms,
                 const HighlightData& hdata,
                 unordered_map<string, double>& wordcoefs,
                 unsigned int ctxwords,
                 Flags flags = TXTS_NONE)
        :  TextSplit(flags), m_terms(matchTerms.begin(), matchTerms.end()),
           m_hdata(hdata), m_wordcoefs(wordcoefs), m_ctxwords(ctxwords) {

        // Take note of the group (phrase/near) terms because we need
        // to compute the position lists for them.
        for (const auto& group : hdata.groups) {
            if (group.size() > 1) {
                for (const auto& term: group) {
                    m_gterms.insert(term);
                }
            }
        }
    }

    // Accept a word and its position. If the word is a matched term,
    // add/update fragment definition.
    virtual bool takeword(const std::string& term, int pos, int bts, int bte) {
        LOGDEB2("takeword: " << term << endl);

        // Remember recent past
        m_prevterms.push_back(pair<int,int>(bts,bte));
        if (m_prevterms.size() > m_ctxwords+1) {
            m_prevterms.pop_front();
        }

        string dumb;
        if (o_index_stripchars) {
            if (!unacmaybefold(term, dumb, "UTF-8", UNACOP_UNACFOLD)) {
                LOGINFO("abstract: unac failed for [" << term << "]\n");
                return true;
            }
        } else {
            dumb = term;
        }

        if (m_terms.find(dumb) != m_terms.end()) {
            // This word is a search term. Extend or create fragment
            LOGDEB2("match: [" << dumb << "] current: " << m_curfrag.first <<
                   ", " << m_curfrag.second << " remain " <<
                   m_remainingWords << endl);
            double coef = m_wordcoefs[dumb];
            if (!m_remainingWords) {
                // No current fragment. Start one
                m_curhitpos = baseTextPosition + pos;
                m_curfrag.first = m_prevterms.front().first;
                m_curfrag.second = m_prevterms.back().second;
#ifdef COMPUTE_HLZONES
                m_curhlzones.push_back(pair<int,int>(bts, bte));
#endif
                m_curterm = term;
                m_curtermcoef = coef;
            } else {
                LOGDEB2("Extending current fragment: " << m_remainingWords <<
                       " -> " << m_ctxwords << endl);
                m_extcount++;
#ifdef COMPUTE_HLZONES
                if (m_prevwordhit) {
                    m_curhlzones.back().second = bte;
                } else {
                    m_curhlzones.push_back(pair<int,int>(bts, bte));
                }
#endif
                if (coef > m_curtermcoef) {
                    m_curterm = term;
                    m_curtermcoef = coef;
                }
            }

#ifdef COMPUTE_HLZONES
            m_prevwordhit = true;
#endif
            m_curfragcoef += coef;
            m_remainingWords = m_ctxwords + 1;
            if (m_extcount > 3) {
                // Limit expansion of contiguous fragments (this is to
                // avoid common terms in search causing long
                // heavyweight meaningless fragments. Also, limit length).
                m_remainingWords = 1;
                m_extcount = 0;
            }

            // If the term is part of a near/phrase group, update its
            // positions list
            if (m_gterms.find(dumb) != m_gterms.end()) {
                // Term group (phrase/near) handling
                m_plists[dumb].push_back(pos);
                m_gpostobytes[pos] = pair<int,int>(bts, bte);
                LOGDEB2("Recorded bpos for " << pos << ": " << bts << " " <<
                        bte << "\n");
            }
        }
#ifdef COMPUTE_HLZONES
        else {
            // Not a matched term
            m_prevwordhit = false;
        }
#endif

        
        if (m_remainingWords) {
            // Fragment currently open. Time to close ?
            m_remainingWords--;
            m_curfrag.second = bte;
            if (m_remainingWords == 0) {
                if (m_totalcoef < 5.0 || m_curfragcoef >= 1.0) {
                    // Don't push bad fragments if we have a lot already
                    m_fragments.push_back(MatchFragment(m_curfrag.first,
                                                     m_curfrag.second,
                                                     m_curfragcoef,
#ifdef COMPUTE_HLZONES
                                                     m_curhlzones,
#endif
                                                     m_curhitpos,
                                                     m_curterm
                                              ));
                }
                m_totalcoef += m_curfragcoef;
                m_curfragcoef = 0.0;
                m_curtermcoef = 0.0;
            }
        }
        return true;
    }
    
    const vector<MatchFragment>& getFragments() {
        return m_fragments;
    }


    // After the text is split: use the group terms positions lists to
    // find the group matches. We process everything as NEAR (no
    // PHRASE specific processing).
    void updgroups() {
        vector<GroupMatchEntry> tboffs;

        // Look for matches to PHRASE and NEAR term groups and finalize
        // the matched regions list (sort it by increasing start then
        // decreasing length). We process all groups as NEAR (ignore order).
        for (unsigned int i = 0; i < m_hdata.groups.size(); i++) {
            if (m_hdata.groups[i].size() > 1) {
                matchGroup(m_hdata, i, m_plists, m_gpostobytes, tboffs);
            }
        }

        // Sort the fragments by increasing start and decreasing width
        std::sort(m_fragments.begin(), m_fragments.end(),
                  [](const MatchFragment& a, const MatchFragment& b) -> bool {
                      if (a.start != b.start)
                          return a.start < b.start;
                      return a.stop - a.start > b.stop - a.stop;
                  }
            );
        
        // Sort the group regions by increasing start and decreasing width.  
        std::sort(tboffs.begin(), tboffs.end(),
                  [](const GroupMatchEntry& a, const GroupMatchEntry& b)
                  -> bool {
                      if (a.offs.first != b.offs.first)
                          return a.offs.first < b.offs.first;
                      return a.offs.second > b.offs.second;
                  }
            );

        // Give a boost to fragments which contain a group match
        // (phrase/near), they are dear to the user's heart.  list are
        // sorted, so we never go back in the fragment list (can
        // always start the search where we previously stopped).
        auto fragit = m_fragments.begin();
        for (const auto& grpmatch : tboffs) {
            LOGDEB2("LOOKING FOR FRAGMENT: group: " << grpmatch.offs.first <<
                   "-" << grpmatch.offs.second << " curfrag " <<
                   fragit->start << "-" << fragit->stop << endl);
            while (fragit->stop < grpmatch.offs.first) {
                fragit++;
                if (fragit == m_fragments.end()) {
                    return;
                }
            }
            if (fragit->start <= grpmatch.offs.first &&
                fragit->stop >= grpmatch.offs.second) {
                // grp in frag
                fragit->coef += 10.0;
            }
        }

        return;
    }
    
private:
    // Past terms because we need to go back for context before a hit
    deque<pair<int,int>>  m_prevterms;
    // Data about the fragment we are building
    pair<int,int> m_curfrag{0,0};
    double m_curfragcoef{0.0};
    unsigned int m_remainingWords{0};
    unsigned int m_extcount{0};
#ifdef COMPUTE_HLZONES
    vector<pair<int,int>> m_curhlzones;
    bool m_prevwordhit{false};
#endif
    // Current sum of fragment weights
    double m_totalcoef{0.0};
    // Position of 1st term match (for page number computations)
    unsigned int m_curhitpos{0};
    // "best" term
    string m_curterm;
    double m_curtermcoef{0.0};

    // Group terms, extracted from m_hdata 
    unordered_set<string> m_gterms;
    // group/near terms word positions.
    map<string, vector<int> > m_plists;
    map<int, pair<int, int> > m_gpostobytes;
    
    // Input
    unordered_set<string> m_terms;
    const HighlightData& m_hdata;
    unordered_map<string, double>& m_wordcoefs;
    unsigned int m_ctxwords;

    // Result: begin and end byte positions of query terms/groups in text
    vector<MatchFragment> m_fragments;  
};

int Query::Native::abstractFromText(
    Rcl::Db::Native *ndb,
    Xapian::docid docid,
    const vector<string>& matchTerms,
    const multimap<double, vector<string>> byQ,
    double totalweight,
    int ctxwords,
    unsigned int maxtotaloccs,
    vector<Snippet>& vabs,
    Chrono&
    )
{
    Xapian::Database& xrdb(ndb->xrdb);
    Xapian::Document xdoc;

    string reason;
    XAPTRY(xdoc = xrdb.get_document(docid), xrdb, reason);
    if (!reason.empty()) {
        LOGERR("abstractFromText: could not get doc: " << reason << endl);
        return ABSRES_ERROR;
    }

    string rawtext, data;
#ifdef RAWTEXT_IN_DATA
    XAPTRY(data = xdoc.get_data(), xrdb, reason);
    if (!reason.empty()) {
        LOGERR("abstractFromText: could not get data: " << reason << endl);
        return ABSRES_ERROR;
    }
    Doc doc;
    if (ndb->dbDataToRclDoc(docid, data, doc)) {
        rawtext = doc.meta["RAWTEXT"];
    }
#endif
#ifdef RAWTEXT_IN_VALUE
    XAPTRY(rawtext = xdoc.get_value(VALUE_RAWTEXT), xrdb, reason);
    if (!reason.empty()) {
        LOGERR("abstractFromText: could not get value: " << reason << endl);
        return ABSRES_ERROR;
    }
    ZLibUtBuf cbuf;
    inflateToBuf(rawtext.c_str(), rawtext.size(), cbuf);
    rawtext.assign(cbuf.getBuf(), cbuf.getCnt());
#endif

    if (rawtext.empty()) {
        LOGDEB0("abstractFromText: no text\n");
        return ABSRES_ERROR;
    }

#if 0 && ! (XAPIAN_MAJOR_VERSION <= 1 && XAPIAN_MINOR_VERSION <= 2)  && \
    (defined(RAWTEXT_IN_DATA) || defined(RAWTEXT_IN_VALUE))
    // Tryout the Xapian internal method.
    string snippet = xmset.snippet(rawtext);
    LOGDEB("SNIPPET: [" << snippet << "] END SNIPPET\n");
#endif

    // We need the q coefs for individual terms
    unordered_map<string, double> wordcoefs;
    for (const auto& mment : byQ) {
        for (const auto& word : mment.second) {
            wordcoefs[word] = mment.first;
        }
    }

    // Note: getTerms() was already called by qualityTerms, so this is
    // a bit wasteful. I guess that the performance impact is
    // negligible though. To be checked ? We need the highlightdata for the
    // phrase/near groups.
    HighlightData hld;
    if (m_q->m_sd) {
        m_q->m_sd->getTerms(hld);
    }

    TextSplitABS splitter(matchTerms, hld, wordcoefs, ctxwords,
                          TextSplit::TXTS_ONLYSPANS);
    splitter.text_to_words(rawtext);
    splitter.updgroups();

    // Sort the fragments by decreasing weight
    const vector<MatchFragment>& res1 = splitter.getFragments();
    vector<MatchFragment> result(res1.begin(), res1.end());
    std::sort(result.begin(), result.end(),
              [](const MatchFragment& a,
                 const MatchFragment& b) -> bool { 
                  return a.coef > b.coef; 
              }
        );


    vector<int> vpbreaks;
    ndb->getPagePositions(docid, vpbreaks);

    // Build the output snippets array by merging the fragments, their
    // main term and the page positions. 
    unsigned int count = 0;
    for (const auto& entry : result) {
        string frag = neutchars(
            rawtext.substr(entry.start, entry.stop - entry.start), cstr_nc);

#ifdef COMPUTE_HLZONES
        // This would need to be modified to take tag parameters
        // instead of the const strings
        static const string starthit("<span style='color: blue;'>");
        static const string endhit("</span>");
        size_t inslen = 0;
        for (const auto& hlzone: entry.hlzones) {
            frag.replace(hlzone.first - entry.start + inslen, 0, starthit);
            inslen += starthit.size();
            frag.replace(hlzone.second - entry.start + inslen, 0, endhit);
            inslen += endhit.size();
        }
#endif
        LOGDEB("=== FRAGMENT: Coef: " << entry.coef << ": " << frag << endl);
        int page = 0;
        if (vpbreaks.size() > 1) {
            page = ndb->getPageNumberForPosition(vpbreaks, entry.hitpos);
            if (page < 0)
                page = 0;
        }
        vabs.push_back(Snippet(page, frag).setTerm(entry.term));
        if (count++ >= maxtotaloccs)
            break;
    }
    return ABSRES_OK;
}

}
