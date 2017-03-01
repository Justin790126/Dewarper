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
    void CreateMap();
    void Dewarp();
    void ShowResult();

private:

    Dewarper();

    cv::Mat backup_src;
    cv::Mat dst;
    cv::Mat img32f;
    cv::Mat map_x;
    cv::Mat map_y;

    float fov = 2*M_PI;
    float f;
    float x0;
    float y0;

    float Thetaud(float x, float y);
    float R(float x, float y);
    float GetImageHeight(float fov);
    float toRadians(float degree) {
        return (degree/180.0) * M_PI;
    }
    float toAlgorithmFOV(float degree) {
        return toRadians(degree) + M_PI;
    }

    char* dewarped_window_name = "Dewarped image";

};

#endif // DEWARPER_H
