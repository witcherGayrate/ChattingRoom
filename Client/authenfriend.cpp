#include "authenfriend.h"
#include "qjsonobject.h"
#include "ui_authenfriend.h"
#include<QScrollBar>
#include"usermgr.h"
#include<QJsonDocument>
#include<QJsonObject>
#include"tcpmgr.h"
AuthenFriend::AuthenFriend(QWidget *parent)
    : QDialog(parent),_label_point(2,6)
    , ui(new Ui::AuthenFriend)
{


    ui->setupUi(this);
    //隐藏对话框标题栏
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    this->setObjectName("AuthenFriend");
    this->setModal(true);

    ui->lab_ed->setPlaceholderText("搜索、添加标签");
    ui->back_ed->setPlaceholderText("一只瞌睡红");

    ui->lab_ed->SetMaxLength(21);
    ui->lab_ed->move(2,2);
    ui->lab_ed->setFixedHeight(20);
    ui->lab_ed->setMaxLength(10);
    ui->input_tip_wid->hide();

    _tip_cur_point = QPoint(5,5);

    _tip_data = {"同学","家人","菜鸟教程","C++ Primer","Rust 程序设计",
                 "Python","nodejs开发指南","go 语言开发指南",
                 "游戏伙伴","金融投资","微信读书","拼多多拼友" };
    //显示更多标签
    connect(ui->more_lab,&ClickedOnceLabel::clicked,this,&AuthenFriend::ShowMoreLabel);
    InitTipLbs();//初始化好友标签列表
    //连接输入标签回车事件
    connect(ui->lab_ed,&CustomLineEdit::returnPressed,this,&AuthenFriend::SlotLabelEnter);
    connect(ui->lab_ed,&CustomLineEdit::textChanged,this,&AuthenFriend::SlotLabelTextChange);
    connect(ui->lab_ed,&CustomLineEdit::editingFinished,this,&AuthenFriend::SlotLabelEditFinished);
    connect(ui->tip_lab,&ClickedOnceLabel::clicked,this,&AuthenFriend::SlotAddFirendLabelByClickTip);

    //滚动条设置
    ui->scrollArea->horizontalScrollBar()->setHidden(true);
    ui->scrollArea->verticalScrollBar()->setHidden(true);
    ui->scrollArea->installEventFilter(this);
    ui->sure_btn->SetState("normal","hover","press");
    ui->cancel_btn->SetState("normal","hover","press");

    //连接确认和取消按钮的槽函数
    connect(ui->cancel_btn, &QPushButton::clicked, this, &AuthenFriend::SlotApplyCancel);
    connect(ui->sure_btn, &QPushButton::clicked, this, &AuthenFriend::SlotApplySure);
}

AuthenFriend::~AuthenFriend()
{
    delete ui;
}


void AuthenFriend::InitTipLbs()
{
    int lines = 1;
    for(std::size_t i = 0; i < _tip_data.size(); i++){

        auto* lb = new ClickedLabel(ui->lab_list);
        lb->SetState("normal", "hover", "pressed", "selected_normal",
                     "selected_hover", "selected_pressed");
        lb->setObjectName("tipslb");
        lb->setText(_tip_data[i]);
        connect(lb, &ClickedLabel::clicked, this, &AuthenFriend::SlotChangeFriendLabelByTip);

        QFontMetrics fontMetrics(lb->font()); // 获取QLabel控件的字体信息
        int textWidth = fontMetrics.horizontalAdvance(lb->text()); // 获取文本的宽度
        int textHeight = fontMetrics.height(); // 获取文本的高度

        if (_tip_cur_point.x() + textWidth + tip_offset > ui->lab_list->width()) {
            lines++;
            if (lines > 2) {
                delete lb;
                return;
            }

            _tip_cur_point.setX(tip_offset);
            _tip_cur_point.setY(_tip_cur_point.y() + textHeight + 15);

        }

        auto next_point = _tip_cur_point;

        AddTipLbs(lb, _tip_cur_point,next_point, textWidth, textHeight);

        _tip_cur_point = next_point;
    }
}

void AuthenFriend::AddTipLbs(ClickedLabel *lb, QPoint cur_point, QPoint &next_point, int text_width, int text_height)
{
    Q_UNUSED(text_height);
    lb->move(cur_point);
    lb->show();
    _add_labels.insert(lb->text(), lb);
    _add_label_keys.push_back(lb->text());
    next_point.setX(lb->pos().x() + text_width + 15);
    next_point.setY(lb->pos().y());
}

bool AuthenFriend::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->scrollArea && event->type() == QEvent::Enter)
    {
        ui->scrollArea->verticalScrollBar()->setHidden(false);
    }
    else if (watched  == ui->scrollArea && event->type() == QEvent::Leave)
    {
        ui->scrollArea->verticalScrollBar()->setHidden(true);
    }
    return QObject::eventFilter(watched, event);
}

void AuthenFriend::SetApplyInfo(std::shared_ptr<ApplyInfo> apply_info)
{
    _apply_info = apply_info;
    ui->back_ed->setPlaceholderText(apply_info->_name);
}

void AuthenFriend::resetLabels()
{
    auto max_width = ui->grid_widget->width();
    auto label_height = 0;
    for(auto iter = _friend_labels.begin(); iter != _friend_labels.end(); iter++){
        //todo... 添加宽度统计
        if( _label_point.x() + iter.value()->width() > max_width) {
            _label_point.setY(_label_point.y()+iter.value()->height()+6);
            _label_point.setX(2);
        }

        iter.value()->move(_label_point);
        iter.value()->show();

        _label_point.setX(_label_point.x()+iter.value()->width()+2);
        _label_point.setY(_label_point.y());
        label_height = iter.value()->height();
    }

    if(_friend_labels.isEmpty()){
        ui->lab_ed->move(_label_point);
        return;
    }

    if(_label_point.x() + MIN_APPLY_LABEL_ED_LEN > ui->grid_widget->width()){
        ui->lab_ed->move(2,_label_point.y()+label_height+6);
    }else{
        ui->lab_ed->move(_label_point);
    }
}

void AuthenFriend::addLabel(QString name)
{
    //朋友便签已存在、
    if(_friend_labels.find(name)!=_friend_labels.end())
    {
        ui->lab_ed->clear();
        return;
    }

    //创建friendlabel并设置名称
    auto tmpLabel = new FriendLabel(ui->grid_widget);
    tmpLabel->SetText(name);
    tmpLabel->setObjectName("FriendLabel");

    auto max_width = ui->grid_widget->width();
    //todo... 添加宽度统计
    if(_label_point.x() + tmpLabel->width() > max_width)//超出宽度准备换行
    {
        _label_point.setY(_label_point.y()+tmpLabel->height()+6);
        _label_point.setX(2);
    }else
    {

    }

    //把新lab移动到新位置,并添加到friendlab容器
    tmpLabel->move(_label_point);
    tmpLabel->show();
    _friend_labels[tmpLabel->Text()] = tmpLabel;
    _friend_label_keys.push_back(tmpLabel->Text());

    //如果点击了friendlabel则关闭
    connect(tmpLabel,&FriendLabel::sig_close,this,&AuthenFriend::SlotRemoveFriendLabel);

    //更新_label_point的位置
    _label_point.setX(_label_point.x()+tmpLabel->width()+2);

    //判断lab_ed的长度是否越界(friendlab 和lab_ed在一个widget中所以也要判断位置)
    if(_label_point.x()+MIN_APPLY_LABEL_ED_LEN >ui->grid_widget->width()){
        //长度越界则换至第二行
        ui->lab_ed->move(2, _label_point.y() + tmpLabel->height() + 2);
    }else
    {
        ui->lab_ed->move(_label_point);
    }

    //friend lab添加之后，lab_ed清空
    ui->lab_ed->clear();

    //修改label 容器grid_widget的高度
    if(ui->grid_widget->height()< _label_point.y() + tmpLabel->height()+2){
        ui->grid_widget->setFixedHeight(_label_point.y() + tmpLabel->height() * 2 + 2);
    }
}

void AuthenFriend::ShowMoreLabel()
{
    qDebug()<< "receive more label clicked";
    ui->more_lab_wid->hide();

    ui->lab_list->setFixedWidth(325);
    _tip_cur_point = QPoint(5, 5);
    auto next_point = _tip_cur_point;
    int textWidth;
    int textHeight;
    //重拍现有的label
    for(auto & added_key : _add_label_keys){
        auto added_lb = _add_labels[added_key];

        QFontMetrics fontMetrics(added_lb->font()); // 获取QLabel控件的字体信息
        textWidth = fontMetrics.horizontalAdvance(added_lb->text()); // 获取文本的宽度
        textHeight = fontMetrics.height(); // 获取文本的高度

        if(_tip_cur_point.x() +textWidth + tip_offset > ui->lab_list->width()){
            _tip_cur_point.setX(tip_offset);
            _tip_cur_point.setY(_tip_cur_point.y()+textHeight+15);
        }
        added_lb->move(_tip_cur_point);

        next_point.setX(added_lb->pos().x() + textWidth + 15);
        next_point.setY(_tip_cur_point.y());

        _tip_cur_point = next_point;
    }

    //添加未添加的
    for(size_t i = 0; i < _tip_data.size(); i++){
        auto iter = _add_labels.find(_tip_data[i]);
        if(iter != _add_labels.end()){
            continue;
        }

        auto* lb = new ClickedLabel(ui->lab_list);
        lb->SetState("normal", "hover", "pressed", "selected_normal",
                     "selected_hover", "selected_pressed");
        lb->setObjectName("tipslb");
        lb->setText(_tip_data[i]);
        connect(lb, &ClickedLabel::clicked, this, &AuthenFriend::SlotChangeFriendLabelByTip);

        QFontMetrics fontMetrics(lb->font()); // 获取QLabel控件的字体信息
        int textWidth = fontMetrics.horizontalAdvance(lb->text()); // 获取文本的宽度
        int textHeight = fontMetrics.height(); // 获取文本的高度

        if (_tip_cur_point.x() + textWidth + tip_offset > ui->lab_list->width()) {

            _tip_cur_point.setX(tip_offset);
            _tip_cur_point.setY(_tip_cur_point.y() + textHeight + 15);

        }

        next_point = _tip_cur_point;

        AddTipLbs(lb, _tip_cur_point, next_point, textWidth, textHeight);

        _tip_cur_point = next_point;
    }

    int diff_height = next_point.y() + textHeight + tip_offset - ui->lab_list->height();
    ui->lab_list->setFixedHeight(next_point.y() + textHeight + tip_offset);

    //qDebug()<<"after resize ui->lb_list size is " <<  ui->lb_list->size();
    ui->scrollcontent->setFixedHeight(ui->scrollcontent->height()+diff_height);
}

void AuthenFriend::SlotLabelEnter()
{
    if(ui->lab_ed->text().isEmpty())
    {
        return;
    }
    auto text = ui->lab_ed->text();
    //加入标签
    addLabel(ui->lab_ed->text());
    //隐藏提示栏
    ui->input_tip_wid->hide();
    auto find_it  = std::find(_tip_data.begin(),_tip_data.end(),text);
    //找到了就设为选中状态
    if(find_it==_tip_data.end())
    {
        _tip_data.push_back(text);
    }
    //判断显示栏是否有该标签
    auto find_add = _add_labels.find(text);
    if(find_add != _add_labels.end())
    {
        find_add.value()->SetCurState(ClickLBState::Selected);
        return ;
    }

    //标签展示栏也增加一个标签，并设置绿色选中
    auto* lb = new ClickedLabel(ui->lab_list);
    lb->SetState("normal","hover","pressed","selected_normal","selected_hover",
                 "selected_pressed");
    lb->setObjectName("tipslb");
    lb->setText(text);
    connect(lb,&ClickedLabel::clicked,this,&AuthenFriend::SlotChangeFriendLabelByTip);

    //获取lab控件的字体信息
    QFontMetrics fontMetrics(lb->font());
    int textWidth = fontMetrics.horizontalAdvance(lb->text());
    int textHeight = fontMetrics.height();//文本高度

    if(_tip_cur_point.x()+textWidth+tip_offset +3>ui->lab_list->width())
    {
        _tip_cur_point.setX(5);
        _tip_cur_point.setY(_tip_cur_point.y()+textHeight+15);
    }

    //更新next_point
    auto next_point = _tip_cur_point;

    AddTipLbs(lb, _tip_cur_point, next_point, textWidth, textHeight);
    _tip_cur_point = next_point;

    int diff_height = next_point.y() + textHeight + tip_offset - ui->lab_list->height();
    ui->lab_list->setFixedHeight(next_point.y() + textHeight + tip_offset);

    lb->SetCurState(ClickLBState::Selected);

    ui->scrollcontent->setFixedHeight(ui->scrollcontent->height() + diff_height);
}

void AuthenFriend::SlotRemoveFriendLabel(QString name)
{
    qDebug() << "receive close signal";

    _label_point.setX(2);
    _label_point.setY(6);

    auto find_iter = _friend_labels.find(name);
    //没有找到name对应的lab直接返回
    if(find_iter == _friend_labels.end()){
        return;
    }

    auto find_key = _friend_label_keys.end();
    for(auto iter = _friend_label_keys.begin(); iter != _friend_label_keys.end();
         iter++){
        if(*iter == name){
            find_key = iter;
            break;
        }
    }

    //找到了name对应的key，就从keys中删除
    if(find_key != _friend_label_keys.end()){
        _friend_label_keys.erase(find_key);
    }

    //删除friend label
    delete find_iter.value();
    //删除name key
    _friend_labels.erase(find_iter);

    //重新显示labs
    resetLabels();

    //已经创建好的标签
    auto find_add = _add_labels.find(name);
    if(find_add == _add_labels.end()){
        return;
    }

    find_add.value()->ResetNormalState();
}

//点击已有标签添加或删除新联系人的标签
void AuthenFriend::SlotChangeFriendLabelByTip(QString lbtext, ClickLBState state)
{
    auto find_iter = _add_labels.find(lbtext);
    //没找到lab
    if(find_iter == _add_labels.end()){
        return;
    }

    //选中则添加
    if(state == ClickLBState::Selected){
        //编写添加逻辑
        addLabel(lbtext);
        return;
    }

    if(state == ClickLBState::Normal){
        //编写删除逻辑
        SlotRemoveFriendLabel(lbtext);
        return;
    }
}

//输入便签时，根据文本变化显示提示栏
void AuthenFriend::SlotLabelTextChange(const QString &text)
{
    //标签输入栏为空
    if (text.isEmpty()) {
        ui->tip_lab->setText("");
        ui->input_tip_wid->hide();
        return;
    }
    //输入的内容是否可以在_tip_data中找到
    auto iter = std::find(_tip_data.begin(), _tip_data.end(), text);
    if (iter == _tip_data.end()) {
        auto new_text = add_prefix + text;
        ui->tip_lab->setText(new_text);
        ui->input_tip_wid->show();
        return;
    }
    ui->tip_lab->setText(text);
    ui->input_tip_wid->show();
}

void AuthenFriend::SlotLabelEditFinished()
{
    //隐藏输入提示wid
    ui->input_tip_wid->hide();
}

//通过点击下方便签来加入用户标签
void AuthenFriend::SlotAddFirendLabelByClickTip(QString text)
{
    int index = text.indexOf(add_prefix);
    if (index != -1) {
        text = text.mid(index + add_prefix.length());
    }
    addLabel(text);
    //标签展示栏也增加一个标签, 并设置绿色选中
    auto find_it = std::find(_tip_data.begin(),_tip_data.end(),text);
    if(find_it==_tip_data.end())
    {
        _tip_data.push_back(text);
    }

    //判断展示栏中是否有该标签
    auto find_add=_add_labels.find(text);
    if(find_add!= _add_labels.end())
    {
        find_add.value()->SetCurState(ClickLBState::Selected);
        return;
    }

    auto* lb = new ClickedLabel(ui->lab_list);
    lb->SetState("normal", "hover", "pressed", "selected_normal",
                 "selected_hover", "selected_pressed");
    lb->setObjectName("tipslb");
    lb->setText(text);
    connect(lb, &ClickedLabel::clicked, this, &AuthenFriend::SlotChangeFriendLabelByTip);
    qDebug() << "ui->lb_list->width() is " << ui->lab_list->width();
    qDebug() << "_tip_cur_point.x() is " << _tip_cur_point.x();

    QFontMetrics fontMetrics(lb->font()); // 获取QLabel控件的字体信息
    int textWidth = fontMetrics.horizontalAdvance(lb->text()); // 获取文本的宽度
    int textHeight = fontMetrics.height(); // 获取文本的高度
    qDebug() << "textWidth is " << textWidth;

    if (_tip_cur_point.x() + textWidth+ tip_offset+3 > ui->lab_list->width()) {

        _tip_cur_point.setX(5);
        _tip_cur_point.setY(_tip_cur_point.y() + textHeight + 15);

    }

    auto next_point = _tip_cur_point;

    AddTipLbs(lb, _tip_cur_point, next_point, textWidth,textHeight);
    _tip_cur_point = next_point;

    int diff_height = next_point.y() + textHeight + tip_offset - ui->lab_list->height();
    ui->lab_list->setFixedHeight(next_point.y() + textHeight + tip_offset);

    lb->SetCurState(ClickLBState::Selected);

    ui->scrollcontent->setFixedHeight(ui->scrollcontent->height()+ diff_height );
}

void AuthenFriend:: SlotApplySure()
{
    qDebug() << "Slot Apply Sure ";
    //添加发送逻辑
    QJsonObject jsonObj;
    auto uid = UserMgr::GetInstance()->GetUid();
    jsonObj["fromuid"] = uid;
    jsonObj["touid"] = _apply_info->_uid;
    QString back_name = "";
    if(ui->back_ed->text().isEmpty()){
        back_name = ui->back_ed->placeholderText();
    }else{
        back_name = ui->back_ed->text();
    }
    jsonObj["back"] = back_name;

    QJsonDocument doc(jsonObj);
    QByteArray jsonData = doc.toJson(QJsonDocument::Compact);

    //发送tcp请求给chat server
    emit TcpMgr::GetInstance()->sig_send_data(ReqId::ID_AUTH_FRIEND_REQ, jsonData);

    this->hide();
    deleteLater();

}

void AuthenFriend::SlotApplyCancel()
{
    qDebug()<<"Slot Apply Cancel";
    this->hide();
    this->deleteLater();
}
