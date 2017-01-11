#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int visited[9999][9999];
int matx[]={-1,-1,-1,1,1,1,0,0};
int maty[]={-1,0,1,-1,0,1,-1,1};
int maxx=0,minx=10000,maxy=0,miny=10000;
int ROW,COL;
Mat img;
Mat finalImage;
Mat simpleThreshold(Mat image);
void drawBoundary(int minx,int miny,int maxx,int maxy,Mat img);
void dfs(int i, int j)
{
        if(i>=maxx)
            maxx=i;
        if(i<=minx)
            minx=i;
        if(j>=maxy)
            maxy=j;
        if(j<=miny)
            miny=j;

    visited[i][j]=1;
    for(int p=0;p<8;p++)
    {
        if(i+matx[p]>=0&&i+matx[p]<ROW&&j+maty[p]>=0&&j+maty[p]<COL)
        if((img.at<uchar>(i+matx[p],j+maty[p])==0)&&!visited[i+matx[p]][j+maty[p]])
            dfs(i+matx[p],j+maty[p]);
    }
}

int main()
{
    Mat image=imread("NeutraText.png",IMREAD_GRAYSCALE);
    ROW=image.rows;
    COL=image.cols;
    img=simpleThreshold(image);
    imwrite("objects.jpg",img);

    int counter=0;
    for(int i=0;i<ROW;i++)
        for(int j=0;j<COL;j++)
    {
        visited[i][j]=0;
    }

    for(int i=0;i<ROW;i++)
        for(int j=0;j<COL;j++)
    {
        if(!visited[i][j]&&img.at<uchar>(i,j)==0)
        {
            dfs(i,j);
            counter++;
            //cout<<"minx: "<<minx<<" miny: "<<miny<<" maxx: "<<maxx<<" maxy: "<<maxy<<endl;
            drawBoundary(minx,miny,maxx,maxy,img);
            maxx=0,minx=10000,maxy=0,miny=10000;
        }
    }
    cout<<"Total Objects in Image: "<<counter;
    namedWindow("Final Image");
    imshow("Final Image",finalImage);
    waitKey(0);
return 0;
}

Mat simpleThreshold(Mat image)
{
    Mat img(ROW,COL,CV_8UC1,Scalar(255));

    for(int i=0;i<image.rows;i++)
        for(int j=0;j<image.cols;j++)
    {
        if(image.at<uchar>(i,j)<90)
        {
            img.at<uchar>(i,j)=0;
        }
        else
        {
            img.at<uchar>(i,j)=255;
        }
    }
    return img;
}

void drawBoundary(int minx,int miny,int maxx,int maxy,Mat image)
{
finalImage=imread("objects.jpg",IMREAD_UNCHANGED);

int padding=2;

int j=miny-padding;
for(int i=minx-padding;i<=maxx+padding;i++)
finalImage.at<uchar>(i,j)=0;

j=maxy+padding;
for(int i=minx-padding;i<=maxx+padding;i++)
finalImage.at<uchar>(i,j)=0;

int i=minx-padding;
for(int j=miny-padding;j<=maxy+padding;j++)
finalImage.at<uchar>(i,j)=0;

i=maxx+padding;
for(int j=miny-padding;j<=maxy+padding;j++)
finalImage.at<uchar>(i,j)=0;

imwrite("objects.jpg",finalImage);
}
