#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QMainWindow::setWindowTitle("Text Editor");

    appMenuBar = this->menuBar();
    file = appMenuBar->addMenu("&File");
    edit = appMenuBar->addMenu("&Edit");
    help = appMenuBar->addMenu("&Help");

    fileNew = new QAction("&New File");
    fileNew->setIcon(QIcon::fromTheme("document-new"));
    fileNew->setShortcut(QKeySequence::New);

    fileOpen = new QAction("&Open File");
    fileOpen->setIcon(QIcon::fromTheme("document-open"));
    fileOpen->setShortcut(QKeySequence::Open);

    fileSave = new QAction("&Save");
    fileSave->setIcon(QIcon::fromTheme("document-save"));
    fileSave->setShortcut(QKeySequence::Save);
    fileSave->setEnabled(false);

    fileSaveAs = new QAction("&Save As");
    fileSaveAs->setIcon(QIcon::fromTheme("document-save-as"));
    fileSaveAs->setShortcut(QKeySequence::SaveAs);

    fileQuit = new QAction("&Quit");
    fileQuit->setIcon(QIcon::fromTheme("application-exit"));
    fileQuit->setShortcut(QKeySequence::Quit);

    file->addAction(fileNew);
    file->addAction(fileOpen);
    file->addSeparator();
    file->addAction(fileSave);
    file->addAction(fileSaveAs);
    file->addSeparator();
    file->addAction(fileQuit);

    editClear = new QAction("&Clear Text Field");
    editToUpper = new QAction("&To Uppercase");
    editToLower = new QAction("&To Lowercase");
    editIncreaseFont = new QAction("&Increase Font Size");
    editDecreaseFont = new QAction("&Decrease Font Size");
    editNewLineWord = new QAction("&Each Word on New Line");

    edit->addAction(editClear);
    edit->addSeparator();
    edit->addAction(editToUpper);
    edit->addAction(editToLower);
    edit->addSeparator();
    edit->addAction(editIncreaseFont);
    edit->addAction(editDecreaseFont);
    edit->addSeparator();
    edit->addAction(editNewLineWord);

    helpAbout = new QAction("&About");
    helpAbout->setIcon(QIcon::fromTheme("help-about"));
    helpGuide = new QAction("&Help");
    helpGuide->setIcon(QIcon::fromTheme("help-faq"));

    help->addAction(helpAbout);
    help->addAction(helpGuide);

    tools = new QToolBar("Tools");
    addToolBar(tools);

    fontSizeSpinBox = new QSpinBox();
    fontSizeSpinBox->setRange(8, 36);
    fontSizeSpinBox->setValue(9);
    fontSizeSpinBox->setFixedWidth(120);
    connect(fontSizeSpinBox, &QSpinBox::valueChanged, this, &MainWindow::changeFontSize);
    tools->addWidget(fontSizeSpinBox);

    clearButton = new QToolButton();
    clearButton->setDefaultAction(editClear);
    tools->addWidget(clearButton);

    toUpperButton = new QToolButton();
    toUpperButton->setDefaultAction(editToUpper);
    tools->addWidget(toUpperButton);

    toLowerButton = new QToolButton();
    toLowerButton->setDefaultAction(editToLower);
    tools->addWidget(toLowerButton);

    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    centralLayout = new QVBoxLayout(centralWidget);
    fileLabel = new QLabel("New file", this);
    centralLayout->addWidget(fileLabel);

    textEdit = new QTextEdit(this);
    centralLayout->addWidget(textEdit);

    fileDock = new QDockWidget("File Commands", this);
    fileDockContents = new QWidget(this);
    fileDock->setAllowedAreas(Qt::LeftDockWidgetArea);
    fileDock->setFixedWidth(200);
    fileLayout = new QVBoxLayout(fileDockContents);

    fileNewButton = new QPushButton(fileNew->text());
    fileOpenButton = new QPushButton(fileOpen->text());
    fileSaveButton = new QPushButton(fileSave->text());
    fileSaveButton->setEnabled(false);
    fileSaveAsButton = new QPushButton(fileSaveAs->text());
    fileQuitButton = new QPushButton(fileQuit->text());

    fileLayout->addWidget(fileNewButton);
    fileLayout->addWidget(fileOpenButton);
    fileLayout->addWidget(fileSaveButton);
    fileLayout->addWidget(fileSaveAsButton);
    fileLayout->addWidget(fileQuitButton);

    fileDockContents->setLayout(fileLayout);
    fileDock->setWidget(fileDockContents);
    addDockWidget(Qt::LeftDockWidgetArea, fileDock);

    editDock = new QDockWidget("Edit Commands", this);
    editDockContents = new QWidget(this);
    editDock->setAllowedAreas(Qt::BottomDockWidgetArea);
    editDock->setFixedHeight(100);
    editLayout = new QGridLayout(editDockContents);

    editClearButton = new QPushButton(editClear->text());
    editToUpperButton = new QPushButton(editToUpper->text());
    editToLowerButton = new QPushButton(editToLower->text());
    editIncreaseFontButton = new QPushButton(editIncreaseFont->text());
    editDecreaseFontButton = new QPushButton(editDecreaseFont->text());
    editNewLineWordButton = new QPushButton(editNewLineWord->text());

    editLayout->addWidget(editClearButton, 0, 0, 2, 1);
    editLayout->addWidget(editToUpperButton, 0, 1);
    editLayout->addWidget(editToLowerButton, 1, 1);
    editLayout->addWidget(editIncreaseFontButton, 0, 2);
    editLayout->addWidget(editDecreaseFontButton, 1, 2);
    editLayout->addWidget(editNewLineWordButton, 0, 3, 2, 1);

    editDockContents->setLayout(editLayout);
    editDock->setWidget(editDockContents);
    addDockWidget(Qt::BottomDockWidgetArea, editDock);


    QMainWindow::setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);

    connect(helpAbout, &QAction::triggered, this, [=]() {
        QMessageBox::about(this, "About Application", "Text Editor\nVersion: 1.0\n\nThis application allows you to create and edit text files.");
    });

    connect(helpGuide, &QAction::triggered, this, [=]() {
        QMessageBox::information(this, "Help", "This is a simple text editor application. Use the menu to create, open, save files, and edit text.");
    });

    connect(editClearButton, &QPushButton::clicked, this, &MainWindow::clearText);
    connect(editToUpperButton, &QPushButton::clicked, this, &MainWindow::convertToUpper);
    connect(editToLowerButton, &QPushButton::clicked, this, &MainWindow::convertToLower);
    connect(editIncreaseFontButton, &QPushButton::clicked, this, &MainWindow::increaseFontSize);
    connect(editDecreaseFontButton, &QPushButton::clicked, this, &MainWindow::decreaseFontSize);
    connect(editNewLineWordButton, &QPushButton::clicked, this, &MainWindow::eachWordOnNewLine);

    connect(fileNewButton, &QPushButton::clicked, this, &MainWindow::newFile);
    connect(fileOpenButton, &QPushButton::clicked, this, &MainWindow::openFile);
    connect(fileSaveButton, &QPushButton::clicked, this, &MainWindow::saveFile);
    connect(fileSaveAsButton, &QPushButton::clicked, this, &MainWindow::saveFileAs);
    connect(fileQuitButton, &QPushButton::clicked, this, &MainWindow::close);

    connect(fileNew, &QAction::triggered, this, &MainWindow::newFile);
    connect(fileOpen, &QAction::triggered, this, &MainWindow::openFile);
    connect(fileSave, &QAction::triggered, this, &MainWindow::saveFile);
    connect(fileSaveAs, &QAction::triggered, this, &MainWindow::saveFileAs);
    connect(fileQuit, &QAction::triggered, this, &MainWindow::close);

    connect(editClear, &QAction::triggered, this, &MainWindow::clearText);
    connect(editToUpper, &QAction::triggered, this, &MainWindow::convertToUpper);
    connect(editToLower, &QAction::triggered, this, &MainWindow::convertToLower);
    connect(editIncreaseFont, &QAction::triggered, this, &MainWindow::increaseFontSize);
    connect(editDecreaseFont, &QAction::triggered, this, &MainWindow::decreaseFontSize);
    connect(editNewLineWord, &QAction::triggered, this, &MainWindow::eachWordOnNewLine);

    connect(clearButton, &QToolButton::clicked, this, &MainWindow::clearText);
    connect(toUpperButton, &QToolButton::clicked, this, &MainWindow::convertToUpper);
    connect(toLowerButton, &QToolButton::clicked, this, &MainWindow::convertToLower);

    connect(textEdit, &QTextEdit::textChanged, this, &MainWindow::onTextChanged);

    currentFile = ("");
    statusBar = new QStatusBar(this);
    setStatusBar(statusBar);

    QMainWindow::showMaximized();

}

MainWindow::~MainWindow() {}

void MainWindow::onTextChanged() {
    if (!currentFile.isEmpty()) {
        fileLabel->setText("Open File: " + currentFile + " *");
    }
}

void MainWindow::newFile() {
    textEdit->clear();
    fileLabel->setText("New file");
    currentFile.clear();
    fileSave->setEnabled(false);
    fileSaveButton->setEnabled(false);
    showTemporaryMessage("New file created!", 2000);
}

void MainWindow::openFile() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open File", QString(), "Text Files (*.txt);;All Files (*)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream in(&file);
        textEdit->setPlainText(in.readAll());
        fileLabel->setText("Open File: " + fileName);
        currentFile = fileName;
        fileSave->setEnabled(true);
        fileSaveButton->setEnabled(true);
        showTemporaryMessage("File opened!", 2000);
    }
}

void MainWindow::saveFile() {
    QFile file(currentFile);
    if (file.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream out(&file);
        out << textEdit->toPlainText();
        fileLabel->setText("Open File: " + currentFile);
        showTemporaryMessage("File saved!", 2000);
    }
}

void MainWindow::saveFileAs() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save File", "New file", "Text Files (*.txt);;All Files (*)");
    currentFile = fileName;
    saveFile();
}

void MainWindow::clearText() {
    textEdit->clear();
}

void MainWindow::convertToUpper() {
    textEdit->setPlainText(textEdit->toPlainText().toUpper());
}

void MainWindow::convertToLower() {
    textEdit->setPlainText(textEdit->toPlainText().toLower());
}

void MainWindow::increaseFontSize() {
    int currentSize = fontSizeSpinBox->value();
    fontSizeSpinBox->setValue(currentSize + 1);
}

void MainWindow::decreaseFontSize() {
    int currentSize = fontSizeSpinBox->value();
    fontSizeSpinBox->setValue(currentSize - 1);
}

void MainWindow::eachWordOnNewLine() {
    QString text = textEdit->toPlainText();
    QString newText = text.replace(" ", "\n");
    textEdit->setPlainText(newText);
}

void MainWindow::changeFontSize(int fontSize) {
    QFont font = this->font();
    font.setPointSize(fontSize);
    this->setFont(font);
}

void MainWindow::showTemporaryMessage(const QString &message, int timeout) {
    statusBar->showMessage(message, timeout);
}

void MainWindow::showNormalMessage(const QString &message) {
    statusBar->showMessage(message);
}

void MainWindow::showPermanentMessage(const QString &message) {
    statusBar->showMessage(message);
}
