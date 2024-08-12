#ifndef CLICKEDLABEL_H
#define CLICKEDLABEL_H

#include <QLabel>
#include <QObject>
#include"global.h"
class ClickedLabel : public QLabel
{
    Q_OBJECT
public:
    ClickedLabel(QWidget *parent=nullptr);
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void enterEvent(QEnterEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
    void SetState(QString normal="",QString hover="",QString press="",
                  QString select="",QString select_hover="",QString select_press="");
    ClickLBState GetCurState();
    bool SetCurState(ClickLBState state);
    void ResetNormalState();
    // QWidget interface
private:
    QString _normal;
    QString _normal_hover;
    QString _normal_press;

    QString _selected;
    QString _selected_hover;
    QString _selected_press;

    ClickLBState _curstate;
signals:
    void clicked(QString,ClickLBState);

};





#endif // CLICKEDLABEL_H
