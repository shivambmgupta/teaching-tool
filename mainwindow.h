#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QPlainTextEdit>
#include "logger.h"
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
    void keyReleaseEvent(QKeyEvent *keyevent);

    void selectShapeEvent(TMShape *shape);
    void selectedShapesEvent();
    void deselectShapeEvent();
    void configureMenuActions();
    void connectMenuActions();


    void removeSelectedShapeEvent();
    void removeSelectedShapesEvent();

    void addShapeEvent(TMShape *shape = nullptr);
    void shapeRemovedEvent();

public slots:
    void onCopyClick();
    void onCutClick();
    void onPasteClick();
    void onGroupClick();
    void onUngroupClick();
    void onSaveAsClick();
    void onOpenClick();
    void onCtrlBackspaceClick();
    void onBackspaceClick();


private slots:
    void on_clear_clicked();
    void on_lineButton_clicked();
    void on_freeHandButton_clicked();
    void on_rectangleButton_clicked();
    void on_circleButton_clicked();
    void on_clearSelected_clicked();
    void on_dsList_itemClicked(QListWidgetItem *item);
    void on_undoButton_clicked();
    bool hasGroup();
    bool hasSelected();

private:
    Ui::MainWindow *ui;

    TMLine *line;
    TMRectangle *rectangle;
    TMFreeHand *freehand;
    TMCircle *circle;

    TMShape * selectedShape = nullptr;
    QList<TMShape*> selectedShapes;

    TMShape * toPaste = nullptr;
    QList<TMShape*> mtoPaste;
    QPoint clickedPoint;

    bool hasDragged = false;

    int FREE_HAND_MODE  = 0;
    int LINE_MODE       = 1;
    int RECTANGLE_MODE  = 2;
    int CIRCLE_MODE     = 3;

    int curr_mode       = 0;
    bool isCTRLPressed = false;

    void styleElements();
    bool isPointInFrame(QPoint point);
    bool isPointInFrame(int x, int y);
    void addToolTips();
    void configureButtons();

    QMessageBox confirmBox;

    int menuOffset = 25;

    QPlainTextEdit *editor;
    QString logFileName;
    Logger *logger;

    QAction *open, *save, *cut, *copy, *paste, *group, *ungroup, *ctrlBksp, *bksp;



};
#endif // MAINWINDOW_H
