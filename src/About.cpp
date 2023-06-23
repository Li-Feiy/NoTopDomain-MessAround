#include "About.h"
#include "ui_About.h"

About::About(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
    this->setStyleSheet("QWidget{color:black;font-size:11px}");
    move(50,50);
    QIcon icon1(":///res/logo.png");
    setWindowIcon(icon1);
    QImage * icon=new QImage;
    icon->load(":///res/logo.png");
    ui->P_label->setPixmap(QPixmap::fromImage(*icon));
    ui->P_label->setScaledContents(true);
    ui->label_5->setOpenExternalLinks(true);
    ui->label_LYX->setOpenExternalLinks(true);
    ui->label_5->setText("<a href=\"http://www.cnblog.com/fron_csl\">极域工具包");
    ui->label_LYX->setText("<a href=\"https://yisous.xyz\">LYX");
}

About::~About()
{
    delete ui;
}

void About::on_About_Button_clicked(){
    close();
}
