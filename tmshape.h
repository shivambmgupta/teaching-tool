#ifndef TMSHAPE_H
#define TMSHAPE_H
#include <QPainter>
#include <QListWidget>

class TMShape
{
public:
    TMShape();
    virtual void draw(QPainter *painter, QListWidget *dsList) = 0;
    virtual bool hasPoint(QPoint point) = 0;
    virtual bool isSelected() = 0;
    virtual void setSelection(bool) = 0;
    virtual void moveShapeBy(int dx, int dy) = 0;
    virtual int getShapeCode() = 0;
};

#endif // TMSHAPE_H
