//--------------------------------------【程序说明】-------------------------------------------
//		程序说明：《OpenCV3编程入门》OpenCV2版书本配套示例程序90
//		程序描述：SURF特征描述
//		开发测试所用操作系统： Windows 7 64bit
//		开发测试所用IDE版本：Visual Studio 2010
//		开发测试所用OpenCV版本：	2.4.9
//		2014年06月 Created by @浅墨_毛星云
//		2014年11月 Revised by @浅墨_毛星云
//------------------------------------------------------------------------------------------------



//---------------------------------【头文件、命名空间包含部分】----------------------------
//		描述：包含程序所使用的头文件和命名空间
//------------------------------------------------------------------------------------------------
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/nonfree/nonfree.hpp>
#include<opencv2/legacy/legacy.hpp>
#include <iostream>
using namespace cv;
using namespace std;



//-----------------------------------【全局函数声明部分】--------------------------------------
//		描述：全局函数的声明
//-----------------------------------------------------------------------------------------------
static void ShowHelpText( );//输出帮助文字


//-----------------------------------【main( )函数】--------------------------------------------
//		描述：控制台应用程序的入口函数，我们的程序从这里开始执行
//-----------------------------------------------------------------------------------------------
int main(  )
{
	//【0】改变console字体颜色
	system("color 1F"); 

	//【0】显示欢迎和帮助文字
	ShowHelpText( );

	//【1】载入素材图
	Mat srcImage1 = imread("1.jpg",1);
	Mat srcImage2 = imread("2.jpg",1);
	if( !srcImage1.data || !srcImage2.data )
	{ printf("读取图片错误，请确定目录下是否有imread函数指定的图片存在~！ \n"); return false; }  

	//【2】使用SURF算子检测关键点
	int minHessian = 700;//SURF算法中的hessian阈值
	SurfFeatureDetector detector( minHessian );//定义一个SurfFeatureDetector（SURF） 特征检测类对象  
	std::vector<KeyPoint> keyPoint1, keyPoints2;//vector模板类，存放任意类型的动态数组

	//【3】调用detect函数检测出SURF特征关键点，保存在vector容器中
	detector.detect( srcImage1, keyPoint1 );
	detector.detect( srcImage2, keyPoints2 );

	//【4】计算描述符（特征向量）
	SurfDescriptorExtractor extractor;
	Mat descriptors1, descriptors2;
	extractor.compute( srcImage1, keyPoint1, descriptors1 );
	extractor.compute( srcImage2, keyPoints2, descriptors2 );

	//【5】使用BruteForce进行匹配
	// 实例化一个匹配器
	BruteForceMatcher< L2<float> > matcher;
	std::vector< DMatch > matches;
	//匹配两幅图中的描述子（descriptors）
	matcher.match( descriptors1, descriptors2, matches );

	//【6】绘制从两个图像中匹配出的关键点
	Mat imgMatches;
	drawMatches( srcImage1, keyPoint1, srcImage2, keyPoints2, matches, imgMatches );//进行绘制

	//【7】显示效果图
	imshow("匹配图", imgMatches );

	waitKey(0);
	return 0;
}

//-----------------------------------【ShowHelpText( )函数】----------------------------------  
//      描述：输出一些帮助信息  
//----------------------------------------------------------------------------------------------  
static void ShowHelpText()  
{  
	//输出欢迎信息和OpenCV版本
	printf("\n\n\t\t\t非常感谢购买《OpenCV3编程入门》一书！\n");
	printf("\n\n\t\t\t此为本书OpenCV2版的第90个配套示例程序\n");
	printf("\n\n\t\t\t   当前使用的OpenCV版本为：" CV_VERSION );
	printf("\n\n  ----------------------------------------------------------------------------\n");
	//输出帮助信息  
	printf(  "\n\n\n\t欢迎来到【SURF特征描述】示例程序\n\n");  
}  
