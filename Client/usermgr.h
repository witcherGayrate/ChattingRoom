#ifndef USERMGR_H
#define USERMGR_H

#include <QObject>
#include"singleton.h"
#include<memory>
#include"userdata.h"
#include<vector>
/*
 * 客户端数据管理和登陆验证回包处理
 *
*/
class UserMgr : public QObject,public Singleton<UserMgr>,
                public std::enable_shared_from_this<UserMgr>
{
    Q_OBJECT
public:
    friend class Singleton<UserMgr>;
    ~UserMgr();
    void SetToken(QString token);
    int GetUid();
    QString GetName();
    std::vector<std::shared_ptr<ApplyInfo>> GetApplyList();//获取申请好友的列表

    bool AlreadyApply(int uid);//判断是否已经在申请列表
    void AddApplyList(std::shared_ptr<ApplyInfo> app);//添加好友申请到申请列表
    void SetUserInfo(std::shared_ptr<UserInfo> user_info);
    std::shared_ptr<UserInfo> GetUserInfo();
    void AppendApplyList(QJsonArray array);
    void AppendFriendList(QJsonArray array);//好友列表
    bool CheckFriendById(int uid);//检查是否已经在好友列表
    void AddFriend(std::shared_ptr<AuthRsp> auth_rsp);//向好友map中添加好友信息
    void AddFriend(std::shared_ptr<AuthInfo> auth_info);
    std::shared_ptr<FriendInfo> GetFriendById(int uid);

    std::vector<std::shared_ptr<FriendInfo>> GetChatListPerPage();//单页获取聊天列表
    bool IsLoadChatFin();//聊天是否加载完成
    void UpdateChatLoadedCount();//已加载的聊天数
    std::vector<std::shared_ptr<FriendInfo>> GetConListPerPage();
    void UpdateContactLoadedCount();
    bool IsLoadConFin();
private:
    UserMgr();
    QString _token;
    std::vector<std::shared_ptr<ApplyInfo>> _apply_list;//todo...可以改进用map
    std::shared_ptr<UserInfo> _user_info;
    QMap<int,std::shared_ptr<FriendInfo>> _friend_map;//好友列表
    std::vector<std::shared_ptr<FriendInfo>> _friend_list;
    int _chat_loaded;
    int _contact_loaded;

};

#endif // USERMGR_H
