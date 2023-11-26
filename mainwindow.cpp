#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QLabel>
#include <QString>
#include <QImage>
#include <QVector>
//#include <funcoes.h>

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
    //Funcoes funcoes;
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

QVector<int> calcularHistograma(QImage imagem) {
    QVector<int> histograma(256, 0);

    for (int y = 0; y < imagem.height(); ++y) {
        for (int x = 0; x < imagem.width(); ++x) {
            int tomDeCinza = qRed(imagem.pixel(x, y));
            ++histograma[tomDeCinza];
        }
    }

    return histograma;
}

QImage equalizarHistograma(const QImage &inputImage) {
    QVector<int> histograma = calcularHistograma(inputImage);

    QVector<int> histogramaCumulativo(256, 0);
    histogramaCumulativo[0] = histograma[0];
    for (int i = 1; i < 256; ++i) {
        histogramaCumulativo[i] = histogramaCumulativo[i - 1] + histograma[i];
    }

    double fatorNormalizacao = 255.0 / (inputImage.width() * inputImage.height());
    for (int i = 0; i < 256; ++i) {
        histogramaCumulativo[i] = static_cast<int>(fatorNormalizacao * histogramaCumulativo[i]);
    }

    QImage imagemEqualizada = inputImage;

    for (int y = 0; y < inputImage.height(); ++y) {
        for (int x = 0; x < inputImage.width(); ++x) {
            int tomDeCinza = qRed(inputImage.pixel(x, y));
            int novoTomDeCinza = histogramaCumulativo[tomDeCinza];
            imagemEqualizada.setPixel(x, y, qRgb(novoTomDeCinza, novoTomDeCinza, novoTomDeCinza));
        }
    }

    return imagemEqualizada;
}

QImage criarImagemHistograma(const QVector<int> &histograma) {
    int largura = 256;
    int altura = 200;
    QImage histogramaImage(largura, altura, QImage::Format_RGB32);
    int valorMaximo = *std::max_element(histograma.begin(), histograma.end());

    for (int i = 0; i < 256; ++i) {
        int alturaBarra = static_cast<int>(static_cast<double>(histograma[i]) / valorMaximo * altura);

        for (int y = 0; y < altura; ++y) {
            QRgb cor = (y < alturaBarra) ? qRgb(255, 255, 255) : qRgb(0, 0, 0);
            histogramaImage.setPixel(i, altura - y - 1, cor);
        }
    }

    return histogramaImage;
}
void salvarImagem(QImage img){
    img.save("imagem_alterada.jpg");
}

QImage zoomOut(QImage img,int fatorSX,int fatorSY){
    int largura = img.width()/fatorSX;
    int altura = img.height()/fatorSY;
    QImage imagemAlterada(largura, altura, QImage::Format_RGB32);
    for (int y = 0; y < altura; y++) {
        for (int x = 0; x < largura; x++) {

            int xEntrada = static_cast<int>(x * fatorSX);
            int yEntrada = static_cast<int>(y * fatorSY);
            QRgb cor = img.pixel(xEntrada, yEntrada);
            imagemAlterada.setPixel(x, y, cor);
        }
    }

    return imagemAlterada;
}

QImage zoomIn(QImage img){
    int largura = img.width()*2;
    int altura = img.height()*2;
    QImage imagemAlterada(largura, altura, QImage::Format_RGB32);

    for (int y = 0; y < altura; y++) {
        for (int x = 0; x < largura; x++) {
            int xEntrada = x / 2;
            int yEntrada = y / 2;
            QRgb cor = img.pixel(xEntrada, yEntrada);
            imagemAlterada.setPixel(x, y, cor);
        }
    }

    for (int y = 1; y < altura - 1; y += 2) {
        for (int x = 0; x < largura; x++) {
            QRgb cor1 = imagemAlterada.pixel(x, y - 1);
            QRgb cor2 = imagemAlterada.pixel(x, y + 1);
            //interpolar uniao
            int uniaoVermelho = (qRed(cor1) + qRed(cor2)) / 2;
            int uniaoVerde = (qGreen(cor1) + qGreen(cor2)) / 2;
            int uniaoAzul = (qBlue(cor1) + qBlue(cor2)) / 2;

            imagemAlterada.setPixel(x, y, qRgb(uniaoVermelho, uniaoVerde, uniaoAzul));
        }
    }

    for (int y = 0; y < altura; y++) {
        for (int x = 1; x < largura - 1; x += 2) {
            QRgb cor1 = imagemAlterada.pixel(x - 1, y);
            QRgb cor2 = imagemAlterada.pixel(x + 1, y);
            int uniaoVermelho = (qRed(cor1) + qRed(cor2)) / 2;
            int uniaoVerde = (qGreen(cor1) + qGreen(cor2)) / 2;
            int uniaoAzul = (qBlue(cor1) + qBlue(cor2)) / 2;
            imagemAlterada.setPixel(x, y, qRgb(uniaoVermelho, uniaoVerde, uniaoAzul));
        }
    }

    return imagemAlterada;
}

QImage rotaEsq(QImage img){
    int largura = img.width();
    int altura = img.height();
    QImage imagemAlterada(altura, largura, QImage::Format_RGB32);

    for (int y = 0; y < altura; ++y) {
        for (int x = 0; x < largura; ++x) {
            int xEntrada = y;
            int yEntrada = largura - 1 - x;
            QRgb color = img.pixel(x, y);
            imagemAlterada.setPixel(xEntrada, yEntrada, color);
        }
    }
    return imagemAlterada;
}

QImage rotaDir(QImage img){
    int largura = img.width();
    int altura = img.height();
    QImage imagemAlterada(altura, largura, QImage::Format_RGB32);

    for (int y = 0; y < altura; ++y) {
        for (int x = 0; x < largura; ++x) {
            int xEntrada = altura - 1 - y;
            int yEntrada = x;
            QRgb color = img.pixel(x, y);
            imagemAlterada.setPixel(xEntrada, yEntrada, color);
        }
    }
    return imagemAlterada;
}

QImage convolucao(QImage img,double filtro[3][3]) {
    QImage resultado = img;

    for (int y = 1; y < img.height() - 1; ++y) {
        for (int x = 1; x < img.width() - 1; ++x) {
            int soma = 0;

            for (int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    QRgb pixel = img.pixel(x - j, y - i);
                    double peso = filtro[i + 1][j + 1];
                    soma += qRed(pixel) * peso;
                }
            }

            if (soma < 0) soma = 0;
            else if (soma > 255) soma = 255;

            if (filtro[1][1] != -1 && filtro[1][1] != 0 && filtro[1][1] != 1) {
                soma += 127;
                if (soma < 0) soma = 0;
                else if (soma > 255) soma = 255;
            }

            QRgb novoPixel = qRgb(soma, soma, soma);
            resultado.setPixel(x, y, novoPixel);
        }
    }

    return resultado;
}
//**


void MainWindow::on_inverteVertical_clicked()
{
    imgOriginal = inverterImagemVertical(imgOriginal);
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
    popupImgAlternada.setImg(imgOriginal);

    //criar janela com historograma(já convertido)
    popup histograma1;
    histograma1.setImg(criarImagemHistograma(calcularHistograma(imgOriginal)));
    //histograma1.setModal(true);
    histograma1.exec();



}


void MainWindow::on_equalizarHistograma_clicked()
{
    //converte pra cinza
    imgOriginal = conversaoCinza(imgOriginal);
    imgOriginal = equalizarHistograma(imgOriginal);
    popupImgAlternada.setImg(imgOriginal);

    //criar janela com historograma(já convertido)
    popup histograma1;
    QVector<int> histogramaEqualizado = calcularHistograma(imgOriginal);
    histograma1.setImg(criarImagemHistograma(histogramaEqualizado));
    histograma1.exec();


}


void MainWindow::on_zoomOut_clicked()
{
    imgOriginal = zoomOut(imgOriginal,fatorSX,fatorSY);
    popupImgAlternada.setImg(imgOriginal);
}


void MainWindow::on_zoomIn_clicked()
{
    imgOriginal = zoomIn(imgOriginal);
    //int largura = imgOriginal.width();
    //int altura = imgOriginal.height();

    popupImgAlternada.setImg(imgOriginal);
    //popupImgAlternada.window()->resize(largura,altura);
}


void MainWindow::on_girarEsquerda_clicked()
{
    imgOriginal = rotaEsq(imgOriginal);
    popupImgAlternada.setImg(imgOriginal);
}


void MainWindow::on_girarDireita_clicked()
{
    imgOriginal = rotaDir(imgOriginal);
    popupImgAlternada.setImg(imgOriginal);
}


void MainWindow::on_fatorSX_valueChanged(int arg1)
{
    fatorSX = arg1;
}


void MainWindow::on_fatorSY_valueChanged(int arg1)
{
    fatorSY = arg1;
}


void MainWindow::on_convolucao_clicked()
{
    if (filtroEntrada == "Gaussiano") {
        double gaussiano[3][3] = {
            {0.0625 , 0.125, 0.0625},
            {0.125, 0.25, 0.125},
            {0.0625, 0.125, 0.0625}
        };
        imgOriginal = convolucao(imgOriginal,gaussiano);
        popupImgAlternada.setImg(imgOriginal);
    } else if (filtroEntrada == "Laplaciano") {
        double laplaciano[3][3] = {
            {0, -1, 0},
            {-1, 4, -1},
            {0, -1, 0}
        };
        imgOriginal = convolucao(imgOriginal,laplaciano);
        popupImgAlternada.setImg(imgOriginal);
    } else if (filtroEntrada == "Passa Altas Generico") {
        double passaAltasGenerico[3][3] = {
            {-1, -1,-1},
            {-1, 8, -1},
            {-1, -1,-1}
        };
        imgOriginal = convolucao(imgOriginal,passaAltasGenerico);
        popupImgAlternada.setImg(imgOriginal);
    } else if (filtroEntrada == "Prewitt Hx") {
        double prewittHX[3][3] = {
            {-1, 0, 1},
            {-1, 0, 1},
            {-1, 0, 1}
        };
        imgOriginal = convolucao(imgOriginal,prewittHX);
        popupImgAlternada.setImg(imgOriginal);
    } else if (filtroEntrada == "Prewitt Hy") {
        double prewittHY[3][3] = {
            {-1,-1,-1},
            {0, 0, 0},
            {1, 1, 1}
        };
        imgOriginal = convolucao(imgOriginal,prewittHY);
        popupImgAlternada.setImg(imgOriginal);
    } else if (filtroEntrada == "Sobel Hx") {
        double sobelHX[3][3] = {
            {-1,-1,-1},
            {-2, 0, 2},
            {1, 1, 1}
        };
        imgOriginal = convolucao(imgOriginal,sobelHX);
        popupImgAlternada.setImg(imgOriginal);
    } else if (filtroEntrada == "Sobel Hy") {
        double sobelHY[3][3] = {
            {-1,-2,-1},
            {0, 0, 0},
            {1, 2, 1}
        };
        imgOriginal = convolucao(imgOriginal,sobelHY);
        popupImgAlternada.setImg(imgOriginal);
    }
    else{
        double gaussiano[3][3] = {
            {0.0625 , 0.125, 0.0625},
            {0.125, 0.25, 0.125},
            {0.0625, 0.125, 0.0625}
        };
        imgOriginal = convolucao(imgOriginal,gaussiano);
        popupImgAlternada.setImg(imgOriginal);
    }
}


void MainWindow::on_convulacaoLista_currentTextChanged(const QString &arg1)
{
    filtroEntrada = arg1;
}

