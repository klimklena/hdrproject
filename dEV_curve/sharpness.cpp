#include <opencv2/opencv.hpp>
#include <opencv2/ximgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

void visualize_exposure_diff( Mat_<float>& im1, Mat_<float>& im2 )
{
    Mat_<float> diff = Mat::zeros(im1.size(), CV_32F);

    int step = 1;
    float min = 10000;

    for( int y = 0; y < im1.rows; y+=step )
    {
        for( int x = 0; x < im1.cols; x+=step )
        {
            float v = im1.at<float>(y,x) - im2.at<float>(y,x);
            if (v < min)
                min = v;
            diff.at<float>(y,x) = v;
        }
    }

    diff -= min;
    diff /= 255;
    cout << min << " " << diff.at<float>(5,5) << endl;
    namedWindow("dEV", WINDOW_AUTOSIZE);
    imshow("dEV", diff);
    waitKey(0);
    return;
}

int main()
{
         //string path = "/home/helen/desktop/motion/";
         string path = "/home/helen/Desktop/disser/HowToDeal2/version_0.1/Data/BabyOnGrass/";
         string img1name = "Img2.tif";
         string img2name = "Img1.tif";
         Mat_<float> img1 = imread(path+img1name, 0);                                        
         Mat_<float> img2 = imread(path+img2name, 0);                                        
                                                                                           
         Size initial_size(img1.size[0], img1.size[1]);                              
         const int scale = 1;                                                        
         Size size( initial_size.height/scale, initial_size.width/scale );           
         resize(img1, img1, size);                                                   
         resize(img2, img2, size);

         visualize_exposure_diff(img1, img2);
         return 0;
}

