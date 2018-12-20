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



void learn(vector<short int>& pic, vector<double>& disp, vector<double>& mean, vector<double>& alfa, double& appr){
    int length = pic.size();
   //alfa
    for(int w=0;w<length;w++){
                alfa[w]=1.0/ (1.0 + (((1.0-appr)*disp[0])/(appr*disp[1]))* exp( -(1.0/2.) * ( pow(pic[w]-mean[1],2)/disp[1]*disp[1] - pow(pic[w]-mean[0],2)/disp[0]*disp[0])));
            if (alfa[w]==0)
                alfa[w]=0.00000001;
            if (alfa[w]!=alfa[w])
                alfa[w]=0.00000001;
    }
    //appr
            double alfa0=0;
            double alfa1=0;
            for(int w=0;w<length;w++)
                alfa0+=alfa[w];
            appr=alfa0/(double)length;
    //mean
             alfa0=0;
             alfa1=0;
            double alfa00=0;
            double alfa11=0;
            for(int w=0;w<length;w++){
                alfa0+=alfa[w]*pic[w];
                alfa1+=(1-alfa[w])*pic[w];
                alfa00+=alfa[w];
                alfa11+=(1-alfa[w]);
            }
            mean[0]=alfa0/alfa00;
            mean[1]=alfa1/alfa11;
    //disp
             alfa0=0;
             alfa1=0;
             alfa00=0;
             alfa11=0;
            for(int w=0;w<length;w++){
                alfa0+=alfa[w]*pow((pic[w]-mean[0]),2);
                alfa1+=(1-alfa[w])*pow((pic[w]-mean[1]),2);
                alfa00+=alfa[w];
                alfa11+=(1-alfa[w]);
            }
            disp[0]=sqrt(alfa0/alfa00);
            disp[1]=sqrt(alfa1/alfa11);
            if (disp[0]!=disp[0])
                disp[0]=0.00000001;
            if (disp[1]!=disp[1])
                disp[1]=0.00000001;
            if (disp[0]==1)
                disp[0]=0.00000001;
            if (disp[1]==0)
                disp[1]=0.00000001;



}
void classify(vector<vector<vector<short int>>>& pic, vector<vector<vector<double>>>& disp, vector<vector<vector<double>>>& mean, vector<vector<vector<double>>>& alfa, vector<vector<double>>& appr, int iter, int thread, int n_t){
    int rows = pic.size();
    int cols = pic[0].size();
    int st_row=(thread-1)*rows/n_t;
    rows=thread*rows/n_t;
    for(int j=st_row; j<rows;j++){
        for(int i=0; i<cols;i++){
            mean[j][i][0]=100;//pic[5][j][i];
            mean[j][i][1]=5;//pic[9][j][i];
            disp[j][i][0]=0.25;
            disp[j][i][1]=20;
            appr[j][i]=0.4;
        }
    }
for(int t=0;t<iter;t++) {
    for(int j=st_row; j<rows;j++){
        for(int i=0; i<cols;i++){
            learn(pic[j][i], disp[j][i], mean[j][i],alfa[j][i],appr[j][i]);
        }
    }
cout<<"Iterr: "<<t<<endl;
}
}
