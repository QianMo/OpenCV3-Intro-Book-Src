
//--------------------------------------【程序说明】-------------------------------------------
//		程序说明：《OpenCV3编程入门》OpenCV2版书本配套示例程序13
//		程序描述：来自OpenCV安装目录下Samples文件夹中的官方示例程序-支持向量机SVM之处理线性不可分数据
//		测试所用操作系统： Windows 7 64bit
//		测试所用IDE版本：Visual Studio 2010
//		测试所用OpenCV版本：	2.4.9
//		2014年11月 Revised by @浅墨_毛星云
//------------------------------------------------------------------------------------------------



//---------------------------------【头文件、命名空间包含部分】----------------------------
//		描述：包含程序所使用的头文件和命名空间
//------------------------------------------------------------------------------------------------
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>

#define NTRAINING_SAMPLES   100         // 每类训练样本的数量
#define FRAC_LINEAR_SEP     0.9f        //  部分(Fraction)线性可分的样本组成部分

using namespace cv;
using namespace std;




//-----------------------------------【ShowHelpText( )函数】----------------------------------
//          描述：输出一些帮助信息
//----------------------------------------------------------------------------------------------
void ShowHelpText()
{
	//输出欢迎信息和OpenCV版本
	printf("\n\n\t\t\t非常感谢购买《OpenCV3编程入门》一书！\n");
	printf("\n\n\t\t\t此为本书OpenCV2版的第13个配套示例程序\n");
	printf("\n\n\t\t\t   当前使用的OpenCV版本为：" CV_VERSION );
	printf("\n\n  ----------------------------------------------------------------------------\n");
}

//-----------------------------------【main( )函数】--------------------------------------------
//		描述：控制台应用程序的入口函数，我们的程序从这里开始
//-------------------------------------------------------------------------------------------------
int main()
{
	//设置视觉表达的参数
	const int WIDTH = 512, HEIGHT = 512;
	Mat I = Mat::zeros(HEIGHT, WIDTH, CV_8UC3);
	ShowHelpText();

	//--------------------- 【1】随机建立训练数据 ---------------------------------------
	Mat trainData(2*NTRAINING_SAMPLES, 2, CV_32FC1);
	Mat labels   (2*NTRAINING_SAMPLES, 1, CV_32FC1);

	RNG rng(100); // 随机生成值

	//建立训练数据的线性可分的组成部分
	int nLinearSamples = (int) (FRAC_LINEAR_SEP * NTRAINING_SAMPLES);

	// 为Class1生成随机点
	Mat trainClass = trainData.rowRange(0, nLinearSamples);
	// 点的x坐标为[0,0.4)
	Mat c = trainClass.colRange(0, 1);
	rng.fill(c, RNG::UNIFORM, Scalar(1), Scalar(0.4 * WIDTH));
	// 点的Y坐标为[0,1)
	c = trainClass.colRange(1,2);
	rng.fill(c, RNG::UNIFORM, Scalar(1), Scalar(HEIGHT));

	// 为Class2生成随机点
	trainClass = trainData.rowRange(2*NTRAINING_SAMPLES-nLinearSamples, 2*NTRAINING_SAMPLES);
	// 点的x坐标为[0.6, 1]
	c = trainClass.colRange(0 , 1); 
	rng.fill(c, RNG::UNIFORM, Scalar(0.6*WIDTH), Scalar(WIDTH));
	// 点的Y坐标为[0, 1)
	c = trainClass.colRange(1,2);
	rng.fill(c, RNG::UNIFORM, Scalar(1), Scalar(HEIGHT));

	//------------------建立训练数据的非线性可分组成部分 ---------------

	// 随机生成Class1和Class2的点
	trainClass = trainData.rowRange(  nLinearSamples, 2*NTRAINING_SAMPLES-nLinearSamples);
	// 点的x坐标为[0.4, 0.6)
	c = trainClass.colRange(0,1);
	rng.fill(c, RNG::UNIFORM, Scalar(0.4*WIDTH), Scalar(0.6*WIDTH)); 
	// 点的y坐标为[0, 1)
	c = trainClass.colRange(1,2);
	rng.fill(c, RNG::UNIFORM, Scalar(1), Scalar(HEIGHT));

	//-------------------------设置类标签 ---------------------------------
	labels.rowRange(                0,   NTRAINING_SAMPLES).setTo(1);  // Class 1
	labels.rowRange(NTRAINING_SAMPLES, 2*NTRAINING_SAMPLES).setTo(2);  // Class 2

	//------------------------ 2. 设置支持向量机的参数 --------------------
	CvSVMParams params;
	params.svm_type    = SVM::C_SVC;
	params.C           = 0.1;
	params.kernel_type = SVM::LINEAR;
	params.term_crit   = TermCriteria(CV_TERMCRIT_ITER, (int)1e7, 1e-6);

	//------------------------ 3. 训练支持向量机 ----------------------------------------------------
	cout << "Starting training process" << endl;
	CvSVM svm;
	svm.train(trainData, labels, Mat(), Mat(), params);
	cout << "Finished training process" << endl;

	//------------------------ 4. 标出决策区域（decision regions） ----------------------------------------
	Vec3b green(0,100,0), blue (100,0,0);
	for (int i = 0; i < I.rows; ++i)
		for (int j = 0; j < I.cols; ++j)
		{
			Mat sampleMat = (Mat_<float>(1,2) << i, j);
			float response = svm.predict(sampleMat);

			if      (response == 1)    I.at<Vec3b>(j, i)  = green;
			else if (response == 2)    I.at<Vec3b>(j, i)  = blue;
		}

		//----------------------- 5. 显示训练数据（training data） --------------------------------------------
		int thick = -1;
		int lineType = 8;
		float px, py;
		// Class 1
		for (int i = 0; i < NTRAINING_SAMPLES; ++i)
		{
			px = trainData.at<float>(i,0);
			py = trainData.at<float>(i,1);
			circle(I, Point( (int) px,  (int) py ), 3, Scalar(0, 255, 0), thick, lineType);
		}
		// Class 2
		for (int i = NTRAINING_SAMPLES; i <2*NTRAINING_SAMPLES; ++i)
		{
			px = trainData.at<float>(i,0);
			py = trainData.at<float>(i,1);
			circle(I, Point( (int) px, (int) py ), 3, Scalar(255, 0, 0), thick, lineType);
		}

		//------------------------- 6. 显示支持向量（support vectors） --------------------------------------------
		thick = 2;
		lineType  = 8;
		int x     = svm.get_support_vector_count();

		for (int i = 0; i < x; ++i)
		{
			const float* v = svm.get_support_vector(i);
			circle( I,  Point( (int) v[0], (int) v[1]), 6, Scalar(128, 128, 128), thick, lineType);
		}

		imwrite("result.png", I);                      //保存图像到文件
		imshow("SVM for Non-Linear Training Data", I); // 显示最终窗口
		waitKey(0);
}
