#include "tmshapeheader.h"

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

QJsonValue TMGroup::toJson()
{
    QJsonObject object;
    object["shapeCode"] = GROUP;
    int i = 1;
    for(TMShape* shape: shapes) {
        object.insert("shape" + QString::number(i), shape->toJson());
        ++i;
    }

    return object;
}

void TMGroup::fromJSON(QJsonObject object)
{
    QList<TMShape*> shapes;
    int shapeCode;
    QJsonObject obj;
    for(int i = 0; i < object.size() - 1; ++i) {
        obj = object["shape" + QString::number(i)].toObject();
        shapeCode = obj["shapeCode"].toInt();
        if(shapeCode == 0) {
            TMFreeHand* fh = new TMFreeHand;
            fh->fromJSON(obj);
            shapes.push_back(fh);
        }
        else if(shapeCode == 1) {
            TMLine* fh = new TMLine;
            fh->fromJSON(obj);
            shapes.push_back(fh);
        }
        else if(shapeCode == 2) {
            TMRectangle* fh = new TMRectangle;
            fh->fromJSON(obj);
            shapes.push_back(fh);
        }
        else if(shapeCode == 3) {
            TMCircle* fh = new TMCircle;
            fh->fromJSON(obj);
            shapes.push_back(fh);
        }
        else if(shapeCode == 4) {
            TMGroup* fh = new TMGroup;
            fh->fromJSON(obj);
            shapes.push_back(fh);
        }
    }

    this->setShapes(shapes);

}
