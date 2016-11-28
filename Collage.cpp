#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;
void reduce_size(Mat img, Mat &image, int height_reducing_factor, int width_reducing_factor);
void addToCollage(Mat img, Mat &collage, int startRow, int startCol);

int main()
{
Mat img=imread("img.jpg");
Mat img2=imread("]img2.jpg");
Mat img3=imread("img3.jpg");
Mat img4=imread("img4.jpg");

Mat collage(600,600,CV_8UC3,Scalar(0,0,0));

Mat image(300,300,CV_8UC3, Scalar(0,0,0));

int height=300;
int width=300;

reduce_size(img,image,height,width);
addToCollage(image,collage,0,0);

reduce_size(img2,image,height,width);
addToCollage(image,collage,0,300);

reduce_size(img3,image,height,width);
addToCollage(image,collage,300,0);

reduce_size(img4,image,height,width);
addToCollage(image,collage,300,300);

namedWindow( "Display window", WINDOW_AUTOSIZE );
imshow( "Display window", collage );

imwrite("collage.jpg",collage);

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

void addToCollage(Mat img, Mat &collage, int startRow, int startCol)
{
    for(int i=0;i<300;i++)
    for(int j=0;j<300;j++)
    {
        Vec3b intensity = img.at<Vec3b>(i,j);
        float blue = intensity.val[0];
        float green = intensity.val[1];
        float red = intensity.val[2];

        collage.at<Vec3b>(i+startRow,j+startCol)={blue,green,red};
    }
}
