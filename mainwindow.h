#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <popup.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_inverteVertical_clicked();

    void on_inverteHorizontal_clicked();

    void on_converteCinza_clicked();

    void on_quantizaCinza_clicked();

    void on_salvaImagem_clicked();

    void on_tonsBox_valueChanged(int arg1);

    void on_copiaImagem_clicked();

    void on_brilhoBox_valueChanged(int arg1);

    void on_brilho_clicked();

    void on_contrasteBox_valueChanged(int arg1);

    void on_contraste_clicked();

    void on_negativo_clicked();

    void on_histograma_clicked();

    void on_equalizarHistograma_clicked();

private:
    Ui::MainWindow *ui;
    QImage imgOriginal;
    QImage imgAlterada;
    QString diretorioArq = "C:/Users/brum-/OneDrive/Documentos/T2MarioBrum";
    QString arq = "/img1.jpg";
    popup popupImgOriginal;
    popup popupImgAlternada;
    int numeroTons = 1;
    int numeroBrilho = 0;
    int numeroContraste = 0;
};
#endif // MAINWINDOW_H
