
#include <opencv2/opencv.hpp>   //for get_gray_pics
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <random>       //for gen_rand and teast write
#include <vector>
#include <chrono>
#include <algorithm>
#include <fstream>


using namespace cv;
using namespace std;

int get_gray_pics( vector<vector<vector<short int>>>& pic, char* file){   //read frim file and
                                                                        //transform images to pic
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
                pic[j][i][w]=(int)backgr_img.at<uchar>(j,i);
            }
        }
        cap >> frame;
    }
    return 0;
}


void gen_rand(vector<short int>& pic, double mean1, double disp1, int n1, double mean2, double disp2, int n2){
            std::random_device rd;

    std::mt19937 e2(rd());

    std::normal_distribution<> dist(mean1, disp1);
    std::normal_distribution<> dist1(mean2, disp2);

    for(int w=0;w<n1; w++){ //get gray set of images
                pic[w]= (int)dist(e2);
              //  std::cout << pic[w]<<' ';
        }
    for (int w=n1;w<n1+n2; w++) {
                pic[w]= (int)dist1(e2);
              //  std::cout << pic[w]<<' ';
        }
unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
shuffle (pic.begin(), pic.end(), std::default_random_engine(seed));
}

void test(vector<vector<vector<short int>>>& pic, double mean1, double disp1, int n1, double mean2, double disp2, int n2){
        for (int j=0; j<10; j++){
            for (int i=0;i<10;i++){
                gen_rand(pic[j][i], mean1, disp1, n1, mean2, disp2, n2);
            }
        }
}

void write_test(vector<vector<double>>& appr, vector<vector<vector<double>>>& disp, vector<vector<vector<double>>>& mean, int rows, int cols){
    ofstream file;
    file.open("result.txt");
    for (int j=0;j<rows;j++){
        for (int i=0; i<cols;i++){
            file << appr[j][i]<<";"<<disp[j][i][0]<<";"<<mean[j][i][0]<<";0"<<endl;
            file << 1-appr[j][i]<<";"<<disp[j][i][1]<<";"<<mean[j][i][1]<<";1"<<endl;
        }
    }
  /*  file <<"pic"<<endl;
    for (int j=0;j<10;j++){
        for (int i=0; i<10;i++){
            file << pic[j][i][2]<<" ";
        }
        file << endl;
    }
    file <<"mean_1"<<endl;
    for (int j=0;j<10;j++){
        for (int i=0; i<10;i++){
            file << mean[j][i][1]<<" ";
        }
        file << endl;
    }
    file <<"mean_0"<<endl;
    for (int j=0;j<10;j++){
        for (int i=0; i<10;i++){
            file << mean[j][i][0]<<" ";
        }
        file << endl;
    }
    file <<"disp_1"<<endl;
    for (int j=0;j<10;j++){
        for (int i=0; i<10;i++){
            file << disp[j][i][1]<<" ";
        }
        file << endl;
    }
    file <<"disp_0"<<endl;
    for (int j=0;j<10;j++){
        for (int i=0; i<10;i++){
            file << disp[j][i][0]<<" ";
        }
        file << endl;
    }*/
    file.close();
}
