#include "tmgroup.h"

TMGroup::TMGroup()
{

}

QList<TMShape *> TMGroup::getShapes() const
{
    return shapes;
}

void TMGroup::setShapes(const QList<TMShape *> &value)
{
    shapes = value;
}

void TMGroup::addShape(TMShape *shape)
{
    if(shape != nullptr)
       this->shapes.push_back(shape);
}

int TMGroup::getShapeCode()
{
    return this->GROUP;
}

void TMGroup::setSelection(bool value)
{
    for(TMShape* shape: shapes) {
        shape->setSelection(value);
    }
}

bool TMGroup::isSelected()
{
    for(TMShape* shape : shapes)
        if(shape->isSelected())
            return true;
    return false;
}

bool TMGroup::hasPoint(QPoint point)
{
    for(TMShape* shape : shapes)
        if(shape->hasPoint(point))
            return true;
    return false;
}

void TMGroup::draw(QPainter *painter, QListWidget *dsList)
{
    if(painter == nullptr) return;

    bool select = isSelected();

    if(dsList != nullptr) {
        QListWidgetItem *item = new QListWidgetItem;
        item->setText("Group");
        dsList->addItem(item);
        if(select) dsList->setCurrentItem(item);
    }

    if(select) {
        for(TMShape* shape : shapes) {
            shape->setSelection(true);
        }
    }

    for(TMShape* shape : shapes)
        shape->draw(painter, nullptr);
}
void TMGroup::moveShapeBy(int dx, int dy)
{
    for(TMShape* shape : shapes)
        shape->moveShapeBy(dx, dy);
}
