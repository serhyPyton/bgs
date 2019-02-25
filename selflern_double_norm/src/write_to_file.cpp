
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
                    if ((abs(alfa[j][i][w]-0.5)>0.3)&&disp[j][i][0]>2){
                        backgr_img.at<uchar>(j,i)=255;}
                    else{
                        backgr_img.at<uchar>(j,i)=0;}//(int)255*(alfa[j][i][w]);
                    /*
                    if (alfa[j][i][w]<0.5)
                        backgr_img.at<uchar>(j,i)=0;
                    else
                        backgr_img.at<uchar>(j,i)=(int)255*(alfa[j][i][w]);*/
                    img.at<uchar>(j,i)=pic[j][i][w];
                }
            }
            oVideoWriter2.write(img);
            oVideoWriter.write(backgr_img);
    }
}



Mat gen_backgr(vector<vector<vector<double>>>& mean, vector<vector<vector<double>>>& disp, vector<vector<double>>& appr, int rows, int cols, vector<vector<vector<double>>>& alfa){
    Mat img(rows,cols,false);
    for (int j=0;j<rows;j++){
        for (int i=0;i<cols;i++){
            if (appr[j][i]>0.5){
            img.at<uchar>(j,i)=mean[j][i][0];}
            else{
            img.at<uchar>(j,i)=mean[j][i][1];
            }
        }
    }

    imwrite("mean_0.jpg", img);
    Mat imgrgb(rows,cols,CV_8SC3);
    double max=0;
    for (int j=0;j<rows;j++){
        for (int i=0;i<cols;i++){
        if (disp[j][i][0]>max)max=disp[j][i][0];}}
    for (int j=0;j<rows;j++){
        for (int i=0;i<cols;i++){
        imgrgb.at<Vec3b>(j,i)[2]= mean[j][i][0];
        imgrgb.at<Vec3b>(j,i)[1]= (disp[j][i][0]/max)*255;
        imgrgb.at<Vec3b>(j,i)[0]= 0;
        }}
    imwrite("mean_00.jpg", imgrgb);
    for (int j=0;j<rows;j++){
        for (int i=0;i<cols;i++){
        imgrgb.at<Vec3b>(j,i)[2]= mean[j][i][0];
        imgrgb.at<Vec3b>(j,i)[1]= (disp[j][i][1]/max)*255;
        imgrgb.at<Vec3b>(j,i)[0]= 0;
        }}
    imwrite("mean_01.jpg", imgrgb);
    for (int j=0;j<rows;j++){
        for (int i=0;i<cols;i++){
            if (disp[j][i][0]<disp[j][i][1])
                if (disp[j][i][0]>0.5)
                    img.at<uchar>(j,i)=mean[j][i][0];
                else
                    img.at<uchar>(j,i)=mean[j][i][1];
            if (disp[j][i][1]<disp[j][i][0])
                if (disp[j][i][1]>0.5)
                    img.at<uchar>(j,i)=mean[j][i][1];
                else
                    img.at<uchar>(j,i)=mean[j][i][0];
        }}
    imwrite("mean.jpg", img);
    for (int j=0;j<rows;j++){
        for (int i=0;i<cols;i++){
        img.at<uchar>(j,i)=mean[j][i][1];}}
    imwrite("mean_11.jpg", img);
    return img;
 /*   for (int j=0;j<rows;j++)
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
    imwrite("mean_with_disp_1.jpg", img);*/

}

void gen_obj(const vector<vector<vector<short int>>>& pic, const Mat backgr){
//gen video of object. background - source image >? n
    int rows = pic.size();
    int cols = pic[0].size();
    int len  = pic[0][0].size();
    Mat img(rows,cols,false);
    Size frameSize(static_cast<int>(cols), static_cast<int>(rows));
    VideoWriter oVideoWriter2 ("obj.avi", CV_FOURCC('P','I','M','1'), 20, frameSize, false); //initialize the VideoWriter object
    for (int w=0;w<len;w++){
        for (int j=0;j<rows;j++){
            for (int i=0;i<cols;i++){
                if (abs(pic[j][i][w]-(int)backgr.at<uchar>(j,i))>w%30)
                   img.at<uchar>(j,i)=pic[j][i][w];
                else
                   img.at<uchar>(j,i)=0;
            }
        }
        oVideoWriter2.write(img);
    }
}

void gen_obj2(vector<vector<vector<double>>>& disp, vector<vector<vector<double>>>& alfa, const vector<vector<vector<short int>>>& pic){
    int rows = pic.size();
    int cols = pic[0].size();
    int len  = pic[0][0].size();
    Mat img(rows,cols,false);
    Size frameSize(static_cast<int>(cols), static_cast<int>(rows));
    VideoWriter oVideoWriter2 ("obj.avi", CV_FOURCC('P','I','M','1'), 20, frameSize, false); //initialize the VideoWriter object
    for (int w=0;w<len;w++){
        for (int j=0;j<rows;j++){
            for (int i=0;i<cols;i++){
                if(disp[j][i][0]-disp[j][i][1]>1){
                int k=0;
                    if (disp[j][i][0]>disp[j][i][1]) k=1;
                if (abs(k-alfa[j][i][w])>0.5)
                    img.at<uchar>(j,i)=pic[j][i][w];
                else
                    img.at<uchar>(j,i)=0;
                }
                else img.at<uchar>(j,i)=0;
            }
        }
        oVideoWriter2.write(img);
    }
}
