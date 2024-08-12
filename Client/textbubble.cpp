#include "textbubble.h"
#include<QEvent>
#include<QTextDocument>
#include<QTextBlock>
#include<QFontMetrics>
TextBubble::TextBubble(ChatRole role, const QString &text, QWidget *parent):BubbleFrame(role,parent)
{
    m_pTextEdit = new QTextEdit();
    m_pTextEdit->setReadOnly(true);
    m_pTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pTextEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QFont font("Microsoft YaHei");
    font.setPointSize(12);
    m_pTextEdit->setFont(font);
    m_pTextEdit->installEventFilter(this);//给textedit安装事件过滤器
    setPlainText(text);
    setWidget(m_pTextEdit);//在bubble区域替换为textbubble
    initStyleSheet();
}

void TextBubble::setPlainText(const QString &text)
{
    //设置文本最大宽度
    m_pTextEdit->setPlainText(text);//textedit自己的setPlainText方法，将text设为文本内容

    qreal doc_margin = m_pTextEdit->document()->documentMargin();//字体的边距
    int margin_left = this->layout()->contentsMargins().left();
    int margin_right = this->layout()->contentsMargins().right();
    QFontMetrics fm(m_pTextEdit->font());//字体的指标
    QTextDocument* doc = m_pTextEdit->document();
    int max_width=0;

    for(auto it = doc->begin();it!=doc->end();it=it.next())
    {
        int txtW = fm.horizontalAdvance(it.text()); //qt6弃用了witd方法
        max_width = max_width < txtW ? txtW : max_width;                 //找到最长的那段
    }

    //设置这个气泡的最大宽度，只需要设置一次
    setMaximumWidth(max_width+doc_margin*2+(margin_left+margin_right));
}

void TextBubble::initStyleSheet()
{
    m_pTextEdit->setStyleSheet("QTextEdit{background:transparent;border:none}");
}

void TextBubble::adjustTextHeight()
{
    qreal doc_margin = m_pTextEdit->document()->documentMargin();//字体到边框的距离
    QTextDocument *doc = m_pTextEdit->document();
    qreal text_height = 0;
    //把每一段的高度相加=文本高
    for(auto it = doc->begin();it!=doc->end();it=it.next())
    {
        QTextLayout* pLayout = it.layout();//每个block的布局
        QRectF text_rect = pLayout->boundingRect();//布局的边界
        text_height+=text_rect.height();
    }
    int vMargin = this->layout()->contentsMargins().top();//frame本身布局的边距
    //设置气泡的高度：文本高+文本边距+texteidt边框到气泡边框的距离
    setFixedHeight(text_height+doc_margin*2+vMargin*2);
}

bool TextBubble::eventFilter(QObject *watched, QEvent *event)
{
    if(watched==m_pTextEdit && event->type()==QEvent::Paint)
    {
        adjustTextHeight();//调整文本高度
    }
    return BubbleFrame::eventFilter(watched,event);
}
