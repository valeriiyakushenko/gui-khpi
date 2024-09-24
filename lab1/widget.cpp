#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->resize(400, 300);
    this->setWindowTitle("lab 1");

    button1 = new QPushButton("B1");
    button2 = new QPushButton("B2");
    button3 = new QPushButton("B3");

    label = new QLabel("label");
    button4 = new QPushButton("button");

    lineEdit = new QLineEdit("line edit");
    gridLayout = new QGridLayout();

    gridButton1 = new QPushButton("W1");
    gridButton2 = new QPushButton("W2");
    gridButton3 = new QPushButton("W3");
    gridButton4 = new QPushButton("W4");

    additionalButton = new QPushButton("Additional Button", this);
    additionalButton->move(this->width() - additionalButton->width() - 10, this->height() - additionalButton->height() - 10);

    gridLayout->addWidget(gridButton1, 0, 0, 1, 1);
    gridLayout->addWidget(gridButton2, 1, 0, 1, 1);
    gridLayout->addWidget(gridButton3, 0, 1, 2, 2);
    gridButton3->setFixedSize(100, 55);
    gridLayout->addWidget(gridButton4, 0, 3, 2, 1);
    gridButton4->setFixedSize(50, 55);

    layout1 = new QHBoxLayout;
    layout1->addWidget(button1, 2);
    layout1->addWidget(button2);
    layout1->addWidget(button3, 3);

    layout2 = new QHBoxLayout;
    layout2->addWidget(label);
    layout2->addWidget(button4);

    layout3 = new QHBoxLayout;
    layout3->addWidget(lineEdit);
    layout3->addLayout(gridLayout);

    rightLayout = new QVBoxLayout;
    rightLayout->addLayout(layout1);
    rightLayout->addLayout(layout2);
    rightLayout->addLayout(layout3);

    leftLayout = new QVBoxLayout;

    mainLayout = new QHBoxLayout(this);
    mainLayout->addLayout(rightLayout, 1);
    mainLayout->addLayout(leftLayout, 1);

}

Widget::~Widget()
{
}
