#include <opencv2/opencv.hpp>
#include <opencv2/ximgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

float calc_integral( Mat_<float>& sh1, Mat_<float>& sh2, int cy, int cx )
{
    int rad = 15;
    float integral = 0;

    for( int y = max(0, cy - rad); y <= min(cy + rad, sh1.rows-1); ++y )
    {
        for( int x = max(0, cx - rad); x <= min(cx + rad, sh1.cols-1); ++x )
        {
            integral += sh1.at<float>(y,x) - sh2.at<float>(y,x);
        }
    }
    integral /= (4*rad*rad);
    //cout << "integral = " << integral << endl;
    return integral;
}
float calc_sharpness( Mat_<float>& img, int y, int x )
{
    float s = 0;
    const int RAD = 1;
    const int DIAM = 2*RAD+1;
    Mat_<int> kernel(1, DIAM*DIAM, CV_8U);
    
    for( int i = 0; i < DIAM*DIAM; ++i)
    {
        kernel.at<int>(i) = -1;
    }
    kernel.at<int>(4) = 8;

    for( int dy = -RAD; dy <= RAD; ++dy )
    {
        if( y + dy < 0 || y + dy >= img.size[0] )
            continue;

        for( int dx = -RAD; dx <= RAD; ++dx )
        {
            if( x + dx >= 0 && x + dx < img.size[1])
            {
                int ind = dx + RAD + (dy + RAD)*DIAM;
                s += kernel(ind)*img.at<float>(y+dy, x+dx);
            }
        }
    }
    return s;
}

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

void calc_beta_mask( Mat_<float>& im1, Mat_<float>& im2 )
{
    Mat_<float> sharpness1 = Mat::zeros(im1.size(), CV_32F);
    Mat_<float> sharpness2 = Mat::zeros(im1.size(), CV_32F);
    Mat_<uchar> beta = Mat::zeros(im1.size(), CV_16U);

    const int RAD = 1;
    const int DIAM = 2*RAD+1;
    const int ADD_BORDER = RAD;
    Size ksize(DIAM, DIAM);
    
    int step = 1;
    float min = 10000;

    for( int y = 0; y < im1.rows; y+=step )
    {
        for( int x = 0; x < im1.cols; x+=step )
        {
            float v1 = calc_sharpness_BREN(im1, y, x);
            float v2 = calc_sharpness_BREN(im2, y, x);
            sharpness1.at<float>(y,x) = v1;
            sharpness2.at<float>(y,x) = v2;
        }
    }
    namedWindow("Sharpness", WINDOW_AUTOSIZE);
    imshow("Sharpness", sharpness1/255);
    waitKey(0);

    float eps = 10;

    for ( int y = 0; y < im1.rows; y += step )
    {
        for( int x = 0; x < im1.cols; x += step )
        {
            beta.at<uchar>(y, x) = calc_integral(sharpness1, sharpness2, y, x) > eps ? 100 : 0;
        }
    }
    cout << "Integrals calculated" << endl;
    namedWindow("Beta mask", WINDOW_AUTOSIZE);
    imshow("Beta mask", beta);
    waitKey(0);
    imwrite("betamask.jpg", beta);
    return;

}


float calc_sharpness_GLVM( Mat_<float>& img, Mat_<float>& imgBlur, int y, int x )
{//the algorithm of sharpness estimation called GLVM in a survey article
    float s = 0;
    const int RAD = 2;
    const int DIAM = 2*RAD+1;
    const int ADD_BORDER = RAD;
    const int W = img.size[1];
    const int H = img.size[0];

    
    if( x < ADD_BORDER || x > W - ADD_BORDER - 1 || y < ADD_BORDER || y > H - ADD_BORDER - 1 )
        return s;
    //Mat_<float>padarray;
    //cv::copyMakeBorder(img, padarray, ADD_BORDER, ADD_BORDER, ADD_BORDER, ADD_BORDER, BORDER_REPLICATE);
    float d = 0; //dispersion
    float mu = 0; //math expectation
    
    for( int dy = -RAD; dy <= RAD; ++dy )
    {
        for( int dx = -RAD; dx <= RAD; ++dx )
        {
            mu += img.at<float>(y+dy, x+dx) - imgBlur.at<float>(y+dy, x+dx);
        }
    }   
    mu /= DIAM*DIAM; //mean

    for( int dy = -RAD; dy <= RAD; ++dy )
    {
        for( int dx = -RAD; dx <= RAD; ++dx )
        {
            d += pow(img.at<float>(y+dy, x+dx) - imgBlur.at<float>(y+dy, x+dx) - mu, 2);
        }
    }  

    return sqrt(d);
}

void visualize_sharpness( Mat_<float>& im1 )
{
    Mat_<float> sharpness = Mat::zeros(im1.size(), CV_32F);

    const int RAD = 1;
    const int DIAM = 2*RAD+1;
    const int ADD_BORDER = RAD;
    Size ksize(DIAM, DIAM);
    Mat_<float> imgBlur;
    cv::GaussianBlur(im1, imgBlur, ksize, 1/*sigma x*/, 1/*sigma y*/);
    
    int step = 1;
    float min = 10000;

    for( int y = 0; y < im1.rows; y+=step )
    {
        for( int x = 0; x < im1.cols; x+=step )
        {
            float v = calc_sharpness_BREN(im1, y, x);
            sharpness.at<float>(y,x) = v;
        }
    }
    namedWindow("Sharpness", WINDOW_AUTOSIZE);
    imshow("Sharpness", sharpness/255);
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

         //visualize_sharpness(img1);
         //visualize_sharpness(img2);
         calc_beta_mask(img1, img2);
         return 0;
}

