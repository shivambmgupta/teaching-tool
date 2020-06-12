#include "tmline.h"
#include <math.h>
#include <QListWidgetItem>

TMLine::TMLine()
{

}

QPoint TMLine::getStart() const
{
    return start;
}

void TMLine::setStart(const QPoint &value)
{
    start = value;
}

QPoint TMLine::getEnd() const
{
    return end;
}

void TMLine::setEnd(const QPoint &value)
{
    end = value;
}

QPen TMLine::getPen() const
{
    return pen;
}

void TMLine::setPen(const QPen &value)
{
    pen = value;
}

bool TMLine::isSelected()
{
    return select;
}

void TMLine::setSelection(bool value)
{
    select = value;
}
int TMLine::getShapeCode()
{
    return LINE;
}

void TMLine::draw(QPainter *painter, QListWidget *dsList)
{
    QListWidgetItem *item = new QListWidgetItem;
    item->setText("Line");
    item->setForeground(pen.color());

    dsList->addItem(item);

    if(select) {
        pen.setStyle(Qt::DashDotDotLine);
        painter->setPen(pen);

        painter->drawRect(start.x() - 5, start.y() - 5, 10, 10);
        painter->drawRect(end.x() - 5, end.y() - 5, 10, 10);

        dsList->setCurrentItem(item);
    }
    else {
        pen.setStyle(Qt::SolidLine);
        painter->setPen(pen);
    }
    painter->drawLine(start, end);
}
bool TMLine::hasPoint(QPoint point)
{
    if(distance(end, point) < 6) return false;
    if(distance(start, point) < 6) return false;
    double dist =  distance(end, start) - distance(point, start) - distance(end, point);
    if(dist >= -6 && dist <= 6) select = true;
    else select = false;
    return select;
}
double TMLine::distance(QPoint a, QPoint b)
{
    double dist = sqrt(pow((a.x() - b.x()), 2) + pow((a.y() - b.y()), 2));
    return dist;
}
void TMLine::moveShapeBy(int dx, int dy)
{
    QPoint tempStart;
    tempStart.setX(start.x() + dx);
    tempStart.setY(start.y() + dy);
    this->setStart(tempStart);
    QPoint tempEnd;
    tempEnd.setX(end.x() + dx);
    tempEnd.setY(end.y() + dy);
    this->setEnd(tempEnd);
}
