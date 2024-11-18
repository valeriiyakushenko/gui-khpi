#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Image Editor");
    resize(1000, 600);

    label = new QLabel(this);
    setCentralWidget(label);

    createMenu();
    createToolBar();
    createDockWidget();

    toolBar->setEnabled(false);

}

MainWindow::~MainWindow()
{

}

void MainWindow::drawImage()
{
    label->setPixmap(tempPixmap.scaled(label->size(), Qt::KeepAspectRatio));
}

void MainWindow::openImage()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
    pixmap.load(fileName);

    if (pixmap.isNull()) {
        QMessageBox::warning(this, "Image Editor", "Cannot load");
        return;
    }

    toolBar->setEnabled(true);
    tempPixmap = pixmap;
    drawImage();
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

void MainWindow::applyChanges()
{
    pixmap = tempPixmap;
    drawImage();
}

void MainWindow::createMenu()
{
    fileMenu = menuBar()->addMenu(tr("&File"));

    fileOpen = fileMenu->addAction(tr("&Open"));
    fileSave = fileMenu->addAction(tr("&Save"));

    connect(fileOpen, &QAction::triggered, this, &MainWindow::openImage);
    connect(fileSave, &QAction::triggered, this, &MainWindow::saveImage);
}

void MainWindow::createToolBar()
{
    toolBar = new QToolBar("Tools");
    addToolBar(toolBar);

    grayscaleAction = toolBar->addAction(tr("Gray"));
    connect(grayscaleAction, &QAction::triggered, this, &MainWindow::grayscaleImage);

    sepiaAction = toolBar->addAction(tr("Sepia"));
    connect(sepiaAction, &QAction::triggered, this, &MainWindow::sepiaImage);

    negativeAction = toolBar->addAction(tr("Negative"));
    connect(negativeAction, &QAction::triggered, this, &MainWindow::negativeImage);

    pixelateAction = toolBar->addAction(tr("Pixelate"));
    connect(pixelateAction, &QAction::triggered, this, &MainWindow::pixelateImage);

    twirlAction = toolBar->addAction(tr("Twirl"));
    connect(twirlAction, &QAction::triggered, this, &MainWindow::twirlImage);

    monochromeAction = toolBar->addAction(tr("Monochrome"));
    connect(monochromeAction, &QAction::triggered, this, &MainWindow::monochromeImage);

    brightnessAction = toolBar->addAction(tr("Brightness"));
    connect(brightnessAction, &QAction::triggered, this, &MainWindow::brightnessImage);

    powAction = toolBar->addAction(tr("Pow"));
    connect(powAction, &QAction::triggered, this, &MainWindow::powImage);

    toolBar->addSeparator();

    applyAction = toolBar->addAction(tr("Apply"));
    connect(applyAction, &QAction::triggered, this, &MainWindow::applyChanges);
}

void MainWindow::createDockWidget()
{
    dockWidgetContent = new QWidget(this);

    dockWidget = new QDockWidget("Parameters", this);
    dockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
    dockWidget->setFixedWidth(180);
    dockWidget->setWidget(dockWidgetContent);

    addDockWidget(Qt::RightDockWidgetArea, dockWidget);
}

void MainWindow::updateDockWidget(QVBoxLayout* layout = nullptr)
{
    if (auto oldLayout = dockWidgetContent->layout()) {
        QLayoutItem *item;
        while ((item = oldLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete oldLayout;
    }

    dockWidgetContent->setLayout(layout ? layout : new QVBoxLayout());
}

void MainWindow::grayscaleImage()
{
    QImage img = pixmap.toImage();

    for(int y = 0; y < img.height(); y++) {
        for(int x = 0; x < img.width(); x++) {
            QColor color = img.pixelColor(x, y);
            int gray = qGray(color.rgb());
            img.setPixelColor(x, y, QColor(gray, gray, gray));
        }
    }

    tempPixmap = QPixmap::fromImage(img);

    drawImage();
    updateDockWidget();
}


void MainWindow::sepiaImage()
{
    QImage img = pixmap.toImage();

    for(int y = 0; y < img.height(); y++) {
        for(int x = 0; x < img.width(); x++) {
            QColor color = img.pixelColor(x, y);
            int r = color.red();
            int g = color.green();
            int b = color.blue();
            int tr = (0.393 * r) + (0.769 * g) + (0.189 * b);
            int tg = (0.349 * r) + (0.686 * g) + (0.168 * b);
            int tb = (0.272 * r) + (0.534 * g) + (0.131 * b);
            img.setPixelColor(x, y, QColor(qMin(tr, 255), qMin(tg, 255), qMin(tb, 255)));
        }
    }

    tempPixmap = QPixmap::fromImage(img);

    drawImage();
    updateDockWidget();
}

void MainWindow::negativeImage()
{
    QImage img = pixmap.toImage();

    for(int y = 0; y < img.height(); y++) {
        for(int x = 0; x < img.width(); x++) {
            QColor color = img.pixelColor(x, y);
            img.setPixelColor(x, y, QColor(255 - color.red(), 255 - color.green(), 255 - color.blue()));
        }
    }

    tempPixmap = QPixmap::fromImage(img);

    drawImage();
    updateDockWidget();
}

void MainWindow::pixelateImage()
{
    tempPixmap = pixmap;
    drawImage();

    pixelateLayout = new QVBoxLayout;
    pixelateLabel = new QLabel("Pixelation: ");
    pixelationSpinBox = new QSpinBox(this);

    pixelationSpinBox->setRange(1, 200);
    pixelationSpinBox->setValue(0);
    pixelationSpinBox->setFixedWidth(120);

    connect(pixelationSpinBox, &QSpinBox::valueChanged, this, [=](int pixelSize) {

        QImage img = pixmap.toImage();
        QImage pixelatedImage = img;

        for (int y = 0; y < img.height(); y += pixelSize) {
            for (int x = 0; x < img.width(); x += pixelSize) {
                QColor avgColor = img.pixelColor(x, y);

                for (int dy = 0; dy < pixelSize && y + dy < img.height(); ++dy) {
                    for (int dx = 0; dx < pixelSize && x + dx < img.width(); ++dx) {
                        pixelatedImage.setPixelColor(x + dx, y + dy, avgColor);
                    }
                }
            }
        }

        tempPixmap = QPixmap::fromImage(pixelatedImage);
        drawImage();
    });

    pixelateLayout->setAlignment(Qt::AlignCenter);
    pixelateLabel->setAlignment(Qt::AlignCenter);
    pixelateLayout->addWidget(pixelateLabel);
    pixelateLayout->addWidget(pixelationSpinBox);
    updateDockWidget(pixelateLayout);
}

void MainWindow::twirlImage()
{
    tempPixmap = pixmap;
    drawImage();

    twirlLayout = new QVBoxLayout;
    twirlLabel = new QLabel("Twirl Strength: ");
    twirlSlider = new QSlider(Qt::Horizontal, this);

    twirlSlider->setRange(0, 50);
    twirlSlider->setValue(0);

    connect(twirlSlider, &QSlider::valueChanged, this, [=](int strength) {
        QImage img = pixmap.toImage();
        QImage twirledImage = img;

        if (strength > 0) {
            int width = img.width();
            int height = img.height();
            QPoint center(width / 2, height / 2);
            double maxDistance = sqrt(pow(width / 2.0, 2) + pow(height / 2.0, 2));

            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    QPoint currentPoint(x, y);
                    double distance = sqrt(pow(currentPoint.x() - center.x(), 2) + pow(currentPoint.y() - center.y(), 2));
                    double r = distance;
                    double twirlAngle = (r / maxDistance) * strength;
                    double angle = atan2(currentPoint.y() - center.y(), currentPoint.x() - center.x());

                    int srcX = static_cast<int>(r * cos(angle + twirlAngle) + center.x());
                    int srcY = static_cast<int>(r * sin(angle + twirlAngle) + center.y());

                    if (srcX >= 0 && srcX < width && srcY >= 0 && srcY < height) {
                        twirledImage.setPixelColor(x, y, img.pixelColor(srcX, srcY));
                    }
                }
            }
        }

        tempPixmap = QPixmap::fromImage(twirledImage);
        drawImage();
    });

    twirlLayout->setAlignment(Qt::AlignCenter);
    twirlLabel->setAlignment(Qt::AlignCenter);
    twirlLayout->addWidget(twirlLabel);
    twirlLayout->addWidget(twirlSlider);
    updateDockWidget(twirlLayout);
}

void MainWindow::monochromeImage()
{
    QImage img = pixmap.toImage();
    tempPixmap = pixmap;
    drawImage();

    monochromeLayout = new QVBoxLayout;
    monochromeLabel = new QLabel("Select Color \n1 - Red\n2 - Green\n3 - Blue");
    colorSpinBox = new QSpinBox(this);

    colorSpinBox->setRange(1, 3);

    connect(colorSpinBox, &QSpinBox::valueChanged, this, [=](int value) {
        QImage modifiedImg = img;
        for(int y = 0; y < modifiedImg.height(); y++) {
            for(int x = 0; x < modifiedImg.width(); x++) {
                QColor color = modifiedImg.pixelColor(x, y);
                if (value == 1) {
                    int red = color.red();
                    modifiedImg.setPixelColor(x, y, QColor(red, 0, 0));
                } else if (value == 2) {
                    int green = color.green();
                    modifiedImg.setPixelColor(x, y, QColor(0, green, 0));
                } else if (value == 3) {
                    int blue = color.blue();
                    modifiedImg.setPixelColor(x, y, QColor(0, 0, blue));
                }
            }
        }
        tempPixmap = QPixmap::fromImage(modifiedImg);
        drawImage();
    });

    monochromeLayout->setAlignment(Qt::AlignCenter);
    monochromeLabel->setAlignment(Qt::AlignCenter);
    monochromeLayout->addWidget(monochromeLabel);
    monochromeLayout->addWidget(colorSpinBox);
    updateDockWidget(monochromeLayout);
}

void MainWindow::brightnessImage()
{
    QImage img = pixmap.toImage();
    tempPixmap = pixmap;
    drawImage();

    brightnessLayout = new QVBoxLayout;
    brightnessLabel = new QLabel("Adjust Brightness");
    brightnessSpinBox = new QSpinBox(this);
    directionCheckBox = new QCheckBox("Reduce", this);

    brightnessSpinBox->setRange(1, 100);

    connect(brightnessSpinBox, &QSpinBox::valueChanged, this, [=](int value) {
        QImage modifiedImg = img;
        for (int y = 0; y < modifiedImg.height(); y++) {
            for (int x = 0; x < modifiedImg.width(); x++) {
                QColor color = modifiedImg.pixelColor(x, y);
                int red = color.red();
                int green = color.green();
                int blue = color.blue();

                if (!directionCheckBox->isChecked()) {
                    red = qMin(255, red + value);
                    green = qMin(255, green + value);
                    blue = qMin(255, blue + value);
                } else {
                    red = qMax(0, red - value);
                    green = qMax(0, green - value);
                    blue = qMax(0, blue - value);
                }

                modifiedImg.setPixelColor(x, y, QColor(red, green, blue));
            }
        }
        tempPixmap = QPixmap::fromImage(modifiedImg);
        drawImage();
    });

    connect(directionCheckBox, &QCheckBox::toggled, this, [=](bool checked) {
        QImage modifiedImg = img;
        for (int y = 0; y < modifiedImg.height(); y++) {
            for (int x = 0; x < modifiedImg.width(); x++) {
                QColor color = modifiedImg.pixelColor(x, y);
                int red = color.red();
                int green = color.green();
                int blue = color.blue();

                if (!checked) {
                    red = qMin(255, red + brightnessSpinBox->value());
                    green = qMin(255, green + brightnessSpinBox->value());
                    blue = qMin(255, blue + brightnessSpinBox->value());
                } else {
                    red = qMax(0, red - brightnessSpinBox->value());
                    green = qMax(0, green - brightnessSpinBox->value());
                    blue = qMax(0, blue - brightnessSpinBox->value());
                }

                modifiedImg.setPixelColor(x, y, QColor(red, green, blue));
            }
        }
        tempPixmap = QPixmap::fromImage(modifiedImg);
        drawImage();
    });

    brightnessLayout->setAlignment(Qt::AlignCenter);
    brightnessLabel->setAlignment(Qt::AlignCenter);
    brightnessLayout->addWidget(brightnessLabel);
    brightnessLayout->addWidget(brightnessSpinBox);
    brightnessLayout->addWidget(directionCheckBox);
    updateDockWidget(brightnessLayout);
}


void MainWindow::powImage()
{
    tempPixmap = pixmap;
    drawImage();

    powLayout = new QVBoxLayout;
    powLabel = new QLabel("Power Transformation: ");
    powSpinBox = new QDoubleSpinBox(this);

    powSpinBox->setRange(0.1, 5.0);
    powSpinBox->setValue(1.0);
    powSpinBox->setSingleStep(0.1);
    powSpinBox->setFixedWidth(120);

    connect(powSpinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [=](double power) {
        QImage img = pixmap.toImage();
        QImage transformedImage = img;

        for (int y = 0; y < img.height(); ++y) {
            for (int x = 0; x < img.width(); ++x) {
                QColor color = img.pixelColor(x, y);

                int red = std::pow(color.red() / 255.0, power) * 255;
                int green = std::pow(color.green() / 255.0, power) * 255;
                int blue = std::pow(color.blue() / 255.0, power) * 255;

                red = qBound(0, red, 255);
                green = qBound(0, green, 255);
                blue = qBound(0, blue, 255);

                transformedImage.setPixelColor(x, y, QColor(red, green, blue));
            }
        }

        tempPixmap = QPixmap::fromImage(transformedImage);
        drawImage();
    });

    powLayout->setAlignment(Qt::AlignCenter);
    powLabel->setAlignment(Qt::AlignCenter);
    powLayout->addWidget(powLabel);
    powLayout->addWidget(powSpinBox);
    updateDockWidget(powLayout);
}


