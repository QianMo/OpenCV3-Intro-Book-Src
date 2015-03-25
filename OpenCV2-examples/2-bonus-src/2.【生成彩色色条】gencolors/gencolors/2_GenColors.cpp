//--------------------------------------【程序说明】-------------------------------------------
//		程序说明：《OpenCV3编程入门》OpenCV2版书本附赠示例程序02
//		程序描述：输入颜色数量，产生着色条带状图像。色带宽20
//		测试所用操作系统： Windows 7 64bit
//		测试所用IDE版本：Visual Studio 2010
//		测试所用OpenCV版本：	2.4.9
//		2014年11月 Revised by @浅墨_毛星云
//------------------------------------------------------------------------------------------------


//---------------------------------【头文件、命名空间包含部分】----------------------------
//		描述：包含程序所使用的头文件和命名空间
//------------------------------------------------------------------------------------------------
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <cstdio>
#include <iostream>
#include <ctime>
using namespace cv;
using namespace std;


//-----------------------------------【全局变量声明部分】---------------------------------------
//		描述：声明全局变量
//---------------------------------------------------------------------------------------------------
char* number="10";//彩条数量



//-----------------------------------【main( )函数】--------------------------------------------
//		描述：控制台应用程序的入口函数，我们的程序从这里开始
//-------------------------------------------------------------------------------------------------
int main(int argc, char** argv)
{

	int colorsCount = atoi(number);
	vector<Scalar> colors;
	theRNG() = (uint64)time(0);
	generateColors( colors, colorsCount );

	int stripWidth = 20;
	Mat strips(300, colorsCount*stripWidth, CV_8UC3);
	for( int i = 0; i < colorsCount; i++ )
	{
		strips.colRange(i*stripWidth, (i+1)*stripWidth) = colors[i];
	}

	imshow( "strips", strips );
	waitKey();

	return 0;
}
