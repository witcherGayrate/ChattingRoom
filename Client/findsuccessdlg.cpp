#include "findsuccessdlg.h"
#include "ui_findsuccessdlg.h"
#include<QDir>
#include"applyfriend.h"
FindSuccessDlg::FindSuccessDlg(QWidget *parent)
    : QDialog(parent)
    , _parent(parent),ui(new Ui::FindSuccessDlg)
{
    ui->setupUi(this);
    //隐藏边框
    this->setWindowFlags(windowFlags()|Qt::FramelessWindowHint);
    setWindowTitle("添加");
    //获取当前应用程序的路径
    QString app_path = QCoreApplication::applicationDirPath();
    QString pix_path = QDir::toNativeSeparators(app_path+QDir::separator()+"static"+QDir::separator()+"head_2.jpg");
    QPixmap head_pix(pix_path);
    head_pix = head_pix.scaled(ui->head_lab->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
    ui->head_lab->setPixmap(head_pix);
    ui->add_friend_btn->SetState("normal","hover","press");
    this->setModal(true);

}

FindSuccessDlg::~FindSuccessDlg()
{
    delete ui;
}

void FindSuccessDlg::SetSearchInfo(std::shared_ptr<SearchInfo> si)
{
    ui->name_lab->setText(si->_name);
    _si  = si;

}
void FindSuccessDlg::on_add_friend_btn_clicked()
{

    //todo.. 添加好友界面
    this->hide();
    //弹出好友申请界面
   auto applyFriend = new ApplyFriend(_parent); //之前因为没有在构造函数参数列表初始化导致_parent未初始化，导致构造ApplyFriend时程序崩溃
   applyFriend->SetSearchInfo(_si);
   applyFriend->setModal(true);
   applyFriend->show();

}

