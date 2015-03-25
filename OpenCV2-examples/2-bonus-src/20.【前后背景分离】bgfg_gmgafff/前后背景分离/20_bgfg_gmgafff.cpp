
//--------------------------------------【程序说明】-------------------------------------------
//		程序说明：《OpenCV3编程入门》OpenCV2版书本附赠示例程序20
//		程序描述：前后背景分离
//		测试所用操作系统： Windows 7 64bit
//		测试所用IDE版本：Visual Studio 2010
//		测试所用OpenCV版本：	2.4.9
//		2014年11月 Revised by @浅墨_毛星云
//------------------------------------------------------------------------------------------------


//---------------------------------【头文件、命名空间包含部分】----------------------------
//		描述：包含程序所使用的头文件和命名空间
//------------------------------------------------------------------------------------------------
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdio.h>
using namespace std;
using namespace cv;


//--------------------------------------【help( )函数】--------------------------------------
//		 描述：输出一些帮助信息
//----------------------------------------------------------------------------------------------
static void help()
{
	printf("\n\n\t此程序展示了视频前后背景分离的方法,采用cvUpdateBGStatModel()方法.\n"
		"\n\n\t程序首先会“学习背景”，然后进行分割。\n"
		"\n\n\t可以用过【Space】空格进行功能切换。\n\n");
}


//-----------------------------------【main( )函数】--------------------------------------------
//		描述：控制台应用程序的入口函数，我们的程序从这里开始
//-------------------------------------------------------------------------------------------------
int main(int argc, const char** argv)
{
	help();
	VideoCapture cap;
	bool update_bg_model = true;

	//cap.open(0);
	cap.open("1.avi");

	if( !cap.isOpened() )
	{
		printf("can not open camera or video file\n");
		return -1;
	}

	namedWindow("image", WINDOW_AUTOSIZE);
	namedWindow("foreground mask", WINDOW_AUTOSIZE);
	namedWindow("foreground image", WINDOW_AUTOSIZE);
	namedWindow("mean background image", WINDOW_AUTOSIZE);

	BackgroundSubtractorMOG2 bg_model;//(100, 3, 0.3, 5);

	Mat img, fgmask, fgimg;

	for(;;)
	{
		cap >> img;

		if( img.empty() )
			break;

		//cvtColor(_img, img, COLOR_BGR2GRAY);

		if( fgimg.empty() )
			fgimg.create(img.size(), img.type());

		//更新模型
		bg_model(img, fgmask, update_bg_model ? -1 : 0);

		fgimg = Scalar::all(0);
		img.copyTo(fgimg, fgmask);

		Mat bgimg;
		bg_model.getBackgroundImage(bgimg);

		imshow("image", img);
		imshow("foreground mask", fgmask);
		imshow("foreground image", fgimg);
		if(!bgimg.empty())
			imshow("mean background image", bgimg );

		char k = (char)waitKey(1);
		if( k == 27 ) break;
		if( k == ' ' )
		{
			update_bg_model = !update_bg_model;
			if(update_bg_model)
				printf("\t>背景更新(Background update)已打开\n");
			else
				printf("\t>背景更新(Background update)已关闭\n");
		}
	}

	return 0;
}
