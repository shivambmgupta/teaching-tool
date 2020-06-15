#ifndef TMGROUP_H
#define TMGROUP_H
#include <tmshape.h>

class TMGroup: public TMShape
{
public:
    TMGroup();

    void draw(QPainter *painter, QListWidget *dsList) override;
    void setSelection(bool) override;
    void moveShapeBy(int dx, int dy) override;
    bool isSelected() override;
    bool hasPoint(QPoint point) override;
    int getShapeCode() override;
    QJsonValue toJson() override;
    void fromJSON(QJsonObject) override;

    void addShape(TMShape*);
    QList<TMShape *> getShapes() const;
    void setShapes(const QList<TMShape *> &value);

private:
    QList<TMShape*> shapes;
    int GROUP = 4;
    bool select = false;


};

#endif // TMGROUP_H
