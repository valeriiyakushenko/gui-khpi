#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMenuBar>
#include <QMenu>
#include <QFileDialog>
#include <QMessageBox>
#include <QToolBar>
#include <QDockWidget>
#include <QSpinBox>
#include <QSlider>
#include <QRadioButton>
#include <QCheckBox>
#include <QPainter>
#include <QPen>
#include <QMouseEvent>
#include <QColorDialog>
#include <QComboBox>
#include <QPainterPath>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openImage();
    void saveImage();
    void setBrush();
    void draw();
    void drawLine();
    void drawCircle();
    void drawPolygon();
    void drawPolygonShape();
    void drawSmile();
    void drawSmileShape();

private:
    void createMenu();
    void createToolBar();
    void createDockWidget();
    void updateDockWidget(QVBoxLayout* content);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);


    QLabel *label;

    QMenu *fileMenu;
    QToolBar *toolBar;
    QVBoxLayout *mainLayout, *dockWidgetLayout, *staticLayout, *dynamicLayout;
    QHBoxLayout *bottomLayout;
    QWidget *centralWidget, *dockWidgetContent, *staticPart, *dynamicPart;
    QDockWidget *dockWidget;
    QPainter *painter;

    QAction *fileOpen, *fileSave, *fileQuit;
    QAction *linebrush, *circlebrush, *polygonbrush, *custombrush;

    QPixmap pixmap;

    QPushButton *colorPicker;
    QSlider *brushSizeSlider;
    QColor currentColor = Qt::black;
    QColor circuitColor = Qt::black;

    QPushButton *bodyColorPicker, *eyeColorPicker, *noseColorPicker;
    QColor bodyColor = Qt::yellow;
    QColor eyeColor = Qt::black;
    QColor noseColor = Qt::red;

    int labelX = 50, labelY = 50, labelW = 700, labelH = 450;
    int x = -1, y = -1;
    int prevX = -1, prevY = -1;
    int brushIndex = -1;
    int brushSize = 5;
    int diameter=10;
    enum brushes{LINE, CIRCLE, POLYGON, CUSTOM};
    Qt::PenStyle penStyle = Qt::SolidLine;


};

#endif // MAINWINDOW_H
