//--------------------------------------【程序说明】-------------------------------------------
//		程序说明：《OpenCV3编程入门》OpenCV2版书本附赠示例程序06
//		程序描述：把图像映射到极指数空间 
//		测试所用操作系统： Windows 7 64bit
//		测试所用IDE版本：Visual Studio 2010
//		测试所用OpenCV版本：	2.4.9
//		2014年11月 Revised by @浅墨_毛星云
//------------------------------------------------------------------------------------------------

/*Authors
* Manuela Chessa, Fabio Solari, Fabio Tatti, Silvio P. Sabatini
*
* manuela.chessa@unige.it, fabio.solari@unige.it
*
* PSPC-lab - University of Genoa
*/

//---------------------------------【头文件、命名空间包含部分】----------------------------
//		描述：包含程序所使用的头文件和命名空间
//------------------------------------------------------------------------------------------------
#include "opencv2/opencv.hpp"
#include <iostream>
#include <cmath>
using namespace cv;
using namespace std;


//-----------------------------------【help( )函数】----------------------------------------
//		 描述：输出一些帮助信息
//----------------------------------------------------------------------------------------------
static void help( )
{
	cout << "\n\n\t此程序用于把图像映射到极指数空间,有如下四种方式可选：\n"<<
		"\n\n\t【n】-采用最邻近像素技术（nearest pixel technique）"<<
		"\n\n\t【b】-采用双线性插值技术（bilinear interpolation technique）"<<
		"\n\n\t【o】-使用重叠的圆形的接受域（overlapping circular receptive fields）"<<
		"\n\n\t【a】-使用相邻的接受域（adjacent receptive fields）";

}


//-----------------------------------【main( )函数】--------------------------------------------
//		描述：控制台应用程序的入口函数，我们的程序从这里开始
//-------------------------------------------------------------------------------------------------
int main(int argc, char** argv)
{
	//载入图片
	Mat img = imread("1.jpg",1); 
	if(img.empty()) 
	{
		cout << "can not load image\n";
		return 0;
	}

	help();

	Size s=img.size();
	int w=s.width, h=s.height;
	int ro0=3; //半径的盲点（radius of the blind spot）
	int R=120;  //环的数量（number of rings）

	//Creation of the four different objects that implement the four log-polar transformations
	//离线计算（Off-line computation）
	Point2i center(w/2,h/2);
	LogPolar_Interp nearest(w, h, center, R, ro0, INTER_NEAREST);
	LogPolar_Interp bilin(w,h, center,R,ro0);
	LogPolar_Overlapping overlap(w,h,center,R,ro0);
	LogPolar_Adjacent adj(w,h,center,R,ro0,0.25);

	namedWindow("Cartesian",1);
	namedWindow("retinal",1);
	namedWindow("cortical",1);
	int wk='n';
	Mat Cortical, Retinal;

	//在线计算（On-line computation）
	for(;;)
	{
		if(wk=='n')
		{
			Cortical=nearest.to_cortical(img);
			Retinal=nearest.to_cartesian(Cortical);
		}
		else if (wk=='b')
		{
			Cortical=bilin.to_cortical(img);
			Retinal=bilin.to_cartesian(Cortical);
		}
		else if (wk=='o')
		{
			Cortical=overlap.to_cortical(img);
			Retinal=overlap.to_cartesian(Cortical);
		}
		else if (wk=='a')
		{
			Cortical=adj.to_cortical(img);
			Retinal=adj.to_cartesian(Cortical);
		}

		imshow("Cartesian", img);
		imshow("cortical", Cortical);
		imshow("retinal", Retinal);

		int c=waitKey(0);
		if (c>0) wk=c;
		if(wk =='q' || (wk & 255) == 27) break;
	}

	return 0;
}