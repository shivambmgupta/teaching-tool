#include "tmline.h"
#include <math.h>
#include <QListWidgetItem>
#include <QJsonObject>

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
    if(painter == nullptr) return;

    if(dsList != nullptr) {
        QListWidgetItem *item = new QListWidgetItem;
        item->setText("Line");
        item->setForeground(pen.color());
        dsList->addItem(item);
        if(select) dsList->setCurrentItem(item);
    }

    if(select) {
        pen.setStyle(Qt::DashDotDotLine);
        painter->setPen(pen);

        painter->drawRect(start.x() - 5, start.y() - 5, 10, 10);
        painter->drawRect(end.x() - 5, end.y() - 5, 10, 10);
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
    if(dist >= -6 && dist <= 6) {
        select = true;
        return true;
    }
    else return false;
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

QJsonValue TMLine::toJson()
{
    QJsonObject jsonObject;
    jsonObject["shapeCode"] = LINE;
    jsonObject["start"] = QJsonObject({
                                          {"x", start.x()},
                                          {"y", start.y()}
                                      });
    jsonObject["end"] = QJsonObject({
                                        {"x", end.x()},
                                        {"y", end.y()}
                                    });

    jsonObject["pen"] = QJsonObject({
                                        {"color", QString(pen.color().name())},
                                        {"width", pen.width()}
                                    });

    return jsonObject;
}

void TMLine::fromJSON(QJsonObject object)
{
  QJsonObject jsonObj = object["start"].toObject();

  QPoint start;
  start.setX(jsonObj["x"].toInt());
  start.setY(jsonObj["y"].toInt());

  jsonObj = object["end"].toObject();
  QPoint end;
  end.setX(jsonObj["x"].toInt());
  end.setY(jsonObj["y"].toInt());

  jsonObj = object["pen"].toObject();
  QPen pen;
  pen.setColor(jsonObj["color"].toString());
  pen.setWidth(jsonObj["width"].toInt());

  this->setStart(start);
  this->setEnd(end);
  this->setPen(pen);

}
