
//--------------------------------------【程序说明】-------------------------------------------
//		程序说明：《OpenCV3编程入门》OpenCV2版书本配套示例程序09
//		程序描述：来自OpenCV安装目录下Samples文件夹中的官方示例程序-利用光流法进行运动目标检测
//		开发测试所用操作系统： Windows 7 64bit
//		开发测试所用IDE版本：Visual Studio 2010
//		开发测试所用OpenCV版本：	2.4.9
//		2014年11月 Revised by @浅墨_毛星云
//------------------------------------------------------------------------------------------------


/************************************************************************
* Copyright(c) 2011  Yang Xian
* All rights reserved.
*
* File:	opticalFlow.cpp
* Brief: lk光流法做运动目标检测
* Version: 1.0
* Author: Yang Xian
* Email: xyang2011@sinano.ac.cn
* Date:	2011/11/18
* History:
************************************************************************/


//---------------------------------【头文件、命名空间包含部分】----------------------------
//		描述：包含程序所使用的头文件和命名空间
//-------------------------------------------------------------------------------------------------
#include <opencv2/video/video.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>
#include <cstdio>

using namespace std;
using namespace cv;





//-----------------------------------【全局函数声明】-----------------------------------------
//		描述：声明全局函数
//-------------------------------------------------------------------------------------------------
void tracking(Mat &frame, Mat &output);
bool addNewPoints();
bool acceptTrackedPoint(int i);

//-----------------------------------【全局变量声明】-----------------------------------------
//		描述：声明全局变量
//-------------------------------------------------------------------------------------------------
string window_name = "optical flow tracking";
Mat gray;	// 当前图片
Mat gray_prev;	// 预测图片
vector<Point2f> points[2];	// point0为特征点的原来位置，point1为特征点的新位置
vector<Point2f> initial;	// 初始化跟踪点的位置
vector<Point2f> features;	// 检测的特征
int maxCount = 500;	// 检测的最大特征数
double qLevel = 0.01;	// 特征检测的等级
double minDist = 10.0;	// 两特征点之间的最小距离
vector<uchar> status;	// 跟踪特征的状态，特征的流发现为1，否则为0
vector<float> err;


//--------------------------------【help( )函数】----------------------------------------------
//		描述：输出帮助信息
//-------------------------------------------------------------------------------------------------
static void help()
{
	//输出欢迎信息和OpenCV版本
	cout <<"\n\n\t\t\t非常感谢购买《OpenCV3编程入门》一书！\n"
		<<"\n\n\t\t\t此为本书OpenCV2版的第9个配套示例程序\n"
		<<	"\n\n\t\t\t   当前使用的OpenCV版本为：" << CV_VERSION 
		<<"\n\n  ----------------------------------------------------------------------------" ;
}


//-----------------------------------【main( )函数】--------------------------------------------
//		描述：控制台应用程序的入口函数，我们的程序从这里开始
//-------------------------------------------------------------------------------------------------
int main()
{

	Mat frame;
	Mat result;

	VideoCapture capture("1.avi");

	help();
	if(capture.isOpened())	// 摄像头读取文件开关
	{
		while(true)
		{
			capture >> frame;

			if(!frame.empty())
			{ 
				tracking(frame, result);
			}
			else
			{ 
				printf(" --(!) No captured frame -- Break!");
				break;
			}

			int c = waitKey(50);
			if( (char)c == 27 )
			{
				break; 
			} 
		}
	}
	return 0;
}

//-------------------------------------------------------------------------------------------------
// function: tracking
// brief: 跟踪
// parameter: frame	输入的视频帧
//			  output 有跟踪结果的视频帧
// return: void
//-------------------------------------------------------------------------------------------------
void tracking(Mat &frame, Mat &output)
{
	cvtColor(frame, gray, CV_BGR2GRAY);
	frame.copyTo(output);
	// 添加特征点
	if (addNewPoints())
	{
		goodFeaturesToTrack(gray, features, maxCount, qLevel, minDist);
		points[0].insert(points[0].end(), features.begin(), features.end());
		initial.insert(initial.end(), features.begin(), features.end());
	}

	if (gray_prev.empty())
	{
		gray.copyTo(gray_prev);
	}
	// l-k光流法运动估计
	calcOpticalFlowPyrLK(gray_prev, gray, points[0], points[1], status, err);
	// 去掉一些不好的特征点
	int k = 0;
	for (size_t i=0; i<points[1].size(); i++)
	{
		if (acceptTrackedPoint(i))
		{
			initial[k] = initial[i];
			points[1][k++] = points[1][i];
		}
	}
	points[1].resize(k);
	initial.resize(k);
	// 显示特征点和运动轨迹
	for (size_t i=0; i<points[1].size(); i++)
	{
		line(output, initial[i], points[1][i], Scalar(0, 0, 255));
		circle(output, points[1][i], 3, Scalar(0, 255, 0), -1);
	}

	// 把当前跟踪结果作为下一此参考
	swap(points[1], points[0]);
	swap(gray_prev, gray);

	imshow(window_name, output);
}

//-------------------------------------------------------------------------------------------------
// function: addNewPoints
// brief: 检测新点是否应该被添加
// parameter:
// return: 是否被添加标志
//-------------------------------------------------------------------------------------------------
bool addNewPoints()
{
	return points[0].size() <= 10;
}

//-------------------------------------------------------------------------------------------------
// function: acceptTrackedPoint
// brief: 决定哪些跟踪点被接受
// parameter:
// return:
//-------------------------------------------------------------------------------------------------
bool acceptTrackedPoint(int i)
{
	return status[i] && ((abs(points[0][i].x - points[1][i].x) + abs(points[0][i].y - points[1][i].y)) > 2);
}


