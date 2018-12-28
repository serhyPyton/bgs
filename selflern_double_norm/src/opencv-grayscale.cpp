#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <math.h>
#include <thread>

#include "write_to_file.cpp"
#include "stuff.cpp"
#include "learn.cpp"

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{

    cout<< "using video: "<<argv[1]<<endl;

    VideoCapture cap(argv[1]); // open video file
    if(!cap.isOpened()) { // check if we succeeded
        cout<<"     file reading failed!"<<endl;
        return -1;}
    Mat frame;
    cap >> frame; // get a new frame from camera
    int frames = cap.get(CV_CAP_PROP_FRAME_COUNT); //get a number of frames
    cout<< "frames: "<< frames<<endl;

    int rows=frame.rows;
    int cols=frame.cols;

    vector<vector<vector<double>>>      mean    (rows, vector<vector<double>>(cols, vector<double>(2)));
    vector<vector<vector<double>>>      disp    (rows, vector<vector<double>>(cols, vector<double>(2)));
    vector<vector<double>>              appr    (rows, vector<double>(cols));
    vector<vector<vector<short int>>>   pic     (rows, vector<vector<short int>>(cols, vector<short int>(frames)));
    vector<vector<vector<double>>>      alfa    (rows, vector<vector<double>>(cols, vector<double>(frames)));
    vector<vector<int>>                 clas    (rows, vector<int>(cols));

    //test(pic, 40, 3, 200, 80, 5, 100);
    get_gray_pics(pic, argv[1]);
    //  cut_video(argv[1]);

    //classify(pic,disp,mean,alfa,appr,rows,cols,frames,q,w,n_t);
    thread t1(classify, ref(pic), ref(disp), ref(mean), ref(alfa), ref(appr), 4, 1, 2);
    thread t2(classify, ref(pic), ref(disp), ref(mean), ref(alfa), ref(appr), 4, 2, 2);
    t1.join();
    t2.join();

    Mat backgr;
    backgr = gen_backgr(mean, disp, appr, rows, cols, alfa);
    gen_obj(pic,backgr);
    write_test(appr, disp, mean, rows, cols);
    write_video(disp, pic, alfa, frame, frames);
    return 0;
//1.23
//0.42
}
