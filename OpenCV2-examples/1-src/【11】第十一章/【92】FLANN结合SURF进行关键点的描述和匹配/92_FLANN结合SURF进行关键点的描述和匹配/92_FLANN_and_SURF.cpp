//--------------------------------------【程序说明】-------------------------------------------
//		程序说明：《OpenCV3编程入门》OpenCV2版书本配套示例程序92
//		程序描述：FLANN结合SURF进行关键点的描述和匹配
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


//-----------------------------------【ShowHelpText( )函数】-----------------------------
//		 描述：输出一些帮助信息
//----------------------------------------------------------------------------------------------
void ShowHelpText()
{
	//输出欢迎信息和OpenCV版本
	printf("\n\n\t\t\t非常感谢购买《OpenCV3编程入门》一书！\n");
	printf("\n\n\t\t\t此为本书OpenCV2版的第92个配套示例程序\n");
	printf("\n\n\t\t\t   当前使用的OpenCV版本为：" CV_VERSION );
	printf("\n\n  ----------------------------------------------------------------------------\n");
}


//--------------------------------------【main( )函数】-----------------------------------------
//          描述：控制台应用程序的入口函数，我们的程序从这里开始执行
//-----------------------------------------------------------------------------------------------
int main( ) 
{
	//【0】改变console字体颜色
	system("color 6F"); 

	void ShowHelpText();

	//【1】载入图像、显示并转化为灰度图
	Mat trainImage = imread("1.jpg"), trainImage_gray;
	imshow("原始图",trainImage);
	cvtColor(trainImage, trainImage_gray, CV_BGR2GRAY);

	//【2】检测Surf关键点、提取训练图像描述符
	vector<KeyPoint> train_keyPoint;
	Mat trainDescriptor;
	SurfFeatureDetector featureDetector(80);
	featureDetector.detect(trainImage_gray, train_keyPoint);
	SurfDescriptorExtractor featureExtractor;
	featureExtractor.compute(trainImage_gray, train_keyPoint, trainDescriptor);

	//【3】创建基于FLANN的描述符匹配对象
	FlannBasedMatcher matcher;
	vector<Mat> train_desc_collection(1, trainDescriptor);
	matcher.add(train_desc_collection);
	matcher.train();

	//【4】创建视频对象、定义帧率
	VideoCapture cap(0);
	unsigned int frameCount = 0;//帧数

	//【5】不断循环，直到q键被按下
	while(char(waitKey(1)) != 'q')
	{
		//<1>参数设置
		int64 time0 = getTickCount();
		Mat testImage, testImage_gray;
		cap >> testImage;//采集视频到testImage中
		if(testImage.empty())
			continue;

		//<2>转化图像到灰度
		cvtColor(testImage, testImage_gray, CV_BGR2GRAY);

		//<3>检测S关键点、提取测试图像描述符
		vector<KeyPoint> test_keyPoint;
		Mat testDescriptor;
		featureDetector.detect(testImage_gray, test_keyPoint);
		featureExtractor.compute(testImage_gray, test_keyPoint, testDescriptor);

		//<4>匹配训练和测试描述符
		vector<vector<DMatch> > matches;
		matcher.knnMatch(testDescriptor, matches, 2);

		// <5>根据劳氏算法（Lowe's algorithm），得到优秀的匹配点
		vector<DMatch> goodMatches;
		for(unsigned int i = 0; i < matches.size(); i++)
		{
			if(matches[i][0].distance < 0.6 * matches[i][1].distance)
				goodMatches.push_back(matches[i][0]);
		}

		//<6>绘制匹配点并显示窗口
		Mat dstImage;
		drawMatches(testImage, test_keyPoint, trainImage, train_keyPoint, goodMatches, dstImage);
		imshow("匹配窗口", dstImage);

		//<7>输出帧率信息
		cout << "当前帧率为：" << getTickFrequency() / (getTickCount() - time0) << endl;
	}

	return 0;
}
