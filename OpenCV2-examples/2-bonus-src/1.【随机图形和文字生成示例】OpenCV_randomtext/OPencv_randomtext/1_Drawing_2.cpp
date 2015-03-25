//--------------------------------------【程序说明】-------------------------------------------
//		程序说明：《OpenCV3编程入门》OpenCV2版书本附赠示例程序01
//		程序描述：随机图形和文字生成示例
//		测试所用操作系统： Windows 7 64bit
//		测试所用IDE版本：Visual Studio 2010
//		测试所用OpenCV版本：	2.4.9
//		2014年11月 Revised by @浅墨_毛星云
//------------------------------------------------------------------------------------------------

//---------------------------------【头文件、命名空间包含部分】----------------------------
//		描述：包含程序所使用的头文件和命名空间
//------------------------------------------------------------------------------------------------
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <stdio.h>
using namespace cv;



//-----------------------------------【全局变量声明部分】---------------------------------------
//		描述：声明全局变量
//---------------------------------------------------------------------------------------------------
const int NUMBER = 100;
const int DELAY = 5;

const int window_width = 900;
const int window_height = 600;
int x_1 = -window_width/2;
int x_2 = window_width*3/2;
int y_1 = -window_width/2;
int y_2 = window_width*3/2;


//-----------------------------------【函数声明部分】-------------------------------------------
//		描述：函数原型声明
//---------------------------------------------------------------------------------------------------
static Scalar randomColor( RNG& rng );
int Drawing_Random_Lines( Mat image, char* window_name, RNG rng );
int Drawing_Random_Rectangles( Mat image, char* window_name, RNG rng );
int Drawing_Random_Ellipses( Mat image, char* window_name, RNG rng );
int Drawing_Random_Polylines( Mat image, char* window_name, RNG rng );
int Drawing_Random_Filled_Polygons( Mat image, char* window_name, RNG rng );
int Drawing_Random_Circles( Mat image, char* window_name, RNG rng );
int Displaying_Random_Text( Mat image, char* window_name, RNG rng );
int Displaying_Big_End( Mat image, char* window_name, RNG rng );




//-----------------------------------【main( )函数】-------------------------------------------
//	描述：控制台应用程序的入口函数，我们的程序从这里开始
//-----------------------------------------------------------------------------------------------
int main( void )
{
  int c;

  //创建窗口
  char window_name[] = "Drawing_2程序窗口";

  // 创建随机的RNG
  RNG rng( 0xFFFFFFFF );

  //初始化矩阵
  Mat image = Mat::zeros( window_height, window_width, CV_8UC3 );

  // 在 DELAY ms内显示窗口
  imshow( window_name, image );
  waitKey( DELAY );

  //线条的绘制
  c = Drawing_Random_Lines(image, window_name, rng);
  if( c != 0 ) return 0;

  // 矩形的绘制
  c = Drawing_Random_Rectangles(image, window_name, rng);
  if( c != 0 ) return 0;

  // 椭圆的绘制
  c = Drawing_Random_Ellipses( image, window_name, rng );
  if( c != 0 ) return 0;

  // 折线的绘制
  c = Drawing_Random_Polylines( image, window_name, rng );
  if( c != 0 ) return 0;

  // 多边形的绘制
  c = Drawing_Random_Filled_Polygons( image, window_name, rng );
  if( c != 0 ) return 0;

  // 绘制圆形
  c = Drawing_Random_Circles( image, window_name, rng );
  if( c != 0 ) return 0;

  // 随机绘制文字
  c = Displaying_Random_Text( image, window_name, rng );
  if( c != 0 ) return 0;

  //绘制最终的谢幕
  c = Displaying_Big_End( image, window_name, rng );
  if( c != 0 ) return 0;

  waitKey(0);
  return 0;
}



//-----------------------------------【randomColor( )函数】-----------------------------------
//		描述：生成随机颜色
//---------------------------------------------------------------------------------------------------
static Scalar randomColor( RNG& rng )
{
  int icolor = (unsigned) rng;
  return Scalar( icolor&255, (icolor>>8)&255, (icolor>>16)&255 );
}



//---------------------------【Drawing_Random_Lines( )函数】-------------------------------
//		描述：绘制随机的线条
//---------------------------------------------------------------------------------------------------
int Drawing_Random_Lines( Mat image, char* window_name, RNG rng )
{
  Point pt1, pt2;

  for( int i = 0; i < NUMBER; i++ )
  {
    pt1.x = rng.uniform( x_1, x_2 );
    pt1.y = rng.uniform( y_1, y_2 );
    pt2.x = rng.uniform( x_1, x_2 );
    pt2.y = rng.uniform( y_1, y_2 );

    line( image, pt1, pt2, randomColor(rng), rng.uniform(1, 10), 8 );
    imshow( window_name, image );
    if( waitKey( DELAY ) >= 0 )
      { return -1; }
  }

  return 0;
}



//---------------------------【Drawing_Random_Ellipses( )函数】----------------------------
//		描述：绘制随机的矩形
//---------------------------------------------------------------------------------------------------
int Drawing_Random_Rectangles( Mat image, char* window_name, RNG rng )
{
  Point pt1, pt2;
  int lineType = 8;
  int thickness = rng.uniform( -3, 10 );

  for( int i = 0; i < NUMBER; i++ )
  {
    pt1.x = rng.uniform( x_1, x_2 );
    pt1.y = rng.uniform( y_1, y_2 );
    pt2.x = rng.uniform( x_1, x_2 );
    pt2.y = rng.uniform( y_1, y_2 );

    rectangle( image, pt1, pt2, randomColor(rng), MAX( thickness, -1 ), lineType );

    imshow( window_name, image );
    if( waitKey( DELAY ) >= 0 )
      { return -1; }
  }

  return 0;
}



//---------------------------【Drawing_Random_Ellipses( )函数】----------------------------
//		描述：绘制随机的椭圆
//---------------------------------------------------------------------------------------------------
int Drawing_Random_Ellipses( Mat image, char* window_name, RNG rng )
{
  int lineType = 8;

  for ( int i = 0; i < NUMBER; i++ )
  {
    Point center;
    center.x = rng.uniform(x_1, x_2);
    center.y = rng.uniform(y_1, y_2);

    Size axes;
    axes.width = rng.uniform(0, 200);
    axes.height = rng.uniform(0, 200);

    double angle = rng.uniform(0, 180);

    ellipse( image, center, axes, angle, angle - 100, angle + 200,
             randomColor(rng), rng.uniform(-1,9), lineType );

    imshow( window_name, image );

    if( waitKey(DELAY) >= 0 )
      { return -1; }
  }

  return 0;
}

//---------------------------【Drawing_Random_Polylines( )函数】--------------------------
//		描述：绘制随机的折线
//---------------------------------------------------------------------------------------------------
int Drawing_Random_Polylines( Mat image, char* window_name, RNG rng )
{
  int lineType = 8;

  for( int i = 0; i< NUMBER; i++ )
  {
    Point pt[2][3];
    pt[0][0].x = rng.uniform(x_1, x_2);
    pt[0][0].y = rng.uniform(y_1, y_2);
    pt[0][1].x = rng.uniform(x_1, x_2);
    pt[0][1].y = rng.uniform(y_1, y_2);
    pt[0][2].x = rng.uniform(x_1, x_2);
    pt[0][2].y = rng.uniform(y_1, y_2);
    pt[1][0].x = rng.uniform(x_1, x_2);
    pt[1][0].y = rng.uniform(y_1, y_2);
    pt[1][1].x = rng.uniform(x_1, x_2);
    pt[1][1].y = rng.uniform(y_1, y_2);
    pt[1][2].x = rng.uniform(x_1, x_2);
    pt[1][2].y = rng.uniform(y_1, y_2);

    const Point* ppt[2] = {pt[0], pt[1]};
    int npt[] = {3, 3};

    polylines(image, ppt, npt, 2, true, randomColor(rng), rng.uniform(1,10), lineType);

    imshow( window_name, image );
    if( waitKey(DELAY) >= 0 )
      { return -1; }
  }
  return 0;
}




//---------------------------【Drawing_Random_Filled_Polygons( )函数】------------------
//		描述：绘制随机的多边形
//---------------------------------------------------------------------------------------------------
int Drawing_Random_Filled_Polygons( Mat image, char* window_name, RNG rng )
{
  int lineType = 8;

  for ( int i = 0; i < NUMBER; i++ )
  {
    Point pt[2][3];
    pt[0][0].x = rng.uniform(x_1, x_2);
    pt[0][0].y = rng.uniform(y_1, y_2);
    pt[0][1].x = rng.uniform(x_1, x_2);
    pt[0][1].y = rng.uniform(y_1, y_2);
    pt[0][2].x = rng.uniform(x_1, x_2);
    pt[0][2].y = rng.uniform(y_1, y_2);
    pt[1][0].x = rng.uniform(x_1, x_2);
    pt[1][0].y = rng.uniform(y_1, y_2);
    pt[1][1].x = rng.uniform(x_1, x_2);
    pt[1][1].y = rng.uniform(y_1, y_2);
    pt[1][2].x = rng.uniform(x_1, x_2);
    pt[1][2].y = rng.uniform(y_1, y_2);

    const Point* ppt[2] = {pt[0], pt[1]};
    int npt[] = {3, 3};

    fillPoly( image, ppt, npt, 2, randomColor(rng), lineType );

    imshow( window_name, image );
    if( waitKey(DELAY) >= 0 )
       { return -1; }
  }
  return 0;
}

//---------------------------【Drawing_Random_Circles( )函数】-----------------------------
//		描述：绘制随机的圆
//---------------------------------------------------------------------------------------------------
int Drawing_Random_Circles( Mat image, char* window_name, RNG rng )
{
  int lineType = 8;

  for (int i = 0; i < NUMBER; i++)
  {
    Point center;
    center.x = rng.uniform(x_1, x_2);
    center.y = rng.uniform(y_1, y_2);

    circle( image, center, rng.uniform(0, 300), randomColor(rng),
            rng.uniform(-1, 9), lineType );

    imshow( window_name, image );
    if( waitKey(DELAY) >= 0 )
      { return -1; }
  }

  return 0;
}

//---------------------------【Displaying_Random_Text( )函数】-----------------------------
//		描述：绘制随机的文字
//---------------------------------------------------------------------------------------------------
int Displaying_Random_Text( Mat image, char* window_name, RNG rng )
{
  int lineType = 8;

  for ( int i = 1; i < NUMBER; i++ )
  {
    Point org;
    org.x = rng.uniform(x_1, x_2);
    org.y = rng.uniform(y_1, y_2);

    putText( image, "Testing text rendering", org, rng.uniform(0,8),
             rng.uniform(0,100)*0.05+0.1, randomColor(rng), rng.uniform(1, 10), lineType);

    imshow( window_name, image );
    if( waitKey(DELAY) >= 0 )
      { return -1; }
  }

  return 0;
}

//---------------------------【Displaying_Big_End( )函数】------------------------------------
//		描述：绘制最终的结束效果
//---------------------------------------------------------------------------------------------------
int Displaying_Big_End( Mat image, char* window_name, RNG )
{
   Size textsize = getTextSize("OpenCV forever!", FONT_HERSHEY_COMPLEX, 3, 5, 0);
   Point org((window_width - textsize.width)/2, (window_height - textsize.height)/2);
   int lineType = 8;

  Mat image2;

  for( int i = 0; i < 255; i += 2 )
  {
    image2 = image - Scalar::all(i);
    putText( image2, "OpenCV forever!", org, FONT_HERSHEY_PLAIN, 3,
             Scalar(50, i, i), 5, lineType );

    imshow( window_name, image2 );
    if( waitKey(DELAY) >= 0 )
       { return -1; }
  }

  return 0;
}
