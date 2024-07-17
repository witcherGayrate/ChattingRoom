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
};

enum Modules{
    REGISTERMOD = 0,
    RESETMOD = 1
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

#endif // GLOBAL_H
