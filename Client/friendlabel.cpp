#include "friendlabel.h"
#include "ui_friendlabel.h"

FriendLabel::FriendLabel(QWidget *parent)
    : QFrame(parent)
    , ui(new Ui::FriendLabel)
{
    ui->setupUi(this);
    ui->close_lab->SetState("normal","hover","pressed",
                            "selected_normal","selected_hover","selected_pressed");
    connect(ui->close_lab,&ClickedLabel::clicked,this,&FriendLabel::slot_close);
}

FriendLabel::~FriendLabel()
{
    delete ui;
}

void FriendLabel::SetText(QString text)
{
    _text = text;
    ui->tip_lab->setText(_text);
    ui->tip_lab->adjustSize();

    QFontMetrics fontMetrics(ui->tip_lab->font()); // 获取QLabel控件的字体信息
    auto textWidth = fontMetrics.horizontalAdvance(ui->tip_lab->text()); // 获取文本的宽度
    auto textHeight = fontMetrics.height(); // 获取文本的高度

    qDebug()<< " ui->tip_lb.width() is " << ui->tip_lab->width();
    qDebug()<< " ui->close_lb->width() is " << ui->close_lab->width();
    qDebug()<< " textWidth is " << textWidth;
    this->setFixedWidth(ui->tip_lab->width()+ui->close_lab->width()+5);
    this->setFixedHeight(textHeight+2);
    qDebug()<< "  this->setFixedHeight " << this->height();
    _width = this->width();
    _height = this->height();

}

QString FriendLabel::Text()
{
    return _text;
}

void FriendLabel::slot_close()
{
    emit sig_close(_text);
}
