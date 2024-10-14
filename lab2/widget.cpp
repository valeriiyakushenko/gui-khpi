#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->resize(500, 300);
    this->setWindowTitle("lab 2");

    invert = new QCheckBox("Invert");

    input = new QLineEdit("");
    comboBox1 = new QComboBox;
    comboBox1->addItems({"kg", "g", "lbs"});
    comboBox1->setFixedHeight(35);
    label = new QLabel("->");
    output = new QLineEdit("");
    output->setReadOnly(true);
    comboBox2 = new QComboBox;
    comboBox2->addItems({"kg", "g", "lbs"});
    comboBox2->setFixedHeight(35);

    clear = new QPushButton("Clear");
    clear->setFixedHeight(35);
    convert = new QPushButton("Convert");
    convert->setFixedHeight(35);
    textSize = new QSpinBox;
    textSize->setFixedHeight(35);
    textSize->setRange(10, 30);
    textSize->setSingleStep(2);

    rb1 = new QRadioButton("Weight");
    rb2 = new QRadioButton("Length");
    rb3 = new QRadioButton("Temperature");
    rb1->setChecked(true);

    firstLayout = new QHBoxLayout;
    firstLayout->addWidget(input);
    firstLayout->addWidget(comboBox1);
    firstLayout->addWidget(label);
    firstLayout->addWidget(output);
    firstLayout->addWidget(comboBox2);

    secondLayout = new QHBoxLayout;
    secondLayout->addWidget(clear);
    secondLayout->addWidget(convert);
    secondLayout->addWidget(textSize);

    groupLayout = new QVBoxLayout;
    groupLayout->addWidget(rb1);
    groupLayout->addWidget(rb2);
    groupLayout->addWidget(rb3);

    radioGroup = new QGroupBox;
    radioGroup->setLayout(groupLayout);
    radioGroup->setFixedSize(180, 120);

    mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(invert);
    mainLayout->addLayout(firstLayout);
    mainLayout->addLayout(secondLayout);
    mainLayout->addWidget(radioGroup);

    connect(clear, &QPushButton::clicked, this, &Widget::clearLineEdit);
    connect(rb1, &QRadioButton::toggled, this, &Widget::changeType);
    connect(rb2, &QRadioButton::toggled, this, &Widget::changeType);
    connect(rb3, &QRadioButton::toggled, this, &Widget::changeType);
    connect(convert, &QPushButton::clicked, this, &Widget::convertUnits);
    connect(invert, &QCheckBox::stateChanged, this, &Widget::invertUpdate);
    connect(textSize, &QSpinBox::valueChanged, this, &Widget::textSizeUpdate);

}

void Widget::clearLineEdit()
{
    input->clear();
    output->clear();
    textSize->setValue(10);
    invert->setChecked(false);
    rb1->setChecked(true);
}

void Widget::changeType()
{
    input->clear();
    output->clear();
    comboBox1->clear();
    comboBox2->clear();

    if (rb1->isChecked()) {
        comboBox1->addItems({"kg", "g", "lbs"});
        comboBox2->addItems({"kg", "g", "lbs"});
    } else if (rb2->isChecked()) {
        comboBox1->addItems({"m", "km", "mi"});
        comboBox2->addItems({"m", "km", "mi"});
    } else if (rb3->isChecked()) {
        comboBox1->addItems({"°C", "°F", "K"});
        comboBox2->addItems({"°C", "°F", "K"});
    }
}

void Widget::convertUnits()
{
    QString inputUnit;
    QString outputUnit;
    bool correct;
    double inputValue;
    double result;

    if (invert->isChecked()) {
        inputUnit = comboBox2->currentText();
        outputUnit = comboBox1->currentText();
        inputValue = output->text().toDouble(&correct);
    } else {
        inputUnit = comboBox1->currentText();
        outputUnit = comboBox2->currentText();
        inputValue = input->text().toDouble(&correct);
    }

    if (!correct) {
        output->setText("Error");
        return;
    }

    result = inputValue;

    if (rb1->isChecked()) {
        if (inputUnit == "kg" && outputUnit == "g") {
            result = inputValue * 1000;
        } else if (inputUnit == "g" && outputUnit == "kg") {
            result = inputValue / 1000;
        } else if (inputUnit == "kg" && outputUnit == "lbs") {
            result = inputValue * 2.20462;
        } else if (inputUnit == "lbs" && outputUnit == "kg") {
            result = inputValue / 2.20462;
        } else if (inputUnit == "g" && outputUnit == "lbs") {
            result = inputValue * 0.00220462;
        } else if (inputUnit == "lbs" && outputUnit == "g") {
            result = inputValue / 0.00220462;
        }
    } else if (rb2->isChecked()) {
        if (inputUnit == "m" && outputUnit == "km") {
            result = inputValue / 1000;
        } else if (inputUnit == "km" && outputUnit == "m") {
            result = inputValue * 1000;
        } else if (inputUnit == "m" && outputUnit == "mi") {
            result = inputValue / 1609.34;
        } else if (inputUnit == "mi" && outputUnit == "m") {
            result = inputValue * 1609.34;
        } else if (inputUnit == "km" && outputUnit == "mi") {
            result = inputValue / 1.60934;
        } else if (inputUnit == "mi" && outputUnit == "km") {
            result = inputValue * 1.60934;
        }
    } else if (rb3->isChecked()) {
        if (inputUnit == "°C" && outputUnit == "°F") {
            result = (inputValue * 9/5) + 32;
        } else if (inputUnit == "°F" && outputUnit == "°C") {
            result = (inputValue - 32) * 5/9;
        } else if (inputUnit == "°C" && outputUnit == "K") {
            result = inputValue + 273.15;
        } else if (inputUnit == "K" && outputUnit == "°C") {
            result = inputValue - 273.15;
        } else if (inputUnit == "°F" && outputUnit == "K") {
            result = (inputValue - 32) * 5/9 + 273.15;
        } else if (inputUnit == "K" && outputUnit == "°F") {
            result = (inputValue - 273.15) * 9/5 + 32;
        }
    }

    if (invert->isChecked()) {
        input->setText(QString::number(result));
    } else {
        output->setText(QString::number(result));
    }
}


void Widget::invertUpdate()
{
    if (invert->isChecked()) {
        label->setText("<-");
        input->clear();
        input->setReadOnly(true);
        output->setReadOnly(false);
    } else {
        label->setText("->");
        output->clear();
        output->setReadOnly(true);
        input->setReadOnly(false);
    }
}

void Widget::textSizeUpdate(int newSize)
{
    QFont font = input->font();
    font.setPointSize(newSize);

    input->setFont(font);
    output->setFont(font);
    label->setFont(font);
}

Widget::~Widget() {}
