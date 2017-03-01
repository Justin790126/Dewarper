#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    dewarper = Dewarper::GetInstance();

    SetSrcImageLabel();
    SetDewarpImageLabel();
}

void MainWindow::SetSrcImageLabel()
{
    src = imread(src_url,CV_LOAD_IMAGE_COLOR);
    if (src.empty()) {
        cerr << "load image failed" << endl;
    }
    dewarper->SetSource(src);

    img_src = MatToQImage(src);
    ui->lblOriginalImg->setPixmap(QPixmap::fromImage(*img_src));
}

QImage* MainWindow::MatToQImage(cv::Mat& img)
{
    cv::cvtColor(img, img, CV_RGB2BGR);
    return new QImage((uchar*)img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnLoadSrc_clicked()
{
    SetSrcImageLabel();
}

void MainWindow::on_btnDewarp_clicked()
{
    SetDewarpImageLabel();
}

void MainWindow::SetDewarpImageLabel()
{
    dewarper->Dewarp();
    dewarped_img = dewarper->GetDewarpedImage();
    img_dewarped = MatToQImage(dewarped_img);
    ui->lblDewarpedImg->setPixmap(QPixmap::fromImage(*img_dewarped));
}


void MainWindow::on_sldPan_valueChanged(int value)
{
    ui->lcdPan->display(value);
    dewarper->SetPan(value);
    SetDewarpImageLabel();
}

void MainWindow::on_sldTIlt_valueChanged(int value)
{
    ui->lcdTilt->display(value);
    dewarper->SetTilt(value);
    SetDewarpImageLabel();
}

void MainWindow::on_sldAccuracy_valueChanged(int value)
{
    ui->lcdAccuracy->display(value);
    dewarper->SetAccuracy(value);
    SetDewarpImageLabel();
}

void MainWindow::on_sldZoom_valueChanged(int value)
{
    ui->lcdZoom->display(value/10.0);
    dewarper->SetZoom(value/10.0);
    SetDewarpImageLabel();
}

void MainWindow::on_btnOpen_clicked()
{

    QFileDialog* fileDialog = new QFileDialog(this);
    fileDialog->setWindowTitle(tr("Open Image"));
    fileDialog->setNameFilter("Images (*.jpg)");
    fileDialog->setDirectory("./");

    if(fileDialog->exec() == QDialog::Accepted) {
        QString path = fileDialog->selectedFiles()[0];
        QByteArray array = path.toLocal8Bit();
        src_url = array.data();

        SetSrcImageLabel();
        SetDewarpImageLabel();
    } else {
        return;
    }
}
