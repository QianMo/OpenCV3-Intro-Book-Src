
//--------------------------------------【程序说明】-------------------------------------------
//		程序说明：《OpenCV3编程入门》OpenCV2版书本附赠示例程序11
//		程序描述：找到最小的封闭轮廓的图像
//		测试所用操作系统： Windows 7 64bit
//		测试所用IDE版本：Visual Studio 2010
//		测试所用OpenCV版本：	2.4.9
//		2014年11月 Revised by @浅墨_毛星云
//------------------------------------------------------------------------------------------------
// Program to find smallest closed contour in image encosing a clicked point
// Author: Samarth Manoj Brahmbhatt, University of Pennsylvania


//---------------------------------【头文件、命名空间包含部分】----------------------------
//		描述：包含程序所使用的头文件和命名空间
//------------------------------------------------------------------------------------------------
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace std;
using namespace cv;

//-----------------------------------【全局变量声明部分】---------------------------------------
//		描述：声明全局变量
//---------------------------------------------------------------------------------------------------
vector<vector<Point> > contours;
vector<Vec4i> heirarchy;
Mat img_all_contours;


//--------------------------【make_contours_closed( )函数】----------------------------
//		 描述：Function to make the contours closed
//----------------------------------------------------------------------------------------------
void make_contours_closed(vector<vector<Point> > contours) {
	for(int i = 0; i < contours.size(); i++) {
		vector<Point> cc;
		approxPolyDP(contours[i], cc, 0.1, true);
		contours[i] = cc;
	}
}


//--------------------------【smallest_contour( )函数】-----------------------------------
//		 描述：Function to find the index of smalledst enclosing contour in 'contours'
//----------------------------------------------------------------------------------------------
int smallest_contour(Point p, vector<vector<Point> > contours, vector<Vec4i> heirarchy) {
	int idx = 0, prev_idx = -1;
	while(idx >= 0) {
		vector<Point> c = contours[idx];
		// Point polygon test
		double d = pointPolygonTest(c, p, false);
		// If point is inside the contour, move to its child... 
		if(d > 0) {
			prev_idx = idx;
			idx = heirarchy[idx][2];
		}
		//...else check the next cotour at the same level
		else idx = heirarchy[idx][0];
	}

	return prev_idx;
}

//--------------------------------------【on_mouse( )函数】-------------------------------
//		 描述：鼠标回调函数
//----------------------------------------------------------------------------------------------
void on_mouse(int event, int x, int y, int, void *) {
	if(event != EVENT_LBUTTONDOWN) return;
	Point p(x, y);

	int idx = smallest_contour(p, contours, heirarchy);

	// If function returned a valid contour index, draw it using a thick red line
	if(idx > 0) {
		vector<vector<Point> > contour_show(1, contours[idx]);
		Mat img_show = img_all_contours.clone();
		drawContours(img_show, contour_show, -1, Scalar(0, 0, 255), 3);
		imshow("Contours", img_show);
	}
	else
		imshow("Contours", img_all_contours);
}


//-----------------------------------【main( )函数】--------------------------------------------
//		描述：控制台应用程序的入口函数，我们的程序从这里开始
//-------------------------------------------------------------------------------------------------
int main() {
	Mat img = imread("1.jpg");
	imshow("原始图",img);

	Mat edges;
	Canny(img, edges, 50, 100);
	findContours(edges, contours, heirarchy, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);
	// Make the contours closed
	make_contours_closed(contours);
	img_all_contours = img.clone();
	// Draw all contours using a thin green line
	drawContours(img_all_contours, contours, -1, Scalar(0, 255, 0));

	namedWindow("Contours", CV_WINDOW_AUTOSIZE);
	imshow("Contours", img_all_contours);

	setMouseCallback("Contours", on_mouse);

	while(char(waitKey(1)) != 'q') {}

	return 0;
}
