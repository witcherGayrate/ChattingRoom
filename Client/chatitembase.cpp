#include "chatitembase.h"
#include<QGridLayout>
ChatItemBase::ChatItemBase(ChatRole role,QWidget *parent)
    : QWidget{parent}
{
    QGridLayout* gridLayout = new QGridLayout(this);
    m_pNameLab = new QLabel(this);
    m_pBubble = new QWidget(this);
    m_pUserIconLab = new QLabel(this);

    m_pNameLab->setObjectName("chat_user_name");
    m_pBubble->setObjectName("chat_user_bubble");
    m_pUserIconLab->setObjectName("chat_user_icon");

    QFont font("Microsoft YaHei");
    font.setPointSize(9);
    m_pNameLab->setFont(font);
    m_pNameLab->setFixedHeight(20);//设为固定高度

    m_pUserIconLab->setScaledContents(true);//图片可伸缩
    m_pUserIconLab->setFixedSize(42,42);

    gridLayout->setVerticalSpacing(3);
    gridLayout->setHorizontalSpacing(3);
    gridLayout->setContentsMargins(3,3,3,3);//设置网格布局的水平垂直间隙和边距

    QSpacerItem* pSpacer = new QSpacerItem(40,20,QSizePolicy::Expanding,QSizePolicy::Minimum);

    //根据chatrole设置不同的布局
    if(role == ChatRole::Self)
    {
        m_pNameLab->setContentsMargins(0,0,8,0);
        m_pNameLab->setAlignment(Qt::AlignRight);
        gridLayout->addWidget(m_pNameLab,0,1,1,1);//第0行第1列，空间大小一行一列
        gridLayout->addWidget(m_pUserIconLab,0,2,2,1,Qt::AlignTop);//位置：0行2列，大小2行1列，顶部对齐
        gridLayout->addWidget(m_pBubble,1,1,1,1);//bubble
        gridLayout->addItem(pSpacer,1,0,1,1);//spacer
        gridLayout->setColumnStretch(0,2);//分别设置第0列的伸缩因子
        gridLayout->setColumnStretch(1,3);
    }else
    {
        m_pNameLab->setContentsMargins(8,0,0,0);
        m_pNameLab->setAlignment(Qt::AlignLeft);
        gridLayout->addWidget(m_pNameLab,0,1,1,1);
        gridLayout->addWidget(m_pUserIconLab,0,0,2,1,Qt::AlignTop);
        gridLayout->addWidget(m_pBubble,1,1,1,1);
        gridLayout->addItem(pSpacer,1,2,1,1);//设置bubble的位置和大小（2，2，1，1）
        gridLayout->setColumnStretch(1,3);//分别设置第0列的伸缩因子
        gridLayout->setColumnStretch(2,2);
    }

    this->setLayout(gridLayout);

}

void ChatItemBase::setUserIcon(const QPixmap pixMap)
{
    m_pUserIconLab->setPixmap(pixMap);
}

void ChatItemBase::setUserName(const QString name)
{
    m_pNameLab->setText(name);
}

void ChatItemBase::setWidget(QWidget *w)
{
    //在布局里重新替换bubble
    QGridLayout* pGLayout = qobject_cast<QGridLayout*>(this->layout());
    pGLayout->replaceWidget(m_pBubble,w);//替换旧的
    delete m_pBubble;
    m_pBubble = w;
}
