#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QRadioButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QSpinBox>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void changeType();
    void clearLineEdit();
    void convertUnits();
    void invertUpdate();
    void textSizeUpdate(int newSize);

private:
    QCheckBox *invert;
    QComboBox *comboBox1, *comboBox2;
    QLabel *label;
    QLineEdit *input, *output;
    QPushButton *clear, *convert;
    QSpinBox *textSize;
    QGroupBox *radioGroup;
    QRadioButton *rb1, *rb2, *rb3;
    QVBoxLayout *mainLayout, *groupLayout;
    QHBoxLayout *firstLayout, *secondLayout;
};

#endif
