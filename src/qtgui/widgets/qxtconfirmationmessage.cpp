#include "qxtconfirmationmessage.h"
/****************************************************************************
* Copyright (c) 2006 - 2011, the LibQxt project.
* See the Qxt AUTHORS file for a list of authors and copyright holders.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of the LibQxt project nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*   "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
* <http://libqxt.org>  <foundation@libqxt.org>
****************************************************************************/

#include <QCoreApplication>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QGridLayout>
#include <QCheckBox>
#include <QSettings>

class QxtConfirmationMessagePrivate : public QxtPrivate<QxtConfirmationMessage>
{
public:
    QXT_DECLARE_PUBLIC(QxtConfirmationMessage)
    void init(const QString& message = QString());

    QString key() const;

    int showAgain();
    void doNotShowAgain(int result);
    void reset();

    bool remember;
    QCheckBox* confirm;
    QString overrideKey;
};

void QxtConfirmationMessagePrivate::init(const QString& message)
{
    remember = false;
    confirm = new QCheckBox(&qxt_p());
    if (!message.isNull())
        confirm->setText(message);
    else
        confirm->setText(QxtConfirmationMessage::tr("Do not show again."));

    QGridLayout* grid = qobject_cast<QGridLayout*>(qxt_p().layout());
    QDialogButtonBox* buttons = qxt_p().findChild<QDialogButtonBox*>();
    if (grid && buttons) {
        const int idx = grid->indexOf(buttons);
        int row, column, rowSpan, columnSpan = 0;
        grid->getItemPosition(idx, &row, &column, &rowSpan, &columnSpan);
        QLayoutItem* buttonsItem = grid->takeAt(idx);
        grid->addWidget(confirm, row, column, rowSpan, columnSpan,
                        Qt::AlignLeft | Qt::AlignTop);
        grid->addItem(buttonsItem, ++row, column, rowSpan, columnSpan);
    }
}

QString QxtConfirmationMessagePrivate::key() const
{
    QString value = overrideKey;
    if (value.isEmpty()) {
        const QString all = qxt_p().windowTitle() + qxt_p().text() + qxt_p().informativeText();
#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
        value = QString::number(qChecksum(all.toLocal8Bit()));
#else
        const QByteArray data = all.toLocal8Bit();
        value = QString::number(qChecksum(data.constData(), data.length()));
#endif
    }
    return value;
}

int QxtConfirmationMessagePrivate::showAgain()
{
    QSettings settings;
    return settings.value(key(), -1).toInt();
}

void QxtConfirmationMessagePrivate::doNotShowAgain(int result)
{
    QSettings settings;
    settings.setValue(key(), result);
}

void QxtConfirmationMessagePrivate::reset()
{
    QSettings settings;
    settings.remove(key());
}

/*!
  \class QxtConfirmationMessage
  \inmodule QxtWidgets
  \brief The QxtConfirmationMessage class provides a confirmation message.

  QxtConfirmationMessage is a confirmation message with checkable
  \bold {"Do not show again."} option. A checked and accepted confirmation
  message is no more shown until reseted.

  Example usage:
  \code
  void MainWindow::closeEvent(QCloseEvent* event)
  {
  static const QString text(tr("Are you sure you want to quit?"));
  if (QxtConfirmationMessage::confirm(this, tr("Confirm"), text) == QMessageBox::No)
  event->ignore();
  }
  \endcode

  \image qxtconfirmationmessage.png "QxtConfirmationMessage in action."

  \bold {Note:} QCoreApplication::organizationName and
  QCoreApplication::applicationName are used for storing
  settings. In case these properties are empty, \bold "QxtWidgets"
  and \bold "QxtConfirmationMessage" are used, respectively.
*/

/*!
  Constructs a new QxtConfirmationMessage with \a parent.
*/
QxtConfirmationMessage::QxtConfirmationMessage(QWidget* parent)
    : QMessageBox(parent)
{
    QXT_INIT_PRIVATE(QxtConfirmationMessage);
    qxt_d().init();
    connect(this, SIGNAL(buttonClicked(QAbstractButton*)),
            this, SLOT(onButtonClicked(QAbstractButton*)));
}

/*!
  Constructs a new QxtConfirmationMessage with \a icon, \a title, \a
  text, \a confirmation, \a buttons, \a parent and \a flags.
*/
QxtConfirmationMessage::QxtConfirmationMessage(
    QMessageBox::Icon icon, const QString& title, const QString& text,
    const QString& confirmation, QMessageBox::StandardButtons buttons,
    QWidget* parent, Qt::WindowFlags flags)
    : QMessageBox(icon, title, text, buttons, parent, flags)
{
    QXT_INIT_PRIVATE(QxtConfirmationMessage);
    qxt_d().init(confirmation);
    connect(this, SIGNAL(buttonClicked(QAbstractButton*)),
            this, SLOT(onButtonClicked(QAbstractButton*)));
}

/*!
  Destructs the confirmation message.
*/
QxtConfirmationMessage::~QxtConfirmationMessage()
{
}

/*!
  Opens an confirmation message box with the specified \a title, \a
  text and \a confirmation.  The standard \a buttons are added to
  the message box. \a defaultButton specifies the button used when
  Enter is pressed. \a defaultButton must refer to a button that was
  given in \a buttons. If \a defaultButton is QMessageBox::NoButton,
  QMessageBox chooses a suitable default automatically.

  Returns the identity of the standard button that was clicked.
  If Esc was pressed instead, the escape button is returned.

  If \a parent is \c 0, the message box is an application modal
  dialog box. If \a parent is a widget, the message box is window
  modal relative to \a parent.
*/
QMessageBox::StandardButton QxtConfirmationMessage::confirm(
    QWidget* parent, const QString& title, const QString& text,
    const QString& confirmation, QMessageBox::StandardButtons buttons,
    QMessageBox::StandardButton defaultButton)
{
    QxtConfirmationMessage msgBox(QMessageBox::NoIcon, title, text,
                                  confirmation, QMessageBox::NoButton, parent);
    QDialogButtonBox* buttonBox = msgBox.findChild<QDialogButtonBox*>();
    Q_ASSERT(buttonBox != 0);

    uint mask = QMessageBox::FirstButton;
    while (mask <= QMessageBox::LastButton) {
        uint sb = buttons & mask;
        mask <<= 1;
        if (!sb)
            continue;
        QPushButton* button = msgBox.addButton((QMessageBox::StandardButton)sb);
        // Choose the first accept role as the default
        if (msgBox.defaultButton())
            continue;
        if ((defaultButton == QMessageBox::NoButton &&
             buttonBox->buttonRole(button) == QDialogButtonBox::AcceptRole)
            || (defaultButton != QMessageBox::NoButton &&
                sb == uint(defaultButton)))
            msgBox.setDefaultButton(button);
    }
    if (msgBox.exec() == -1)
        return QMessageBox::Cancel;
    return msgBox.standardButton(msgBox.clickedButton());
}

/*!
  \property QxtConfirmationMessage::confirmationText
  \brief the confirmation text

  The default value is \bold {"Do not show again."}
*/
QString QxtConfirmationMessage::confirmationText() const
{
    return qxt_d().confirm->text();
}

void QxtConfirmationMessage::setConfirmationText(const QString& confirmation)
{
    qxt_d().confirm->setText(confirmation);
}

/*!
  \property QxtConfirmationMessage::overrideSettingsKey
  \brief the override key used for settings

  When no \bold overrideSettingsKey has been set, the key is calculated with
  qChecksum() based on title, text and confirmation message.

  The default value is an empty string.
*/
QString QxtConfirmationMessage::overrideSettingsKey() const
{
    return qxt_d().overrideKey;
}

void QxtConfirmationMessage::setOverrideSettingsKey(const QString& key)
{
    qxt_d().overrideKey = key;
}

/*!
  \property QxtConfirmationMessage::rememberOnReject
  \brief whether \bold {"Do not show again."} option is stored even
  if the message box is rejected (eg. user presses Cancel).

  The default value is \c false.
*/
bool QxtConfirmationMessage::rememberOnReject() const
{
    return qxt_d().remember;
}

void QxtConfirmationMessage::setRememberOnReject(bool remember)
{
    qxt_d().remember = remember;
}

/*!
  Shows the confirmation message if necessary. The confirmation message is not
  shown in case \bold {"Do not show again."} has been checked while the same
  confirmation message was earlierly accepted.

  A confirmation message is identified by the combination of title,
  QMessageBox::text and optional QMessageBox::informativeText.

  A clicked button with role QDialogButtonBox::AcceptRole or
  QDialogButtonBox::YesRole is considered as "accepted".

  \warning This function does not reimplement but shadows QMessageBox::exec().

  \sa QWidget::windowTitle, QMessageBox::text, QMessageBox::informativeText
*/
int QxtConfirmationMessage::exec()
{
    int res = qxt_d().showAgain();
    if (res == -1)
        res = QMessageBox::exec();
    return res;
}

void QxtConfirmationMessage::onButtonClicked(QAbstractButton *button)
{
    QDialogButtonBox* buttons = this->findChild<QDialogButtonBox*>();
    Q_ASSERT(buttons != 0);

    int role = buttons->buttonRole(button);
    if (qxt_d().confirm->isChecked() &&
        (qxt_d().remember || role != QDialogButtonBox::RejectRole))
    {
        qxt_d().doNotShowAgain(0);
    }
}

/*!
  Resets this instance of QxtConfirmationMessage. A reseted confirmation
  message is shown again until user checks \bold {"Do not show again."} and
  accepts the confirmation message.
*/
void QxtConfirmationMessage::reset()
{
    qxt_d().reset();
}
