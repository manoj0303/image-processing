#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <opencv2/core/core.hpp>
using namespace cv;
using namespace std;

void convertToGray(Mat img, Mat &image);
int main()
{
    Mat img=imread("sample.jpg",IMREAD_COLOR);
    namedWindow("test");
    imshow("test",img);

    Mat image(img.rows,img.cols,CV_8UC1,Scalar(0));
    convertToGray(img,image);

    namedWindow("gray");
    imshow("gray",image);

    waitKey(0);
return 0;
}

void convertToGray(Mat img, Mat &image)
{
    for(int i=0;i<img.rows;i++)
        for(int j=0;j<img.cols;j++)
    {
        Vec3b intensity=img.at<Vec3b>(i,j);
        uchar grayIntensity=intensity[0]*0.11+intensity[1]*0.39+intensity[2]*0.5;
        image.at<uchar>(i,j)=grayIntensity;
    }
}
