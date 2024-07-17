#include "resetdialog.h"
#include "ui_resetdialog.h"


ResetDialog::ResetDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ResetDialog)
{
    ui->setupUi(this);

    //设置新密码的输入模式
    ui->newpwd_edit->setEchoMode(QLineEdit::Password);
    ui->newpwd_visible->setProperty("state","normal");
    repolish(ui->newpwd_visible);//刷新样式

    //设置密码可见图标的鼠标形状
    ui->newpwd_visible->setCursor(Qt::PointingHandCursor);
    //初始化可见图标的属性
    ui->newpwd_visible->SetState("unvisible","unvisible_hover","","visible",
                                 "visible_hover","");
    //连接可见图标的信号槽
    connect(ui->newpwd_visible,&ClickedLabel::clicked,this,[this](){
        auto curstate = ui->newpwd_visible->GetCurState();
        if(curstate==ClickLBState::Normal)
        {
            ui->newpwd_edit->setEchoMode(QLineEdit::Password);
        }else
            {
            ui->newpwd_edit->setEchoMode(QLineEdit::Normal);
        }
        qDebug()<<"Label was clicked!";
    });

    //连接输入错误监测的信号
    connect(ui->user_edit,&QLineEdit::editingFinished,this,[this](){
        checkUserValid();
    });

    connect(ui->email_edit,&QLineEdit::editingFinished,this,[this](){
        checkEmailValid();
    });

    connect(ui->newpwd_edit,&QLineEdit::editingFinished,this,[this](){
        checkPassValid();
    });

    connect(ui->verifycode_edit,&QLineEdit::editingFinished,this,[this](){
        checkVerifyValid();
    });
    //清除错误提示
    ui->lab_error_tip->clear();
    //获取验证码按钮初始设为不可用
    ui->btn_verifycode->setEnabled(false);
    //连接reset相关信号和处理回调函数
    initHandlers(); //注册处理HTTP回包的回调函数
    connect(HttpMgr::GetInstance().get(),&HttpMgr::sig_reset_mod_finish,this,
            &ResetDialog::slot_reset_mod_finish);

}

ResetDialog::~ResetDialog()
{
    delete ui;
}

void ResetDialog::showTip(QString str, bool ok)
{
    if(ok)
    {
        ui->lab_error_tip->setProperty("state","normal");
    }
    else
    {
        ui->lab_error_tip->setProperty("state","err");
    }
    ui->lab_error_tip->setText(str);
    repolish(ui->lab_error_tip);
}

bool ResetDialog::checkUserValid()
{
    if(ui->user_edit->text() == ""){
        AddTipErr(TipErr::TIP_USER_ERR, tr("用户名不能为空"));
        return false;
    }
    DelTipErr(TipErr::TIP_USER_ERR);
    return true;
}

bool ResetDialog::checkPassValid()
{
    auto pass = ui->newpwd_edit->text();
    if(pass.length() < 6 || pass.length()>15){
        //提示长度不准确
        AddTipErr(TipErr::TIP_PWD_ERR, tr("密码长度应为6~15"));
        return false;
    }

    // 创建一个正则表达式对象，按照上述密码要求
    // 这个正则表达式解释：
    // ^[a-zA-Z0-9!@#$%^&*]{6,15}$ 密码长度至少6，可以是字母、数字和特定的特殊字符
    QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*]{6,15}$");
    bool match = regExp.match(pass).hasMatch();
    if(!match){
        //提示字符非法
        AddTipErr(TipErr::TIP_PWD_ERR, tr("不能包含非法字符"));
        return false;;
    }

    DelTipErr(TipErr::TIP_PWD_ERR);
    return true;
}

bool ResetDialog::checkEmailValid()
{
    //验证邮箱的地址正则表达式
    auto email = ui->email_edit->text();
    // 邮箱地址的正则表达式
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch(); // 执行正则表达式匹配
    if(!match){
        //提示邮箱不正确
        AddTipErr(TipErr::TIP_EMAIL_ERR, tr("邮箱地址不正确"));
        return false;
    }
    //获取验证码按钮可用
    ui->btn_verifycode->setEnabled(true);
    DelTipErr(TipErr::TIP_EMAIL_ERR);
    return true;
}

bool ResetDialog::checkVerifyValid()
{
    auto pass = ui->verifycode_edit->text();
    if(pass.isEmpty()){
        AddTipErr(TipErr::TIP_VERIFY_ERR, tr("验证码不能为空"));
        return false;
    }
    DelTipErr(TipErr::TIP_VERIFY_ERR);
    return true;
}

void ResetDialog::AddTipErr(TipErr te, QString tips)
{
    _tip_errs[te] = tips;
    showTip(tips,false);

}

void ResetDialog::DelTipErr(TipErr te)
{
    _tip_errs.remove(te);
    if(_tip_errs.empty()){
        ui->lab_error_tip->clear();
        return;
    }
    showTip(_tip_errs.first(), false);
}

void ResetDialog::on_btn_verifycode_clicked()
{
    qDebug()<<"receive verify btn clicked ";
    auto email = ui->email_edit->text();
    auto bcheck = checkEmailValid();
    if(!bcheck){
        return;
    }

    //发送Http请求获取验证码
    QJsonObject json_obj;
    json_obj["email"] = email;
    HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix+"/get_verifycode"),
                                        json_obj,ReqId::ID_GET_VERIFY_CODE,Modules::RESETMOD);
}

void ResetDialog::slot_reset_mod_finish(ReqId id, QString res, ErrorCodes err)
{
    if(err!=ErrorCodes::SUCCESS)
    {
        showTip(tr("网络请求错误"),false);
        return;
    }

    //解析JSON字符串，res转为QByteArray
    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());
    //json解析错误
    if(jsonDoc.isNull())
    {
        showTip(tr("json解析错误"),false);
        return;
    }
    //json解析错误
    if(!jsonDoc.isObject()){
        showTip(tr("json解析错误"),false);
        return;
    }

    //调用对应的逻辑，根据id回调
    _handlers[id](jsonDoc.object());

    return;
}

void ResetDialog::initHandlers()
{
    //注册获取验证码回包逻辑
    _handlers.insert(ReqId::ID_GET_VERIFY_CODE,[this](QJsonObject jsonObj){
        int err = jsonObj["error"].toInt();
        if(err!= ErrorCodes::SUCCESS)
        {
            showTip(tr("参数错误"),false);
            return;
        }
        auto email = jsonObj["email"].toString();
        showTip(tr("验证码已发送到邮箱,注意查收"),true);
        qDebug()<<"email is "<<email;
    });

    //注册，注册用户回包逻辑
    _handlers.insert(ReqId::ID_RESET_PWD,[this](QJsonObject jsonObj){
        int error = jsonObj["error"].toInt();
        if(error != ErrorCodes::SUCCESS){
            showTip(tr("参数错误"),false);
            return;
        }
        auto email = jsonObj["email"].toString();
        showTip(tr("重置成功,点击返回登录"), true);
        qDebug()<< "email is " << email ;
        qDebug()<< "user uuid is " <<  jsonObj["uuid"].toString();
    });
}


void ResetDialog::on_btn_sure_clicked()
{
    bool valid = checkUserValid();
    if(!valid){
        return;
    }
    valid = checkEmailValid();
    if(!valid){
        return;
    }
    valid = checkPassValid();
    if(!valid){
        return;
    }
    valid = checkVerifyValid();
    if(!valid){
        return;
    }

    //发送Http重置用户请求
    QJsonObject json_obj;
    json_obj["user"] = ui->user_edit->text();
    json_obj["email"] = ui->email_edit->text();
    json_obj["passwd"] = xorString(ui->newpwd_edit->text());
    json_obj["verifycode"] = ui->verifycode_edit->text(); //json字段值
    HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix+"/reset_pwd"),
                                        json_obj,ReqId::ID_RESET_PWD,Modules::RESETMOD);
}

//直接返回按钮
void ResetDialog::on_btn_return_clicked()
{
    qDebug()<<"clicked return button";
    //发送swtichLogin信号，mainwindow接收信号返回切换回lognin
    emit switchLogin();
}

