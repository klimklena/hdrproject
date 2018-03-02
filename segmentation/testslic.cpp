#include <opencv2/opencv.hpp>
#include <opencv2/ximgproc.hpp>
//#include <opencv2/slic.hpp>

using namespace std;
using namespace cv;

void find_corners(Mat im1, Mat im2)
{
    Mat dst, dst_norm, dst_norm_scaled;
    dst = Mat::zeros(im1.size(), CV_32FC1);

    int blockSize = 2;
    int apertureSize = 3;
    double k = 0.04;

    cornerHarris(im1, dst, blockSize, apertureSize, k, BORDER_DEFAULT);

    normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
    convertScaleAbs(dst_norm, dst_norm_scaled);

    int thresh = 70;

    for( int j = 0; j < dst_norm.rows; ++j )
    {
        for( int i = 0; i < dst_norm.cols; ++i )
        {
            if( (int)dst_norm.at<float>(j,i) > thresh )
            {
                circle(dst_norm_scaled, Point(i,j), 5, Scalar(0), 2, 8, 0 );
            }
        }
    }

    namedWindow("Corners", CV_WINDOW_AUTOSIZE);
    imshow("Corners", dst_norm_scaled);
    waitKey(0);
    return;
}

void build_superpixels(Mat_<float>&im1, Mat_<float>&im2)
{
    int psize = 20;
    int minsize = psize*0.5;
    Size minSize(minsize, minsize);
    Ptr<cv::ximgproc::SuperpixelSLIC> superpix1 = cv::ximgproc::createSuperpixelSLIC(im1, 101/*alg*/, psize);
    superpix1->iterate(100);
    superpix1->enforceLabelConnectivity(minsize);
    Mat_<uchar> res1(im1.size[0], im1.size[1]);
    superpix1->getLabelContourMask(res1, true);

    Ptr<cv::ximgproc::SuperpixelSLIC> superpix2 = cv::ximgproc::createSuperpixelSLIC(im2, 101/*alg*/, psize);
    superpix2->iterate(50);
    superpix2->enforceLabelConnectivity(minsize);
    Mat_<uchar> res2(im2.size[0], im2.size[1]);
    superpix2->getLabelContourMask(res2, true);

    imshow("img1", res1);
    waitKey(0);
    imshow("img2", res2);
    waitKey(0);

    Mat_<int> labels;
    superpix1->getLabels(labels);
    cout << "Num. of superpixels: " << superpix1->getNumberOfSuperpixels() << endl;
    //for( int i = 0; i < labels.size[0]; ++i )
    //    for( int j = 0; j < labels.size[1]; ++j )
    //    {
    //        cout << labels[i][j] << " ";
    //    }

    find_corners(res1, res2);
    return;
}

int main()
{
    string path = "/home/helen/Desktop/motion/";
    string img1name = "Im1.tif";//"IMG_0091.CR2.jpeg";
    string img2name = "Im2.tif";//"IMG_0093.CR2.jpeg";
    Mat img1 = imread(path+img1name, 0);
    Mat img2 = imread(path+img2name, 0);
    
    Size initial_size(img1.size[0], img1.size[1]);
    const int SCALE = 1;
    Size size( initial_size.height/SCALE, initial_size.width/SCALE );
    resize(img1, img1, size);
    resize(img2, img2, size);

    Mat_<float> im1;
    Mat_<float> im2;
    cout << CV_32F << endl;
    img1.convertTo(im1, CV_32F);
    img2.convertTo(im2, CV_32F);

    //normalize to the range of [0,1]:
    im1 = im1 / 255;
    im2 = im2 / 255;

    build_superpixels(im1, im2);
  return 0;
}
