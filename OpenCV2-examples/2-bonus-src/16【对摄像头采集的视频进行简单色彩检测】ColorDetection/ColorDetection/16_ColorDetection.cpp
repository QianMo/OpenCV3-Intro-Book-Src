//--------------------------------------【程序说明】-------------------------------------------
//		程序说明：《OpenCV3编程入门》OpenCV2版书本附赠示例程序16
//		程序描述：对摄像头采集的视频进行简单色彩检测
//		测试所用操作系统： Windows 7 64bit
//		测试所用IDE版本：Visual Studio 2010
//		测试所用OpenCV版本：	2.4.9
//		2014年11月 Revised by @浅墨_毛星云
//------------------------------------------------------------------------------------------------

// Program to display a video from attached default camera device and detect colored blobs using simple R G and B thresholding
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
Mat frame, frame_thresholded;
int rgb_slider = 0, low_slider = 30, high_slider = 100;
int low_r = 30, low_g = 30, low_b = 30, high_r = 100, high_g = 100, high_b = 100;



//-----------------------------------【on_rgb_trackbar( )函数】------------------------------
//		描述：滑动条回调函数
//-------------------------------------------------------------------------------------------------
void on_rgb_trackbar(int, void *) {
	switch(rgb_slider) {
	case 0:
		setTrackbarPos("Low threshold", "Segmentation", low_r);
		setTrackbarPos("High threshold", "Segmentation", high_r);
		break;
	case 1:
		setTrackbarPos("Low threshold", "Segmentation", low_g);
		setTrackbarPos("High threshold", "Segmentation", high_g);
		break;
	case 2:
		setTrackbarPos("Low threshold", "Segmentation", low_b);
		setTrackbarPos("High threshold", "Segmentation", high_b);
		break;
	}
}


//------------------------------【on_low_thresh_trackbar( )函数】--------------------------
//		描述：滑动条回调函数
//-------------------------------------------------------------------------------------------------
void on_low_thresh_trackbar(int, void *) {
	switch(rgb_slider) {
	case 0:
		low_r = min(high_slider - 1, low_slider);
		setTrackbarPos("Low threshold", "Segmentation", low_r);
		break;
	case 1:
		low_g = min(high_slider - 1, low_slider);
		setTrackbarPos("Low threshold", "Segmentation", low_g);
		break;
	case 2:
		low_b = min(high_slider - 1, low_slider);
		setTrackbarPos("Low threshold", "Segmentation", low_b);
		break;
	}
}



//------------------------------【on_high_thresh_trackbar( )函数】-------------------------
//		描述：滑动条回调函数
//-------------------------------------------------------------------------------------------------
void on_high_thresh_trackbar(int, void *) {
	switch(rgb_slider) {
	case 0:
		high_r = max(low_slider + 1, high_slider);
		setTrackbarPos("High threshold", "Segmentation", high_r);
		break;
	case 1:
		high_g = max(low_slider + 1, high_slider);
		setTrackbarPos("High threshold", "Segmentation", high_g);
		break;
	case 2:
		high_b = max(low_slider + 1, high_slider);
		setTrackbarPos("High threshold", "Segmentation", high_b);
		break;
	}
}



//-----------------------------------【main( )函数】--------------------------------------------
//		描述：控制台应用程序的入口函数，我们的程序从这里开始
//-------------------------------------------------------------------------------------------------
int main()
{
	//从摄像头载入视频
	VideoCapture cap(0);
	//设置视频长宽
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 860);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 640);

	if(!cap.isOpened())
	{
		cout << "Capture could not be opened succesfully" << endl;
		return -1;
	}

	namedWindow("Video");
	namedWindow("Segmentation");

	createTrackbar("0. R\n1. G\n2.B", "Segmentation", &rgb_slider, 2, on_rgb_trackbar);
	createTrackbar("Low threshold", "Segmentation", &low_slider, 255, on_low_thresh_trackbar);
	createTrackbar("High threshold", "Segmentation", &high_slider, 255, on_high_thresh_trackbar);

	double time = 0;
	unsigned int frames = 0;
	while(char(waitKey(1)) != 'q' && cap.isOpened())
	{
		frames++;
		double t0 = getTickCount();
		cap >> frame;
		// 视频是否为空
		if(frame.empty())
		{
			cout << "Video over" << endl;
			break;
		}

		inRange(frame, Scalar(low_b, low_g, low_r), Scalar(high_b, high_g, high_r), frame_thresholded);

		imshow("Video", frame);
		imshow("Segmentation", frame_thresholded);
		time += (getTickCount() - t0) / getTickFrequency();
		cout << frames / time << " fps" << endl;
	}

	return 0;
}
