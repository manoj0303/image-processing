#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;


int matx[3][3]={-1,0,1,-2,0,2,-1,0,1};
int maty[3][3]={1,2,1,0,0,0,-1,-2,-1};

int x_cd[9]={-1,-1,-1,1,1,1,0,0,0};
int y_cd[9]={-1,0,1,-1,0,1,-1,1,0};

void sobel(Mat img,Mat &image);
int setSum(Mat img,int p,int q,int matrix[3][3]);

int arr[8]={0};
void gaussian(Mat img,Mat &image);
int meanFilter(Mat img,int p,int q);
int medianFilter(Mat img,int p,int q);

int main()
{
    Mat img=imread("Bikesgray.jpg",IMREAD_GRAYSCALE);

    namedWindow( "Original window", WINDOW_AUTOSIZE );
    imshow( "Original window", img );

    Mat image(img.rows,img.cols,CV_8UC1, Scalar(255));

    gaussian(img,image);

    Mat image2(img.rows,img.cols,CV_8UC1, Scalar(255));

    sobel(image,image2);

    namedWindow( "Filter window", WINDOW_AUTOSIZE );
    imshow( "Filter window", image2 );

    imwrite("filteredSobel.jpg",image2);

    waitKey(0);
    return 0;
}

void gaussian(Mat img,Mat &image)
{
    for(int i=1;i<img.rows-1;i++)
    for(int j=1;j<img.cols-1;j++)
    {
       //int value=meanFilter(img,i,j);
       int value=medianFilter(img,i,j);
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

void sobel(Mat img,Mat &image)
{
    for(int i=1;i<img.rows-1;i++)
    for(int j=1;j<img.cols-1;j++)
    {
       int value1=setSum(img,i,j,matx);
       int value2=setSum(img,i,j,maty);
       image.at<uchar>(i,j)=sqrt(value1*value1+value2*value2);
    }
}

int setSum(Mat img,int p,int q,int matrix[3][3])
{
    int sum=0;
    Scalar intensity;
    for(int i=0;i<9;i++)
    {
        intensity=img.at<uchar>(p+x_cd[i],q+y_cd[i]);
        sum=sum+intensity[0]*matrix[1+x_cd[i]][1+y_cd[i]];
    }
    return sum;
}
