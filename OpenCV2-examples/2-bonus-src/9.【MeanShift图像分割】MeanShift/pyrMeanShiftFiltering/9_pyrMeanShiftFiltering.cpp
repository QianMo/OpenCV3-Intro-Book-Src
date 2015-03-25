
//--------------------------------------【程序说明】-------------------------------------------
//		程序说明：《OpenCV3编程入门》OpenCV2版书本附赠示例程序09
//		程序描述：MeanShift图像分割
//		测试所用操作系统： Windows 7 64bit
//		测试所用IDE版本：Visual Studio 2010
//		测试所用OpenCV版本：	2.4.9
//		2014年11月 Revised by @浅墨_毛星云
//------------------------------------------------------------------------------------------------



//---------------------------------【头文件、命名空间包含部分】----------------------------
//		描述：包含程序所使用的头文件和命名空间
//------------------------------------------------------------------------------------------------
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
using namespace cv;
using namespace std;


//--------------------------------------【help( )函数】--------------------------------------
//		 描述：输出一些帮助信息
//----------------------------------------------------------------------------------------------
static void help()
{
	cout << "\n\t此程序演示了OpenCV中MeanShift图像分割的使用。\n"
		<< "\n\t程序运行后我们可以通过3个滑动条调节分割效果。调节滑动条后可能会有些许卡顿，请耐心等待\n"
		<< "\n\t3个滑动条代表的参数分别为空间窗的半径 （spatialRad）、色彩窗的半径（colorRad）、最大图像金字塔级别（maxPyrLevel）\n"
		<< endl;
}


//This colors the segmentations
static void floodFillPostprocess( Mat& img, const Scalar& colorDiff=Scalar::all(1) )
{
	CV_Assert( !img.empty() );
	RNG rng = theRNG();
	Mat mask( img.rows+2, img.cols+2, CV_8UC1, Scalar::all(0) );
	for( int y = 0; y < img.rows; y++ )
	{
		for( int x = 0; x < img.cols; x++ )
		{
			if( mask.at<uchar>(y+1, x+1) == 0 )
			{
				Scalar newVal( rng(256), rng(256), rng(256) );
				floodFill( img, mask, Point(x,y), newVal, 0, colorDiff, colorDiff );
			}
		}
	}
}

string winName = "meanshift";
int spatialRad, colorRad, maxPyrLevel;
Mat img, res;



static void meanShiftSegmentation( int, void* )
{
	cout << "spatialRad=" << spatialRad << "; "
		<< "colorRad=" << colorRad << "; "
		<< "maxPyrLevel=" << maxPyrLevel << endl;
	pyrMeanShiftFiltering( img, res, spatialRad, colorRad, maxPyrLevel );
	floodFillPostprocess( res, Scalar::all(2) );
	imshow( winName, res );
}



//-----------------------------------【main( )函数】--------------------------------------------
//		描述：控制台应用程序的入口函数，我们的程序从这里开始
//-------------------------------------------------------------------------------------------------
int main(int argc, char** argv)
{

	help();

	img = imread( "1.jpg" );
	if( img.empty() )
		return -1;
	imshow("原始图",img);
	spatialRad = 10;
	colorRad = 10;
	maxPyrLevel = 1;

	namedWindow( winName, WINDOW_AUTOSIZE );

	createTrackbar( "spatialRad", winName, &spatialRad, 80, meanShiftSegmentation );
	createTrackbar( "colorRad", winName, &colorRad, 60, meanShiftSegmentation );
	createTrackbar( "maxPyrLevel", winName, &maxPyrLevel, 5, meanShiftSegmentation );

	meanShiftSegmentation(0, 0);
	waitKey();
	return 0;
}
