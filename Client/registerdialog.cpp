#include "registerdialog.h"
#include "ui_registerdialog.h"
#include"global.h"

#include<QMessageBox>
#include"httpmgr.h"
#include<QRandomGenerator>
RegisterDialog::RegisterDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RegisterDialog),_countdown(5)
{
    ui->setupUi(this);

    ui->lineEdit_code->setEchoMode(QLineEdit::Password);
    ui->lineEdit_code_sure->setEchoMode(QLineEdit::Password);
    ui->lab_tip->setProperty("state","normal");//设置lab_tip的状态
    repolish(ui->lab_tip);//刷新状态

    //设置大头照
    QPixmap headPhtoo(":/icons/cat2.png");
    headPhtoo = headPhtoo.scaled(ui->lab_photo->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
    ui->lab_photo->setPixmap(headPhtoo);

    //捕获httpmgr注册成功信号，registerdialog做出响应slot
    connect(HttpMgr::GetInstance().get(),&HttpMgr::sig_reg_mod_finish,this,&RegisterDialog::slot_reg_mod_finish);

    initHttpHandlers();
    ui->lab_tip->clear();

    //至少邮箱合法之后，才使用获取验证码按钮
    ui->btn_acquire->setEnabled(false);

    //输入合法性检查
    connect(ui->lineEdit_user,&QLineEdit::editingFinished,[this]()
            {
        checkUserValid();
    });

    connect(ui->lineEdit_email, &QLineEdit::editingFinished, this, [this](){
        checkEmailValid();
    });

    connect(ui->lineEdit_code, &QLineEdit::editingFinished, this, [this](){
        checkPassValid();
    });
    connect(ui->lineEdit_code_sure, &QLineEdit::editingFinished, this, [this](){
        checkConfirmValid();
    });
    connect(ui->lineEdit_verifycode, &QLineEdit::editingFinished, this, [this](){
        checkVarifyValid();
    });

    //鼠标形状
    ui->pass_visible->setCursor(Qt::PointingHandCursor);
    ui->confirm_visible->setCursor(Qt::PointingHandCursor);

    //眼睛状态
    ui->pass_visible->SetState("unvisible","unvisible_hover","","visible",
                               "visible_hover","");
    ui->confirm_visible->SetState("unvisible","unvisible_hover","","visible",
                               "visible_hover","");

    connect(ui->confirm_visible,&ClickedLabel::clicked,this,[this](){

        auto state = ui->confirm_visible->GetCurState();
        if(state==ClickLBState::Normal)
            {
            ui->lineEdit_code_sure->setEchoMode(QLineEdit::Password);
        }else
            {
            ui->lineEdit_code_sure->setEchoMode(QLineEdit::Normal);
        }
        qDebug()<<"Label was clicked!";
    });

    connect(ui->pass_visible,&ClickedLabel::clicked,this,[this](){

        auto state = ui->pass_visible->GetCurState();
        if(state==ClickLBState::Normal)
        {
            ui->lineEdit_code->setEchoMode(QLineEdit::Password);
        }else
        {
            ui->lineEdit_code->setEchoMode(QLineEdit::Normal);
        }
        qDebug()<<"Label was clicked!";
    });

    //创建定时器
    _countdown_timer = new QTimer(this);

    //创建信号和槽，计时器归零发送返回登录界面的信号，否则显示倒计时
    connect(_countdown_timer,&QTimer::timeout,[this](){
        if(_countdown==0)
            {
            _countdown_timer->stop();
            emit sigSwitchLogin();
            return;
        }
        _countdown--;
        auto str = QString("注册成功，%1 s后返回登录").arg(_countdown);
        ui->lab_tip_return->setText(str);
    });
}

RegisterDialog::~RegisterDialog()
{
    qDebug()<<"destruct registerDialog";
    delete ui;
}

//获取验证码的槽函数
void RegisterDialog::on_btn_acquire_clicked()
{
    auto email = ui->lineEdit_email->text();
    //匹配邮箱格式
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch();//格式是否匹配
    if(match)
    {
        //发送http请求获取验证码
        QJsonObject json_obj;
        json_obj["email"]=email;
        HttpMgr::GetInstance()->PostHttpReq(gate_url_prefix+"/get_verifycode",json_obj,ReqId::ID_GET_VERIFY_CODE,
                                            Modules::REGISTERMOD);
    }else
    {
        showTip(tr("邮箱地址不正确"),false);
        //QMessageBox::information(this,"错误提示","邮箱地址不正确");
    }

}

//http请求验证码成功，开始解析response传来的信息
void RegisterDialog::slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err)
{
    if(err!=ErrorCodes::SUCCESS)
    {
        showTip(tr("网络请求错误"),false);
        return;
    }
    //解析Json字符串,res转化为QByteArray
    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());//字符串反序列为json文件
    if(jsonDoc.isNull())
    {
        showTip(tr("json解析失败"),false);
        return;
    }
    //json解析错误
    if(!jsonDoc.isObject())
    {
        showTip(tr("json解析失败"),false);
        return;
    }

    //调用reques_id对应的回调函数，界面显示结果
    _handlers[id](jsonDoc.object());
    return;
}

void RegisterDialog::initHttpHandlers()
{
    //注册获取验证码回包逻辑
    _handlers.insert(ReqId::ID_GET_VERIFY_CODE,[this](const QJsonObject& jsonObj){
        int error = jsonObj["error"].toInt();
        if(error!=ErrorCodes::SUCCESS)
            {
            showTip(tr("参数错误"),false);
            return;
        }
        auto email = jsonObj["email"].toString();
        showTip(tr("验证码已经发送到邮箱，注意查收"),true);
        qDebug()<<"email is"<<email;
    });


    //注册用户回包逻辑
    _handlers.insert(ReqId::ID_REG_USER,[this](QJsonObject jsonObj){
        int error = jsonObj["error"].toInt();
        if(error!=ErrorCodes::SUCCESS)
            {
            showTip(tr("参数错误"),false);
            return;
        }
        auto email = jsonObj["email"].toString();
        showTip(tr("用户注册成功"),true);
        qDebug()<<"user uuid is "<< jsonObj["uid"].toString();
        qDebug()<<"email is " << email;
        ChangeTipPage();//切换界面
    });
}

void RegisterDialog::showTip(QString str,bool ok)
{
    if(ok)
    {
        ui->lab_tip->setProperty("state","normal");
    }
    else
    {
        ui->lab_tip->setProperty("state","err");
    }
    ui->lab_tip->setText(str);
    repolish(ui->lab_tip);
}
void RegisterDialog::AddTipErr(TipErr te,QString tips)
{
    _tip_errs[te] = tips;
    showTip(tips,false);
}

void RegisterDialog::DelTipErr(TipErr te)
{
    _tip_errs.remove(te);
    if(_tip_errs.empty())
    {
        ui->lab_tip->clear();
        return;
    }
    showTip(_tip_errs.first(),false);
}
bool RegisterDialog::checkUserValid()
{
    if(ui->lineEdit_user->text()=="")
    {
        AddTipErr(TipErr::TIP_USER_ERR,tr("用户不能为空"));
        return false;
    }

    DelTipErr(TipErr::TIP_USER_ERR);
    return true;
}

bool RegisterDialog::checkEmailValid()
{
    //验证邮箱的地址正则表达式
    auto email = ui->lineEdit_email->text();
    // 邮箱地址的正则表达式
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch(); // 执行正则表达式匹配
    if(!match){
        //提示邮箱不正确
        AddTipErr(TipErr::TIP_EMAIL_ERR, tr("邮箱地址不正确"));
        return false;
    }
    //此时发送按钮可用
    ui->btn_acquire->setEnabled(true);

    DelTipErr(TipErr::TIP_EMAIL_ERR);
    return true;
}

bool RegisterDialog::checkPassValid()
{
    auto pass = ui->lineEdit_code->text();
    auto confirm = ui->lineEdit_code_sure->text();
    if(pass.length() < 6 || pass.length()>15){
        //提示长度不准确
        AddTipErr(TipErr::TIP_PWD_ERR, tr("密码长度应为6~15"));
        return false;
    }
    // 创建一个正则表达式对象，按照上述密码要求
    // 这个正则表达式解释：
    // ^[a-zA-Z0-9!@#$%^&*]{6,15}$ 密码长度至少6，可以是字母、数字和特定的特殊字符
    QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*.]{6,15}$");
    bool match = regExp.match(pass).hasMatch();
    if(!match){
        //提示字符非法
        AddTipErr(TipErr::TIP_PWD_ERR, tr("不能包含非法字符"));
        return false;;
    }
    DelTipErr(TipErr::TIP_PWD_ERR);

    //直接在这里检查确认密码也可以
    if(pass!=confirm)
    {
        //提示密码不匹配
        AddTipErr(TipErr::TIP_PWD_CONFIRM,tr("密码和确认密码不匹配"));
        return false;
    }else
    {
        DelTipErr(TipErr::TIP_PWD_CONFIRM);
    }
    return true;
}

bool RegisterDialog::checkVarifyValid()
{
    auto pass = ui->lineEdit_verifycode->text();
    if(pass.isEmpty()){
        AddTipErr(TipErr::TIP_VERIFY_ERR, tr("验证码不能为空"));
        return false;
    }
    DelTipErr(TipErr::TIP_VERIFY_ERR);
    return true;
}

bool RegisterDialog::checkConfirmValid()
{
    auto pass = ui->lineEdit_code->text();
    auto confirm = ui->lineEdit_code_sure->text();
    if(confirm.length() < 6 || confirm.length()>15){
        //提示长度不准确
        AddTipErr(TipErr::TIP_CONFIRM_ERR, tr("密码长度应为6~15"));
        return false;
    }
    // 创建一个正则表达式对象，按照上述密码要求
    // 这个正则表达式解释：
    // ^[a-zA-Z0-9!@#$%^&*]{6,15}$ 密码长度至少6，可以是字母、数字和特定的特殊字符
    QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*.]{6,15}$");
    bool match = regExp.match(confirm).hasMatch();
    if(!match){
        //提示字符非法
        AddTipErr(TipErr::TIP_CONFIRM_ERR, tr("不能包含非法字符"));
        return false;;
    }
    DelTipErr(TipErr::TIP_CONFIRM_ERR);


    if(pass!=confirm)
    {
        AddTipErr(TipErr::TIP_PWD_CONFIRM,tr("密码和确认密码不匹配"));
        return false;
    }else
    {
        DelTipErr(TipErr::TIP_PWD_CONFIRM);
    }
    return true;

}

void RegisterDialog::ChangeTipPage()
{
    _countdown_timer->stop();
    ui->stackedWidget->setCurrentWidget(ui->page_2);

    //启动定时器，设置间隔1000毫秒（1秒）
    _countdown_timer->start(1000);
}

void RegisterDialog::on_btn_sure_clicked()
{
    if(ui->lineEdit_user->text()=="")
    {
        showTip(tr("用户名不能为空"),false);
        return;
    }

    if(ui->lineEdit_email->text()=="")
    {
        showTip(tr("邮箱不能为空"),false);
        return;
    }

    if(ui->lineEdit_code->text() == "")
    {
        showTip(tr("密码不能为空"),false);
        return;
    }

    if(ui->lineEdit_code_sure->text() == ""){
        showTip(tr("确认密码不能为空"), false);
        return;
    }
    if(ui->lineEdit_code->text() != ui->lineEdit_code_sure->text()){
        showTip(tr("密码和确认密码不匹配"), false);
        return;
    }
    if(ui->lineEdit_verifycode->text() == ""){
        showTip(tr("验证码不能为空"), false);
        return;
    }

    //发送http请求注册用户
    QJsonObject json_obj;
    json_obj["user"] = ui->lineEdit_user->text();
    json_obj["email"] = ui->lineEdit_email->text();
    json_obj["passwd"] = xorString(ui->lineEdit_code->text()); //字符加密传输

    json_obj["sex"]=0;

    int randomValue = QRandomGenerator::global()->bounded(100);
    int head_i = randomValue%heads.size();

    json_obj["icon"] = heads[head_i];
    json_obj["nick"] = ui->lineEdit_user->text();
    json_obj["confirm"] = xorString(ui->lineEdit_code_sure->text());
    json_obj["verifycode"] = ui->lineEdit_verifycode->text();

    HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix+"/user_register"),
                                        json_obj,ReqId::ID_REG_USER,Modules::REGISTERMOD);

}


void RegisterDialog::on_return_btn_clicked()
{
    _countdown_timer->stop();
    emit sigSwitchLogin(); //mainwindow 连接信号
}

