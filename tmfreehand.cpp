#include "tmfreehand.h"
#include "math.h"
#include "QListWidgetItem"

TMFreeHand::TMFreeHand()
{

}

QVector<QPoint> TMFreeHand::getPoints() const
{
    return points;
}

void TMFreeHand::setPoints(const QVector<QPoint> &value)
{
    points = value;
}

QPen TMFreeHand::getPen() const
{
    return pen;
}
bool TMFreeHand::isSelected()
{
    return select;
}
void TMFreeHand::setSelection(bool value)
{
    select = value;
}
void TMFreeHand::setPen(const QPen &value)
{
    pen = value;
}
int TMFreeHand::getShapeCode()
{
    return FREE_HAND;
}
void TMFreeHand::draw(QPainter *painter, QListWidget *dsList)
{
    if(painter == nullptr) return;

    if(dsList != nullptr) {
        QListWidgetItem *item = new QListWidgetItem;
        item->setText("Free Hand");
        item->setForeground(pen.color());
        dsList->addItem(item);
        if(select) dsList->setCurrentItem(item);
    }

    int l = points.size() - 1;

    if(select){
        pen.setStyle(Qt::DashDotDotLine);
        painter->setPen(pen);
        int gap;
        gap = l/20 > 5 ? l/20 : 5;
        for(int i = 0; i < l; ++i) {
            painter->drawLine(points[i], points[i + 1]);
            if(i%gap == 0) painter->drawRect(points[i].x() -5, points[i].y() -5, 10, 10);
        }
    }
    else {
        pen.setStyle(Qt::SolidLine);
        painter->setPen(pen);
        for(int i = 0; i < l; ++i)
         painter->drawLine(points[i], points[i + 1]);
    }
}
bool TMFreeHand::hasPoint(QPoint point)
{
    int l = points.size() - 1;
    int pThresholdDiff = 6;
    int nThresholdDiff = pThresholdDiff * -1;
    for(int i = 2; i < l - 5; ++i) {
     double dist =  distance(points[i + 1], points[i]) - distance(point, points[i]) - distance(points[i + 1], point);
     if(dist >= nThresholdDiff && dist <= pThresholdDiff) {
         select = true;
         return true;
     }
    }
    return false;
}
void TMFreeHand::addPoint(QPoint point)
{
    points.push_back(point);
}
double TMFreeHand::distance(QPoint a, QPoint b)
{
    double dist = sqrt(pow((a.x() - b.x()), 2) + pow((a.y() - b.y()), 2));
    return dist;
}
void TMFreeHand::moveShapeBy(int dx, int dy)
{
    for(QPoint& point : points) {
        point.setX(point.x() + dx);
        point.setY(point.y() + dy);
    }
}
