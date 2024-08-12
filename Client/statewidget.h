#ifndef STATEWIDGET_H
#define STATEWIDGET_H
#include"global.h"
#include <QLabel>
#include <QWidget>

class StateWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StateWidget(QWidget *parent = nullptr);

    void SetState(QString normal="",QString hover="",QString press="",
                  QString select="",QString select_hover="",QString select_press="");

    ClickLBState GetCurState();
    void ClearState();

    void SetSelected(bool bselected);
    void AddRedPoint();
    void ShowRedPoint(bool show=true);

signals:
    void clicked(void);
    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void enterEvent(QEnterEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;
private:
    QString _normal;
    QString _normal_hover;
    QString _normal_press;
    QString _selected;
    QString _selected_hover;
    QString _selected_press;

    ClickLBState _curstate;
    QLabel * _red_point;
};



#endif // STATEWIDGET_H
