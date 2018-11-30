#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <fstream>
//#include "stuff.cpp"

using namespace cv;
using namespace std;

 void write_files(vector<vector<vector<double>>>& disp, vector<vector<vector<double>>>& mean, vector<vector<vector<double>>>& alfa, vector<vector<double>>& appr, int rows, int cols){

 ofstream res;
 res.open("resuls.txt");

 res << "Iter 1"<<endl;

res << "mean for 0"<<endl;
for (int j=0; j<rows; j++){
    for (int i=0; i<cols; i++){
        res << mean[j][i][0]<<" ";
    }
    res << endl;
}
res << "mean for 1"<<endl;
for (int j=0; j<rows; j++){
    for (int i=0; i<cols; i++){
        res << mean[j][i][1]<<" ";
    }
    res << endl;
}

res << "disp for 0"<<endl;
for (int j=0; j<rows; j++){
    for (int i=0; i<cols; i++){
        res << disp[j][i][0]<<" ";
    }
    res << endl;
}
res << "disp for 1"<<endl;
for (int j=0; j<rows; j++){
    for (int i=0; i<cols; i++){
        res << disp[j][i][1]<<" ";
    }
    res << endl;
}
res << "appr for 0"<<endl;
for (int j=0; j<rows; j++){
    for (int i=0; i<cols; i++){
        res << appr[j][i]<<" ";
    }
    res << endl;
}
res << "appr for 1"<<endl;
for (int j=0; j<rows; j++){
    for (int i=0; i<cols; i++){
        res << 1-appr[j][i]<<" ";
    }
    res << endl;
}

 }

 void write_mean(int rows, int cols, int k, const vector<vector<vector<double>>>& mean, const vector<vector<vector<double>>>& disp,  const vector<vector<double>>& appr){
    Mat img(rows,cols,false);
     for (int j=0;j<rows;j++)
        for (int i=0;i<cols;i++){
             if (disp[j][i][0]<disp[j][i][1] && appr[j][i]>0.7) img.at<uchar>(j,i)=mean[j][i][0];
             else img.at<uchar>(j,i)=mean[j][i][1];
            }
        std::stringstream ss;
        ss << "mens/mean_with_disp_" << k << ".jpg";
    imwrite(ss.str(), img);
 }

void learn(vector<vector<vector<short int>>>& pic, vector<vector<vector<double>>>& disp, vector<vector<vector<double>>>& mean, vector<vector<vector<double>>>& alfa, vector<vector<double>>& appr, int rows, int cols, int length, int iter, int thread, int n_t){
    int st_row=(thread-1)*rows/n_t;
    rows=thread*rows/n_t;

    for(int j=st_row; j<rows;j++){
        for(int i=0; i<cols;i++){
            mean[j][i][0]=pic[5][j][i];
            mean[j][i][1]=pic[9][j][i];
            disp[j][i][0]=3;
            disp[j][i][1]=20;
            appr[j][i]=0.4;
        }
    }

for(int t=0;t<iter;t++) {

   //alfa
        for(int w=0;w<length;w++){
        for(int j=st_row; j<rows;j++){
        for(int i=0; i<cols;i++){
            alfa[j][i][w]=1.0/ (1.0 + (((1.0-appr[j][i])*disp[j][i][0])/(appr[j][i]*disp[j][i][1]))* exp( -(1.0/2) * ( pow(pic[w][j][i]-mean[j][i][1],2)/disp[j][i][1]*disp[j][i][1] - pow(pic[w][j][i]-mean[j][i][0],2)/disp[j][i][0]*disp[j][i][0])));
        }
        }
        }
    //appr
    for(int j=st_row; j<rows;j++){
        for(int i=0; i<cols;i++){
            double alfa0=0;
            double alfa1=0;
            for(int w=0;w<length;w++){
                alfa0+=alfa[j][i][w];
               // alfa1+=alfa[j][i][w][1];
            }
            appr[j][i]=alfa0/(double)length;
           // appr[j][i][1]=1-appr[j][i][0];//alfa1/200.0;
        }
    }
    //mean
    for(int j=st_row; j<rows;j++){
        for(int i=0; i<cols;i++){
            double alfa0=0;
            double alfa1=0;
            double alfa00=0;
            double alfa11=0;
            for(int w=0;w<length;w++){
                alfa0+=alfa[j][i][w]*pic[w][j][i];
                alfa1+=(1-alfa[j][i][w])*pic[w][j][i];
                alfa00+=alfa[j][i][w];
                alfa11+=(1-alfa[j][i][w]);
            }
            mean[j][i][0]=alfa0/alfa00;
            mean[j][i][1]=alfa1/alfa11;
        }
    }
   // write_mean(rows,cols,t,mean,disp,appr);
    //disp
    for(int j=st_row; j<rows;j++){
        for(int i=0; i<cols;i++){
            double alfa0=0;
            double alfa1=0;
            double alfa00=0;
            double alfa11=0;
            for(int w=0;w<length;w++){
                alfa0+=alfa[j][i][w]*pow((pic[w][j][i]-mean[j][i][0]),2);
                alfa1+=(1-alfa[j][i][w])*pow((pic[w][j][i]-mean[j][i][1]),2);
                alfa00+=alfa[j][i][w];
                alfa11+=(1-alfa[j][i][w]);
            }
            disp[j][i][0]=sqrt(alfa0/alfa00);
            disp[j][i][1]=sqrt(alfa1/alfa11);
        }
    }

cout<<"iter: "<<t<<endl;
//write_files(disp, mean, alfa, appr, rows, cols);
}
//return 0;
}
