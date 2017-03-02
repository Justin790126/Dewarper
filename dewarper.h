#ifndef DEWARPER_H
#define DEWARPER_H

#define _USE_MATH_DEFINES

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <cmath>
#include <iostream>


using namespace std;
using namespace cv;


class Dewarper
{
public:
    static Dewarper* GetInstance();

    void SetSource(cv::Mat& src);
    void SetPan(float new_pan);
    void SetZoom(float new_zoom);
    void SetTilt(float new_tilt);
    void SetAccuracy(float new_accuracy);
    void SetFOVx(float new_fovx);
    void SetFOVy(float new_fovy);
    void CreateMap();
    void Dewarp();
    void ShowResult();

    cv::Mat GetDewarpedImage(int bit = 8) {
        if (bit == 8) dst.convertTo(dst, CV_8UC3, 255.0);
        return dst;
    }
    cv::Mat GetOriginalImage(int bit = 8) {
        if (bit == 8) img32f.convertTo(img32f,CV_8UC3, 255.0);
        return img32f;
    }

private:

    Dewarper();

    cv::Mat backup_src;
    cv::Mat dst;
    cv::Mat img32f;
    cv::Mat map_x;
    cv::Mat map_y;

    float fov = M_PI * (187/180.0);
    float f;
    float x0;
    float y0;
    int accuracy = 4;

    float Thetaud(float x, float y);
    float R(float x, float y);
    float GetImageHeight(float fov, int new_accuracy = 4);
    float toRadians(float degree) {
        return (degree/180.0) * M_PI;
    }

    float pan = 0, tilt = 0, zoom = 1;
    float fovx_ratio = 1.0;
    float fovy_ratio = 1.1;

    char* dewarped_window_name = "Dewarped image";

};

#endif // DEWARPER_H
