#include "popup.h"
#include "ui_popup.h"

popup::popup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::popup)
{
    ui->setupUi(this);

    setWindowTitle("Historograma Pop-up");
    //resize(512,400);
    //setFixedSize(512, 400);
    //ui->imgLabel->setPixmap(QPixmap::fromImage(img));

}

popup::~popup()
{
    delete ui;
}

QImage popup::getImg() const
{
    return img;
}

void popup::setImg(const QImage& img)
{
    ui->imgLabel->setPixmap(QPixmap::fromImage(img));
    ui->imgLabel->setAlignment(Qt::AlignCenter);
}
