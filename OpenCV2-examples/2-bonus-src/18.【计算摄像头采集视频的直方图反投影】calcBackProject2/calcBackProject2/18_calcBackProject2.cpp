
//--------------------------------------【程序说明】-------------------------------------------
//		程序说明：《OpenCV3编程入门》OpenCV2版书本附赠示例程序18
//		程序描述：计算摄像头采集视频的直方图反投影
//		测试所用操作系统： Windows 7 64bit
//		测试所用IDE版本：Visual Studio 2010
//		测试所用OpenCV版本：	2.4.9
//		2014年11月 Revised by @浅墨_毛星云
//------------------------------------------------------------------------------------------------
// Program to illustrate histogram backprojection 
// Author: Samarth Manoj Brahmbhatt, University of Pennsylvania


//---------------------------------【头文件、命名空间包含部分】----------------------------
//		描述：包含程序所使用的头文件和命名空间
//------------------------------------------------------------------------------------------------
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
using namespace std;


//-----------------------------------【全局变量声明部分】---------------------------------------
//		描述：声明全局变量
//---------------------------------------------------------------------------------------------------
Mat frame_hsv, frame, mask;
MatND hist; //2D直方图
int conn = 4, val = 255, flags = conn + (val << 8) + CV_FLOODFILL_MASK_ONLY;
bool selected = false;
// 直方图相关参数
float hrange[] = {0, 179}, srange[] = {0, 255};
const float *ranges[] = {hrange, srange};


//-----------------------------------【on_mouse( )函数】-------------------------------------
//		描述：鼠标回调函数
//-------------------------------------------------------------------------------------------------
void on_mouse(int event, int x, int y, int, void *) {
	if(event != EVENT_LBUTTONDOWN) return;

	selected = true;

	// floodFill
	Point p(x, y);
	mask = Scalar::all(0);
	floodFill(frame, mask, p, Scalar(255, 255, 255), 0, Scalar(10, 10, 10), Scalar(10, 10, 10), flags);
	Mat _mask = mask.rowRange(1, mask.rows-1).colRange(1, mask.cols-1);

	// number of bins in the histogram for each channel
	int histSize[] = {50, 50}, channels[] = {0, 1};

	// calculate and normalize histogram
	calcHist(&frame_hsv, 1, channels, _mask, hist, 2, histSize, ranges);
	normalize(hist, hist, 0, 255, NORM_MINMAX, -1, Mat());
}



//-----------------------------------【main( )函数】--------------------------------------------
//		描述：控制台应用程序的入口函数，我们的程序从这里开始
//-------------------------------------------------------------------------------------------------
int main( ) 
{
	//从摄像头读入视频
	VideoCapture cap(0);

	//检测是否读取成功
	if(!cap.isOpened()) {
		cout << "Capture could not be opened succesfully" << endl;
		return -1;
	}

	namedWindow("Video");
	namedWindow("Backprojection");

	setMouseCallback("Video", on_mouse);

	while(char(waitKey(1)) != 'q' && cap.isOpened()) {
		cap >> frame;
		if(!selected) mask.create(frame.rows+2, frame.cols+2, CV_8UC1);
		// Check if the video is over
		if(frame.empty()) {
			cout << "Video over" << endl;
			break;
		}
		cvtColor(frame, frame_hsv, CV_BGR2HSV);

		// backproject on the HSV image
		Mat frame_backprojected = Mat::zeros(frame.size(), CV_8UC1);        
		if(selected) {
			int channels[] = {0, 1};
			calcBackProject(&frame_hsv, 1, channels, hist, frame_backprojected, ranges);
		}

		imshow("Video", frame);
		imshow("Backprojection", frame_backprojected);
	}

	return 0;
}
