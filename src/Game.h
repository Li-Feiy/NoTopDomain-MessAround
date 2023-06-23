#ifndef GAME_H
#define GAME_H

#include <QKeyEvent>
#include <QPainter>
#include <QThread>
#include <QWidget>
#include <windows.h>

namespace Ui
{
class Game;
}

class Game : public QWidget
{
    Q_OBJECT

  public:
    explicit Game(QWidget *parent = nullptr);
    ~Game();
    Ui::Game *ui;
    bool is_Start = false;
    bool is_Over=false;
    //(30,40)~(315,295)
    enum Direction
    {
        Up = 1,
        Down,
        Left,
        Right
    };
    struct Snake
    {
        int x[1005], y[1005], leng, dir;
    };
    Snake snake;
    struct Point
    {
        int x, y;
    };
    Point point;
  private slots:
    void on_GameStart_Button_clicked();
    void on_GameRestart_Button_clicked();
    void on_GameExit_Button_clicked();

  private:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);
};

#endif // GAME_H
