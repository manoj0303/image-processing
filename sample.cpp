#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

class ImageProcessing
{
    Mat image;
    char *imageType=NULL;

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
            Mat image2(height,width,CV_8UC3,Scalar(255,255,255));
            image=image2;
        }

        if(type=="gray")
        {
            Mat image2(height,width,CV_8UC1,Scalar(255));
            image=image2;
        }
        imageType=type;
    }

    int height();
    int width();
    Mat getImage();
    char* getImageType();
    void setImage(Mat img);
    ImageProcessing displayImage();
    ImageProcessing saveImage(char*);
    ImageProcessing toGray();
    ImageProcessing resizeImage(int height,int width,char *type);
    ImageProcessing sobel();
    ImageProcessing prewitt();
    ImageProcessing mirror();
    ImageProcessing findDifferenceImage(ImageProcessing);
    ImageProcessing meanFilter();
    ImageProcessing medianFilter();
    ImageProcessing grayHistogram();
    ImageProcessing colorHistogram();
    ImageProcessing simpleThreshold(int);
    ImageProcessing imageDilation(int,int);
    ImageProcessing imageErosion(int,int);
    ImageProcessing cloneImage();
};

int main()
{
    ImageProcessing image("sample.jpg","color");
    //ImageProcessing img2(100,100,"gray");
    //img2.displayImage();
    //ImageProcessing image2("markedSample.jpg","color");
    //image.toGray().displayImage();
    //image.resizeImage(600,600,"color").displayImage();
    //image.mirror().displayImage();
    //image.meanFilter().displayImage();
    // image.medianFilter().sobel().saveImage().displayImage();
    //image.findDifferenceImage(image2).displayImage();
    //image.colorHistogram().displayImage();
    //image.medianFilter().simpleThreshold(90).displayImage();
    image.toGray().simpleThreshold(90).imageDilation(17,17).imageErosion(17,17).displayImage().saveImage("morph.jpg");
    return 0;
}

int ImageProcessing::height()
{
    return image.rows;
}

int ImageProcessing::width()
{
    return image.cols;
}

char* ImageProcessing::getImageType()
{
    return imageType;
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

ImageProcessing ImageProcessing::displayImage()
{
    namedWindow("Image");
    imshow("Image",image);
    waitKey(0);
    return *this;
}

ImageProcessing ImageProcessing::saveImage(char* name)
{
    imwrite(name,image);
    return *this;
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

ImageProcessing ImageProcessing::findDifferenceImage(ImageProcessing targetImage)
{
    //Need to fix bugs
    Mat original;
    Mat target;

    if(imageType=="color")
        original=toGray().getImage();
    else
        original=getImage();

    char* targetImageType=targetImage.getImageType();

    if(targetImageType=="color")
        target=targetImage.toGray().getImage();
    else
        target=targetImage.getImage();

    ImageProcessing outputImage(original.rows,original.cols,"gray");
    Mat output=outputImage.getImage();

    for(int i=0;i<original.rows;i++)
        for(int j=0;j<original.cols;j++)
    {
        Scalar intensity1=original.at<uchar>(i,j);
        Scalar intensity2=target.at<uchar>(i,j);
        if(intensity1.val[0]!=intensity2.val[0])
        output.at<uchar>(i,j)=0;
    }
    return outputImage;
}

ImageProcessing ImageProcessing::meanFilter()
{
    Mat img;
    if(imageType=="color")
        img=toGray().getImage();
    else
        img=getImage();

    ImageProcessing filteredImage(img.rows,img.cols,"gray");
    Mat targetImage=filteredImage.getImage();

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

ImageProcessing ImageProcessing::medianFilter()
{
    Mat img;
    if(imageType=="color")
        img=toGray().getImage();
    else
        img=getImage();

    ImageProcessing filteredImage(img.rows,img.cols,"gray");
    Mat targetImage=filteredImage.getImage();

    int x_cd[8]={-1,-1,-1,1,1,1,0,0};
    int y_cd[8]={-1,0,1,-1,0,1,-1,1};
    int arr[8];

    for(int i=1;i<img.rows-1;i++)
    for(int j=1;j<img.cols-1;j++)
    {
    int value=0;
    Scalar intensity;
    int p;
    for(int k=0;k<8;k++)
    {
        intensity=img.at<uchar>(i+x_cd[k],j+y_cd[k]);
        p=k-1;
        while(p>=0&&arr[p]>intensity[0])
        {
            arr[p+1]=arr[p];
            p--;
        }
        arr[p+1]=intensity[0];
    }
    value=(arr[4]+arr[3])/2;
    targetImage.at<uchar>(i,j)=value;
    }
    return filteredImage;
}

ImageProcessing ImageProcessing::sobel()
{
    Mat img;
    if(imageType=="color")
        img=toGray().getImage();
    else
        img=getImage();

    ImageProcessing imageWithEdges(img.rows,img.cols,"gray");
    Mat targetImage=imageWithEdges.getImage();

    int matx[3][3]={-1,0,1,-2,0,2,-1,0,1};
    int maty[3][3]={1,2,1,0,0,0,-1,-2,-1};
    int x_cd[9]={-1,-1,-1,1,1,1,0,0,0};
    int y_cd[9]={-1,0,1,-1,0,1,-1,0,1};

    for(int i=1;i<img.rows-1;i++)
    for(int j=1;j<img.cols-1;j++)
    {
       int sumX=0;
       int sumY=0;
       Scalar intensity;

       for(int k=0;k<9;k++)
        {
            intensity=img.at<uchar>(i+x_cd[k],j+y_cd[k]);
            sumX=sumX+intensity[0]*matx[1+x_cd[k]][1+y_cd[k]];
            sumY=sumY+intensity[0]*maty[1+x_cd[k]][1+y_cd[k]];
        }
       targetImage.at<uchar>(i,j)=sqrt(sumX*sumX+sumY*sumY);
    }

    return imageWithEdges;
}

ImageProcessing ImageProcessing::prewitt()
{
    Mat img;
    if(imageType=="color")
        img=toGray().getImage();
    else
        img=getImage();

    ImageProcessing imageWithEdges(img.rows,img.cols,"gray");
    Mat targetImage=imageWithEdges.getImage();

    int matx[3][3]={-1,0,1,-1,0,1,-1,0,1};
    int maty[3][3]={-1,-1,-1,0,0,0,1,1,1};
    int x_cd[9]={-1,-1,-1,1,1,1,0,0,0};
    int y_cd[9]={-1,0,1,-1,0,1,-1,0,1};

    for(int i=1;i<img.rows-1;i++)
    for(int j=1;j<img.cols-1;j++)
    {
       int sumX=0;
       int sumY=0;
       Scalar intensity;

       for(int k=0;k<9;k++)
        {
            intensity=img.at<uchar>(i+x_cd[k],j+y_cd[k]);
            sumX=sumX+intensity[0]*matx[1+x_cd[k]][1+y_cd[k]];
            sumY=sumY+intensity[0]*maty[1+x_cd[k]][1+y_cd[k]];
        }
       targetImage.at<uchar>(i,j)=sqrt(sumX*sumX+sumY*sumY);
    }

    return imageWithEdges;
}

ImageProcessing ImageProcessing::grayHistogram()
{
    Mat img;
    if(imageType=="color")
    {
        img=toGray().getImage();
    }
    else
    {
        img=getImage();
    }
    int hist[256]={0};
    for(int i=0;i<img.rows;i++)
        for(int j=0;j<img.cols;j++)
    {
        Scalar intensity=img.at<uchar>(i,j);
        uchar index=intensity[0];
        hist[index]=hist[index]+1;
    }

    ImageProcessing histogram(img.rows,img.cols,"gray");
    Mat histogramImage=histogram.getImage();

    for(int i=0;i<256;i++)
    {
        int startRow=200;
        int startCol=300;
        for(int j=0;j<hist[i]/100;j++)
        {
            histogramImage.at<uchar>(startRow,startCol+i)=0;
            startRow--;
        }
    }
    return histogram;
}

ImageProcessing ImageProcessing::colorHistogram()
{
    Mat img=getImage();

    int r_hist[256]={0};
    int g_hist[256]={0};
    int b_hist[256]={0};

    for(int i=0;i<img.rows;i++)
        for(int j=0;j<img.cols;j++)
    {
        Vec3b intensity=img.at<Vec3b>(i,j);
        uchar b_index=intensity[0];
        uchar g_index=intensity[1];
        uchar r_index=intensity[2];

        b_hist[b_index]=b_hist[b_index]+1;
        g_hist[g_index]=g_hist[g_index]+1;
        r_hist[r_index]=r_hist[r_index]+1;
    }

    ImageProcessing histogram(1500,1000,"gray");
    Mat histogramImage=histogram.getImage();

    for(int i=0;i<256;i++)
    {
        int startRow=400;
        int startCol=50;
        for(int j=0;j<b_hist[i]/100;j++)
        {
            histogramImage.at<uchar>(startRow,startCol+i)=0;
            startRow--;
        }
    }

    for(int i=0;i<256;i++)
    {
        int startRow=400;
        int startCol=350;
        for(int j=0;j<g_hist[i]/100;j++)
        {
            histogramImage.at<uchar>(startRow,startCol+i)=0;
            startRow--;
        }
    }

    for(int i=0;i<256;i++)
    {
        int startRow=400;
        int startCol=650;
        for(int j=0;j<r_hist[i]/100;j++)
        {
            histogramImage.at<uchar>(startRow,startCol+i)=0;
            startRow--;
        }
    }
    return histogram;
}

ImageProcessing ImageProcessing::simpleThreshold(int threshold)
{
    ImageProcessing newImage(image.rows,image.cols,"gray");
    Mat img=newImage.getImage();

    for(int i=0;i<image.rows;i++)
        for(int j=0;j<image.cols;j++)
    {
        if(image.at<uchar>(i,j)<threshold)
        {
            img.at<uchar>(i,j)=0;
        }
        else
        {
            img.at<uchar>(i,j)=255;
        }
    }
    return newImage;
}

ImageProcessing ImageProcessing::imageDilation(int row,int col)
{
    int struc_element[row][col];

    int midx=row/2;
    int midy=col/2;
    bool flag=false;
    ImageProcessing newImage(image.rows,image.cols,"gray");
    Mat img=newImage.getImage();
    for(int i=midx;i<image.rows-midx;i++)
    {
        for(int j=midy;j<image.cols-midy;j++)
        {
        //0:Object , 255:Background
        if(image.at<uchar>(i,j)==255)
        {
            for(int p=i-midx;p<i+midx+1;p++)
            {
            for(int q=j-midy;q<j+midy+1;q++)
            {
                if(image.at<uchar>(p,q)==0)
                {
                    flag=true;
                    break;
                }
            }
                if(flag)
                    break;
            }
            if(flag)
                img.at<uchar>(i,j)=0;
            else
                img.at<uchar>(i,j)=255;
        }
        else
        {
            img.at<uchar>(i,j)=0;
        }
        flag=false;
        }
    }
    return newImage;
}

ImageProcessing ImageProcessing::imageErosion(int row,int col)
{
    int struc_element[row][col];

    int midx=row/2;
    int midy=col/2;
    bool flag=false;
    ImageProcessing newImage(image.rows,image.cols,"gray");
    Mat img=newImage.getImage();
    for(int i=midx;i<image.rows-midx;i++)
    {
        for(int j=midy;j<image.cols-midy;j++)
        {
        //0:Object , 255:Background
        if(image.at<uchar>(i,j)==0)
        {
            for(int p=i-midx;p<i+midx+1;p++)
            {
            for(int q=j-midy;q<j+midy+1;q++)
            {
                if(image.at<uchar>(p,q)==255)
                {
                    flag=true;
                    break;
                }
            }
                if(flag)
                    break;
            }
            if(flag)
                img.at<uchar>(i,j)=255;
            else
                img.at<uchar>(i,j)=0;
        }
        else
        {
            img.at<uchar>(i,j)=255;
        }
        flag=false;
        }
    }
    return newImage;
}

ImageProcessing ImageProcessing::cloneImage()
{
    int ROWS=image.rows;
    int COLS=image.cols;
    Mat img;

    if(imageType=="gray")
    {
    ImageProcessing newImage=ImageProcessing(ROWS,COLS,"gray");
    img=newImage.getImage();

    for(int i=0;i<ROWS;i++)
        for(int j=0;j<COLS;j++)
    {
        img.at<uchar>(i,j)=image.at<uchar>(i,j);
    }
    return newImage;
    }
    else
    {
    ImageProcessing newImage=ImageProcessing(ROWS,COLS,"color");
    img=newImage.getImage();
    for(int i=0;i<ROWS;i++)
        for(int j=0;j<COLS;j++)
    {
        Vec3b intensity=image.at<Vec3b>(i,j);
        uchar blue = intensity[0];
        uchar green = intensity[1];
        uchar red = intensity[2];
        img.at<Vec3b>(i,j)={blue,green,red};
    }
    return newImage;
    }
}
