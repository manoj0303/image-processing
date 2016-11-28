#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;
void reduce_size(Mat img, Mat &image, int height, int width);

int main()
{
Mat img=imread("img.jpg");
int row=img.rows;
int col=img.cols;

namedWindow( "Original window", WINDOW_AUTOSIZE );
imshow( "Original window", img );

int height=300;
int width=400;

Mat image(height,width,CV_8UC3, Scalar(0,0,0));
reduce_size(img,image,height,width);

namedWindow( "Display window", WINDOW_AUTOSIZE );
imshow( "Display window", image );

imwrite("resized.jpg",image);

waitKey(0);
return 0;
}

void reduce_size(Mat img, Mat &image, int height, int width)
{
    int row=img.rows;
    int col=img.cols;

    int height_reducing_factor=row/height;
    int width_reducing_factor=col/width;
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            Vec3b intensity = img.at<Vec3b>(height_reducing_factor*i, width_reducing_factor*j);
            float blue = intensity.val[0];
            float green = intensity.val[1];
            float red = intensity.val[2];
            image.at<Vec3b>(i,j)={blue,green,red};
        }
    }
}
