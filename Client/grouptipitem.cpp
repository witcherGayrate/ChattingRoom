 #include "grouptipitem.h"
#include "ui_grouptipitem.h"

GroupTipItem::GroupTipItem(QWidget *parent)
    : ListItemBase(parent),_tip("")
    , ui(new Ui::GroupTipItem)
{
    ui->setupUi(this);
    this->SetItemType(ListItemType::GROUP_TIP_ITEM);
}

GroupTipItem::~GroupTipItem()
{
    delete ui;
}

void GroupTipItem::SetGroupTip(QString tip)
{
    _tip = tip;
    ui->label->setText(tip);
}

QSize GroupTipItem::sizeHint() const
{
    return QSize(250,25);
}
