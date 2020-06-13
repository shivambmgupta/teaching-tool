#ifndef CANVAS_H
#define CANVAS_H

#include <QFrame>
#include <QPainter>
#include <QList>
#include <QListWidget>
#include "tmshapeheader.h"

namespace Ui {
class Canvas;
}

class Canvas : public QFrame
{
    Q_OBJECT


public:
    explicit Canvas(QWidget *parent = nullptr);
    ~Canvas();
    void paintEvent(QPaintEvent *event) override;
    QList<TMShape *> tmshapes;

    TMShape* getSelectedChild();
    TMShape* getSelectedChild(QPoint point);
    TMShape* getSelectedChildWD(QPoint point); //WD stands for
                                               //without diabling others.
    QListWidget *getDsFrame() const;
    void setDsFrame(QListWidget *value);
    void selectChild(int);
    bool hasSelectedChild();
    void getShapeDrawn(TMShape *);

private:
    Ui::Canvas *ui;
    QListWidget *dsFrame;
    TMShape *drawOnly;

};

#endif // CANVAS_H
