#include "MainWindow.h"
#include "./ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    initMainWindow();


    //signal-slot
    connect(startScreen->getStartButton(), &QPushButton::clicked, this, &MainWindow::showGameScreen);


    connect(startScreen->getQuitButton(), &QPushButton::clicked, this, &QWidget::close);

    connect(gameScreen,&GameScreen::goBackToStartScreen,this,&MainWindow::showStartScreen);



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
