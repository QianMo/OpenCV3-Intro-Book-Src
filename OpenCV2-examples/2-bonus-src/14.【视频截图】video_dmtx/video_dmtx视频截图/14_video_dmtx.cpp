//--------------------------------------【程序说明】-------------------------------------------
//		程序说明：《OpenCV3编程入门》OpenCV2版书本附赠示例程序14
//		程序描述：视频截图
//		测试所用操作系统： Windows 7 64bit
//		测试所用IDE版本：Visual Studio 2010
//		测试所用OpenCV版本：	2.4.9
//		2014年11月 Revised by @浅墨_毛星云
//------------------------------------------------------------------------------------------------

/*
 * starter_video.cpp
 *
 *  Created on: Nov 23, 2010
 *      Author: Ethan Rublee
 *
 * A starter sample for using opencv, get a video stream and display the images
 * Use http://datamatrix.kaywa.com/  to generate datamatrix images using strings of length 3 or less.
 * easy as CV_PI right?
 */


//---------------------------------【头文件、命名空间包含部分】----------------------------
//		描述：包含程序所使用的头文件和命名空间
//------------------------------------------------------------------------------------------------
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <vector>
#include <stdio.h>
using namespace cv;
using namespace std;



//--------------------------------------【process( )函数】----------------------------------
//		 描述：进行处理
//----------------------------------------------------------------------------------------------
int process(VideoCapture& capture)
{
    int n = 0;
    char filename[200];
    string window_name = "video | q or esc to quit";
    cout << " \n\t按下【Space】空格键可以截图，图片将存放在工程目录下。\n\n\t【Esc】和【q】键 - 退出程序。 " << endl;
    namedWindow(window_name, CV_WINDOW_KEEPRATIO); //resizable window;
    Mat frame;
    for (;;)
    {
        capture >> frame;
        if (frame.empty())
            break;
        cv::Mat gray;
        cv::cvtColor(frame,gray,COLOR_RGB2GRAY);
        vector<string> codes;
        Mat corners;
        findDataMatrix(gray, codes, corners);
        drawDataMatrixCodes(frame, codes, corners);
        imshow(window_name, frame);
        char key = (char) waitKey(1); //delay N millis, usually long enough to display and capture input
        switch (key)
        {
            case 'q':
            case 'Q':
            case 27: //escape key
                return 0;
            case ' ': //Save an image
                sprintf(filename, "视频截图%.3d.jpg", n++);
                imwrite(filename, frame);
                cout << "\n\t>保存了 " << filename <<"文件到工程目录下"<< endl;
                break;
            default:
                break;
        }
    }
    return 0;
}


//-----------------------------------【main( )函数】--------------------------------------------
//		描述：控制台应用程序的入口函数，我们的程序从这里开始
//-------------------------------------------------------------------------------------------------
int main( )
{

    VideoCapture capture("1.avi"); //从文件载入视频
	//VideoCapture capture(0); //从摄像头载入视频
    if (!capture.isOpened())
    {
        cerr << "Failed to open a video device or video file!\n" << endl;
        return 1;
    }

    return process(capture);
}
