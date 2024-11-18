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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void drawImage();
    void openImage();
    void saveImage();
    void applyChanges();

    void grayscaleImage();
    void sepiaImage();
    void negativeImage();
    void pixelateImage();
    void twirlImage();
    void monochromeImage();
    void brightnessImage();
    void powImage();

private:
    void createMenu();
    void createToolBar();
    void createDockWidget();
    void updateDockWidget(QVBoxLayout* content);

    QLabel *label;

    QMenu *fileMenu;
    QToolBar *toolBar;
    QVBoxLayout *mainLayout, *dockWidgetLayout;
    QHBoxLayout *bottomLayout;
    QWidget *centralWidget, *dockWidgetContent;
    QDockWidget *dockWidget;

    QAction *fileOpen, *fileSave;
    QAction *grayscaleAction, *sepiaAction, *negativeAction, *pixelateAction, *twirlAction, *monochromeAction, *brightnessAction, *powAction, *applyAction;

    QVBoxLayout *pixelateLayout;
    QLabel *pixelateLabel;
    QSpinBox *pixelationSpinBox;

    QVBoxLayout *twirlLayout;
    QLabel *twirlLabel;
    QSlider *twirlSlider;

    QVBoxLayout *monochromeLayout;
    QLabel *monochromeLabel;
    QSpinBox *colorSpinBox;

    QVBoxLayout *brightnessLayout;
    QLabel *brightnessLabel;
    QSpinBox *brightnessSpinBox;
    QCheckBox *directionCheckBox;

    QVBoxLayout *powLayout;
    QLabel *powLabel;
    QDoubleSpinBox *powSpinBox;

    QPixmap pixmap;
    QPixmap tempPixmap;
};

#endif // MAINWINDOW_H
