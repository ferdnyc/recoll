#!/bin/sh

topdir=`dirname $0`/..
. $topdir/shared.sh

initvariables $0

(
    recollq -q '"Aw, yeah! It'"'"'s even got"'
    recollq -q '"def myfunc"' ext:ipynb
    recollq -q '"#@title 素材打包"'
    recollq -q '"2 informative features and 2 noise features"'
) 2> $mystderr | egrep -v '^Recoll query: ' > $mystdout

checkresult
