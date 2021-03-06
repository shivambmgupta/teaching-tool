#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "math.h"
#include <QMouseEvent>
#include <QDebug>
#include <QKeyEvent>
#include <QKeySequence>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    editor = new QPlainTextEdit;
    logFileName = "c:/temp/log.txt";
    logger = new Logger(this, logFileName, editor);

    styleElements();
    addToolTips();
    configureMenuActions();
    configureButtons();
    connectMenuActions();

    this->ui->frame->setDsFrame(this->ui->dsList);
    this->ui->dsList->setSelectionMode(QAbstractItemView::ExtendedSelection);

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


    this->confirmBox.setWindowTitle("Confirmation");
    this->confirmBox.setText("Are you sure to override?");
    this->confirmBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    this->confirmBox.setDefaultButton(QMessageBox::Yes);
    this->confirmBox.setWindowIcon(QPixmap(":icons/icons/appIcon.png"));

    logger->write("Elements are styled.");

}

void MainWindow::configureButtons()
{
    //Setting up icons
    this->setWindowIcon(QIcon(":icons/icons/appIcon.png"));
    this->setWindowTitle("TMPainter");
    this->ui->freeHandButton->setIcon(QIcon(":icons/icons/pencil.png"));
    this->ui->lineButton->setIcon(QIcon(":icons/icons/line.png"));
    this->ui->rectangleButton->setIcon(QIcon(":icons/icons/rectangle.png"));
    this->ui->circleButton->setIcon(QIcon(":icons/icons/circle.png"));
    this->ui->undoButton->setIcon(QIcon(":/icons/icons/undo.png"));
    this->ui->clearSelected->setIcon(QIcon(":/icons/icons/eraser.png"));
    this->ui->clear->setIcon(QIcon(":/icons/icons/searchCancelIcon.png"));


    //Disabling buttons
    this->ui->clearSelected->setEnabled(false); // Remove button initially set disabled.
    this->ui->undoButton->setEnabled(false);
    this->ui->clear->setEnabled(false);

    //Disabling Menu
    this->save->setEnabled(false);

    this->copy->setEnabled(false);
    this->cut->setEnabled(false);
    this->paste->setEnabled(true);
    this->bksp->setEnabled(false);
    this->ctrlBksp->setEnabled(false);
    this->group->setEnabled(false);
    this->ungroup->setEnabled(false);

    logger->write("Buttons are configured.");

}

void MainWindow::addToolTips()
{
    this->ui->lineButton->setToolTip("Line");
    this->ui->freeHandButton->setToolTip("Pen");
    this->ui->rectangleButton->setToolTip("Rectangle");
    this->ui->circleButton->setToolTip("Circle");
    this->ui->clear->setToolTip("Clear all");
    this->ui->clearSelected->setToolTip("Clear selected");
    this->ui->undoButton->setToolTip("undo");

    logger->write("Tool tips added.");
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    int x = event->x();
    int y = event->y() - menuOffset;
    int xu = ui->frame->geometry().left();
    int yu = ui->frame->geometry().top();

    if(!isPointInFrame(x, y)) {
        return;
    }
    this->clickedPoint.setX(x - xu);
    this->clickedPoint.setY(y - yu);

    TMShape *sShape;

    if(this->isCTRLPressed) {
        if(selectedShape != nullptr) {
            this->selectedShapes.push_back(selectedShape);
            this->selectedShapesEvent();
            this->selectedShape = nullptr;

            logger->write("A selected shape is added to selected shapes");
        }
        sShape = this->ui->frame->getSelectedChildWD(clickedPoint);
        if(sShape != nullptr) {
            this->selectedShapes.push_back(sShape);
            this->selectedShapesEvent();
            this->ui->clearSelected->setEnabled(true);
            this->ui->frame->update();

            logger->write("A shape is added selected shapes");

            return;
        }
    }
    logger->write("No multiple shapes are selected");

    this->removeSelectedShapesEvent();

    sShape = this->ui->frame->getSelectedChild(QPoint(x - xu, y - yu));

    if(sShape != nullptr) {

        logger->write("A shape is selcted");

        this->selectShapeEvent(sShape);
        this->ui->frame->update();
        return;
    }
    logger->write("Nothing is selected.");
    this->deselectShapeEvent();

    if(event->button() != Qt::LeftButton) return;

    QPen pen;

    QString penColor = this->ui->penColorCB->currentText();
    int penWidth = this->ui->penWidthCB->currentText().toInt();

    pen.setWidth(penWidth);
    pen.setColor(penColor);

    logger->write("Pen has been created");

    QPoint p(x - xu, y - yu);
    if(curr_mode == LINE_MODE) {
       line = new TMLine;
       line->setStart(p);
       line->setPen(pen);

       logger->write("Line has been created.");
    }

    else if(curr_mode == RECTANGLE_MODE) {
       rectangle = new TMRectangle;
       rectangle->setLeftUpperX(p.x());
       rectangle->setLeftUpperY(p.y());
       rectangle->setPen(pen);

       logger->write("Rectangle object is created.");
    }

    else if(curr_mode == FREE_HAND_MODE) {
       freehand = new TMFreeHand;
       freehand->addPoint(QPoint(p));
       freehand->setPen(pen);

       logger->write("Free hand drawing object is created.");
    }

    else if(curr_mode == CIRCLE_MODE) {
       circle = new TMCircle;
       circle->setCenter(p);
       circle->setPen(pen);

       logger->write("Circle object is created.");
    }

    this->hasDragged = false;

}
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{

    if(!(event->buttons() & Qt::LeftButton && isPointInFrame(event->x(), event->y())))
        return;

    QPoint p(event->x() - ui->frame->geometry().left(),
             event->y() - ui->frame->geometry().top() - menuOffset);

    if(selectedShape != nullptr) {
        selectedShape->moveShapeBy(p.x() - clickedPoint.x(), p.y() - clickedPoint.y());
        this->clickedPoint = p;
        this->ui->frame->update();
        return;
    }

    if(curr_mode == LINE_MODE) {
       line->setEnd(p);
       this->ui->frame->getShapeDrawn(line);
     }

    else if(curr_mode == RECTANGLE_MODE) {     

       int width = p.x() - rectangle->getLeftUpperX();
       if(width < 0) width *= -1;
       int height = p.y() - rectangle->getLeftUpperY();
       if(height < 0) height *= -1;

       rectangle->setWidth(width);
       rectangle->setHeight(height);
       this->ui->frame->getShapeDrawn(rectangle);
    }

    else if(curr_mode == FREE_HAND_MODE) {
       freehand->addPoint(QPoint(p));
       this->ui->frame->getShapeDrawn(freehand);
    }

    else if (curr_mode == CIRCLE_MODE) {
       float dist = sqrt(pow((circle->getCenter().x() - p.x()), 2)
                        + pow((circle->getCenter().y() - p.y()), 2));
       circle->setRadius(dist);
       this->ui->frame->getShapeDrawn(circle);
    }

    this->hasDragged = true;
    this->addShapeEvent();
}
void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(!(event->button() == Qt::LeftButton && isPointInFrame(event->x(), event->y())))
              return;

    QPoint p(event->x() - ui->frame->geometry().left(),
             event->y() - ui->frame->geometry().top() - menuOffset);

    if(selectedShape != nullptr) {
        selectedShape->moveShapeBy(p.x() - clickedPoint.x(), p.y() - clickedPoint.y());

        logger->write("Shape has been dragged.");

        this->ui->frame->update();
        this->clickedPoint = p;
        return;
    }

    if(curr_mode == LINE_MODE && hasDragged) {
        line->setEnd(p);
        this->addShapeEvent(line);              //shape add event

        logger->write("Line added to the DS");
    }

    else if(curr_mode == RECTANGLE_MODE && hasDragged) {
        int width = p.x() - rectangle->getLeftUpperX();
        if(width < 0) width *= -1;
        rectangle->setWidth(width);
        int height = p.y() - rectangle->getLeftUpperY();
        if(height < 0) height *= -1;
        rectangle->setHeight(height);

        this->addShapeEvent(rectangle);         //shape add event

        logger->write("Rectangle added to the DS.");
    }

    else if(curr_mode == FREE_HAND_MODE && hasDragged) {
        freehand->addPoint(QPoint(p));
        this->addShapeEvent(freehand);          //shape add event

        logger->write("Free hand drawing added to the DS.");
    }

    else if(curr_mode == CIRCLE_MODE && hasDragged) {
        float dist = sqrt(pow((circle->getCenter().x() - p.x()), 2)
                        + pow((circle->getCenter().y() - p.y()), 2));
        circle->setRadius(dist);
        this->addShapeEvent(circle);             //shape add event

        logger->write("circle is added to the DS.");
    }

    this->ui->frame->update();
    this->hasDragged = false;
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
    logger->write("MODE CHANGED TO LINE");
}

void MainWindow::on_freeHandButton_clicked()
{
    curr_mode = FREE_HAND_MODE;
    logger->write("MODE CHANGED TO THE FREE_HAND");
}

void MainWindow::on_rectangleButton_clicked()
{
    curr_mode = RECTANGLE_MODE;
    logger->write("MODE CHANGED TO RECTANGLE");
}

void MainWindow::on_circleButton_clicked()
{
    curr_mode = CIRCLE_MODE;
    logger->write("MODE CHANGED TO CIRCLE");
}


void MainWindow::on_clear_clicked()
{
    this->ui->frame->tmshapes.clear();

    /*
     * The following line may not seem technically right,
     * but it is logically right.
     * It sets selected shape to nullptr,
     * disables the all the buttons, because it
     * satisfies the if condition.
     */

    this->removeSelectedShapeEvent();

    this->ui->frame->update();

    logger->write("FRAME HAS BEEN CLEARED");
}

void MainWindow::on_clearSelected_clicked()
{
    if(!this->selectedShapes.isEmpty()) {
        this->removeSelectedShapesEvent();
        logger->write("Mutiple shapes are cleared.");
    }
    else {
        this->removeSelectedShapeEvent();
        logger->write("Single shape is cleared.");
    }
    this->ui->frame->update();

}

void MainWindow::on_undoButton_clicked()
{
    int shapeCount = this->ui->frame->tmshapes.size();
    if(shapeCount == 0) return;

    TMShape *toRemove = this->ui->frame->tmshapes[shapeCount - 1]; //The last shape added
    if(toRemove == selectedShape) {
        this->removeSelectedShapeEvent();
        logger->write("selected shaped removed by undoing");
    }
    else {
        this->ui->frame->tmshapes.pop_back();
        shapeRemovedEvent();
        logger->write("last added shape undone");
    }
    this->ui->frame->update();
}



void MainWindow::on_dsList_itemClicked(QListWidgetItem *item)
{
    int N = this->ui->frame->tmshapes.size() - 1;
    int rowToSelect = this->ui->dsList->row(item);
    rowToSelect =  N - rowToSelect;
    this->ui->frame->selectChild(rowToSelect);
    this->selectShapeEvent(ui->frame->tmshapes[rowToSelect]);
    this->ui->frame->update();

    logger->write("DS List item clicked");
}

void MainWindow::keyPressEvent(QKeyEvent *keyevent)
{
    if (keyevent->matches(QKeySequence::Copy)) {
        this->onCopyClick();
    }
    else if (keyevent->matches(QKeySequence::Cut)) {
        this->onCutClick();
    }
    if (keyevent->matches(QKeySequence::Paste)) {
       this->onPasteClick();
    }

    if(keyevent->key() == Qt::Key_Control) {
        this->isCTRLPressed = true;
        logger->write("Control key is pressed.");
    }

    if(this->isCTRLPressed && keyevent->key() == Qt::Key_G) {
        this->onGroupClick();
    }

    if(this->isCTRLPressed && keyevent->key() == Qt::Key_U){
        this->onUngroupClick();
    }

    if(this->isCTRLPressed && keyevent->key() == Qt::Key_S) {
        this->onSaveAsClick();
    }

    if(this->isCTRLPressed && keyevent->key() == Qt::Key_J) {
        this->onOpenClick();
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *keyevent)
{
    if(keyevent->key() == Qt::Key_Control) {
        this->isCTRLPressed = false;
        logger->write("CTRL key has been released.");
    }

    if(keyevent->key() == Qt::Key_Backspace) {
        if(isCTRLPressed) {
           onCtrlBackspaceClick();
        }
        else {
           onBackspaceClick();
        }
    }
}

void MainWindow::selectShapeEvent(TMShape *shape)
{
    if(shape == nullptr) return;

    this->selectedShape = shape;
    this->ui->clearSelected->setEnabled(true);

    this->ctrlBksp->setEnabled(true);
    this->group->setEnabled(false);

    if(shape->getShapeCode() == 4) {
        this->ungroup->setEnabled(true);
        this->copy->setEnabled(false);
        this->cut->setEnabled(false);
    }
    else {
        this->copy->setEnabled(true);
        this->cut->setEnabled(true);
        this->ungroup->setEnabled(false);
    }

}

void MainWindow::selectedShapesEvent()
{
    this->copy->setEnabled(false);
    this->cut->setEnabled(false);
    this->group->setEnabled(true);
    this->ungroup->setEnabled(false);
    this->ctrlBksp->setEnabled(true);
}


void MainWindow::deselectShapeEvent()
{
    this->selectedShape = nullptr;
    this->ui->clearSelected->setEnabled(false);

    this->ctrlBksp->setEnabled(false);
    this->copy->setEnabled(false);
    this->cut->setEnabled(false);
    this->group->setEnabled(false);
    this->ungroup->setEnabled(false);

}

void MainWindow::removeSelectedShapeEvent()
{
    if(this->selectedShape != nullptr)
        this->ui->frame->tmshapes.removeOne(selectedShape);

    this->selectedShape = nullptr;
    this->ui->clearSelected->setEnabled(false);

    this->ctrlBksp->setEnabled(false);
    this->copy->setEnabled(false);
    this->cut->setEnabled(false);
    this->group->setEnabled(false);
    this->ungroup->setEnabled(false);

    this->shapeRemovedEvent();
}

void MainWindow::removeSelectedShapesEvent()
{

    selectedShapes.clear();
    this->ui->clearSelected->setEnabled(false);

    this->ctrlBksp->setEnabled(false);
    this->copy->setEnabled(false);
    this->cut->setEnabled(false);
    this->group->setEnabled(false);
    this->ungroup->setEnabled(false);

    this->shapeRemovedEvent();

}

void MainWindow::addShapeEvent(TMShape *shape)
{
    if(shape != nullptr)
           this->ui->frame->tmshapes.push_back(shape);

    this->ui->undoButton->setEnabled(true);
    this->ui->clear->setEnabled(true);

    this->save->setEnabled(true);
    this->bksp->setEnabled(true);
    if(!selectedShapes.isEmpty()) this->ungroup->setEnabled(true);
    if(hasSelected()) {
        this->ctrlBksp->setEnabled(true);
    }
    if(selectedShape != nullptr) {
        this->cut->setEnabled(true);
        this->copy->setEnabled(true);
    }

}

void MainWindow::shapeRemovedEvent()
{
    if(this->ui->frame->tmshapes.isEmpty()) {
        this->ui->undoButton->setEnabled(false);
        this->ui->clear->setEnabled(false);

        this->save->setEnabled(false);
        this->bksp->setEnabled(false);
        this->group->setEnabled(false);
        this->ungroup->setEnabled(false);
        this->ctrlBksp->setEnabled(false);
        this->cut->setEnabled(false);
        this->copy->setEnabled(false);

    }

}

void MainWindow::configureMenuActions()
{
    open = this->ui->menuFile->actions().at(0);
    save = this->ui->menuFile->actions().at(1);

    copy = this->ui->menuEdit->actions().at(0);
    cut  = this->ui->menuEdit->actions().at(1);
    paste = this->ui->menuEdit->actions().at(2);

    bksp = this->ui->menuEdit->actions().at(4);
    ctrlBksp = this->ui->menuEdit->actions().at(5);

    group = this->ui->menuEdit->actions().at(7);
    ungroup = this->ui->menuEdit->actions().at(8);

}

void MainWindow::connectMenuActions()
{
    connect(open, SIGNAL(triggered()), this, SLOT(onOpenClick()));
    connect(save, SIGNAL(triggered()), this, SLOT(onSaveAsClick()));

    connect(copy, SIGNAL(triggered()), this, SLOT(onCopyClick()));
    connect(cut, SIGNAL(triggered()), this, SLOT(onCutClick()));
    connect(paste, SIGNAL(triggered()), this, SLOT(onPasteClick()));

    connect(bksp, SIGNAL(triggered()), this, SLOT(onBackspaceClick()));
    connect(ctrlBksp, SIGNAL(triggered()), this, SLOT(onCtrlBackspaceClick()));

    connect(group, SIGNAL(triggered()), this, SLOT(onGroupClick()));
    connect(ungroup, SIGNAL(triggered()), this, SLOT(onUngroupClick()));

}

void MainWindow::onCutClick()
{
    if(selectedShape != nullptr) {
        this->toPaste = selectedShape;
        this->removeSelectedShapeEvent();
        this->ui->frame->update();
        logger->write("single shape has been cut.");
    }
}

void MainWindow::onCopyClick()
{
    if(selectedShape != nullptr) {
        this->toPaste = selectedShape;
        logger->write("single shape copied.");
    }
}


void MainWindow::onOpenClick()
{
    QString fileName = QFileDialog::getOpenFileName(this,
           tr("Open Shapes"), "C:/",
           tr("Address Book (*.tmt);"));

    if(fileName != "" && !this->ui->frame->tmshapes.isEmpty()) {
        int status = confirmBox.exec();
        if(status != QMessageBox::Yes) return;
    }

    this->ui->frame->tmshapes.clear();
    for(TMShape* shape : this->ui->frame->loadFile(fileName)) {
        this->addShapeEvent(shape);         //shape add event
    }
    this->ui->frame->update();
    this->isCTRLPressed = false;
    logger->write("New DS loaded.");

}

void MainWindow::onSaveAsClick()
{
    if(this->ui->frame->tmshapes.isEmpty()) {
       logger->write("Nothing to save still ctrl + s pressed.");
    }
    else {

        QString fileName = QFileDialog::getSaveFileName(this,
                tr("Save Shapes"), "C:/",
                tr("Shapes (*.tmt);"));

         if(fileName == "") return;

         this->ui->frame->saveFile(fileName);
         this->isCTRLPressed = false;

         logger->write("DS Saved.");
    }

}

void MainWindow::onGroupClick()
{
    if(!this->selectedShapes.isEmpty()) {
        TMGroup* group = new TMGroup;
        for(TMShape* shape: selectedShapes) {
            group->addShape(shape);
            this->ui->frame->tmshapes.removeOne(shape);
        }
        this->selectedShapes.clear();
        this->addShapeEvent(group);
         this->ui->frame->update();
        logger->write("Multiple shape grouped.");
    }
}

void MainWindow::onUngroupClick()
{
    TMShape* shape = this->ui->frame->getSelectedChild();
    //4 is the shape code for the group.
    if(shape != nullptr && shape->getShapeCode() == 4) {
        TMGroup* group = (TMGroup*)shape;
        this->ui->frame->tmshapes.removeOne(shape);
        for(TMShape* s : group->getShapes()) {
            this->addShapeEvent(s);
        }
        group->getShapes().clear();
        this->ui->frame->update();
        this->isCTRLPressed = false;
        logger->write("Selected group shape ungrouped.");
    }

}

void MainWindow::onPasteClick()
{
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

           this->addShapeEvent(newShape);       //shape add event

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

            this->addShapeEvent(newLine);       //shape add event

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
            this->addShapeEvent(newShape);      //shape add event

        }else
        if(tmpShapeCode == 3) {
            TMCircle *oldCircle = (TMCircle*)toPaste;
            TMCircle *newCircle = new TMCircle;
            newCircle->setCenter(clickedPoint);
            newCircle->setRadius(oldCircle->getRadius());
            newCircle->setPen(oldCircle->getPen());
            newCircle->setSelection(false);
            this->addShapeEvent(newCircle);         //shape add event
        }

        this->ui->frame->update();
        logger->write("Single shape pasted.");
    }

}

void MainWindow::onCtrlBackspaceClick()
{
    this->on_clearSelected_clicked();
    logger->write("selected shape(s) cleared by pressing ctrl + bkspc");
    this->isCTRLPressed = false;
}

void MainWindow::onBackspaceClick()
{
    this->on_undoButton_clicked();
    logger->write("recent shape cleared by bkspc");
}

bool MainWindow::hasGroup()
{
    for(TMShape* shape : this->ui->frame->tmshapes)
    {
        if(shape->getShapeCode() == 4) return true;
    }
    return false;
}

bool MainWindow::hasSelected()
{
    if(selectedShape == nullptr && selectedShapes.isEmpty()) return  false;
    return true;
}



