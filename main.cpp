

#include <QCoreApplication>
#include <dewarper.h>



int main(int argc, char* argv[]) {
    char* src_url = "./C1ZZV_w_Marks.jpg";

    Mat src = imread(src_url,CV_LOAD_IMAGE_COLOR);
    if (src.empty()) {
        cerr << "load image failed" << endl;
        return -1;
    }

    Dewarper* dewarper = Dewarper::GetInstance();
    dewarper->SetSource(src);
    dewarper->Dewarp();
    dewarper->ShowResult();

    return 0;
}
