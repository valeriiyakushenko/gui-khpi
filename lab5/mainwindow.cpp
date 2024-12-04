#include "mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Image Editor");
    resize(1000, 600);

    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    label = new QLabel(centralWidget);
    label->setFixedSize(labelW, labelH);
    label->move(labelX, labelY);

    pixmap = QPixmap(label->size());
    pixmap.fill(QColor("white"));
    label->setPixmap(pixmap.scaled(label->size()));

    painter = new QPainter(&pixmap);

    createMenu();
    createToolBar();
    createDockWidget();
}

MainWindow::~MainWindow()
{

}

void MainWindow::openImage()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
    if (!fileName.isEmpty()) {
        if (painter->isActive()) {
            painter->end();
        }

        QPixmap newPixmap;
        if (!newPixmap.load(fileName)) {
            QMessageBox::warning(this, "Image Editor", "Cannot load image.");
            return;
        }

        pixmap = newPixmap;
        label->setPixmap(pixmap.scaled(label->size()));

        delete painter;
        painter = new QPainter(&pixmap);
    }
}

void MainWindow::saveImage()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QString(), tr("Image file (*.jpg)"));

    if (pixmap.isNull()) {
        QMessageBox::warning(this, "Image Editor", "Cannot save");
        return;
    }
    pixmap.save(fileName);
}

void MainWindow::setBrush()
{
    QAction *buttonSender = qobject_cast<QAction*>(sender());

    if(buttonSender == this->linebrush) {
        brushIndex = LINE;
        drawLine();
    }
    else if(buttonSender == this->circlebrush) {
        brushIndex = CIRCLE;
        drawCircle();
    }
    else if(buttonSender == this->polygonbrush) {
        brushIndex = POLYGON;
        drawPolygon();
    }
    else if(buttonSender == this->custombrush) {
        brushIndex = CUSTOM;
        drawSmile();
    }
}

void MainWindow::draw()
{
    switch (brushIndex) {
    case LINE:
        painter->setPen(QPen(currentColor, brushSize));
        painter->drawLine(prevX, prevY, x, y);
        break;
    case CIRCLE:
        painter->setBrush(currentColor);
        painter->setPen(QPen(circuitColor, 1, penStyle));
        painter->drawEllipse(QPoint(x, y), diameter / 2, diameter / 2);
        break;
    case POLYGON:
        drawPolygonShape();
        break;
    case CUSTOM:
        drawSmileShape();
        break;
    default:
        break;
    }
    label->setPixmap(pixmap.scaled(label->size()));
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    x = event->pos().x() - labelX;
    y = event->pos().y() - 100;
    if (brushIndex == CIRCLE || brushIndex == POLYGON || brushIndex == CUSTOM){
        draw();
    }
    prevX = x;
    prevY = y;
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    x = event->pos().x() - labelX;
    y = event->pos().y() - 100;
    if (brushIndex > -1){
        draw();
    }
    prevX = x;
    prevY = y;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape)
        brushIndex = -1;
}

void MainWindow::createMenu()
{
    fileMenu = menuBar()->addMenu(tr("&File"));

    fileOpen = fileMenu->addAction(tr("&Open"));
    fileSave = fileMenu->addAction(tr("&Save"));
    fileQuit = fileMenu->addAction(tr("&Quit"));

    connect(fileOpen, &QAction::triggered, this, &MainWindow::openImage);
    connect(fileSave, &QAction::triggered, this, &MainWindow::saveImage);
    connect(fileQuit, &QAction::triggered, this, &QWidget::close);
}

void MainWindow::createToolBar()
{
    toolBar = new QToolBar("Tools");
    addToolBar(toolBar);

    linebrush = toolBar->addAction(tr("Line"));
    circlebrush = toolBar->addAction(tr("Circle"));
    polygonbrush = toolBar->addAction(tr("Polygon"));
    custombrush = toolBar->addAction(tr("Custom"));

    connect(linebrush, &QAction::triggered, this, &MainWindow::setBrush);
    connect(circlebrush, &QAction::triggered, this, &MainWindow::setBrush);
    connect(polygonbrush, &QAction::triggered, this, &MainWindow::setBrush);
    connect(custombrush, &QAction::triggered, this, &MainWindow::setBrush);
}

void MainWindow::createDockWidget()
{
    dockWidgetContent = new QWidget(this);
    mainLayout = new QVBoxLayout(dockWidgetContent);

    staticPart = new QWidget(dockWidgetContent);
    staticLayout = new QVBoxLayout(staticPart);
    staticPart->setLayout(staticLayout);

    QLabel *colorLabel = new QLabel("Brush Color:");
    colorPicker = new QPushButton();
    colorPicker->setStyleSheet("background-color: " + currentColor.name());
    connect(colorPicker, &QPushButton::clicked, this, [=]() {
        currentColor = QColorDialog::getColor(currentColor, this);
        colorPicker->setStyleSheet("background-color: " + currentColor.name());
    });

    QLabel *sizeLabel = new QLabel("Brush Size:");
    brushSizeSlider = new QSlider(Qt::Horizontal);
    brushSizeSlider->setRange(1, 20);
    brushSizeSlider->setValue(5);
    connect(brushSizeSlider, &QSlider::valueChanged, this, [=](int value) {
        brushSize = value;
    });

    staticLayout->addWidget(colorLabel);
    staticLayout->addWidget(colorPicker);
    staticLayout->addWidget(sizeLabel);
    staticLayout->addWidget(brushSizeSlider);

    dynamicPart = new QWidget(dockWidgetContent);
    dynamicLayout = new QVBoxLayout(dynamicPart);
    dynamicPart->setLayout(dynamicLayout);

    mainLayout->addWidget(staticPart);
    mainLayout->addWidget(dynamicPart);

    dockWidgetContent->setLayout(mainLayout);
    dockWidget = new QDockWidget("Parameters", this);
    dockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
    dockWidget->setFixedWidth(180);
    dockWidget->setWidget(dockWidgetContent);

    addDockWidget(Qt::RightDockWidgetArea, dockWidget);
}

void MainWindow::updateDockWidget(QVBoxLayout* layout)
{
    if (auto oldLayout = dynamicPart->layout()) {
        QLayoutItem *item;
        while ((item = oldLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete oldLayout;
    }

    dynamicPart->setLayout(layout ? layout : new QVBoxLayout());
}

void MainWindow::drawLine()
{
    dynamicLayout = new QVBoxLayout();
    updateDockWidget(dynamicLayout);
}

void MainWindow::drawCircle()
{
    dynamicLayout = new QVBoxLayout();

    QLabel *circuitColorLabel = new QLabel("Circuit Color:");
    QPushButton *circuitColorPicker = new QPushButton();
    circuitColorPicker->setStyleSheet("background-color: " + circuitColor.name());
    connect(circuitColorPicker, &QPushButton::clicked, this, [=]() {
        circuitColor = QColorDialog::getColor(circuitColor, this);
        if (circuitColor.isValid()) {
            circuitColorPicker->setStyleSheet("background-color: " + circuitColor.name());
        }
    });

    QLabel *penStyleLabel = new QLabel("Pen Style:");
    QComboBox *penStyleComboBox = new QComboBox();
    penStyleComboBox->addItem("Solid");
    penStyleComboBox->addItem("Dashed");
    penStyleComboBox->addItem("Dotted");
    penStyleComboBox->addItem("Dash-Dotted");
    connect(penStyleComboBox, &QComboBox::currentIndexChanged, this, [=](int index) {
        switch (index) {
        case 0:
            penStyle = Qt::SolidLine;
            break;
        case 1:
            penStyle = Qt::DashLine;
            break;
        case 2:
            penStyle = Qt::DotLine;
            break;
        case 3:
            penStyle = Qt::DashDotLine;
            break;
        }
    });

    QLabel *diameterLabel = new QLabel("Diameter:");
    QSpinBox *diameterSpinBox = new QSpinBox();
    diameterSpinBox->setRange(1, 1000);
    diameterSpinBox->setValue(10);
    connect(diameterSpinBox, &QSpinBox::valueChanged, this, [=](int value) {
        diameter = value;
    });

    dynamicLayout->addWidget(circuitColorLabel);
    dynamicLayout->addWidget(circuitColorPicker);
    dynamicLayout->addWidget(penStyleLabel);
    dynamicLayout->addWidget(penStyleComboBox);
    dynamicLayout->addWidget(diameterLabel);
    dynamicLayout->addWidget(diameterSpinBox);

    updateDockWidget(dynamicLayout);
}

void MainWindow::drawPolygon()
{
    dynamicLayout = new QVBoxLayout();

    QLabel *circuitColorLabel = new QLabel("Circuit Color:");
    QPushButton *circuitColorPicker = new QPushButton();
    circuitColorPicker->setStyleSheet("background-color: " + circuitColor.name());
    connect(circuitColorPicker, &QPushButton::clicked, this, [=]() {
        circuitColor = QColorDialog::getColor(circuitColor, this);
        if (circuitColor.isValid()) {
            circuitColorPicker->setStyleSheet("background-color: " + circuitColor.name());
        }
    });

    QLabel *penStyleLabel = new QLabel("Pen Style:");
    QComboBox *penStyleComboBox = new QComboBox();
    penStyleComboBox->addItem("Solid");
    penStyleComboBox->addItem("Dashed");
    penStyleComboBox->addItem("Dotted");
    penStyleComboBox->addItem("Dash-Dotted");
    connect(penStyleComboBox, &QComboBox::currentIndexChanged, this, [=](int index) {
        switch (index) {
        case 0:
            penStyle = Qt::SolidLine;
            break;
        case 1:
            penStyle = Qt::DashLine;
            break;
        case 2:
            penStyle = Qt::DotLine;
            break;
        case 3:
            penStyle = Qt::DashDotLine;
            break;
        }
    });

    QLabel *diameterLabel = new QLabel("Diameter:");
    QSpinBox *diameterSpinBox = new QSpinBox();
    diameterSpinBox->setRange(1, 1000);
    diameterSpinBox->setValue(10);
    connect(diameterSpinBox, &QSpinBox::valueChanged, this, [=](int value) {
        diameter = value;
    });

    dynamicLayout->addWidget(circuitColorLabel);
    dynamicLayout->addWidget(circuitColorPicker);
    dynamicLayout->addWidget(penStyleLabel);
    dynamicLayout->addWidget(penStyleComboBox);
    dynamicLayout->addWidget(diameterLabel);
    dynamicLayout->addWidget(diameterSpinBox);

    updateDockWidget(dynamicLayout);
}

void MainWindow::drawPolygonShape()
{
    QPolygon polygon;
    QPoint center(x, y);
    double angleStep = 2 * M_PI / 7;
    for (int i = 0; i < 7; ++i) {
        double angle = i * angleStep;
        int px = center.x() + (diameter / 2 * cos(angle));
        int py = center.y() + (diameter / 2 * sin(angle));
        polygon << QPoint(px, py);
    }
    painter->setBrush(currentColor);
    painter->setPen(QPen(circuitColor, 1, penStyle));
    painter->drawPolygon(polygon);
}

void MainWindow::drawSmile()
{
    dynamicLayout = new QVBoxLayout();

    QLabel *colorLabel = new QLabel("Body Color:");
    bodyColorPicker = new QPushButton();
    bodyColorPicker->setStyleSheet("background-color: " + bodyColor.name());
    connect(bodyColorPicker, &QPushButton::clicked, this, [=]() {
        bodyColor = QColorDialog::getColor(bodyColor, this);
        bodyColorPicker->setStyleSheet("background-color: " + bodyColor.name());
    });

    QLabel *eyeColorLabel = new QLabel("Eye Color:");
    eyeColorPicker = new QPushButton();
    eyeColorPicker->setStyleSheet("background-color: " + eyeColor.name());
    connect(eyeColorPicker, &QPushButton::clicked, this, [=]() {
        eyeColor = QColorDialog::getColor(eyeColor, this);
        eyeColorPicker->setStyleSheet("background-color: " + eyeColor.name());
    });

    QLabel *noseColorLabel = new QLabel("Nose Color:");
    noseColorPicker = new QPushButton();
    noseColorPicker->setStyleSheet("background-color: " + noseColor.name());
    connect(noseColorPicker, &QPushButton::clicked, this, [=]() {
        noseColor = QColorDialog::getColor(noseColor, this);
        noseColorPicker->setStyleSheet("background-color: " + noseColor.name());
    });

    dynamicLayout->addWidget(colorLabel);
    dynamicLayout->addWidget(bodyColorPicker);
    dynamicLayout->addWidget(eyeColorLabel);
    dynamicLayout->addWidget(eyeColorPicker);
    dynamicLayout->addWidget(noseColorLabel);
    dynamicLayout->addWidget(noseColorPicker);

    updateDockWidget(dynamicLayout);
}

void MainWindow::drawSmileShape()
{
    QPainterPath path;

    path.addEllipse(x - 30, y - 30, 60, 60);

    painter->setBrush(bodyColor);
    painter->setPen(Qt::NoPen);

    painter->drawPath(path);

    painter->setBrush(eyeColor);
    painter->drawEllipse(x - 15, y - 10, 10, 10);
    painter->drawEllipse(x + 5, y - 10, 10, 10);

    QPainterPath nosePath;
    QRectF noseRect(x - 10, y, 20, 10);
    nosePath.moveTo(x - 10, y + 5);
    nosePath.arcTo(noseRect, 0, -180);

    painter->setBrush(noseColor);
    painter->setPen(Qt::NoPen);
    painter->drawPath(nosePath);
}




