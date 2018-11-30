
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

int get_gray_pics( vector<vector<vector<short int>>>& pic, char* file){

    VideoCapture cap(file); // open video file
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    Mat frame;
    cap >> frame; // get a new frame from camera
    int frames = cap.get(CV_CAP_PROP_FRAME_COUNT);
    int rows=frame.rows;
    int cols=frame.cols;
    cout<<"rows "<<rows<<" cols "<<cols<<endl;

    for(int w=0;w<frames; w++){ //get gray set of images
        Mat backgr_img;
        cvtColor( frame, backgr_img, CV_RGB2GRAY);
        for(int j=0; j<rows;j++){
            for(int i=0; i<cols;i++){
                pic[w][j][i]=(int)backgr_img.at<uchar>(j,i);
            }
        }
        cap >> frame;
    }
    return 0;
}


