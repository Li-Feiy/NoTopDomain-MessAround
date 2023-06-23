#ifndef FEEDBACK_H
#define FEEDBACK_H

#include <QWidget>

namespace Ui {
class Feedback;
}

class Feedback : public QWidget
{
    Q_OBJECT

public:
    explicit Feedback(QWidget *parent = nullptr);
    ~Feedback();

private slots:
    void on_Feedback_Button_clicked();

private:
    Ui::Feedback *ui;
};

#endif // FEEDBACK_H
