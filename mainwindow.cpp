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
    int pan = -1 * value;
    ui->lcdPan->display(-pan);
    dewarper->SetPan(pan);
    SetDewarpImageLabel();
}

void MainWindow::on_sldTIlt_valueChanged(int value)
{
    int tilt = 1 * value;
    ui->lcdTilt->display(tilt);
    dewarper->SetTilt(tilt);
    SetDewarpImageLabel();
}

void MainWindow::on_sldAccuracy_valueChanged(int value)
{
    int accuracy = value;
    ui->lcdAccuracy->display(accuracy);
    dewarper->SetAccuracy(accuracy);
    SetDewarpImageLabel();
}

void MainWindow::on_sldZoom_valueChanged(int value)
{
    float zoom = value/10.0;
    ui->lcdZoom->display(zoom);
    dewarper->SetZoom(zoom);
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

void MainWindow::on_sldFOVx_valueChanged(int value)
{
    float fovx = value/10.0;
    ui->lcdFOVx->display(fovx);
    dewarper->SetFOVx(fovx);
    SetDewarpImageLabel();
}

void MainWindow::on_sldFOVy_valueChanged(int value)
{
    float fovy = value/10.0;
    ui->lcdFOVy->display(fovy);
    dewarper->SetFOVy(fovy);
    SetDewarpImageLabel();
}
