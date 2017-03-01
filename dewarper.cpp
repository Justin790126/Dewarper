#include "dewarper.h"

Dewarper* Dewarper::GetInstance() {
    static Dewarper* instance = new Dewarper();
    return instance;
}

Dewarper::Dewarper() {}

void Dewarper::SetSource(cv::Mat& src)
{
    backup_src = src.clone();

    backup_src.convertTo(img32f, CV_32FC3, 1/255.0);

    dst = cv::Mat(backup_src.size(), backup_src.type());

    f = dst.size().width / fov;

    x0 = dst.rows / 2.0;
    y0 = dst.cols / 2.0;

    CreateMap();
}

void Dewarper::SetPan(float new_pan)
{
    if (new_pan != pan) {
        pan = f * tan(toRadians(new_pan));
        CreateMap();
    }
}

void Dewarper::SetTilt(float new_tilt)
{
    if (new_tilt != tilt) {
        tilt = f * tan(toRadians(new_tilt));
        CreateMap();
    }
}
void Dewarper::SetAccuracy(float new_accuracy)
{
    if (new_accuracy != accuracy) {
        accuracy = new_accuracy;
        CreateMap();
    }
}

void Dewarper::SetZoom(float new_zoom)
{
    if (new_zoom != zoom) {
        zoom = new_zoom;
        CreateMap();
    }
}

void Dewarper::CreateMap()
{
    map_x.create(dst.size(), CV_32FC1);
    map_y.create(dst.size(), CV_32FC1);

    for (int i = 0; i < dst.rows; i++) {
        for (int j = 0; j < dst.cols; j++) {
            float x = i + pan;
            float y = j + tilt;
            float u = i;
            float v = j;

            float fov = Thetaud(x, y);
            float fovx = fov;
            float fovy = fov;
            float radial_distance = zoom * R(x,y);

            u = x0 + (i - x0) * GetImageHeight(fovx, accuracy) / radial_distance;
            v = y0 + (j - y0) * GetImageHeight(fovy, accuracy) / radial_distance;

            map_x.at<float>(j, i) = static_cast<float>(u);
            map_y.at<float>(j, i) = static_cast<float>(v);

        }
    }

}

float Dewarper::Thetaud(float x, float y)
{
    float theta_dewarped_in_radians = std::atan(R(x, y)/f);
    return theta_dewarped_in_radians;
}

float Dewarper::R(float x, float y)
{
    float radius_to_center = sqrt(pow(x - x0, 2) + pow(y - y0, 2));
    return radius_to_center;
}

float Dewarper::GetImageHeight(float fov, int new_accuracy)
{
    // Write distortion function here
    accuracy = new_accuracy;

    float a0 = f;
    float a1 = 0.0252459377 * f;
    float a2 = 0.00792063849 * f;
    float a3 = -0.00144634191 * f;
    float a4 = 0.0000558271591 * f;

    if (accuracy == 0) {
        a1 = a2 = a3 = a4 = 0;
    } else if (accuracy == 1) {
        a2 = a3 = a4 = 0;
    } else if (accuracy == 2) {
        a3 = a4 = 0;
    } else if (accuracy == 3) {
        a4 = 0;
    } else if (accuracy == 4) {

    }

    float image_height = a0 * fov + a1 * pow(fov, 2) + a2 * pow(fov, 4) +
            a3 * pow(fov, 6) + a4 * pow(fov, 8);
    return image_height;
}

void Dewarper::Dewarp()
{
    cv::remap(img32f, dst, map_x, map_y, CV_INTER_CUBIC, BORDER_CONSTANT, Scalar(0, 0, 0));
}


void Dewarper::ShowResult()
{
    namedWindow("Origin", CV_WINDOW_NORMAL);
    imshow("Origin", img32f);

    namedWindow(dewarped_window_name, CV_WINDOW_NORMAL);
    imshow(dewarped_window_name, dst);

    waitKey(0);
    destroyAllWindows();

}
