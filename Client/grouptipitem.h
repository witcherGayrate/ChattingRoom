#ifndef GROUPTIPITEM_H
#define GROUPTIPITEM_H


#include"listitembase.h"
namespace Ui {
class GroupTipItem;
}

class GroupTipItem : public ListItemBase
{
    Q_OBJECT

public:
    explicit GroupTipItem(QWidget *parent = nullptr);
    ~GroupTipItem();
    void SetGroupTip(QString tip);
    QSize sizeHint() const override;
private:
    QString _tip;
    Ui::GroupTipItem *ui;
};

#endif // GROUPTIPITEM_H
