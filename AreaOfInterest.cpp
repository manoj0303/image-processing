#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;
int ROW,COL;
Mat areaOfInterest(Mat,int,int,int,int,int,int,int,int);
int main()
{
    Mat image=imread("bing.jpg",IMREAD_COLOR);
    ROW=image.rows;
    COL=image.cols;
    int innerPadding=min(ROW/5,COL/5);
    int outerPadding=min(ROW/10,COL/10);

    Mat img=areaOfInterest(image,outerPadding,outerPadding,ROW-outerPadding,COL-outerPadding,innerPadding,innerPadding,ROW-innerPadding,COL-innerPadding);

    namedWindow("Final Image");
    imshow("Final Image",img);
    imwrite("frame.jpg",img);
    waitKey(0);
return 0;
}

Mat areaOfInterest(Mat image,int O_startRow,int O_startCol,int O_endRow,int O_endCol,int I_startRow,int I_startCol,int I_endRow,int I_endCol)
{
    Mat img(ROW,COL,CV_8UC3,Scalar(255,255,255));
    for(int i=O_startRow;i<=O_endRow;i++)
    for(int j=O_startCol;j<=O_endCol;j++)
    {
        if(!(i>I_startRow&&i<I_endRow&&j>I_startCol&&j<I_endCol))
        {
            Vec3b intensity=image.at<Vec3b>(i,j);
            uchar blue=intensity[0];
            uchar green=intensity[1];
            uchar red=intensity[2];
            img.at<Vec3b>(i,j)={blue,green,red};
        }
    }
    return img;
}
