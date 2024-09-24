#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;

    QPushButton *button1;
    QPushButton *button2;
    QPushButton *button3;

    QLabel *label;
    QPushButton *button4;

    QLineEdit *lineEdit;
    QGridLayout *gridLayout;

    QPushButton *gridButton1;
    QPushButton *gridButton2;
    QPushButton *gridButton3;
    QPushButton *gridButton4;

    QPushButton *additionalButton;


    QHBoxLayout *layout1;
    QHBoxLayout *layout2;
    QHBoxLayout *layout3;
    QVBoxLayout *rightLayout;
    QVBoxLayout *leftLayout;
    QHBoxLayout *mainLayout;

};

#endif
