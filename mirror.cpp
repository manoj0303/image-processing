#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;
void mirrorImage(Mat &image,Mat &image2,int row,int col);

int main()
{
    Mat image=imread("sample.jpg", IMREAD_GRAYSCALE);
    int row=image.rows;
    int col=image.cols;

    Mat image2(row,col,CV_8UC1,Scalar(255));

    namedWindow( "Original Image", WINDOW_AUTOSIZE );
    imshow( "Original Image", image );

    mirrorImage(image,image2,row,col);

    namedWindow( "Mirror Image", WINDOW_AUTOSIZE );
    imshow( "Mirror Image", image2 );
    waitKey(0);
    return 0;
}

void mirrorImage(Mat &image,Mat &image2,int row,int col)
{
    for(int i=0;i<row;i++)
        for(int j=0;j<col;j++)
    {
        Scalar intensity1=image.at<uchar>(i,j);
        image2.at<uchar>(i,col-j-1)=intensity1.val[0];
    }
}
