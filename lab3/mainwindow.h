#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFont>
#include <QSpinBox>
#include <QToolButton>
#include <QDockWidget>
#include <QTextEdit>
#include <QDockWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QString>
#include <QStatusBar>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void changeFontSize(int index);
    void newFile();
    void openFile();
    void saveFile();
    void saveFileAs();
    void clearText();
    void onTextChanged();
    void convertToUpper();
    void convertToLower();
    void increaseFontSize();
    void decreaseFontSize();
    void eachWordOnNewLine();
    void showTemporaryMessage(const QString &message, int timeout);
    void showNormalMessage(const QString &message);
    void showPermanentMessage(const QString &message);


private:
    QToolBar *tools;
    QMenuBar *appMenuBar;
    QMenu *file, *edit, *help;
    QAction *fileNew, *fileOpen, *fileSave, *fileSaveAs, *fileQuit;
    QAction *editClear, *editToUpper, *editToLower, *editIncreaseFont, *editDecreaseFont, *editNewLineWord;
    QAction *helpAbout, *helpGuide;
    QSpinBox *fontSizeSpinBox;
    QToolButton *clearButton, *toUpperButton, *toLowerButton;
    QLabel *fileLabel;
    QTextEdit *textEdit;
    QWidget *centralWidget, *fileDockContents, *editDockContents;
    QVBoxLayout *centralLayout, *fileLayout;
    QDockWidget *fileDock, *editDock;
    QGridLayout *editLayout;
    QString currentFile;
    QStatusBar *statusBar;
    QPushButton *editClearButton, *editToUpperButton, *editToLowerButton, *editIncreaseFontButton, *editDecreaseFontButton, *editNewLineWordButton;
    QPushButton *fileNewButton, *fileOpenButton, *fileSaveButton, *fileSaveAsButton, *fileQuitButton;
};

#endif
