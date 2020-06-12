#ifndef TMRECTANGLE_H
#define TMRECTANGLE_H
#include "tmshape.h"
#include <QListWidget>

class TMRectangle : public TMShape
{
public:
    TMRectangle();

    void draw(QPainter *painter, QListWidget *dsList) override;
    bool hasPoint(QPoint point) override;
    void setSelection(bool value) override;
    bool isSelected() override;
    void moveShapeBy(int dx, int dy) override;
    int getShapeCode() override;

    int getLeftUpperX() const;
    void setLeftUpperX(int value);

    int getLeftUpperY() const;
    void setLeftUpperY(int value);

    int getWidth() const;
    void setWidth(int value);

    int getHeight() const;
    void setHeight(int value);

    QPen getPen() const;
    void setPen(const QPen &value);

private:
    int leftUpperX;
    int leftUpperY;
    int width;
    int height;
    bool select = false;
    int RECTANGLE = 2;
    QPen pen;
    QListWidgetItem *item;
};

#endif // TMRECTANGLE_H
