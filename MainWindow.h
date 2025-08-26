#pragma once
#include <QMainWindow>
#include <QStackedWidget>
#include "StartScreen.h"
#include "GameScreen.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void showStartScreen();
    void showGameScreen();



    void initMainWindow();

private:
    Ui::MainWindow *ui;
    QStackedWidget * stackWidget;

    StartScreen * startScreen;
    GameScreen* gameScreen;

private slots:
    void loadGame();
    void openHelpDoc();



};

