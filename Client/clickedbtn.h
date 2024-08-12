#ifndef CLICKEDBTN_H
#define CLICKEDBTN_H

#include <QPushButton>

class ClickedBtn : public QPushButton
{
    Q_OBJECT
public:
    ClickedBtn(QWidget* parent = nullptr);
    ~ClickedBtn();
    void SetState(QString normal,QString hover,QString press);

    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
    virtual void enterEvent(QEnterEvent *event) override;
private:
    QString _normal;
    QString _hover;
    QString _press;
};



#endif // CLICKEDBTN_H
