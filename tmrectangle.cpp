#include "tmrectangle.h"

TMRectangle::TMRectangle()
{

}

int TMRectangle::getLeftUpperX() const
{
    return leftUpperX;
}

void TMRectangle::setLeftUpperX(int value)
{
    leftUpperX = value;
}

int TMRectangle::getLeftUpperY() const
{
    return leftUpperY;
}

void TMRectangle::setLeftUpperY(int value)
{
    leftUpperY = value;
}

int TMRectangle::getWidth() const
{
    return width;
}

void TMRectangle::setWidth(int value)
{
    width = value;
}

int TMRectangle::getHeight() const
{
    return height;
}

void TMRectangle::setHeight(int value)
{
    height = value;
}

QPen TMRectangle::getPen() const
{
    return pen;
}

void TMRectangle::setSelection(bool value)
{
    select = value;
}
bool TMRectangle::isSelected()
{
    return select;
}
int TMRectangle::getShapeCode()
{
    return RECTANGLE;
}

void TMRectangle::setPen(const QPen &value)
{
    pen = value;
}
void TMRectangle::draw(QPainter *painter, QListWidget *dsList)
{
    QListWidgetItem *item= new QListWidgetItem;
    item->setText("Rectangle");
    item->setForeground(pen.color());
    dsList->addItem(item);

    if(select) {
        pen.setStyle(Qt::DashDotDotLine);
        painter->setPen(pen);
        painter->drawRect(leftUpperX - 5, leftUpperY - 5, 10, 10);
        painter->drawRect(leftUpperX - 5 + width, leftUpperY - 5 , 10, 10);
        painter->drawRect(leftUpperX - 5 + width/2, leftUpperY - 5 , 10, 10);
        painter->drawRect(leftUpperX - 5, leftUpperY + height/2 - 5 , 10, 10);
        painter->drawRect(leftUpperX - 5, leftUpperY + height - 5, 10, 10);
        painter->drawRect(leftUpperX - 5 + width, leftUpperY - 5 + height/2, 10, 10);
        painter->drawRect(leftUpperX - 5 + width, leftUpperY  - 5 + height, 10, 10);
        painter->drawRect(leftUpperX - 5 + width/2, leftUpperY - 5 + height, 10, 10);
        dsList->setCurrentItem(item);
    }
    else {
        pen.setStyle(Qt::SolidLine);
        painter->setPen(pen);
    }
    painter->drawRect(leftUpperX, leftUpperY, width, height);
}
bool TMRectangle::hasPoint(QPoint point)
{
    int x = point.x();
    int y = point.y();

    int leaveEndPointOffset = 3;
    int pThresholdDiff = 3;
    int nThresholdDiff = pThresholdDiff * -1;

    if(x >= leftUpperX + leaveEndPointOffset && x <= (leftUpperX + width - leaveEndPointOffset)
                          && (y - leftUpperY <= pThresholdDiff && y - leftUpperY >= nThresholdDiff))
        select = true;
    else if(x >= leftUpperX  + leaveEndPointOffset && x <= (leftUpperX + width- leaveEndPointOffset)
            && (y - leftUpperY - height <= pThresholdDiff && y - leftUpperY - height >= nThresholdDiff))
        select = true;
    else if(y >= leftUpperY + leaveEndPointOffset && y <= leftUpperY - leaveEndPointOffset + height
                                    && x - leftUpperX <= pThresholdDiff && x - leftUpperX >= nThresholdDiff)
        select = true;
    else if(y >= leftUpperY + leaveEndPointOffset && y <= leftUpperY - leaveEndPointOffset + height
            && x - leftUpperX - width <= pThresholdDiff && x - leftUpperX - width >= nThresholdDiff)
         select = true;
    else select = false;

    return select;
}
void TMRectangle::moveShapeBy(int dx, int dy)
{
    this->setLeftUpperX(leftUpperX + dx);
    this->setLeftUpperY(leftUpperY + dy);
}
