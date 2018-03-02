#include <opencv2/opencv.hpp>
#include <opencv2/ximgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

void calc_optical_flow( Mat& im1, Mat& im2 )
{
    UMat uflow;
    Mat flow;
    int numIter = 140;
    int numLevels = 5;

    calcOpticalFlowFarneback(im1, im2, uflow, 0.9, numLevels, numIter, 3, 8, 1.2, 0);
    uflow.copyTo(flow);

    int step = 10;
    for( int y = 0; y < im1.rows; y+=step )
    {
        for( int x = 0; x < im1.cols; x+=step )
        {
            const Point2f atxy = flow.at<Point2f>(y,x)*5;
            line(im1, Point(x,y), Point(x+atxy.x, y+atxy.y), Scalar(255,0,0));
            circle(im1, Point(x,y), 1, Scalar(0,0,255), -1);
        }
    }

    namedWindow("Flow", WINDOW_AUTOSIZE);
    imshow("Flow", im1);
    waitKey(0);
    return;
}

int main()
{
         //string path = "/home/helen/desktop/motion/";
         string path = "/home/helen/Desktop/disser/HowToDeal2/version_0.1/Data/BabyOnGrass/";
         string img1name = "Img1.tif";
         string img2name = "Img2.tif";
         Mat img1 = imread(path+img1name, 0);                                        
         Mat img2 = imread(path+img2name, 0);                                        
                                                                                           
         Size initial_size(img1.size[0], img1.size[1]);                              
         const int scale = 1;                                                        
         Size size( initial_size.height/scale, initial_size.width/scale );           
         resize(img1, img1, size);                                                   
         resize(img2, img2, size);

         calc_optical_flow(img1, img2);
         return 0;
}

