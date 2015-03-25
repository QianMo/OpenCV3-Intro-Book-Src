//--------------------------------------【程序说明】-------------------------------------------
//		程序说明：《OpenCV3编程入门》OpenCV2版书本配套示例程序85
//		程序描述：cornerHarris函数用法示例
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
#include <opencv2/imgproc/imgproc.hpp>  
using namespace cv;  

int main()  
{  
	//以灰度模式载入图像并显示
	Mat srcImage = imread("1.jpg", 0);  
	imshow("原始图", srcImage);  

	//进行Harris角点检测找出角点
	Mat cornerStrength;  
	cornerHarris(srcImage, cornerStrength, 2, 3, 0.01);

	//对灰度图进行阈值操作，得到二值图并显示  
	Mat harrisCorner;  
	threshold(cornerStrength, harrisCorner, 0.00001, 255, THRESH_BINARY);  
	imshow("角点检测后的二值效果图", harrisCorner);  

	waitKey(0);  
	return 0;  
}  
