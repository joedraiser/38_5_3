#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QGraphicsScene>
#include <QGraphicsBlurEffect>
#include <QGraphicsPixmapItem>
#include <QPainter>

QImage blurImage(QImage source, int blurValue)
{
    if (source.isNull()) return QImage();
    QGraphicsScene scene;
    QGraphicsPixmapItem item;
    item.setPixmap(QPixmap::fromImage(source));

    auto* blur = new QGraphicsBlurEffect;
    blur->setBlurRadius(blurValue);
    item.setGraphicsEffect(blur);
    scene.addItem(&item);
    QImage result(source.size(), QImage::Format_ARGB32);
    result.fill(Qt::transparent);
    QPainter painter(&result);
    scene.render(&painter, QRectF(), QRectF(0, 0, source.width(), source.height()));
    return result;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    blurSlider=ui->blurSlider;
    openFileButton=ui->openFile;
    image=ui->image;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openFileFunc()
{
    QString filePath;
    filePath = QFileDialog::getOpenFileName(
                    nullptr,
                    "Open",
                    "C:/",
                    "images (*.jpg , *.jpeg)");

    initImage=QImage(filePath);
    QImage blurredImage = blurImage(initImage, blurSlider->value());
    image->setPixmap(QPixmap::fromImage(blurredImage));
}

void MainWindow::newBlurLevel(int blurLevel)
{
    QImage blurredImage = image->pixmap()->toImage();
    blurredImage = blurImage(initImage, blurLevel);
    image->setPixmap(QPixmap::fromImage(blurredImage));
}
