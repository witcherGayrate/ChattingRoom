#ifndef PICTUREBUBBLE_H
#define PICTUREBUBBLE_H

#include"bubbleframe.h"
class PictureBubble:public BubbleFrame
{
public:
    PictureBubble(const QPixmap &picture ,ChatRole role,QWidget* parent=nullptr);
};

#endif // PICTUREBUBBLE_H