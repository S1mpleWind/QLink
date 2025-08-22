#pragma once
#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QLayout>
#include <QPushButton>

class StartScreen : public QWidget
{
    Q_OBJECT

public:
    explicit StartScreen(QWidget* parent = nullptr);
    ~ StartScreen();

    //getter functions
    QPushButton* getHelperButton();
    QPushButton* getStartButton();
    QPushButton* getModeButton();
    QPushButton* getReadButton();
    QPushButton* getQuitButton();

protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;



private:
    //Menu UI
    QHBoxLayout* mainLayout;
    QWidget* buttonContainer;
    QVBoxLayout* menuLayout;

    QPushButton helperButton;
    QPushButton startButton;
    QPushButton modeButton;
    QPushButton readButton;
    QPushButton quitButton;
    void adjustButtonSizes();

    int gameMode;


private slots:
    void changeMode();

signals:
    void switchmode(int);
};
