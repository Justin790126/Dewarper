#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QLabel>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <dewarper.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnLoadSrc_clicked();

    void on_btnDewarp_clicked();

    void on_sldPan_valueChanged(int value);

    void on_sldTIlt_valueChanged(int value);

    void on_sldAccuracy_valueChanged(int value);


    void on_sldZoom_valueChanged(int value);

    void on_btnOpen_clicked();

private:
    Ui::MainWindow *ui;

    char* src_url = "./C1ZZV_w_Marks.jpg";
    Dewarper* dewarper;

    cv::Mat src;
    cv::Mat dewarped_img;
    QImage* img_src;
    QImage* img_dewarped;

    QImage* MatToQImage(cv::Mat& img);

    void SetSrcImageLabel();
    void SetDewarpImageLabel();
};

#endif // MAINWINDOW_H
