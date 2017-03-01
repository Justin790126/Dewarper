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

    f = 2 * dst.size().width / fov;

    x0 = dst.rows / 2.0;
    y0 = dst.cols / 2.0;

    CreateMap();
}

void Dewarper::CreateMap()
{
    map_x.create(dst.size(), CV_32FC1);
    map_y.create(dst.size(), CV_32FC1);

    for (int i = 0; i < dst.rows; i++) {
        for (int j = 0; j < dst.cols; j++) {
            float u = i;
            float v = j;

            float fov = Thetaud(i, j);
            float fovx = fov;
            float fovy = 1.1*fov;
            float radial_distance = R(i,j);

            u = x0 + (i - x0) * GetImageHeight(fovx) / radial_distance;
            v = y0 + (j - y0) * GetImageHeight(fovy) / radial_distance;

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

float Dewarper::GetImageHeight(float fov)
{
    // Write distortion function here
    float a0 = f;
    float a1 = 0.0252459377 * f;
    float a2 = 0.00792063849 * f;
    float a3 = -0.00144634191 * f;
    float a4 = 0.0000558271591 * f;

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
