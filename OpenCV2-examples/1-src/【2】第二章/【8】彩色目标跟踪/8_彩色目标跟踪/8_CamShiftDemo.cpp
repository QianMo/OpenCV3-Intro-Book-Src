//--------------------------------------【程序说明】-------------------------------------------
//		程序说明：《OpenCV3编程入门》OpenCV2版书本配套示例程序08
//		程序描述：来自OpenCV安装目录下Samples文件夹中的官方示例程序-彩色目标跟踪操作
//		开发测试所用操作系统： Windows 7 64bit
//		开发测试所用IDE版本：Visual Studio 2010
//		开发测试所用OpenCV版本：	2.4.9
//		2014年11月 Revised by @浅墨_毛星云
//------------------------------------------------------------------------------------------------

//---------------------------------【头文件、命名空间包含部分】----------------------------
//		描述：包含程序所使用的头文件和命名空间
//-------------------------------------------------------------------------------------------------
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <ctype.h>

using namespace cv;
using namespace std;



//-----------------------------------【全局变量声明】-----------------------------------------
//		描述：声明全局变量
//-------------------------------------------------------------------------------------------------
Mat image;
bool backprojMode = false;
bool selectObject = false;
int trackObject = 0;
bool showHist = true;
Point origin;
Rect selection;
int vmin = 10, vmax = 256, smin = 30;


//--------------------------------【onMouse( )回调函数】------------------------------------
//		描述：鼠标操作回调
//-------------------------------------------------------------------------------------------------
static void onMouse( int event, int x, int y, int, void* )
{
	if( selectObject )
	{
		selection.x = MIN(x, origin.x);
		selection.y = MIN(y, origin.y);
		selection.width = std::abs(x - origin.x);
		selection.height = std::abs(y - origin.y);

		selection &= Rect(0, 0, image.cols, image.rows);
	}

	switch( event )
	{
	case CV_EVENT_LBUTTONDOWN:
		origin = Point(x,y);
		selection = Rect(x,y,0,0);
		selectObject = true;
		break;
	case CV_EVENT_LBUTTONUP:
		selectObject = false;
		if( selection.width > 0 && selection.height > 0 )
			trackObject = -1;
		break;
	}
}

//--------------------------------【help( )函数】----------------------------------------------
//		描述：输出帮助信息
//-------------------------------------------------------------------------------------------------
static void ShowHelpText()
{
	cout <<"\n\n\t\t\t非常感谢购买《OpenCV3编程入门》一书！\n"
		<<"\n\n\t\t\t此为本书OpenCV2版的第8个配套示例程序\n"
		<<	"\n\n\t\t\t   当前使用的OpenCV版本为：" << CV_VERSION 
		<<"\n\n  ----------------------------------------------------------------------------" ;

	cout << "\n\n\t此Demo显示了基于均值漂移的追踪（tracking）技术\n"
		"\t请用鼠标框选一个有颜色的物体，对它进行追踪操作\n";

	cout << "\n\n\t操作说明： \n"
		"\t\t用鼠标框选对象来初始化跟踪\n"
		"\t\tESC - 退出程序\n"
		"\t\tc - 停止追踪\n"
		"\t\tb - 开/关-投影视图\n"
		"\t\th - 显示/隐藏-对象直方图\n"
		"\t\tp - 暂停视频\n";
}

const char* keys =
{
	"{1|  | 0 | camera number}"
};


//-----------------------------------【main( )函数】--------------------------------------------
//		描述：控制台应用程序的入口函数，我们的程序从这里开始
//-------------------------------------------------------------------------------------------------
int main( int argc, const char** argv )
{
	ShowHelpText();

	VideoCapture cap;
	Rect trackWindow;
	int hsize = 16;
	float hranges[] = {0,180};
	const float* phranges = hranges;

	cap.open(0);

	if( !cap.isOpened() )
	{
		cout << "不能初始化摄像头\n";
	}

	namedWindow( "Histogram", 0 );
	namedWindow( "CamShift Demo", 0 );
	setMouseCallback( "CamShift Demo", onMouse, 0 );
	createTrackbar( "Vmin", "CamShift Demo", &vmin, 256, 0 );
	createTrackbar( "Vmax", "CamShift Demo", &vmax, 256, 0 );
	createTrackbar( "Smin", "CamShift Demo", &smin, 256, 0 );

	Mat frame, hsv, hue, mask, hist, histimg = Mat::zeros(200, 320, CV_8UC3), backproj;
	bool paused = false;

	for(;;)
	{
		if( !paused )
		{
			cap >> frame;
			if( frame.empty() )
				break;
		}

		frame.copyTo(image);

		if( !paused )
		{
			cvtColor(image, hsv, COLOR_BGR2HSV);

			if( trackObject )
			{
				int _vmin = vmin, _vmax = vmax;

				inRange(hsv, Scalar(0, smin, MIN(_vmin,_vmax)),
					Scalar(180, 256, MAX(_vmin, _vmax)), mask);
				int ch[] = {0, 0};
				hue.create(hsv.size(), hsv.depth());
				mixChannels(&hsv, 1, &hue, 1, ch, 1);

				if( trackObject < 0 )
				{
					Mat roi(hue, selection), maskroi(mask, selection);
					calcHist(&roi, 1, 0, maskroi, hist, 1, &hsize, &phranges);
					normalize(hist, hist, 0, 255, CV_MINMAX);

					trackWindow = selection;
					trackObject = 1;

					histimg = Scalar::all(0);
					int binW = histimg.cols / hsize;
					Mat buf(1, hsize, CV_8UC3);
					for( int i = 0; i < hsize; i++ )
						buf.at<Vec3b>(i) = Vec3b(saturate_cast<uchar>(i*180./hsize), 255, 255);
					cvtColor(buf, buf, CV_HSV2BGR);

					for( int i = 0; i < hsize; i++ )
					{
						int val = saturate_cast<int>(hist.at<float>(i)*histimg.rows/255);
						rectangle( histimg, Point(i*binW,histimg.rows),
							Point((i+1)*binW,histimg.rows - val),
							Scalar(buf.at<Vec3b>(i)), -1, 8 );
					}
				}

				calcBackProject(&hue, 1, 0, hist, backproj, &phranges);
				backproj &= mask;
				RotatedRect trackBox = CamShift(backproj, trackWindow,
					TermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1 ));
				if( trackWindow.area() <= 1 )
				{
					int cols = backproj.cols, rows = backproj.rows, r = (MIN(cols, rows) + 5)/6;
					trackWindow = Rect(trackWindow.x - r, trackWindow.y - r,
						trackWindow.x + r, trackWindow.y + r) &
						Rect(0, 0, cols, rows);
				}

				if( backprojMode )
					cvtColor( backproj, image, COLOR_GRAY2BGR );
				ellipse( image, trackBox, Scalar(0,0,255), 3, CV_AA );
			}
		}
		else if( trackObject < 0 )
			paused = false;

		if( selectObject && selection.width > 0 && selection.height > 0 )
		{
			Mat roi(image, selection);
			bitwise_not(roi, roi);
		}

		imshow( "CamShift Demo", image );
		imshow( "Histogram", histimg );

		char c = (char)waitKey(10);
		if( c == 27 )
			break;
		switch(c)
		{
		case 'b':
			backprojMode = !backprojMode;
			break;
		case 'c':
			trackObject = 0;
			histimg = Scalar::all(0);
			break;
		case 'h':
			showHist = !showHist;
			if( !showHist )
				destroyWindow( "Histogram" );
			else
				namedWindow( "Histogram", 1 );
			break;
		case 'p':
			paused = !paused;
			break;
		default:
			;
		}
	}

	return 0;
}
