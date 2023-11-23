#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QLabel>
#include <QString>
#include <QImage>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    imgOriginal = QImage(diretorioArq + arq);
    imgAlterada = imgOriginal;

    //  ui->labelImgOriginal->setPixmap(QPixmap::fromImage(imgOriginal));
    //ui->labelImgAlterada->setPixmap(QPixmap::fromImage(imgAlterada));
    //  ui->labelImgAlterada->setPixmap(QPixmap::fromImage(imgOriginal));

    popupImgOriginal.setWindowTitle("Imagem Original");
    popupImgAlternada.setWindowTitle("Imagem Alterada");
    popupImgOriginal.setImg(imgOriginal);
    popupImgAlternada.setImg(imgOriginal);
    popupImgOriginal.show();
    popupImgAlternada.show();
}
MainWindow::~MainWindow()
{
    delete ui;
}

QImage inverterImagemVertical(QImage img) {
    int largura = img.width();
    int altura = img.height();
    QImage imagemAlterada(largura, altura, QImage::Format_RGB32);
    for (int y = 0; y < altura; y++) {
        for (int x = 0; x < largura; x++) {
            QRgb pixel = img.pixel(x, y);
            imagemAlterada.setPixel(largura - x - 1, y, pixel);
        }
    }
    return imagemAlterada;
}

QImage inverterImagemHorizontal(QImage img) {
    int largura = img.width();
    int altura = img.height();
    QImage imagemAlterada(largura, altura, QImage::Format_RGB32);
    for (int y = 0; y < altura; y++) {
        for (int x = 0; x < largura; x++) {
            QRgb pixel = img.pixel(x, y);
            imagemAlterada.setPixel(x, altura - y - 1, pixel);
        }
    }
    return imagemAlterada;
}

QImage conversaoCinza(QImage img) {
    int largura = img.width();
    int altura = img.height();
    QImage imagemAlterada(largura, altura, QImage::Format_RGB32);
    for (int y = 0; y < altura; y++) {
        for (int x = 0; x < largura; x++) {
            QColor cor(img.pixel(x, y));
            int R = cor.red();
            int G = cor.green();
            int B = cor.blue();
            double L = 0.299 * R + 0.587 * G + 0.114 * B;
            cor.setRgb(L, L, L);
            imagemAlterada.setPixel(x, y, cor.rgb());
        }
    }

    return imagemAlterada;
}

QImage quantizacaoCinza(QImage img,int n) {
    int largura = img.width();
    int altura = img.height();
    QImage imagemAlterada(largura, altura, QImage::Format_RGB32);
    int menorTom = 255;
    int maiorTom = 0;
    for (int y = 0; y < altura; y++) {
        for (int x = 0; x < largura; x++) {
            QColor cor(img.pixel(x, y));
            int tom = cor.red();
            if(tom > maiorTom){
                maiorTom = tom;
            }
            if(tom < menorTom){
                menorTom = tom;
            }
        }
    }
    int tamanhoIntensidade = maiorTom - menorTom + 1;

    if(n >= tamanhoIntensidade){
        return img;
    }
    int tamanhoBin = tamanhoIntensidade/n;
    for (int y = 0; y < altura; y++) {
        for (int x = 0; x < largura; x++) {
            QColor cor(img.pixel(x, y));
            int tomOriginal = cor.red();
            int bin = (tomOriginal - menorTom) / tamanhoBin;
            int tomQuantizado = static_cast<int>(menorTom + (bin + 0.5) * tamanhoBin);
            cor.setRgb(tomQuantizado, tomQuantizado, tomQuantizado);
            imagemAlterada.setPixel(x, y, cor.rgb());
        }
    }
    return imagemAlterada;
}

QImage brilho(QImage img, int valor) {
    int largura = img.width();
    int altura = img.height();
    QImage imagemAlterada(largura, altura, QImage::Format_RGB32);
    for (int y = 0; y < altura; y++) {
        for (int x = 0; x < largura; x++) {
            QColor cor(img.pixel(x, y));
            int R = qBound(0, cor.red() + valor, 255);
            int G = qBound(0, cor.green() + valor, 255);
            int B = qBound(0, cor.blue() + valor, 255);
            cor.setRgb(R, G, B);
            imagemAlterada.setPixel(x, y, cor.rgb());
        }
    }

    return imagemAlterada;
}

QImage contraste(QImage img, int valor) {
    int largura = img.width();
    int altura = img.height();
    QImage imagemAlterada(largura, altura, QImage::Format_RGB32);
    for (int y = 0; y < altura; y++) {
        for (int x = 0; x < largura; x++) {
            QColor cor(img.pixel(x, y));
            int R = qBound(0, cor.red() , 255);
            int G = qBound(0, cor.green() + valor, 255);
            int B = qBound(0, cor.blue() + valor, 255);

            cor.setRgb(R, G, B);
            imagemAlterada.setPixel(x, y, cor.rgb());
        }
    }

    return imagemAlterada;
}

QImage negativo(QImage img) {
    int largura = img.width();
    int altura = img.height();
    QImage imagemAlterada(largura, altura, QImage::Format_RGB32);
    for (int y = 0; y < altura; y++) {
        for (int x = 0; x < largura; x++) {
            QColor cor(img.pixel(x, y));
            int R = 255 - cor.red();
            int G = 255 - cor.green();
            int B = 255 - cor.blue();

            cor.setRgb(R, G, B);
            imagemAlterada.setPixel(x, y, cor.rgb());
        }
    }

    return imagemAlterada;
}

#include <QPainter>
#include <QtGlobal>

QImage histograma(QImage img) {
    int vector[256] = {0};
    //QImage imgTonsDeCinza = conversaoCinza(img);
    int largura = img.width();
    int altura = img.height();
    for (int y = 0; y < altura; y++) {
        for (int x = 0; x < largura; x++) {
            QColor cor(img.pixel(x, y));
            vector[cor.value()]++;
        }
    }
    // Configuração do histograma
    int histSize = 256;
    // Encontre a contagem máxima para normalização
    int maxCount = 0;
    for (int i = 0; i < histSize; ++i) {
        maxCount = qMax(maxCount, vector[i]);
    }

    // Cria uma imagem para visualizar o histograma
    int imgWidth = 512, imgHeight = 400;
    QImage histogramaImage(imgWidth, imgHeight, QImage::Format_RGB32);
    histogramaImage.fill(Qt::white);

    // Desenha o histograma na imagem
    QPainter painter(&histogramaImage);
    painter.setRenderHint(QPainter::Antialiasing);

    for (int i = 0; i < histSize; ++i) {
        int binHeight = qRound(static_cast<double>(vector[i]) / maxCount * imgHeight);

        // Desenha a barra do histograma
        painter.fillRect(i * (imgWidth / histSize), imgHeight - binHeight,
                         imgWidth / histSize, binHeight, Qt::black);
    }

    painter.end();

    return histogramaImage;
}


QImage equalizarHistograma(QImage img) {
    int largura = img.width();
    int altura = img.height();
    QImage imagemAlterada(largura, altura, QImage::Format_RGB32);
    for (int y = 0; y < altura; y++) {
        for (int x = 0; x < largura; x++) {
            QColor cor(img.pixel(x, y));
            int R = 255 - cor.red();
            int G = 255 - cor.green();
            int B = 255 - cor.blue();

            cor.setRgb(R, G, B);
            imagemAlterada.setPixel(x, y, cor.rgb());
        }
    }

    return imagemAlterada;
}

void salvarImagem(QImage img){
    img.save("imagem_alterada.jpg");
}



void MainWindow::on_inverteVertical_clicked()
{
    imgOriginal = inverterImagemVertical(imgOriginal);
    //ui->labelImgAlterada->setPixmap(QPixmap::fromImage(imgOriginal));
    popupImgAlternada.setImg(imgOriginal);
}



void MainWindow::on_inverteHorizontal_clicked()
{
    imgOriginal = inverterImagemHorizontal(imgOriginal);
    //ui->labelImgAlterada->setPixmap(QPixmap::fromImage(imgOriginal));
    popupImgAlternada.setImg(imgOriginal);
}


void MainWindow::on_converteCinza_clicked()
{
    imgOriginal = conversaoCinza(imgOriginal);
    //ui->labelImgAlterada->setPixmap(QPixmap::fromImage(imgOriginal));
    popupImgAlternada.setImg(imgOriginal);
}


void MainWindow::on_quantizaCinza_clicked()
{
    imgOriginal = quantizacaoCinza(imgOriginal,numeroTons);
    //ui->labelImgAlterada->setPixmap(QPixmap::fromImage(imgOriginal));
    popupImgAlternada.setImg(imgOriginal);
}


void MainWindow::on_salvaImagem_clicked()
{
    imgOriginal.save( diretorioArq + "/imagem_alterada.jpg");
}


void MainWindow::on_tonsBox_valueChanged(int arg1)
{
    numeroTons = arg1;
}


void MainWindow::on_copiaImagem_clicked()
{
    imgOriginal = QImage(diretorioArq + arq);
    //ui->labelImgAlterada->setPixmap(QPixmap::fromImage(imgOriginal));
    popupImgAlternada.setImg(imgOriginal);
}

void MainWindow::on_brilhoBox_valueChanged(int arg1)
{
    numeroBrilho = arg1;
}


void MainWindow::on_brilho_clicked()
{
    imgOriginal = brilho(imgOriginal,numeroBrilho);
    //ui->labelImgAlterada->setPixmap(QPixmap::fromImage(imgOriginal));
    popupImgAlternada.setImg(imgOriginal);
}


void MainWindow::on_contrasteBox_valueChanged(int arg1)
{
    numeroContraste = arg1;
}


void MainWindow::on_contraste_clicked()
{
    imgOriginal = contraste(imgOriginal,numeroContraste);
    //ui->labelImgAlterada->setPixmap(QPixmap::fromImage(imgOriginal));
    popupImgAlternada.setImg(imgOriginal);
}


void MainWindow::on_negativo_clicked()
{
    imgOriginal = negativo(imgOriginal);
    //ui->labelImgAlterada->setPixmap(QPixmap::fromImage(imgOriginal));
    popupImgAlternada.setImg(imgOriginal);
}


void MainWindow::on_histograma_clicked()
{
    //converte pra cinza
    imgOriginal = conversaoCinza(imgOriginal);
    //ui->labelImgAlterada->setPixmap(QPixmap::fromImage(imgOriginal));
    popupImgAlternada.setImg(imgOriginal);

    //criar janela com historograma(já convertido)
    popup histograma1;
    histograma1.setImg(histograma(imgOriginal));
    //histograma1.setModal(true);
    histograma1.exec();



}


void MainWindow::on_equalizarHistograma_clicked()
{
    //converte pra cinza
    imgOriginal = conversaoCinza(imgOriginal);
    //ui->labelImgAlterada->setPixmap(QPixmap::fromImage(imgOriginal));
    popupImgAlternada.setImg(imgOriginal);

}

