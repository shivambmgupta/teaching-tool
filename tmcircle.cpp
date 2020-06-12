#include "tmcircle.h"
#include "math.h"

TMCircle::TMCircle()
{
    select = false;
}

QPoint TMCircle::getCenter() const
{
    return center;
}

void TMCircle::setCenter(const QPoint &value)
{
    center = value;
}

float TMCircle::getRadius() const
{
    return radius;
}

void TMCircle::setRadius(float value)
{
    radius = value;
}

QPen TMCircle::getPen() const
{
    return pen;
}

void TMCircle::setPen(const QPen &value)
{
    pen = value;
}

bool TMCircle::isSelected()
{
    return this->select;
}
void TMCircle::setSelection(bool value)
{
    this->select = value;
}
int TMCircle::getShapeCode()
{
    return CIRCLE;
}
void TMCircle::draw(QPainter *painter, QListWidget *dsList)
{
    QListWidgetItem *item;
    item = new QListWidgetItem;
    item->setText("Circle");
    item->setForeground(pen.color());

    dsList->addItem(item);

    if(select) {
        pen.setStyle(Qt::DashDotDotLine);
        painter->setPen(pen);
        int x = center.x();
        int y = center.y();
        painter->drawRect(x - 5, y - 5 + radius, 10, 10);
        painter->drawRect(x - radius - 5, y - 5, 10, 10);
        painter->drawRect(x + radius - 5, y - 5, 10, 10);
        painter->drawRect(x - 5, y - 5 - radius, 10, 10);
        dsList->setCurrentItem(item);
    }
    else{
        pen.setStyle(Qt::SolidLine);
        painter->setPen(pen);
    }
    painter->drawEllipse(center, (int)radius, (int)radius);
}
bool TMCircle::hasPoint(QPoint point)
{

    int dist = sqrt(pow((center.x() - point.x()), 2) + pow((center.y() - point.y()), 2));
    int minDist = dist - 4;
    int maxDist = dist + 4;
    if(radius >= minDist && radius <= maxDist)
        select = true;
    else select = false;

    return  select;
}
void TMCircle::moveShapeBy(int dx, int dy)
{
    QPoint tempP;
    tempP.setX(center.x() + dx);
    tempP.setY(center.y() + dy);
    this->setCenter(tempP);
}

