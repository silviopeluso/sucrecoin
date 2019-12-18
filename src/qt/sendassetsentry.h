// Copyright (c) 2011-2015 The Bitcoin Core developers
// Copyright (c) 2017 The Sucrecoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef SUCRECOIN_QT_SENDASSETSENTRY_H
#define SUCRECOIN_QT_SENDASSETSENTRY_H

#include "walletmodel.h"

#include <QStackedWidget>

class WalletModel;
class PlatformStyle;

namespace Ui {
    class SendAssetsEntry;
}

/**
 * A single entry in the dialog for sending sucrecoins.
 * Stacked widget, with different UIs for payment requests
 * with a strong payee identity.
 */
class SendAssetsEntry : public QStackedWidget
{
    Q_OBJECT

public:
    explicit SendAssetsEntry(const PlatformStyle *platformStyle, const QStringList myAssetsNames, QWidget *parent = 0);
    ~SendAssetsEntry();

    void setModel(WalletModel *model);
    bool validate();
    SendAssetsRecipient getValue();

    /** Return whether the entry is still empty and unedited */
    bool isClear();

    void setValue(const SendAssetsRecipient &value);
    void setAddress(const QString &address);

    /** Set up the tab chain manually, as Qt messes up the tab chain by default in some cases
     *  (issue https://bugreports.qt-project.org/browse/QTBUG-10907).
     */
    QWidget *setupTabChain(QWidget *prev);

    void setFocus();

public Q_SLOTS:
    void clear();

Q_SIGNALS:
    void removeEntry(SendAssetsEntry *entry);
    void payAmountChanged();
    void subtractFeeFromAmountChanged();

private Q_SLOTS:
    void deleteClicked();
    void on_payTo_textChanged(const QString &address);
    void on_addressBookButton_clicked();
    void on_pasteButton_clicked();
    void onAssetSelected(int index);
    void onSendOwnershipChanged();

private:
    SendAssetsRecipient recipient;
    Ui::SendAssetsEntry *ui;
    WalletModel *model;
    const PlatformStyle *platformStyle;

    bool updateLabel(const QString &address);
};

#endif // SUCRECOIN_QT_SENDASSETSENTRY_H
