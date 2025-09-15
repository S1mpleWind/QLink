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


    //信号与槽的连接
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


//显示开始菜单
void MainWindow::showStartScreen()
{
    //设置游戏状态为暂停，等到进入游戏界面时再恢复
    gameScreen->setPause();

    //切换主控件
    stackWidget->setCurrentWidget(startScreen);
}


//显示游戏界面
void MainWindow::showGameScreen()
{
    //进入游戏时恢复游戏状态，进入计时
    gameScreen->recoverGame();

    stackWidget->setCurrentWidget(gameScreen);
}


//从本地的JSON文件中读取并恢复游戏
void MainWindow::loadGame()
{
    gameScreen->loadGame();
    gameScreen->recoverGame();
    stackWidget->setCurrentWidget(gameScreen);
}



//初始化mainwindow界面
void MainWindow::initMainWindow()
{
    // 创建stack
    stackWidget = new QStackedWidget(this);

    //创建子窗口 开始界面和游戏界面 并添加到stack中
    startScreen = new StartScreen;
    gameScreen = new GameScreen;

    stackWidget->addWidget(startScreen);
    stackWidget->addWidget(gameScreen);

    //default widget
    stackWidget->setCurrentWidget(startScreen);


    //设置中央组件
    setCentralWidget(stackWidget);

    //设置标题
    setWindowTitle("Don't Starve Link");
    resize(1400, 1000);
}



//打开帮助文档
void MainWindow::openHelpDoc()
{
    //打开文件并全部读取到QString中
    QFile file(":/instruct/instructor.md");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QString content = file.readAll();

    //设置一个对话框由于显示md效果
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Markdown Viewer");
    dialog->resize(600, 400);

    //创建浏览器对象并设置md格式，用来显示帮助文档的预览
    QTextBrowser *browser = new QTextBrowser(dialog);
    browser->setMarkdown(content);
    browser->setReadOnly(true);

    QVBoxLayout *layout = new QVBoxLayout(dialog);
    layout->addWidget(browser);
    dialog->setLayout(layout);

    // 模态窗口，点击关闭按钮即可退出
    // 同时阻塞程序的主进程
    dialog->exec();
}

