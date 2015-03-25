
//--------------------------------------【程序说明】-------------------------------------------
//		程序说明：《OpenCV3编程入门》OpenCV2版书本附赠示例程序17
//		程序描述：跟踪分割运动中的物体
//		测试所用操作系统： Windows 7 64bit
//		测试所用IDE版本：Visual Studio 2010
//		测试所用OpenCV版本：	2.4.9
//		2014年11月 Revised by @浅墨_毛星云
//------------------------------------------------------------------------------------------------


//---------------------------------【头文件、命名空间包含部分】----------------------------
//		描述：包含程序所使用的头文件和命名空间
//------------------------------------------------------------------------------------------------
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/background_segm.hpp"
#include <stdio.h>
#include <string>
using namespace cv;


//--------------------------------------【help( )函数】--------------------------------------
//		 描述：输出一些帮助信息
//----------------------------------------------------------------------------------------------
static void help()
{
	printf("\n"
		"\n\n\t此程序演示了一种寻找轮廓，连接组件，清除背景的简单方法，实现跟踪分割视频中运动的物体\n"
		"\n\n\t程序运行开始后，便开始“学习背景”.\n"
		"\n\n\t可以通过【Space】空格键来切换是否打开“背景学习”技术。\n");

}


//--------------------------------------【refineSegments( )函数】-------------------------
//		 描述：进行片段的提炼
//----------------------------------------------------------------------------------------------
static void refineSegments(const Mat& img, Mat& mask, Mat& dst)
{
	int niters = 3;

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	Mat temp;

	dilate(mask, temp, Mat(), Point(-1,-1), niters);
	erode(temp, temp, Mat(), Point(-1,-1), niters*2);
	dilate(temp, temp, Mat(), Point(-1,-1), niters);

	findContours( temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );

	dst = Mat::zeros(img.size(), CV_8UC3);

	if( contours.size() == 0 )
		return;

	// iterate through all the top-level contours,
	// draw each connected component with its own random color
	int idx = 0, largestComp = 0;
	double maxArea = 0;

	for( ; idx >= 0; idx = hierarchy[idx][0] )
	{
		const vector<Point>& c = contours[idx];
		double area = fabs(contourArea(Mat(c)));
		if( area > maxArea )
		{
			maxArea = area;
			largestComp = idx;
		}
	}
	Scalar color( 123, 0, 255 );
	drawContours( dst, contours, largestComp, color, CV_FILLED, 8, hierarchy );
}



//-----------------------------------【main( )函数】--------------------------------------------
//		描述：控制台应用程序的入口函数，我们的程序从这里开始
//-------------------------------------------------------------------------------------------------
int main(int argc, char** argv)
{
	VideoCapture cap;
	bool update_bg_model = true;

	help();

	cap.open("1.avi");

	if( !cap.isOpened() )
	{
		printf("\nCan not open camera or video file\n");
		return -1;
	}

	Mat tmp_frame, bgmask, out_frame;

	cap >> tmp_frame;
	if(!tmp_frame.data)
	{
		printf("can not read data from the video source\n");
		return -1;
	}

	namedWindow("video", 1);
	namedWindow("segmented", 1);

	BackgroundSubtractorMOG bgsubtractor;
	bgsubtractor.set("noiseSigma", 10);

	for(;;)
	{
		cap >> tmp_frame;
		if( !tmp_frame.data )
			break;
		bgsubtractor(tmp_frame, bgmask, update_bg_model ? -1 : 0);
		refineSegments(tmp_frame, bgmask, out_frame);
		imshow("video", tmp_frame);
		imshow("segmented", out_frame);
		int keycode = waitKey(1);
		if( keycode == 27 )
			break;
		if( keycode == ' ' )
		{
			update_bg_model = !update_bg_model;
			printf("\n\t>‘背景学习’技术的状态 = %d\n",update_bg_model);
		}
	}

	return 0;
}
