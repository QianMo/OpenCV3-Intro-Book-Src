//--------------------------------------【程序说明】-------------------------------------------
//		程序说明：《OpenCV3编程入门》OpenCV3版书本配套示例程序20
//		程序描述：使用OpenCV进行基本的绘图操作
//		开发测试所用操作系统： Windows 7 64bit
//		开发测试所用IDE版本：Visual Studio 2010
//		开发测试所用OpenCV版本：	3.0 beta
//		2014年11月 Created by @浅墨_毛星云
//		2014年12月 Revised by @浅墨_毛星云
//------------------------------------------------------------------------------------------------


//---------------------------------【头文件、命名空间包含部分】----------------------------
//          描述：包含程序所使用的头文件和命名空间
//------------------------------------------------------------------------------------------------
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

//OpenCV3需加入头文件：
#include <opencv2/imgproc/imgproc.hpp>



//-----------------------------------【宏定义部分】-------------------------------------------- 
//		描述：定义一些辅助宏 
//------------------------------------------------------------------------------------------------ 
#define WINDOW_NAME1 "【绘制图1】"        //为窗口标题定义的宏 
#define WINDOW_NAME2 "【绘制图2】"        //为窗口标题定义的宏 
#define WINDOW_WIDTH 600//定义窗口大小的宏



//--------------------------------【全局函数声明部分】-------------------------------------
//		描述：全局函数声明
//-----------------------------------------------------------------------------------------------
void DrawEllipse( Mat img, double angle );//绘制椭圆
void DrawFilledCircle( Mat img, Point center );//绘制圆
void DrawPolygon( Mat img );//绘制多边形
void DrawLine( Mat img, Point start, Point end );//绘制线段



//-----------------------------------【ShowHelpText( )函数】----------------------------------
//          描述：输出一些帮助信息
//----------------------------------------------------------------------------------------------
void ShowHelpText()
{
	system("color 5F");
	//输出欢迎信息和OpenCV版本
	printf("\n\n\t\t\t非常感谢购买《OpenCV3编程入门》一书！\n");
	printf("\n\n\t\t\t此为本书OpenCV3版的第20个配套示例程序\n");
	printf("\n\n\t\t\t   当前使用的OpenCV版本为：" CV_VERSION );
	printf("\n\n  ----------------------------------------------------------------------------\n");
}




//---------------------------------------【main( )函数】--------------------------------------
//		描述：控制台应用程序的入口函数，我们的程序从这里开始执行
//-----------------------------------------------------------------------------------------------
int main( void )
{

	// 创建空白的Mat图像
	Mat atomImage = Mat::zeros( WINDOW_WIDTH, WINDOW_WIDTH, CV_8UC3 );
	Mat rookImage = Mat::zeros( WINDOW_WIDTH, WINDOW_WIDTH, CV_8UC3 );

	ShowHelpText();
	// ---------------------<1>绘制化学中的原子示例图------------------------

	//【1.1】先绘制出椭圆
	DrawEllipse( atomImage, 90 );
	DrawEllipse( atomImage, 0 );
	DrawEllipse( atomImage, 45 );
	DrawEllipse( atomImage, -45 );

	//【1.2】再绘制圆心
	DrawFilledCircle( atomImage, Point( WINDOW_WIDTH/2, WINDOW_WIDTH/2) );

	// ----------------------------<2>绘制组合图-----------------------------
	//【2.1】先绘制出椭圆
	DrawPolygon( rookImage );

	// 【2.2】绘制矩形
	rectangle( rookImage,
		Point( 0, 7*WINDOW_WIDTH/8 ),
		Point( WINDOW_WIDTH, WINDOW_WIDTH),
		Scalar( 0, 255, 255 ),
		-1,
		8 );

	// 【2.3】绘制一些线段
	DrawLine( rookImage, Point( 0, 15*WINDOW_WIDTH/16 ), Point( WINDOW_WIDTH, 15*WINDOW_WIDTH/16 ) );
	DrawLine( rookImage, Point( WINDOW_WIDTH/4, 7*WINDOW_WIDTH/8 ), Point( WINDOW_WIDTH/4, WINDOW_WIDTH ) );
	DrawLine( rookImage, Point( WINDOW_WIDTH/2, 7*WINDOW_WIDTH/8 ), Point( WINDOW_WIDTH/2, WINDOW_WIDTH ) );
	DrawLine( rookImage, Point( 3*WINDOW_WIDTH/4, 7*WINDOW_WIDTH/8 ), Point( 3*WINDOW_WIDTH/4, WINDOW_WIDTH ) );

	// ---------------------------<3>显示绘制出的图像------------------------
	imshow( WINDOW_NAME1, atomImage );
	moveWindow( WINDOW_NAME1, 0, 200 );
	imshow( WINDOW_NAME2, rookImage );
	moveWindow( WINDOW_NAME2, WINDOW_WIDTH, 200 );

	waitKey( 0 );
	return(0);
}



//-------------------------------【DrawEllipse( )函数】--------------------------------
//		描述：自定义的绘制函数，实现了绘制不同角度、相同尺寸的椭圆
//-----------------------------------------------------------------------------------------
void DrawEllipse( Mat img, double angle )
{
	int thickness = 2;
	int lineType = 8;

	ellipse( img,
		Point( WINDOW_WIDTH/2, WINDOW_WIDTH/2 ),
		Size( WINDOW_WIDTH/4, WINDOW_WIDTH/16 ),
		angle,
		0,
		360,
		Scalar( 255, 129, 0 ),
		thickness,
		lineType );
}


//-----------------------------------【DrawFilledCircle( )函数】---------------------------
//		描述：自定义的绘制函数，实现了实心圆的绘制
//-----------------------------------------------------------------------------------------
void DrawFilledCircle( Mat img, Point center )
{
	int thickness = -1;
	int lineType = 8;

	circle( img,
		center,
		WINDOW_WIDTH/32,
		Scalar( 0, 0, 255 ),
		thickness,
		lineType );
}


//-----------------------------------【DrawPolygon( )函数】--------------------------
//		描述：自定义的绘制函数，实现了凹多边形的绘制
//--------------------------------------------------------------------------------------
void DrawPolygon( Mat img )
{
	int lineType = 8;

	//创建一些点
	Point rookPoints[1][20];
	rookPoints[0][0]  = Point(    WINDOW_WIDTH/4,   7*WINDOW_WIDTH/8 );
	rookPoints[0][1]  = Point(  3*WINDOW_WIDTH/4,   7*WINDOW_WIDTH/8 );
	rookPoints[0][2]  = Point(  3*WINDOW_WIDTH/4,  13*WINDOW_WIDTH/16 );
	rookPoints[0][3]  = Point( 11*WINDOW_WIDTH/16, 13*WINDOW_WIDTH/16 );
	rookPoints[0][4]  = Point( 19*WINDOW_WIDTH/32,  3*WINDOW_WIDTH/8 );
	rookPoints[0][5]  = Point(  3*WINDOW_WIDTH/4,   3*WINDOW_WIDTH/8 );
	rookPoints[0][6]  = Point(  3*WINDOW_WIDTH/4,     WINDOW_WIDTH/8 );
	rookPoints[0][7]  = Point( 26*WINDOW_WIDTH/40,    WINDOW_WIDTH/8 );
	rookPoints[0][8]  = Point( 26*WINDOW_WIDTH/40,    WINDOW_WIDTH/4 );
	rookPoints[0][9]  = Point( 22*WINDOW_WIDTH/40,    WINDOW_WIDTH/4 );
	rookPoints[0][10] = Point( 22*WINDOW_WIDTH/40,    WINDOW_WIDTH/8 );
	rookPoints[0][11] = Point( 18*WINDOW_WIDTH/40,    WINDOW_WIDTH/8 );
	rookPoints[0][12] = Point( 18*WINDOW_WIDTH/40,    WINDOW_WIDTH/4 );
	rookPoints[0][13] = Point( 14*WINDOW_WIDTH/40,    WINDOW_WIDTH/4 );
	rookPoints[0][14] = Point( 14*WINDOW_WIDTH/40,    WINDOW_WIDTH/8 );
	rookPoints[0][15] = Point(    WINDOW_WIDTH/4,     WINDOW_WIDTH/8 );
	rookPoints[0][16] = Point(    WINDOW_WIDTH/4,   3*WINDOW_WIDTH/8 );
	rookPoints[0][17] = Point( 13*WINDOW_WIDTH/32,  3*WINDOW_WIDTH/8 );
	rookPoints[0][18] = Point(  5*WINDOW_WIDTH/16, 13*WINDOW_WIDTH/16 );
	rookPoints[0][19] = Point(    WINDOW_WIDTH/4,  13*WINDOW_WIDTH/16 );

	const Point* ppt[1] = { rookPoints[0] };
	int npt[] = { 20 };

	fillPoly( img,
		ppt,
		npt,
		1,
		Scalar( 255, 255, 255 ),
		lineType );
}


//-----------------------------------【DrawLine( )函数】--------------------------
//		描述：自定义的绘制函数，实现了线的绘制
//---------------------------------------------------------------------------------
void DrawLine( Mat img, Point start, Point end )
{
	int thickness = 2;
	int lineType = 8;
	line( img,
		start,
		end,
		Scalar( 0, 0, 0 ),
		thickness,
		lineType );
}