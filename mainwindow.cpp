#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "math.h"
#include <QMouseEvent>
#include <QDebug>
#include <QKeyEvent>
#include <QKeySequence>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    styleElements();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::styleElements()
{
    QString mainFrameStyleSheet = "background-color: rgb(253, 235, 142);";
    QString frameStyleSheet     = "background-color: rgb(255, 255, 255); border: 2px solid black";
    QString dsFrameStyleSheet   = "border: 2px solid black; background-color: rgb(178, 186, 187);";

    this->setStyleSheet(mainFrameStyleSheet);
    this->ui->frame->setStyleSheet(frameStyleSheet);
    this->ui->dsFrame->setStyleSheet(dsFrameStyleSheet);

    this->ui->frame->setDsFrame(this->ui->dsList);

    this->setWindowIcon(QIcon(":icons/icons/appIcon.png"));
    this->setWindowTitle("TMPainter");
    this->ui->freeHandButton->setIcon(QIcon(":icons/icons/pencil.png"));
    this->ui->lineButton->setIcon(QIcon(":icons/icons/line.png"));
    this->ui->rectangleButton->setIcon(QIcon(":icons/icons/rectangle.png"));
    this->ui->circleButton->setIcon(QIcon(":icons/icons/circle.png"));
    this->ui->undoButton->setIcon(QIcon(":/icons/icons/undo.png"));
    this->ui->clearSelected->setIcon(QIcon(":/icons/icons/eraser.png"));
    this->ui->clear->setIcon(QIcon(":/icons/icons/searchCancelIcon.png"));

    this->ui->clearSelected->setEnabled(false); // Remove button initially set disabled.
    this->ui->undoButton->setEnabled(false);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    int x = event->x();
    int y = event->y();
    int xu = ui->frame->geometry().left();
    int yu = ui->frame->geometry().top();

    if(!isPointInFrame(x, y)) return;

    this->clickedPoint.setX(x - xu);
    this->clickedPoint.setY(y - yu);

    TMShape *sShape = this->ui->frame->getSelectedChild(QPoint(x - xu, y - yu));

    if(sShape != nullptr) {
        this->isPressedToSelect = true;
        this->selectedShape = sShape;
        this->ui->clearSelected->setEnabled(true);
        this->ui->frame->update();
     }
    else {
          this->ui->clearSelected->setEnabled(false);
          isPressedToSelect = false;
          selectedShape = nullptr;
    }

    if(event->button() != Qt::LeftButton) return;

    QPen pen;

    QString penColor = this->ui->penColorCB->currentText();
    int penWidth = this->ui->penWidthCB->currentText().toInt();

    pen.setWidth(penWidth);
    pen.setColor(penColor);

    QPoint p(x - xu, y - yu);
    if(curr_mode == LINE_MODE) {
       line = new TMLine;
       line->setStart(p);
       line->setPen(pen);
       pressed = true;
       return;
    }
    if(curr_mode == RECTANGLE_MODE) {
       rectangle = new TMRectangle;
       rectangle->setLeftUpperX(p.x());
       rectangle->setLeftUpperY(p.y());
       rectangle->setPen(pen);
       pressed = true;
       return;
    }
    if(curr_mode == FREE_HAND_MODE) {
       freehand = new TMFreeHand;
       freehand->addPoint(QPoint(p));
       freehand->setPen(pen);
       pressed = true;
       return;
     }
     if(curr_mode == CIRCLE_MODE) {
         circle = new TMCircle;
         circle->setCenter(p);
         circle->setPen(pen);
         pressed = true;
         return;
     }
}
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(!(event->buttons() & Qt::LeftButton && pressed && isPointInFrame(event->x(), event->y())))
        return;

    QPoint p(event->x() - ui->frame->geometry().left(),
             event->y() - ui->frame->geometry().top() );

    if(selectedShape != nullptr) {
        selectedShape->moveShapeBy(p.x() - clickedPoint.x(), p.y() - clickedPoint.y());
        this->ui->frame->update();
        this->clickedPoint = p;
        return;
    }


    if(curr_mode == LINE_MODE) {
       if(!(this->ui->frame->tmshapes.contains(line)))
                this->ui->frame->tmshapes.push_back(line);
        line->setEnd(p);
    }

    else if(curr_mode == RECTANGLE_MODE) {
     if(!(this->ui->frame->tmshapes.contains(rectangle)))
           this->ui->frame->tmshapes.push_back(rectangle);

        int width = p.x() - rectangle->getLeftUpperX();
        if(width < 0) width *= -1;
        int height = p.y() - rectangle->getLeftUpperY();
        if(height < 0) height *= -1;

        rectangle->setWidth(width);
        rectangle->setHeight(height);
    }

    else if(curr_mode == FREE_HAND_MODE) {
     if(!(this->ui->frame->tmshapes.contains(freehand)))
           this->ui->frame->tmshapes.push_back(freehand);
        freehand->addPoint(QPoint(p));
    }

    else if (curr_mode == CIRCLE_MODE) {
        if(!(this->ui->frame->tmshapes.contains(circle)))
            this->ui->frame->tmshapes.push_back(circle);
        float dist = sqrt(pow((circle->getCenter().x() - p.x()), 2)
                        + pow((circle->getCenter().y() - p.y()), 2));
        circle->setRadius(dist);
    }

    if(this->ui->frame->tmshapes.size() != 0) this->ui->undoButton->setEnabled(true);
    this->ui->frame->update();
}
void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(!(event->button() == Qt::LeftButton && pressed && isPointInFrame(event->x(), event->y())))
              return;

    QPoint p(event->x() - ui->frame->geometry().left(),
             event->y() - ui->frame->geometry().top() );

    if(selectedShape != nullptr) {
        selectedShape->moveShapeBy(p.x() - clickedPoint.x(), p.y() - clickedPoint.y());
        this->ui->frame->update();
        this->clickedPoint = p;
        return;
    }


    if(curr_mode == LINE_MODE)
        line->setEnd(p);

    else if(curr_mode == RECTANGLE_MODE) {
        int width = p.x() - rectangle->getLeftUpperX();
        if(width < 0) width *= -1;
        rectangle->setWidth(width);
        int height = p.y() - rectangle->getLeftUpperY();
        if(height < 0) height *= -1;
        rectangle->setHeight(height);
    }
    else if(curr_mode == FREE_HAND_MODE)
         freehand->addPoint(QPoint(p));

    else if(curr_mode == CIRCLE_MODE) {
        float dist = sqrt(pow((circle->getCenter().x() - p.x()), 2)
                        + pow((circle->getCenter().y() - p.y()), 2));
        circle->setRadius(dist);
    }
    this->ui->frame->update();
    pressed = false;
}

void MainWindow::on_clear_clicked()
{
    this->ui->frame->tmshapes.clear();
    ui->frame->tmshapes.removeOne(selectedShape);
    selectedShape = nullptr;
    isPressedToSelect = false;
    ui->clearSelected->setEnabled(false);
    this->ui->undoButton->setEnabled(false);
    if(this->ui->frame->tmshapes.size() == 0) this->ui->undoButton->setEnabled(false);
    this->ui->frame->update();
}
bool MainWindow::isPointInFrame(QPoint point)
{
    int x =  point.x();
    int y =  point.y();
    int xu = ui->frame->geometry().left();
    int yu = ui->frame->geometry().top();
    int xv = ui->frame->geometry().right();
    int yv = ui->frame->geometry().bottom();
    return x >= xu && x <= xv && y >= yu && y <= yv;
}
bool MainWindow::isPointInFrame(int x, int y)
{
    return isPointInFrame(QPoint(x, y));
}
void MainWindow::on_lineButton_clicked()
{
    curr_mode = LINE_MODE;
}

void MainWindow::on_freeHandButton_clicked()
{
    curr_mode = FREE_HAND_MODE;
}

void MainWindow::on_rectangleButton_clicked()
{
    curr_mode = RECTANGLE_MODE;
}

void MainWindow::on_circleButton_clicked()
{
    curr_mode = CIRCLE_MODE;
}

void MainWindow::on_clearSelected_clicked()
{
    ui->frame->tmshapes.removeOne(selectedShape);
    ui->frame->update();
    selectedShape = nullptr;
    isPressedToSelect = false;
    ui->clearSelected->setEnabled(false);
}

void MainWindow::on_dsList_itemClicked(QListWidgetItem *item)
{
    int N = this->ui->frame->tmshapes.size() - 1;
    int rowToSelect = this->ui->dsList->row(item);
    rowToSelect =  N - rowToSelect;
    this->ui->frame->selectChild(rowToSelect);
    selectedShape = ui->frame->tmshapes[rowToSelect];
    isPressedToSelect = true;
    this->ui->clearSelected->setEnabled(true);
    this->ui->frame->update();
}

void MainWindow::on_undoButton_clicked()
{
    if(this->ui->frame->tmshapes.size() == 0) return;
    this->ui->frame->tmshapes.pop_back();
    this->ui->frame->update();
    if(this->ui->frame->tmshapes.size() == 0) this->ui->undoButton->setEnabled(false);
}
void MainWindow::keyPressEvent(QKeyEvent *keyevent)
{
    if (keyevent->matches(QKeySequence::Copy)) {
         //qDebug() << "Ctrl + c have been pressed";
        if(selectedShape != nullptr)
          this->toPaste = selectedShape;
    }
    else if (keyevent->matches(QKeySequence::Cut)) {
        if(selectedShape != nullptr) {
         this->toPaste = selectedShape;
         this->ui->frame->tmshapes.removeOne(selectedShape);
        }
    }
    if (keyevent->matches(QKeySequence::Paste)) {
        if(this->toPaste != nullptr) {
            int tmpShapeCode = toPaste->getShapeCode();
            if(tmpShapeCode == 0) {
               TMFreeHand* oldShape = (TMFreeHand*)toPaste;
               TMFreeHand* newShape = new TMFreeHand;

               QVector<QPoint> oldPoints = oldShape->getPoints();
               int hDiff = clickedPoint.x() - (oldPoints[0].x() + oldPoints[oldPoints.size() - 1].x())/2;
               int vDiff = clickedPoint.y() - (oldPoints[0].y() + oldPoints[oldPoints.size() - 1].y())/2;

               for(QPoint point: oldPoints) {
                 newShape->addPoint(QPoint(point.x() + hDiff, point.y() + vDiff));
               }

               newShape->setPen(oldShape->getPen());
               newShape->setSelection(false);

               this->ui->frame->tmshapes.push_back(newShape);

            }else
            if(tmpShapeCode == 1) {
                TMLine *oldLine = (TMLine*)toPaste;
                TMLine *newLine = new TMLine;
                newLine->setPen(oldLine->getPen());
                newLine->setSelection(false);

                int oldLineStartX = oldLine->getStart().x();
                int oldLineStartY = oldLine->getStart().y();
                int oldLineEndX = oldLine->getEnd().x();
                int oldLineEndY = oldLine->getEnd().y();
                int midPointX = (oldLineStartX + oldLineEndX)/2;
                int midPointY =(oldLineStartY + oldLineEndY)/2;
                int horizontalDiff = clickedPoint.x() - midPointX;
                int verticalDiff = clickedPoint.y() - midPointY;

                newLine->setStart(QPoint(oldLineStartX + horizontalDiff, oldLineStartY + verticalDiff));
                newLine->setEnd(QPoint(oldLineEndX + horizontalDiff, oldLineEndY + verticalDiff));

                this->ui->frame->tmshapes.push_back(newLine);

            }else
            if(tmpShapeCode == 2) {
                TMRectangle *oldShape = (TMRectangle*)toPaste;
                TMRectangle *newShape =new TMRectangle;
                newShape->setPen(oldShape->getPen());
                int width = oldShape->getWidth();
                newShape->setWidth(width);
                int height = oldShape->getHeight();
                newShape->setHeight(height);
                newShape->setSelection(false);
                newShape->setLeftUpperX(clickedPoint.x() - width/2);
                newShape->setLeftUpperY(clickedPoint.y() - height/2);
                this->ui->frame->tmshapes.push_back(newShape);
            }else
            if(tmpShapeCode == 3) {
                TMCircle *oldCircle = (TMCircle*)toPaste;
                TMCircle *newCircle = new TMCircle;
                newCircle->setCenter(clickedPoint);
                newCircle->setRadius(oldCircle->getRadius());
                newCircle->setPen(oldCircle->getPen());
                newCircle->setSelection(false);
                this->ui->frame->tmshapes.push_back(newCircle);
            }
        }

    }
    this->ui->frame->update();
}
