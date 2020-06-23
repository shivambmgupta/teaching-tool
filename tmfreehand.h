#ifndef TMFREEHAND_H
#define TMFREEHAND_H
#include "tmshape.h"
#include <QVector>
#include <QListWidget>
class TMFreeHand : public TMShape
{
public:
    TMFreeHand();

    void draw(QPainter *painter, QListWidget *dsList) override;
    bool hasPoint(QPoint point) override;
    bool isSelected() override;
    void setSelection(bool) override;
    void moveShapeBy(int dx, int dy) override;
    int getShapeCode() override;
    QJsonValue toJson() override;
    void fromJSON(QJsonObject) override;

    QVector<QPoint> getPoints() const;
    void setPoints(const QVector<QPoint> &value);
    void addPoint(QPoint);
    QPen getPen() const;
    void setPen(const QPen &value);

private:
    QVector<QPoint> points;
    QPen pen;
    bool select = false;
    int FREE_HAND = 0;
    double distance(QPoint a, QPoint b);
};


#endif // TMFREEHAND_H
