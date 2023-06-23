#include "Game.h"
#include "ui_Game.h"

class CheckThread : public QThread
{
  public:
    CheckThread(Game *nG, QWidget *parent = nullptr);
    bool Start = false;
    Game *g;

  protected:
    void run();
};

CheckThread::CheckThread(Game *nG, QWidget *parent) : QThread(parent)
{
    g = nG;
}

void CheckThread::run()
{
    srand((unsigned)time(0));
    while (1)
    {
        g->ui->Score_LcdNumber->display(g->snake.leng);
        if (Start)
        {
            switch (g->snake.dir)
            {
            case 1:
                for (int i = g->snake.leng; i >= 2; i--)
                {
                    g->snake.x[i] = g->snake.x[i - 1];
                    g->snake.y[i] = g->snake.y[i - 1];
                }
                g->snake.y[1] -= 10;

                break;
            case 3:
                for (int i = g->snake.leng; i >= 2; i--)
                {
                    g->snake.x[i] = g->snake.x[i - 1];
                    g->snake.y[i] = g->snake.y[i - 1];
                }
                g->snake.x[1] -= 10;

                break;
            case 2:
                for (int i = g->snake.leng; i >= 2; i--)
                {
                    g->snake.x[i] = g->snake.x[i - 1];
                    g->snake.y[i] = g->snake.y[i - 1];
                }
                g->snake.y[1] += 10;
                break;
            case 4:
                for (int i = g->snake.leng; i >= 2; i--)
                {
                    g->snake.x[i] = g->snake.x[i - 1];
                    g->snake.y[i] = g->snake.y[i - 1];
                }
                g->snake.x[1] += 10;
                break;
            }
            if (g->snake.x[1] > 315)
            {
                g->snake.x[1] = 30;
            }
            if (g->snake.x[1] < 30)
            {
                g->snake.x[1] = 315;
            }
            if (g->snake.y[1] > 295)
            {
                g->snake.y[1] = 40;
            }
            if (g->snake.y[1] < 40)
            {
                g->snake.y[1] = 295;
            }
            if ((g->point.x * 10 + 30) <= (g->snake.x[1] + 10) && (g->point.y * 10 + 40) <= (g->snake.y[1]) &&
                (g->point.x * 10 + 30) >= (g->snake.x[1]) && (g->point.y * 10 + 40) >= (g->snake.y[1]))
            {
                g->point.x = rand() % 19;
                g->point.y = rand() % 15;
                g->snake.leng++;
            }
        }
        else
        {
            memset(g->snake.x, 0, sizeof(g->snake.x));
            memset(g->snake.y, 0, sizeof(g->snake.y));
            g->ui->GameStart_Button->setEnabled(true);
            g->snake.x[1] = 30 + 10 * 6;
            g->snake.y[1] = 40 + 10 * 6;
            g->snake.x[2] = 30 + 10 * 7;
            g->snake.y[2] = 40 + 10 * 6;
            g->snake.x[3] = 30 + 10 * 8;
            g->snake.y[3] = 40 + 10 * 6;
            g->snake.leng = 3;
            g->snake.dir = 3;
            g->point.x = rand() % 19;
            g->point.y = rand() % 15;
            g->is_Over = true;
            g->repaint();
            break;
        }
        for (int i = 2; i <= g->snake.leng; i++)
        {
            if (g->snake.x[1] == g->snake.x[i] && g->snake.y[1] == g->snake.y[i])
            {
                g->is_Start = false;
                Start = false;
            }
        }
        g->update();
        Sleep(200);
    }
    g->repaint();
}

Game::Game(QWidget *parent) : QWidget(parent), ui(new Ui::Game)
{
    ui->setupUi(this);
    this->setStyleSheet("QWidget{color:black;}");
    srand((unsigned)time(0));
    move(50, 50);
    setWindowOpacity(0.8);
    QIcon icon(":///res/logo.png");
    setWindowIcon(icon);
    snake.x[1] = 30 + 10 * 6;
    snake.y[1] = 40 + 10 * 6;
    snake.x[2] = 30 + 10 * 7;
    snake.y[2] = 40 + 10 * 6;
    snake.x[3] = 30 + 10 * 8;
    snake.y[3] = 40 + 10 * 6;
    snake.leng = 3;
    snake.dir = Left;
    point.x = rand() % 19;
    point.y = rand() % 15;
}

Game::~Game()
{
    delete ui;
}

void Game::on_GameStart_Button_clicked()
{
    is_Start = true;
    is_Over = false;
    CheckThread *ct = new CheckThread(this);
    ct->Start = true;
    ct->start();
    update();
    ui->GameStart_Button->setEnabled(false);
}

void Game::on_GameRestart_Button_clicked()
{
    is_Start = false;
    memset(snake.x, 0, sizeof(snake.x));
    memset(snake.y, 0, sizeof(snake.y));
    ui->GameStart_Button->setEnabled(true);
    snake.x[1] = 30 + 10 * 6;
    snake.y[1] = 40 + 10 * 6;
    snake.x[2] = 30 + 10 * 7;
    snake.y[2] = 40 + 10 * 6;
    snake.x[3] = 30 + 10 * 8;
    snake.y[3] = 40 + 10 * 6;
    snake.leng = 3;
    snake.dir = Left;
    srand((unsigned)time(0));
    point.x = rand() % 19;
    point.y = rand() % 15;
    update();
}

void Game::on_GameExit_Button_clicked()
{
    close();
}

void Game::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    if (is_Start)
    {
        painter.drawRect(snake.x[1], snake.y[1], 10, 10);
        painter.fillRect(snake.x[1], snake.y[1], 10, 10, QColor(0, 160, 230));
        for (int i = 2; i <= snake.leng; i++)
        {
            painter.drawRect(snake.x[i], snake.y[i], 10, 10);
            painter.fillRect(snake.x[i], snake.y[i], 10, 10, QColor(0, 230, 160));
        }
        if ((point.x * 10 + 30) % 10 != 0)
            point.x = rand() % 19;
        if ((point.y * 10 + 40) % 10 != 0)
            point.y = rand() % 15;
        painter.drawRect(30 + point.x * 10, 40 + point.y * 10, 10, 10);
        painter.fillRect(30 + point.x * 10, 40 + point.y * 10, 10, 10, QColor(239, 136, 190));
    }
    if (is_Over)
    {
        QFont font("Cascadia Mono", 30, QFont::ExtraLight, false);
        painter.setFont(font);
        painter.drawText((this->width() - 225) / 2, (this->height()) / 2, QString("GAME OVER!"));
    }
}

void Game::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_W:
        if (snake.dir != Down)
            snake.dir = Up;
        break;
    case Qt::Key_A:
        if (snake.dir != Right)
            snake.dir = Left;
        break;
    case Qt::Key_S:
        if (snake.dir != Up)
            snake.dir = Down;
        break;
    case Qt::Key_D:
        if (snake.dir != Left)
            snake.dir = Right;
        break;
        // case Qt::Key_Return:
        //     switch (snake.dir)
        //     {
        //     case 1:
        //         for (int i = snake.leng; i >= 2; i--)
        //         {
        //             snake.x[i] = snake.x[i - 1];
        //             snake.y[i] = snake.y[i - 1];
        //         }
        //         snake.y[1] -= 10;

        //         break;
        //     case 3:
        //         for (int i = snake.leng; i >= 2; i--)
        //         {
        //             snake.x[i] = snake.x[i - 1];
        //             snake.y[i] = snake.y[i - 1];
        //         }
        //         snake.x[1] -= 10;

        //         break;
        //     case 2:
        //         for (int i = snake.leng; i >= 2; i--)
        //         {
        //             snake.x[i] = snake.x[i - 1];
        //             snake.y[i] = snake.y[i - 1];
        //         }
        //         snake.y[1] += 10;
        //         break;
        //     case 4:
        //         for (int i = snake.leng; i >= 2; i--)
        //         {
        //             snake.x[i] = snake.x[i - 1];
        //             snake.y[i] = snake.y[i - 1];
        //         }
        //         snake.x[1] += 10;
        //         break;
        //     }
        //     if (snake.x[1] > 315)
        //     {
        //         snake.x[1] = 30;
        //     }
        //     if (snake.x[1] < 30)
        //     {
        //         snake.x[1] = 315;
        //     }
        //     if (snake.y[1] > 295)
        //     {
        //         snake.y[1] = 40;
        //     }
        //     if (snake.y[1] < 40)
        //     {
        //         snake.y[1] = 295;
        //     }
        //     break;
    }
    update();
}