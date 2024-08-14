#ifndef SEARCHLIST_H
#define SEARCHLIST_H
#include<QWheelEvent>
#include <QListWidget>
#include <QObject>
#include<QEvent>
#include<QScrollBar>
#include<QDebug>
#include<QDialog>
#include<memory>
#include"userdata.h"
#include"loadingdlg.h"

class SearchList : public QListWidget
{
    Q_OBJECT
public:
    SearchList(QWidget* parent=nullptr);
    void CloseFindDlg();
    void SetSearchEdit(QWidget* edit);

    // QObject interface
protected:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
private:
    void waitPending(bool pending = true);
    bool _send_pending;
    void addTipItem();
    std::shared_ptr<QDialog> _find_dlg;
    QWidget* _search_edit;
    LoadingDlg * _loadingDialog;
private slots:
    void slot_item_clicked(QListWidgetItem* item);
    void slot_user_search(std::shared_ptr<SearchInfo> si);
signals:
    void sig_jump_chat_item(std::shared_ptr<SearchInfo> si);
};



#endif // SEARCHLIST_H
