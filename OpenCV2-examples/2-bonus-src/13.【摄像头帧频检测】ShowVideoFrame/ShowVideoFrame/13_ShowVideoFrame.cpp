
//--------------------------------------【程序说明】-------------------------------------------
//		程序说明：《OpenCV3编程入门》OpenCV2版书本附赠示例程序13
//		程序描述：摄像头帧频检测
//		测试所用操作系统： Windows 7 64bit
//		测试所用IDE版本：Visual Studio 2010
//		测试所用OpenCV版本：	2.4.9
//		2014年11月 Revised by @浅墨_毛星云
//------------------------------------------------------------------------------------------------
//Code to check the OpenCV installation on Raspberry Pi and mesaure frame rate
//Author: Samarth Manoj Brahmbhatt, University of Pennsyalvania


//---------------------------------【头文件、命名空间包含部分】----------------------------
//		描述：包含程序所使用的头文件和命名空间
//------------------------------------------------------------------------------------------------
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace std;
using namespace cv;


//-----------------------------------【main( )函数】--------------------------------------------
//		描述：控制台应用程序的入口函数，我们的程序从这里开始
//-------------------------------------------------------------------------------------------------
int main()
{

	VideoCapture cap(0);
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 800);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 600);

	Mat im, im_g;
	double time = 0;
	unsigned int frames = 0;
	while(char(waitKey(1)) != 'q') {
		double t0 = getTickCount();
		cap >> im;
		cvtColor(im, im_g, CV_BGR2GRAY);
		frames++;
		imshow("cap1", im);
		imshow("cap2", im_g);
		time += (getTickCount() - t0) / getTickFrequency();
		cout << frames / time << " fps" << endl;
	}

	return 0;
}
