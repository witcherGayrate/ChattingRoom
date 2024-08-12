#ifndef GLOBAL_H
#define GLOBAL_H

#include<QWidget>
#include<QStyle>
#include<functional>
#include<mutex>
#include<memory>
#include<iostream>
#include<QByteArray>
#include<QNetworkReply>
#include<QRegularExpression>
#include<vector>

extern QString gate_url_prefix;

/*
 *@brief repolish 用来刷新qss的状态
*/
extern std::function<void(QWidget*)> repolish;
extern std::function<QString(QString)> xorString;//加密字符串
enum ReqId{
    ID_GET_VERIFY_CODE = 1001,//获取验证码
    ID_REG_USER = 1002,//注册用户
    ID_RESET_PWD = 1003,//重置密码
    ID_LOGIN_USER = 1004,//用户登录
    ID_CHAT_LOGIN = 1005,//连接chatserver
    ID_CHAT_LOGIN_RSP = 1006,//登陆聊天服务器回包
    ID_SEARCH_USER_REQ = 1007,//用户搜索请求
    ID_SEARCH_USER_RSP = 1008,//搜索用户回包
    ID_ADD_FRIEND_REQ = 1009,//添加好友申请
    ID_ADD_FRIEND_RSP = 1010,//申请添加好友回复
    ID_NOTIFY_ADD_FRIEND_REQ = 1011,//通知用户添加好友申请
    ID_AUTH_FRIEND_REQ = 1013,//认证好友请求
    ID_AUTH_FRIEND_RSP = 1014,//认证好友回复
    ID_NOTIFY_AUTH_FRIEND_REQ = 1015,//通知用户认证好友申请
    ID_TEXT_CHAT_MSG_REQ = 1017,//文本聊天信息请求
    ID_TEXT_CHAT_MSG_RSP  = 1018,  //文本聊天信息回复
    ID_NOTIFY_TEXT_CHAT_MSG_REQ = 1019, //通知用户文本聊天信息
};

enum Modules{
    REGISTERMOD = 0,
    RESETMOD = 1,
    LOGINMOD = 2,
};

enum ErrorCodes{
    SUCCESS = 0,
    ERR_JSON=1,//json解析失败
    ERR_NETWORK =2,//网络错误
};

enum ClickLBState
{
    Normal=0,
    Selected =1
};

enum TipErr{
    TIP_SUCCESS = 0,
    TIP_EMAIL_ERR =1,
    TIP_PWD_ERR=2,
    TIP_CONFIRM_ERR=3,
    TIP_PWD_CONFIRM =4,//确认密码不匹配
    TIP_VERIFY_ERR=5,
    TIP_USER_ERR=6
};

struct ServerInfo
{
    int Uid;
    QString Host;
    QString Port;
    QString Token;
};
//聊天界面集中模式
enum ChatUIMode
{
    SearchMode,//搜索模式
    ChatMode,//聊天模式
    ContactMode,//联系模式
};

//自定义QListItemType的几种类型
enum ListItemType{
    CHAT_USER_ITEM,//聊天用户
    CONTACT_USER_ITEM,//联系人用户
    SEARCH_USER_ITEM,//搜索到的用户
    ADD_USER_TIP_ITEM,//提示添加用户
    INVALID_ITEM,//不可点击条目
    GROUP_TIP_ITEM,//分组提示条目
    LINE_ITEM,//分割线
    APPLY_FRIEND_ITEM,//好友申请
};

//聊天角色
enum ChatRole
{
    Self,
    Other,
};

struct MsgInfo
{
    QString msgFlag;//”text,image,file“
    QString content;//表示文件和图像的url，文本信息
    QPixmap pixmap;//文件和图片的缩略图
};

//预定义的一些用户
const std::vector<QString> names{
    "爱吃的胖红",
    "胖奇不想动",
    "AAA犟驴红",
    "乌歪达不溜",
    "春风秋雨",
    "赫尔",
    "小马哥",
    "熬夜波比",
    "贪吃波比"

};
const std::vector<QString> heads{
    ":/icons/head_1.jpg",
    ":/icons/head_2.jpg",
    ":/icons/head_3.jpg",
    ":/icons/head_4.jpg",
    ":/icons/head_5.jpg",
    ":/icons/head_6.png",
    ":/icons/head_7.png",
    ":/icons/head_8.png",
    ":/icons/head_9.png",
    ":/icons/head_10.png",
    ":/icons/head_12.png",
    ":/icons/head_13.png",
};
const std::vector<QString> strs
{
    "走到哪儿了？你是不是边走边玩手机！",
    "食堂门口见。",
    "什么时候发工资，我没衣服穿了。",
    "今天又胖了两斤",
    "牛皮！",
    "几点下班？三缺一。",
    "这天气能把我晒成化了",
    "你真是个可爱的小胖子",
    "想出去玩，不想上班！",
};
//申请好友标签输入框最低长度
const int MIN_APPLY_LABEL_ED_LEN = 40;

const QString add_prefix = "添加标签";

const int tip_offset = 5;

const int CHAT_COUNT_PER_PAGE=13;
#endif // GLOBAL_H
