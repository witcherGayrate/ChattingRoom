#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QDialog>
#include"global.h"
#include"statewidget.h"
#include"userdata.h"
#include"QListWidgetItem"
namespace Ui {
class ChatDialog;
}

class ChatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChatDialog(QWidget *parent = nullptr);
    ~ChatDialog();

    //添加用户列表
    void addChatUserList();
    void initUserInfoList();//初始化vector
private:
    void ShowSearch(bool bsearch=false);
    void AddLBGroup(StateWidget* lb);//侧边栏图标
    void ClearLabelState(StateWidget* lb);//反向清除状态
    void handleGlobalMousePress(QMouseEvent *event);
    ChatUIMode _mode;//控制sidebar
    ChatUIMode _state;//search edit控制显示的list
    bool _b_loading;
    QList<StateWidget*> _lb_list;
    QMap<int,QListWidgetItem*> _chat_items_added;//已经添加的chatitem
    int _cur_chat_uid;//当天聊天id
    void SetSelectChatPage(int uid=0);
    void loadMoreChatUser();
    void loadMoreConUser();
    //装饰用的useriteminfo
    QVector<QString> _strs;
    QVector<QString> _names;
    QVector<QString> _heads;

    Ui::ChatDialog *ui;
private slots:
    void slot_loading_chat_user();
    void slot_side_chat();
    void slot_side_contact();
    void slot_text_changed(const QString &str);
    void slot_loading_contact_user();
public slots:
    void slot_apply_friend(std::shared_ptr<AddFriendApply> apply);
    void slot_add_auth_friend(std::shared_ptr<AuthInfo> auth_info);
    void slot_auth_rsp(std::shared_ptr<AuthRsp> auth_rsp);
    void slot_jump_chat_item(std::shared_ptr<SearchInfo>si);
    void SetSelectChatItem(int uid);
    // QObject interface
protected:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
};





#endif // CHATDIALOG_H
