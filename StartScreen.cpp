#include "StartScreen.h"
#include <QPushButton>

StartScreen::StartScreen(QWidget* parent) :
     QWidget(parent)
    ,helperButton("帮助")
    ,startButton("开始游戏")
    ,modeButton("游戏模式：单人")
    ,quitButton("退出游戏")
    ,readButton("读取存档")
{
    mainLayout = new QHBoxLayout(this);

    buttonContainer = new QWidget(this);
    menuLayout = new QVBoxLayout(buttonContainer);


    //用Policy来设置自动缩放
    for (QPushButton* btn : {&helperButton, &startButton, &modeButton, &readButton, &quitButton}) {
        menuLayout->addWidget(btn);
        btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    }

    mainLayout->addStretch(2);            // 左留白
    mainLayout->addWidget(buttonContainer);
    mainLayout->addStretch(9);            // 右留白

    setLayout(mainLayout);

    adjustButtonSizes();

    connect(&modeButton, &QPushButton::clicked , this ,&StartScreen::changeMode);

}



StartScreen::~StartScreen()
{

}

void StartScreen::changeMode()
{
    if(gameMode == 1)
    {
        modeButton.setText("游戏模式：双人");
        gameMode = 2;

    }

    else
    {
        modeButton.setText("游戏模式：单人");
        gameMode = 1;
    }

    qDebug()<<"gamemode"<<gameMode;
    emit switchmode(gameMode);
}

void StartScreen::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    QPixmap background(":/img/images/background_img/startmenubg.png");

    // 缩放图片，保持原始纵横比，并保证覆盖整个窗口
    QPixmap scaledBackground = background.scaled(size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

    // 计算偏移量以使图片居中显示
    QPoint centerOffset((scaledBackground.width() - width()) / 2,
                        (scaledBackground.height() - height()) / 2);

    painter.drawPixmap(-centerOffset, scaledBackground);
}



//getter functions
QPushButton* StartScreen::getHelperButton() {
    return &helperButton;
}

QPushButton* StartScreen::getStartButton() {
    return &startButton;
}

QPushButton* StartScreen::getModeButton() {
    return &modeButton;
}

QPushButton* StartScreen::getReadButton() {
    return &readButton;
}

QPushButton* StartScreen::getQuitButton() {
    return &quitButton;
}

void StartScreen::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    adjustButtonSizes();
}

void StartScreen::adjustButtonSizes() {
    int totalWidth = width();

    // 设置按钮容器宽度占窗口宽度的 30% 左右
    int containerWidth = qMin(400,
                              qMax(static_cast<int>(totalWidth * 0.3),170));
    buttonContainer->setFixedWidth(containerWidth);

    // 按钮高度和字体大小动态调整
    int btnHeight = qMax(30, height() / 10);

    QFont font;
    font.setPointSize(qMin(static_cast<int>(btnHeight / 3),
                           static_cast<int>(containerWidth/14)));

    for (QPushButton* btn : {&helperButton, &startButton, &modeButton, &readButton, &quitButton}) {
        btn->setFixedHeight(btnHeight);
        btn->setFont(font);
    }
}
