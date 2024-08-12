#include "chatview.h"
#include<QScrollBar>
#include<QTimer>
#include<QHBoxLayout>
#include<QEvent>
#include<QStyleOption>
#include<QPainter>
ChatView::ChatView(QWidget* parent):QWidget(parent),_is_appended(false)
{
    QVBoxLayout *pMainLayout = new QVBoxLayout();
    this->setLayout(pMainLayout); //layout1
    //设置边距
    pMainLayout->setContentsMargins(0,0,0,0);//设置左上右下边距位0,0,0,0

    //添加scrollarea
    _m_ScrollArea = new QScrollArea();
    _m_ScrollArea->setObjectName("chat_area");
    pMainLayout->addWidget(_m_ScrollArea);//滚动区域添加到布局中

    QWidget* w = new QWidget(this);
    w->setAutoFillBackground(true);
    w->setObjectName("chat_bg");
    QVBoxLayout* sub_VLayout = new QVBoxLayout();
    sub_VLayout->addWidget(new QWidget(),100000);//伸缩因子
    w->setLayout(sub_VLayout);//将含有大小为100000的widget的布局添加到w
    _m_ScrollArea->setWidget(w);//给滚动布局设置widget

    //设置滚动条
    _m_ScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//不显示垂直滚动条
    QScrollBar *pVScrollBar = _m_ScrollArea->verticalScrollBar();
    connect(pVScrollBar,&QScrollBar::rangeChanged,this,&ChatView::onVScrollBarMoved);

    //把垂直滚动条放在scrollarea区域内右侧
    QHBoxLayout* scrol_HLayout = new QHBoxLayout();
    scrol_HLayout->addWidget(pVScrollBar,0,Qt::AlignRight);
    scrol_HLayout->setContentsMargins(0,0,0,0);
    _m_ScrollArea->setLayout(scrol_HLayout);
    pVScrollBar->setHidden(true);

    _m_ScrollArea->setWidgetResizable(true);//widget可调整大小
    _m_ScrollArea->installEventFilter(this);//给scrollarea安装事件过滤器，this是过滤器对象，scrollarea是被监听对象
    initStyleSheet();

}

void ChatView::appendChatItem(QWidget *item)
{
    QVBoxLayout* vl = qobject_cast<QVBoxLayout*>(_m_ScrollArea->widget()->layout());
    vl->insertWidget(vl->count()-1,item);
    _is_appended= true;//滚动条相应调整
}

void ChatView::prependChatItem(QWidget *item)
{

}

void ChatView::insertChatItem(QWidget *before, QWidget *item)
{

}

void ChatView::initStyleSheet()
{

}

bool ChatView::eventFilter(QObject *watched, QEvent *event)
{
    if(watched==_m_ScrollArea && event->type()==QEvent::Enter)
    {
        _m_ScrollArea->verticalScrollBar()->setHidden(_m_ScrollArea->verticalScrollBar()->maximum()==0);
    }
    else if(watched==_m_ScrollArea && event->type()==QEvent::Leave)
    {
        _m_ScrollArea->verticalScrollBar()->setHidden(true);
    }
    //继续交给基类处理
    return QWidget::eventFilter(watched,event);
}

void ChatView::paintEvent(QPaintEvent *event)
{
    QStyleOption opt ;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget,&opt,&p,this);
}

void ChatView::onVScrollBarMoved(int min, int max)
{
    if(_is_appended)
    {
        QScrollBar* pVScrollBar = _m_ScrollArea->verticalScrollBar();
        pVScrollBar->setSliderPosition(pVScrollBar->maximum());
        //限制快速下滑
        QTimer::singleShot(500,[this]()
                           {
            _is_appended=false;
        });
    }
}
