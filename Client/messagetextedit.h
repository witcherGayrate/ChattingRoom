#ifndef MESSAGETEXTEDIT_H
#define MESSAGETEXTEDIT_H
#include<QObject>
#include <QTextEdit>
#include<QVector>
#include"global.h"
#include<QDragEnterEvent>
#include<QFileInfo>
#include<QMimeData>
#include<QFileIconProvider>
#include<QPainter>


#include"chatitembase.h"
#include"picturebubble.h"
#include"textbubble.h"

class MessageTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit MessageTextEdit(QWidget* parent=nullptr);
    ~MessageTextEdit();

    QVector<MsgInfo> getMsgList();//获取消息列表

    void insertFileFromUrl(const QStringList &urls);//从url列表插入文件
signals:
    void send();

    // QWidget interface
protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void dragEnterEvent(QDragEnterEvent *event) override;
    virtual void dropEvent(QDropEvent *event) override;

private:
    void insertImages(const QString &url);
    void insertTextFile(const QString &url);

    // QTextEdit interface
    virtual bool canInsertFromMimeData(const QMimeData *source) const override;
    virtual void insertFromMimeData(const QMimeData *source) override;
private:
    bool isImage(QString url);//判断文件是否为图片
    void insertMsgList(QVector<MsgInfo> &list,QString flag, QString text, QPixmap pix);

    QStringList getUrl(QString text);
    QPixmap getFileIconPixmap(const QString &url);//获取文件图标及大小信息，并转化成图片
    QString getFileSize(qint64 size);//获取文件大小
private slots:
    void textEditChanged();
    //void slot_edit_sendmsg();//响应发送信号
private:
    //消息容器
    QVector<MsgInfo> mMsgList;
    QVector<MsgInfo> mGetMsgList;
signals:
    void sig_edit_send_packeditem(ChatItemBase*);//发送包装好的chatitem到chatpage
};


#endif // MESSAGETEXTEDIT_H
