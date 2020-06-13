#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tmshapeheader.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *keyevent);

    void selectShapeEvent(TMShape *shape);
    void deselectShapeEvent();
    void removeSelectedShapeEvent();
    void addShapeEvent(TMShape *shape = nullptr);
    void shapeRemovedEvent();

private slots:
    void on_clear_clicked();
    void on_lineButton_clicked();
    void on_freeHandButton_clicked();
    void on_rectangleButton_clicked();
    void on_circleButton_clicked();
    void on_clearSelected_clicked();
    void on_dsList_itemClicked(QListWidgetItem *item);
    void on_undoButton_clicked();

private:
    Ui::MainWindow *ui;

    TMLine *line;
    TMRectangle *rectangle;
    TMFreeHand *freehand;
    TMCircle *circle;

    TMShape * selectedShape = nullptr;
    TMShape * toPaste = nullptr;
    QPoint clickedPoint;
    bool hasDragged = false;

    int FREE_HAND_MODE  = 0;
    int LINE_MODE       = 1;
    int RECTANGLE_MODE  = 2;
    int CIRCLE_MODE     = 3;

    int curr_mode       = 0;

    void styleElements();
    bool isPointInFrame(QPoint point);
    bool isPointInFrame(int x, int y);
    void addToolTips();
    void configureButtons();
};
#endif // MAINWINDOW_H
