//--------------------------------------【程序说明】-------------------------------------------
//		程序说明：《OpenCV3编程入门》OpenCV2版书本附赠示例程序03
//		程序描述：卡尔曼滤波
//		测试所用操作系统： Windows 7 64bit
//		测试所用IDE版本：Visual Studio 2010
//		测试所用OpenCV版本：	2.4.9
//		2014年11月 Revised by @浅墨_毛星云
//------------------------------------------------------------------------------------------------



//---------------------------------【头文件、命名空间包含部分】----------------------------
//		描述：包含程序所使用的头文件和命名空间
//------------------------------------------------------------------------------------------------
#include "opencv2/video/tracking.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdio.h>
using namespace cv;



//---------------------------------【calcPoint( )函数】--------------------------------------
//		 描述：计算点坐标
//----------------------------------------------------------------------------------------------
static inline Point calcPoint(Point2f center, double R, double angle)
{
	return center + Point2f((float)cos(angle), (float)-sin(angle))*(float)R;
}


//--------------------------------------【help( )函数】--------------------------------------
//		 描述：输出一些帮助信息
//----------------------------------------------------------------------------------------------
static void help()
{
	printf("\n\t用OpenCV动态绘制卡尔曼滤波。\n");
	printf("\n\t任意按键重置轨迹，并更新速度\n");
	printf("\n\tESC键-程序结束\n");
}


//-----------------------------------【main( )函数】--------------------------------------------
//		描述：控制台应用程序的入口函数，我们的程序从这里开始
//-------------------------------------------------------------------------------------------------
int main(int, char**)
{
	help();
	Mat img(500, 500, CV_8UC3);
	KalmanFilter KF(2, 1, 0);
	Mat state(2, 1, CV_32F); /* (phi, delta_phi) */
	Mat processNoise(2, 1, CV_32F);
	Mat measurement = Mat::zeros(1, 1, CV_32F);
	char code = (char)-1;

	for(;;)
	{
		randn( state, Scalar::all(0), Scalar::all(0.1) );
		KF.transitionMatrix = *(Mat_<float>(2, 2) << 1, 1, 0, 1);

		setIdentity(KF.measurementMatrix);
		setIdentity(KF.processNoiseCov, Scalar::all(1e-5));
		setIdentity(KF.measurementNoiseCov, Scalar::all(1e-1));
		setIdentity(KF.errorCovPost, Scalar::all(1));

		randn(KF.statePost, Scalar::all(0), Scalar::all(0.1));

		for(;;)
		{
			Point2f center(img.cols*0.5f, img.rows*0.5f);
			float R = img.cols/3.f;
			double stateAngle = state.at<float>(0);
			Point statePt = calcPoint(center, R, stateAngle);

			Mat prediction = KF.predict();
			double predictAngle = prediction.at<float>(0);
			Point predictPt = calcPoint(center, R, predictAngle);

			randn( measurement, Scalar::all(0), Scalar::all(KF.measurementNoiseCov.at<float>(0)));

			// generate measurement
			measurement += KF.measurementMatrix*state;

			double measAngle = measurement.at<float>(0);
			Point measPt = calcPoint(center, R, measAngle);

			// plot points
#define drawCross( center, color, d )                                 \
	line( img, Point( center.x - d, center.y - d ),                \
	Point( center.x + d, center.y + d ), color, 1, CV_AA, 0); \
	line( img, Point( center.x + d, center.y - d ),                \
	Point( center.x - d, center.y + d ), color, 1, CV_AA, 0 )

			img = Scalar::all(0);
			drawCross( statePt, Scalar(255,255,255), 3 );
			drawCross( measPt, Scalar(0,0,255), 3 );
			drawCross( predictPt, Scalar(0,255,0), 3 );
			line( img, statePt, measPt, Scalar(0,0,255), 3, CV_AA, 0 );
			line( img, statePt, predictPt, Scalar(0,255,255), 3, CV_AA, 0 );

			if(theRNG().uniform(0,4) != 0)
				KF.correct(measurement);

			randn( processNoise, Scalar(0), Scalar::all(sqrt(KF.processNoiseCov.at<float>(0, 0))));
			state = KF.transitionMatrix*state + processNoise;

			imshow( "Kalman", img );
			code = (char)waitKey(100);

			if( code > 0 )
				break;
		}
		if( code == 27 || code == 'q' || code == 'Q' )
			break;
	}

	return 0;
}
