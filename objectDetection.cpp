#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;
void findObject(Mat &image,Mat &image2,int row,int col);

int main()
{
    Mat image=imread("sample.jpg", IMREAD_GRAYSCALE);
    Mat image2=imread("markedSample.jpg", IMREAD_GRAYSCALE);
    int row=image.rows;
    int col=image.cols;


    namedWindow( "Original Image", WINDOW_AUTOSIZE );
    imshow( "Original Image", image );

    namedWindow( "Distorted Image", WINDOW_AUTOSIZE );
    imshow( "Distorted Image", image2 );

    findObject(image,image2,row,col);

    namedWindow( "Mirror Image", WINDOW_AUTOSIZE );
    imshow( "Mirror Image", image2 );
    waitKey(0);
    return 0;
}

void findObject(Mat &image, Mat &image2,int row,int col)
{
    for(int i=0;i<row;i++)
        for(int j=0;j<col;j++)
    {
        Scalar intensity1=image.at<uchar>(i,j);
        Scalar intensity2=image2.at<uchar>(i,j);
        if(intensity1.val[0]==intensity2.val[0])
        image2.at<uchar>(i,j)=255;
    }
}
