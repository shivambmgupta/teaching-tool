#ifndef TMCIRCLE_H
#define TMCIRCLE_H
#include "tmshape.h"

class TMCircle : public TMShape
{
public:

    TMCircle();

    void setSelection(bool) override;
    void draw(QPainter *painter, QListWidget *dsList) override;
    bool isSelected() override;
    bool hasPoint(QPoint point) override;
    void moveShapeBy(int dx, int dy) override;
    int getShapeCode() override;

    QJsonValue toJson() override;;
    void fromJSON(QJsonObject) override;

    QPoint getCenter() const;
    void setCenter(const QPoint &value);

    float getRadius() const;
    void setRadius(float value);

    QPen getPen() const;
    void setPen(const QPen &value);

private:

    QPoint center;
    float radius;
    QPen pen;
    bool select;
    int CIRCLE = 3;
    double distanceFromCenter(QPoint point);
};

#endif // TMCIRCLE_H
