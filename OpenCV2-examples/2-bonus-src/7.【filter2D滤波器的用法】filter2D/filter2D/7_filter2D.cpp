
//--------------------------------------【程序说明】-------------------------------------------
//		程序说明：《OpenCV3编程入门》OpenCV2版书本附赠示例程序07
//		程序描述：filter2D滤波器的用法
//		测试所用操作系统： Windows 7 64bit
//		测试所用IDE版本：Visual Studio 2010
//		测试所用OpenCV版本：	2.4.9
//		2014年11月 Revised by @浅墨_毛星云
//------------------------------------------------------------------------------------------------

// Program to apply a simple filter matrix to an image
// Author: Samarth Manoj Brahmbhatt, University of Pennsylvania

//---------------------------------【头文件、命名空间包含部分】----------------------------
//		描述：包含程序所使用的头文件和命名空间
//------------------------------------------------------------------------------------------------
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace std;
using namespace cv;



//-----------------------------------【main( )函数】--------------------------------------------
//		描述：控制台应用程序的入口函数，我们的程序从这里开始
//-------------------------------------------------------------------------------------------------
int main() 
{

	//Mat img = imread("image.jpg", CV_LOAD_IMAGE_GRAYSCALE), img_filtered;
	Mat img = imread("1.jpg"), img_filtered;

	// 滤波器内核，用于检测垂直边缘(Filter kernel for detecting vertical edges )
	float vertical_fk[5][5] = {{0,0,0,0,0}, {0,0,0,0,0}, {-1,-2,6,-2,-1}, {0,0,0,0,0}, {0,0,0,0,0}};

	// 滤波器内核，用于检测水平边缘(Filter kernel for detecting horizontal edges )
	float horizontal_fk[5][5] = {{0,0,-1,0,0}, {0,0,-2,0,0}, {0,0,6,0,0}, {0,0,-2,0,0}, {0,0,-1,0,0}};

	Mat filter_kernel = Mat(5, 5, CV_32FC1, vertical_fk);

	//应用滤波器
	filter2D(img, img_filtered, -1, filter_kernel);

	namedWindow("Image");
	namedWindow("Filtered image");
	imshow("Image", img);
	imshow("Filtered image", img_filtered);

	imwrite("filtered_image.jpg", img_filtered);

	while(char(waitKey(1)) != 'q') {}

	return 0;
}  
