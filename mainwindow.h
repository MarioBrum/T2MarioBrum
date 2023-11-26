#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <popup.h>
//#include <funcoes.h>

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

    void on_zoomOut_clicked();

    void on_zoomIn_clicked();

    void on_girarEsquerda_clicked();

    void on_girarDireita_clicked();

    void on_fatorSX_valueChanged(int arg1);

    void on_fatorSY_valueChanged(int arg1);

    void on_convolucao_clicked();

    void on_convulacaoLista_currentTextChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    QImage imgOriginal;
    QImage imgAlterada;
    QString diretorioArq = "C:/Users/brum-/OneDrive/Documentos/T2MarioBrum";
    QString arq = "/img1.jpg";
    popup popupImgOriginal;
    popup popupImgAlternada;
    QString filtroEntrada;

    //Funcoes metodos;
    int numeroTons = 0;
    int numeroBrilho = 0;
    int numeroContraste = 0;
    int fatorSX = 1;
    int fatorSY = 1;
};
#endif // MAINWINDOW_H
