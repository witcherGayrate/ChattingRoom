#ifndef CUSTOMLINEEDIT_H
#define CUSTOMLINEEDIT_H

#include <QLineEdit>

class CustomLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    CustomLineEdit(QWidget* parent=nullptr);
    void SetMaxLength(int maxLen);
    // QWidget interface
protected:
    virtual void focusOutEvent(QFocusEvent *event) override;
private:
    void limitTextLength(QString text);
    int _max_len;
signals:
    void sig_focus_out();
};



#endif // CUSTOMLINEEDIT_H
