#include "Feedback.h"
#include "ui_Feedback.h"

Feedback::Feedback(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Feedback)
{
    ui->setupUi(this);
    this->setStyleSheet("QWidget{color:black;font-size:11px}");
    move(50,50);
    QIcon icon(":///res/logo.png");
    setWindowIcon(icon);
}

Feedback::~Feedback()
{
    delete ui;
}

void Feedback::on_Feedback_Button_clicked(){
    close();
}
