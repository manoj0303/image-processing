#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

class ImageProcessing
{
    Mat image;
    char *imageType=NULL;

    //int matx[3][3]={-1,0,1,-2,0,2,-1,0,1};
    //int maty[3][3]={1,2,1,0,0,0,-1,-2,-1};
    //int x_cd[9]={-1,-1,-1,1,1,1,0,0,0};
    //int y_cd[9]={-1,0,1,-1,0,1,-1,1,0};

    //helper functions

public:

    ImageProcessing(char *path,char *type)
    {
        if(type=="color")
        image=imread(path,IMREAD_COLOR);

        if(type=="gray")
        image=imread(path,IMREAD_GRAYSCALE);

        imageType=type;
    }

    ImageProcessing(int height,int width,char *type)
    {
        if(type=="color")
        {
            Mat image2(height,width,CV_8UC3,Scalar(0,0,0));
            image=image2;
        }

        if(type=="gray")
        {
            Mat image2(height,width,CV_8UC1,Scalar(0));
            image=image2;
        }
        imageType=type;
    }

    int height();
    int width();
    Mat getImage();
    void setImage(Mat img);
    void displayImage();
    void saveImage();
    ImageProcessing toGray();
    ImageProcessing resizeImage(int height,int width,char *type);
    ImageProcessing sobel();
    ImageProcessing prewitt();
    ImageProcessing mirror();
    ImageProcessing findDifferenceImage(ImageProcessing);
    ImageProcessing meanFilter();
    ImageProcessing medianFilter();
};

int main()
{
    ImageProcessing image("sample.jpg","color");
    //ImageProcessing image2("markedSample.jpg","color");
    //image.toGray().displayImage();
    //image.resizeImage(300,100,"color").displayImage();
   // image.mirror().displayImage();
      image.meanFilter();
 //   image.toGray().findDifferenceImage(image2.toGray()).displayImage();
}

int ImageProcessing::height()
{
    return image.rows;
}

int ImageProcessing::width()
{
    return image.cols;
}

Mat ImageProcessing::getImage()
{
    return image;
}

ImageProcessing ImageProcessing::toGray()
{
    ImageProcessing grayImage(image.rows,image.cols,"gray");
    Mat img=grayImage.getImage();

    for(int i=0;i<image.rows;i++)
        for(int j=0;j<image.cols;j++)
    {
        Vec3b intensity=image.at<Vec3b>(i,j);
        uchar grayIntensity=intensity[0]*0.11+intensity[1]*0.39+intensity[2]*0.5;
        img.at<uchar>(i,j)=grayIntensity;
    }
    return grayImage;
}

void ImageProcessing::displayImage()
{
    namedWindow("Image");
    imshow("Image",image);
    waitKey(0);
}

void ImageProcessing::saveImage()
{
    imwrite("image.jpg",image);
}

ImageProcessing ImageProcessing::resizeImage(int height,int width,char *type)
{
    ImageProcessing resizedImage(height,width,type);
    Mat img=resizedImage.getImage();

    int rowsInOriginal=image.rows;
    int colsInOriginal=image.cols;
    int height_reducing_factor=rowsInOriginal/height;
    int width_reducing_factor=colsInOriginal/width;

    if(type=="color")
    {
        for(int i=0;i<height;i++)
        for(int j=0;j<width;j++)
        {
            Vec3b intensity = image.at<Vec3b>(height_reducing_factor*i, width_reducing_factor*j);
            uchar blue = intensity[0];
            uchar green = intensity[1];
            uchar red = intensity[2];
            img.at<Vec3b>(i,j)={blue,green,red};
        }
    }

    if(type=="gray")
    {
        for(int i=0;i<height;i++)
        for(int j=0;j<width;j++)
        {
            Scalar intensity = image.at<uchar>(height_reducing_factor*i, width_reducing_factor*j);
            uchar gray = intensity[0];
            img.at<uchar>(i,j)=gray;
        }
    }
    return resizedImage;
}

void ImageProcessing::setImage(Mat img)
{
    image=img;
}

ImageProcessing ImageProcessing::mirror()
{
    ImageProcessing mirrorImage(image.rows,image.cols,imageType);
    Mat img=mirrorImage.getImage();

    if(imageType=="color")
    {
        for(int i=0;i<image.rows;i++)
        for(int j=0;j<image.cols;j++)
        {
            Vec3b intensity = image.at<Vec3b>(i,j);
            uchar blue = intensity[0];
            uchar green = intensity[1];
            uchar red = intensity[2];
            img.at<Vec3b>(i,image.cols-j-1)={blue,green,red};
        }
    }

    if(imageType=="gray")
    {
        for(int i=0;i<image.rows;i++)
        for(int j=0;j<image.cols;j++)
        {
            Scalar intensity=image.at<uchar>(i,j);
            img.at<uchar>(i,image.cols-j-1)=intensity[0];
        }
    }
    return mirrorImage;
}
/*
ImageProcessing ImageProcessing::findDifferenceImage(ImageProcessing targetImage)
{

    //need to fix bugs
    Mat target=targetImage.getImage();

    ImageProcessing outputImage(image.rows,image.cols,"gray");
    Mat output=outputImage.getImage();
    for(int i=0;i<image.rows;i++)
        for(int j=0;j<image.cols;j++)
    {
        Scalar intensity1=image.at<uchar>(i,j);
        Scalar intensity2=target.at<uchar>(i,j);
        if(intensity1.val[0]!=intensity2.val[0])
        output.at<uchar>(i,j)=255;
    }
    return outputImage;
}
*/
ImageProcessing ImageProcessing::meanFilter()
{
    Mat img=toGray().getImage();

    ImageProcessing filteredImage(img.rows,img.cols,"gray");
    Mat targetImage=filteredImage.getImage();
    //filteredImage.displayImage();

    int x_cd[8]={-1,-1,-1,1,1,1,0,0};
    int y_cd[8]={-1,0,1,-1,0,1,-1,1};

    for(int i=1;i<img.rows-1;i++)
    for(int j=1;j<img.cols-1;j++)
    {
    int value=0;
    Scalar intensity;
    for(int k=0;k<8;k++)
    {
        intensity=img.at<uchar>(i+x_cd[k],j+y_cd[k]);
        value+=intensity[0];
    }
    value=value/8;
    targetImage.at<uchar>(i,j)=value;
    }
    return filteredImage;
}
