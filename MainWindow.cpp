#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include <QFile>
#include <QDialog>
#include <QTextBrowser>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    initMainWindow();


    //signal-slot
    connect(startScreen->getStartButton(), &QPushButton::clicked, this, &MainWindow::showGameScreen);


    connect(startScreen->getQuitButton(), &QPushButton::clicked, this, &QWidget::close);


    connect(startScreen->getReadButton(),&QPushButton::clicked, this, &MainWindow::loadGame);

    connect(startScreen->getHelperButton(),&QPushButton::clicked, this, &MainWindow::openHelpDoc);

    connect(gameScreen,&GameScreen::goBackToStartScreen,this,&MainWindow::showStartScreen);

    connect(startScreen,&StartScreen::switchmode,gameScreen,&GameScreen::setMode);




}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showStartScreen()
{
    gameScreen->setPause();
    // remove psuse condition

    stackWidget->setCurrentWidget(startScreen);
}

void MainWindow::showGameScreen()
{

    gameScreen->recoverGame();
    // remove psuse condition
    stackWidget->setCurrentWidget(gameScreen);
    //gameScreen
}

void MainWindow::loadGame()
{
    gameScreen->loadGame();
    gameScreen->recoverGame();
    stackWidget->setCurrentWidget(gameScreen);
}

void MainWindow::initMainWindow()
{
    // 创建界面
    stackWidget = new QStackedWidget(this);
    startScreen = new StartScreen;
    gameScreen = new GameScreen;

    stackWidget->addWidget(startScreen);
    stackWidget->addWidget(gameScreen);

    //default widget
    stackWidget->setCurrentWidget(startScreen);


    //设置中央组件
    setCentralWidget(stackWidget);

    setWindowTitle("Don't Starve Link");
    resize(1400, 1000);
}


void MainWindow::openHelpDoc()
{

    QFile file(":/instruct/instructor.md");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QString content = file.readAll();


    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Markdown Viewer");
    dialog->resize(600, 400);

    QTextBrowser *browser = new QTextBrowser(dialog);
    browser->setMarkdown(content);   // <-- Markdown 预览
    browser->setReadOnly(true);

    QVBoxLayout *layout = new QVBoxLayout(dialog);
    layout->addWidget(browser);
    dialog->setLayout(layout);

    dialog->exec(); // 模态窗口，点击关闭按钮即可退出
}

