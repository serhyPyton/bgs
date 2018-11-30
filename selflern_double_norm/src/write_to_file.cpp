
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <math.h>
#include <sstream>
using namespace cv;
using namespace std;

void write_video(const vector<vector<vector<double>>>& disp, const vector<vector<vector<short int>>>& pic, const vector<vector<vector<double>>>& alfa, Mat& frame, int frames){
   double cols = frame.cols; //get the width of frames of the video
   double rows = frame.rows; //get the height of frames of the video

   cout << "Frame Size = " << cols << "x" << rows << endl;

   Size frameSize(static_cast<int>(cols), static_cast<int>(rows));

    VideoWriter oVideoWriter ("mask.avi", CV_FOURCC('P','I','M','1'), 20, frameSize, false); //initialize the VideoWriter object
    VideoWriter oVideoWriter2 ("gray.avi", CV_FOURCC('P','I','M','1'), 20, frameSize, false); //initialize the VideoWriter object

   if ( !oVideoWriter.isOpened() ) //if not initialize the VideoWriter successfully, exit the program
   {
        cout << "ERROR: Failed to write the video" << endl;
   }
    Mat img;
    Mat backgr_img;
        cvtColor( frame, img, CV_RGB2GRAY);
        cvtColor( frame, backgr_img, CV_RGB2GRAY);

    for (int w=0;w<frames;w++){
            for (int j=0;j<rows;j++){
                for (int i=0;i<cols;i++){
                    if ((alfa[j][i][w]>0.5 && disp[j][i][1]>disp[j][i][0]) || (alfa[j][i][w]<0.5 && disp[j][i][1]<disp[j][i][0]))
                        backgr_img.at<uchar>(j,i)=0;
                    else
                        backgr_img.at<uchar>(j,i)=(int)255*(alfa[j][i][w]);
                    /*
                    if (alfa[j][i][w]<0.5)
                        backgr_img.at<uchar>(j,i)=0;
                    else
                        backgr_img.at<uchar>(j,i)=(int)255*(alfa[j][i][w]);*/
                    img.at<uchar>(j,i)=pic[w][j][i];
                }
            }
            oVideoWriter2.write(img);
            oVideoWriter.write(backgr_img);
    }
}



int gen_backgr(const vector<vector<vector<double>>>& mean, const vector<vector<vector<double>>>& disp, vector<vector<double>>& appr, int rows, int cols){
    Mat img(rows,cols,false);
    for (int j=0;j<rows;j++)
        for (int i=0;i<cols;i++)
            img.at<uchar>(j,i)=mean[j][i][0];
    imwrite("mean_0.jpg", img);
    for (int j=0;j<rows;j++)
        for (int i=0;i<cols;i++)
            img.at<uchar>(j,i)=mean[j][i][1];
    imwrite("mean_1.jpg", img);
    for (int j=0;j<rows;j++)
        for (int i=0;i<cols;i++){
             if (disp[j][i][0]<disp[j][i][1] && appr[j][i]>0.7) img.at<uchar>(j,i)=mean[j][i][0];
             else img.at<uchar>(j,i)=mean[j][i][1];
            }
    imwrite("mean_with_disp_0.jpg", img);
    for (int j=0;j<rows;j++)
        for (int i=0;i<cols;i++){
             if (disp[j][i][0]>disp[j][i][1]) img.at<uchar>(j,i)=mean[j][i][0];
             else img.at<uchar>(j,i)=mean[j][i][1];
            }
    imwrite("mean_with_disp_1.jpg", img);

    return 0;
}

