#ifndef FINDSUCCESSDLG_H
#define FINDSUCCESSDLG_H

#include <QDialog>
#include<memory>
#include"userdata.h"
namespace Ui {
class FindSuccessDlg;
}

class FindSuccessDlg : public QDialog
{
    Q_OBJECT

public:
    explicit FindSuccessDlg(QWidget *parent = nullptr);
    ~FindSuccessDlg();
    void SetSearchInfo(std::shared_ptr<SearchInfo> si);

private:
    std::shared_ptr<SearchInfo> _si;
    QWidget *_parent;//传数据给父窗口
    Ui::FindSuccessDlg *ui;
private slots:
    void on_add_friend_btn_clicked();
};



#endif // FINDSUCCESSDLG_H
