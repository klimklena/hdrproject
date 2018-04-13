#include <opencv2/opencv.hpp>
#include <opencv2/ximgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

float calc_sharpness_BREN( Mat_<float>& img, int y, int x )
{//the algorithm of sharpness estimation called BREN in a survey article
    float s = 0;
    const int RAD = 1;
    const int DIAM = 2*RAD+1;
    const int BREN_STEP = 1;
    const int ADD_BORDER = BREN_STEP + RAD;
    const int W = img.size[1];
    const int H = img.size[0];

    if( x < ADD_BORDER || x > W - ADD_BORDER - 1 || y < ADD_BORDER || y > H - ADD_BORDER - 1 )
        return s;
    //Mat_<float>padarray;
    //cv::copyMakeBorder(img, padarray, ADD_BORDER, ADD_BORDER, ADD_BORDER, ADD_BORDER, BORDER_REPLICATE);
 
    
    for( int dy = -RAD; dy <= RAD; ++dy )
    {
        for( int dx = -RAD; dx <= RAD; ++dx )
        {
            int xleft = dx + x - BREN_STEP;
            int xright = dx + x + BREN_STEP;
            int yup = (dy + y - BREN_STEP);
            int ydown = (dy + y + BREN_STEP);
            s += pow(img.at<float>(y+dy, xleft) - img.at<float>(y+dy, xright), 2.);
            s += pow(img.at<float>(yup, x+dx) - img.at<float>(ydown,x+dx), 2.);
        }
    }   
    return s/(2*DIAM*DIAM);
}

void construct_new_ref( Mat& im1, Mat& im2, Mat_<uchar>& mask )
{
    Mat new_ref = Mat::zeros(im1.size(), im1.type());
    int step = 1;
    
    for( int y = 0; y < im1.rows; y+=step )
    {
        for( int x = 0; x < im1.cols; x+=step )
        {
            new_ref.at<Vec3b>(y, x).val[0] = mask.at<uchar>(y, x) ? im2.at<Vec3b>(y, x).val[0] : im1.at<Vec3b>(y, x).val[0];
            new_ref.at<Vec3b>(y, x).val[1] = mask.at<uchar>(y, x) ? im2.at<Vec3b>(y, x).val[1] : im1.at<Vec3b>(y, x).val[1];
            new_ref.at<Vec3b>(y, x).val[2] = mask.at<uchar>(y, x) ? im2.at<Vec3b>(y, x).val[2] : im1.at<Vec3b>(y, x).val[2];
        }
    }
    namedWindow("New Ref", WINDOW_AUTOSIZE);
    imshow("New Ref", new_ref);
    waitKey(0);
    imwrite("new_ref.tif", new_ref);

    return;
}

int main()
{
         //string path = "/home/helen/desktop/motion/";
         string path = "/home/helen/Desktop/disser/HowToDeal2/version_0.1/Data/BabyOnGrass/";
         string img1name = "Img2.tif";
         string img2name = "Img1.tif";
         string maskname = "betamask.jpg";
         Mat img1 = imread(path+img1name);                   
         Mat img2 = imread(path+img2name);    
         Mat_<uchar> betamask = imread(maskname, 0);

         Size initial_size(img1.size[0], img1.size[1]);                              
         const int scale = 1;                                                        
         Size size( initial_size.height/scale, initial_size.width/scale );           
         //resize(img1, img1, size);                                   
         //resize(img2, img2, size);
         //resize(betamask, betamask, size);

         //visualize_sharpness(img1);
         //visualize_sharpness(img2);
         construct_new_ref(img1, img2, betamask);
         return 0;
}

