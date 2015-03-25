
//--------------------------------------【程序说明】-------------------------------------------
//		程序说明：《OpenCV3编程入门》OpenCV2版书本配套示例程序95
//		程序描述：ORB的关键点和描述符的提取，使用FLANN-LSH进行匹配
//		开发测试所用操作系统： Windows 7 64bit
//		开发测试所用IDE版本：Visual Studio 2010
//		开发测试所用OpenCV版本：	2.4.9
//		2014年06月 Created by @浅墨_毛星云
//		2014年11月 Revised by @浅墨_毛星云
//------------------------------------------------------------------------------------------------



//---------------------------------【头文件、命名空间包含部分】----------------------------
//		描述：包含程序所使用的头文件和命名空间
//------------------------------------------------------------------------------------------------
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/features2d/features2d.hpp>
using namespace cv;
using namespace std;



//-----------------------------------【ShowHelpText( )函数】----------------------------------
//          描述：输出一些帮助信息
//----------------------------------------------------------------------------------------------
static void ShowHelpText()
{
	//输出欢迎信息和OpenCV版本
	printf("\n\n\t\t\t非常感谢购买《OpenCV3编程入门》一书！\n");
	printf("\n\n\t\t\t此为本书OpenCV2版的第72个配套示例程序\n");
	printf("\n\n\t\t\t   当前使用的OpenCV版本为：" CV_VERSION );
	printf("\n\n  ----------------------------------------------------------------------------\n");
	//输出一些帮助信息
	printf("\n\t欢迎来到【ORB算法描述提取，配合FLANN-LSH进行匹配】示例程序\n"); 
	printf( "\n\n\t按键操作说明: \n\n" 
		"\t\t键盘按键【ESC】- 退出程序\n" );
}


//--------------------------------------【main( )函数】-----------------------------------------
//          描述：控制台应用程序的入口函数，我们的程序从这里开始执行
//-----------------------------------------------------------------------------------------------
int main(	) 
{
	//【0】改变console字体颜色
	system("color 2F"); 

	//【0】显示帮助文字
	ShowHelpText();

	//【0】载入源图，显示并转化为灰度图
	Mat srcImage = imread("1.jpg");
	imshow("原始图",srcImage);
	Mat grayImage;
	cvtColor(srcImage, grayImage, CV_BGR2GRAY);

	//------------------检测SIFT特征点并在图像中提取物体的描述符----------------------

	//【1】参数定义
	OrbFeatureDetector featureDetector;
	vector<KeyPoint> keyPoints;
	Mat descriptors;

	//【2】调用detect函数检测出特征关键点，保存在vector容器中
	featureDetector.detect(grayImage, keyPoints);

	//【3】计算描述符（特征向量）
	OrbDescriptorExtractor featureExtractor;
	featureExtractor.compute(grayImage, keyPoints, descriptors);

	//【4】基于FLANN的描述符对象匹配
	flann::Index flannIndex(descriptors, flann::LshIndexParams(12, 20, 2), cvflann::FLANN_DIST_HAMMING);

	//【5】初始化视频采集对象
	VideoCapture cap(0);

	unsigned int frameCount = 0;//帧数

	//【6】轮询，直到按下ESC键退出循环
	while(1)
	{
		double time0 = static_cast<double>(getTickCount( ));//记录起始时间
		Mat  captureImage, captureImage_gray;//定义两个Mat变量，用于视频采集
		cap >>  captureImage;//采集视频帧
		if( captureImage.empty())//采集为空的处理
			continue;

		//转化图像到灰度
		cvtColor( captureImage, captureImage_gray, CV_BGR2GRAY);//采集的视频帧转化为灰度图

		//【7】检测SIFT关键点并提取测试图像中的描述符
		vector<KeyPoint> captureKeyPoints;
		Mat captureDescription;

		//【8】调用detect函数检测出特征关键点，保存在vector容器中
		featureDetector.detect(captureImage_gray, captureKeyPoints);

		//【9】计算描述符
		featureExtractor.compute(captureImage_gray, captureKeyPoints, captureDescription);

		//【10】匹配和测试描述符，获取两个最邻近的描述符
		Mat matchIndex(captureDescription.rows, 2, CV_32SC1), matchDistance(captureDescription.rows, 2, CV_32FC1);
		flannIndex.knnSearch(captureDescription, matchIndex, matchDistance, 2, flann::SearchParams());//调用K邻近算法

		//【11】根据劳氏算法（Lowe's algorithm）选出优秀的匹配
		vector<DMatch> goodMatches;
		for(int i = 0; i < matchDistance.rows; i++) 
		{
			if(matchDistance.at<float>(i, 0) < 0.6 * matchDistance.at<float>(i, 1)) 
			{
				DMatch dmatches(i, matchIndex.at<int>(i, 0), matchDistance.at<float>(i, 0));
				goodMatches.push_back(dmatches);
			}
		}

		//【12】绘制并显示匹配窗口
		Mat resultImage;
		drawMatches( captureImage, captureKeyPoints, srcImage, keyPoints, goodMatches, resultImage);
		imshow("匹配窗口", resultImage);

		//【13】显示帧率
		cout << ">帧率= " << getTickFrequency() / (getTickCount() - time0) << endl;

		//按下ESC键，则程序退出
		if(char(waitKey(1)) == 27) break;
	}

	return 0;
}


