#include "canvas.h"
#include "ui_canvas.h"
#include <QDebug>
#include <QPaintEvent>

Canvas::Canvas(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::Canvas)
{
    ui->setupUi(this);
    this->drawOnly = nullptr;
}

Canvas::~Canvas()
{
    delete ui;
}
void Canvas::paintEvent(QPaintEvent *event)
{
  QFrame::paintEvent(event);
  QPainter painter(this);

  dsFrame->clear();

  if(drawOnly != nullptr) {
    this->drawOnly->draw(&painter, dsFrame);
    this->drawOnly = nullptr;
  }

  for (int i = tmshapes.size() - 1; i >= 0; --i) {
     tmshapes[i]->draw(&painter, dsFrame);
  }

}
TMShape* Canvas::getSelectedChild(QPoint point)
{
  TMShape* shape = nullptr;
  bool flag = true;
  for(int i = tmshapes.size() - 1; i >= 0; --i)
  {
    if(tmshapes[i]->hasPoint(point) && flag)
    {
        shape = tmshapes[i];
        flag = false;
    }
    else
    {
       tmshapes[i]->setSelection(false);
    }
  }
  return shape;
}

TMShape* Canvas::getSelectedChildWD(QPoint point)
{
    for(int i = tmshapes.size() - 1; i >= 0; --i)
    {
      if(tmshapes[i]->hasPoint(point))
      {
         return tmshapes[i];
      }
    }
    return nullptr;
}

TMShape* Canvas::getSelectedChild()
{
    for(TMShape* shape : tmshapes)
        if(shape->isSelected())
            return shape;
    return nullptr;
}

QListWidget* Canvas::getDsFrame() const
{
    return dsFrame;
}

void Canvas::setDsFrame(QListWidget *value)
{
    dsFrame = value;
}
void Canvas::selectChild(int row)
{
    for(int i = 0; i < tmshapes.size(); ++i) {
        if(i == row)
            tmshapes[i]->setSelection(true);
        else
            tmshapes[i]->setSelection(false);
    }
}
bool Canvas::hasSelectedChild()
{
    for(TMShape *shape : tmshapes)
        if(shape->isSelected()) return true;
    return false;
}

void Canvas::getShapeDrawn(TMShape *shape)
{
    this->drawOnly = shape;
    this->update();
}


void Canvas::saveFile(QString fileName)
{
    QFile toSave(fileName);
    if(!toSave.open(QFile::WriteOnly)){
        qDebug() << "Error opening file" << '\n';
        return;
    }

    if(!toSave.exists()){
       return;
    }

    QJsonArray jsonArray;
    for(TMShape* shape: tmshapes) {
        jsonArray.append(shape->toJson());
    }

    QJsonDocument doc(jsonArray);
    toSave.write(doc.toJson());

}

QList<TMShape*> Canvas::loadFile(QString fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return QList<TMShape*>();

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());

    if(!doc.isArray()) {
      qDebug() << "Wrong file" << '\n';
      return QList<TMShape*>();
    }

    QJsonArray jsonArray = doc.array();

    QList<TMShape*> shapes;
    QJsonObject obj;
    int shapeCode;
    for(const auto& value : jsonArray) {
        obj = value.toObject();
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

    return shapes;


}















