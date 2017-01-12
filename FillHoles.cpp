#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int visited[9999][9999];
int matx[]={-1,-1,-1,1,1,1,0,0};
int maty[]={-1,0,1,-1,0,1,-1,1};
int ROW,COL;
Mat img;
Mat simpleThreshold(Mat image);

void dfs(int i, int j)
{
    visited[i][j]=1;
    img.at<uchar>(i,j)=0;

    for(int p=0;p<8;p++)
    {
        if(i+matx[p]>=0&&i+matx[p]<ROW&&j+maty[p]>=0&&j+maty[p]<COL)
        if((img.at<uchar>(i+matx[p],j+maty[p])==255)&&!visited[i+matx[p]][j+maty[p]])
        {
            dfs(i+matx[p],j+maty[p]);
        }
    }
}

int main()
{
    Mat image=imread("textBlock.png",IMREAD_GRAYSCALE);
    ROW=image.rows;
    COL=image.cols;
    img=simpleThreshold(image);

    for(int i=0;i<ROW;i++)
        for(int j=0;j<COL;j++)
    {
        visited[i][j]=0;
    }

    for(int i=0;i<ROW;i++)
    {
        for(int j=0;j<COL;j++)
        {
            if(!visited[i][j]&&img.at<uchar>(i,j)==255)
            {
                dfs(i,j);
                i=ROW;
                j=COL;
            }
        }
    }

    for(int i=0;i<ROW;i++)
        for(int j=0;j<COL;j++)
            {
                if(img.at<uchar>(i,j)==255)
                    image.at<uchar>(i,j)=0;
            }

    namedWindow("test");
    imshow("test",image);
    imwrite("mod.png",image);
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
