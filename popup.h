#ifndef POPUP_H
#define POPUP_H

#include <QImage>
#include <QDialog>

namespace Ui {
class popup;
}

class popup : public QDialog
{
    Q_OBJECT

public:
    explicit popup(QWidget *parent = nullptr);
    QImage getImg() const;
    void setImg(const QImage& img);
    ~popup();

private:
    QImage img;
    Ui::popup *ui;
};

#endif // POPUP_H
