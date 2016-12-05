#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int x_cd[8]={-1,-1,-1,1,1,1,0,0};
int y_cd[8]={-1,0,1,-1,0,1,-1,1};
int arr[8]={0};
void gaussian(Mat img,Mat &image);
int meanFilter(Mat img,int p,int q);
int medianFilter(Mat img,int p,int q);

int main()
{
    Mat img=imread("sample.jpg",IMREAD_GRAYSCALE);

    namedWindow( "Original window", WINDOW_AUTOSIZE );
    imshow( "Original window", img );

    Mat image(img.rows,img.cols,CV_8UC1, Scalar(255));

    gaussian(img,image);

    namedWindow( "Filter window", WINDOW_AUTOSIZE );
    imshow( "Filter window", image );
    imwrite( "image2.jpg", image );

    waitKey(0);
    return 0;
}

void gaussian(Mat img,Mat &image)
{
    for(int i=1;i<img.rows-1;i++)
    for(int j=1;j<img.cols-1;j++)
    {
       int value=meanFilter(img,i,j);
       //int value=medianFilter(img,i,j);
       image.at<uchar>(i,j)=value;
    }
}

int meanFilter(Mat img,int p,int q)
{
    int sum=0;
    Scalar intensity;
    for(int i=0;i<8;i++)
    {
        intensity=img.at<uchar>(p+x_cd[i],q+y_cd[i]);
        sum=sum+intensity[0];
    }
    return sum/8;
}

int medianFilter(Mat img,int p,int q)
{
    int sum=0;
    Scalar intensity;
    int k;
    for(int i=0;i<8;i++)
    {
        intensity=img.at<uchar>(p+x_cd[i],q+y_cd[i]);
        k=i-1;
        while(k>=0&&arr[k]>intensity[0])
        {
            arr[k+1]=arr[k];
            k--;
        }
        arr[k+1]=intensity[0];
    }
    return (arr[4]+arr[3])/2;
}
