#include "Help.h"
#include "ui_Help.h"

Help::Help(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Help)
{
    ui->setupUi(this);
    this->setStyleSheet("QWidget{color:black;font-size:11px}");
    move(50,50);
    QIcon icon(":///res/logo.png");
    setWindowIcon(icon);
}

Help::~Help()
{
    delete ui;
}

void Help::on_Help_Button_clicked(){
    close();
}