#ifndef TMLINE_H
#define TMLINE_H
#include "tmshape.h"
#include <QListWidget>

class TMLine : public TMShape
{
public:
    TMLine();
    void draw(QPainter *painter, QListWidget* dsList) override;
    bool hasPoint(QPoint point) override;
    bool isSelected() override;
    void setSelection(bool value) override;
    void moveShapeBy(int dx, int dy) override;
    int getShapeCode() override;

    QPoint getStart() const;
    void setStart(const QPoint &value);

    QPoint getEnd() const;
    void setEnd(const QPoint &value);

    QPen getPen() const;
    void setPen(const QPen &value);



private:
    QPoint start;
    QPoint end;
    QPen pen;
    bool select = false;
    int LINE = 1;
    double distance(QPoint a, QPoint b);
};

#endif // TMLINE_H
