
//--------------------------------------【程序说明】-------------------------------------------
//		程序说明：《OpenCV3编程入门》OpenCV2版书本附赠示例程序19
//		程序描述：绘制由摄像头采集视频的直方图
//		测试所用操作系统： Windows 7 64bit
//		测试所用IDE版本：Visual Studio 2010
//		测试所用OpenCV版本：	2.4.9
//		2014年11月 Revised by @浅墨_毛星云
//------------------------------------------------------------------------------------------------


//---------------------------------【头文件、命名空间包含部分】-----------------------------
//		描述：包含程序所使用的头文件和命名空间
//-------------------------------------------------------------------------------------------------
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <stdio.h>
using namespace cv;
using namespace std;



//-----------------------------------【全局变量声明部分】---------------------------------------
//		描述：声明全局变量
//---------------------------------------------------------------------------------------------------
int nFrame_num=0;
bool pause=false;
bool tracking=false;
Rect preselectROI,selectROI;//用于存放手选的矩形
bool comp=true;

Mat rhist,ghist,bhist;
int channels[]={0,1,2};
//const int histsize[]={256,256,256};
const int histsize[]={16,16,16};
const int histsize1=16;
float rranges[]={0,255};
float granges[]={0,255};
float branges[]={0,255};
float range[]={0,255};
const float *ranges1={range};//这里的ranges就相当于一个双指针了
const float *ranges[]={rranges,granges,branges};//ranges是个双指针，且前面一定要用const，即不可改变常量，提高程序的可读性和稳健性
//const float *ranges[]={{0,255},{0,255},{0,255}};



//-----------------------------------【onMouse( )函数】--------------------------------------
//		描述：鼠标回调函数
//-------------------------------------------------------------------------------------------------
void onMouse(int event,int x,int y,int,void *)
{
    if(event==CV_EVENT_LBUTTONDOWN)
    {
        selectROI.x=x;
        selectROI.y=y;
        tracking=false;
    }
    else if(event==CV_EVENT_LBUTTONUP)
    {
        selectROI.width=x-selectROI.x;
        selectROI.height=y-selectROI.y;
        tracking=true;
        comp=true;
        nFrame_num++;//选定后才算真正意义上的第一帧
        if(nFrame_num>=10)nFrame_num=10;//防止nFrame_num溢出
    }
}

//-----------------------------------【main( )函数】--------------------------------------------
//		描述：控制台应用程序的入口函数，我们的程序从这里开始
//-------------------------------------------------------------------------------------------------
int  main(int argc, const char* argv[])
{
    Mat frame,img;
    Mat staRoiHist;
    MatND RoiHist;
    int DRAW_H=400,DRAW_W=400;
    Mat draw(DRAW_W,DRAW_H,CV_8UC3,Scalar(0,0,0));//建立一个显示直方图的图片，背景为纯黑色
    int DRAW_BIN_W=cvRound(DRAW_W/histsize1);

    //打开摄像头
    VideoCapture cam(0);
    if(!cam.isOpened())
        return -1;


	cout<<"\n\n\n\n\t程序运行成功，请用鼠标在窗口中两次框选需要计算直方图的区域。"<<endl;
    //鼠标捕捉
    namedWindow("camera",1);
    namedWindow("rgb_hist",1);
    setMouseCallback("camera",onMouse,0);//这里用的是面向对象的思想，只要有鼠标动作就会调用鼠标响应函数

    while(1)
    {
        if(!pause)//暂停按钮只需控制视频的读取
        {        
        cam>>frame;
        if(frame.empty())
            break;//break此处跳出的是while语句，一般是跳出for或while语句，不要理解为跳出if语句
        }

        if(tracking)
        {
            Mat RoiImage(frame,selectROI);

			//-------------------------------------------------------------------------------------------------------------------
			//	calcHist():计算图像块的直方图矩阵                                  
			//	calcHist(),第1个参数为原数组区域列表；第二个参数为有计算几个原数组；参数3为需要统计的通道索引数；参数4为操作掩码
			//	第5个参数为存放目标直方图矩阵；参数6为需要计算的直方图的维数；参数7为每一维的bin的个数；参数8为每一维数值的取值范围
            //	参数10为每个bin的大小是否相同的标志，默认为1，即bin的大小都相同；参数11为直方图建立时清除内存痕迹标志，默认为0，即清除
			//-------------------------------------------------------------------------------------------------------------------
            calcHist(&RoiImage,1,channels,Mat(),RoiHist,3,histsize,ranges);//原数组区域RoiImage,1个源，需要统计的通道索引为{0,1,2},
                                                                            //目标直方图RoiHist，3维，每一维的bin数histsize,取值范围为
                                                                            //ranges,实际上计算出的目标矩阵类似一维矩阵。
                                                                            

            //---------------------------------------------------------------------------------------------------------------------
            //	normalize()：根据某种范数或者数值范围归一化数组
            //	normalize(),参数1表示需要归一化的数组；参数2为归一化后的目的数组;参数3表示输出数值的最小值/最大值或者输出数值的范数;
            //	参数4表示输出数值的最小值/最大值;参数5表示归一化数组使用的归一化类型，默认值为使用L2范数;参数6为对应元素的掩膜矩阵
            //	默认值为空，即不采用掩膜操作                                        
			//---------------------------------------------------------------------------------------------------------------------
            normalize(RoiHist,RoiHist);//使用L2范数将RoiHist直方图原地归一化

            vector<Mat> rgb_planes;//这里的vector为向量，向量的数据类型为Mat结构体，向量的长度为3
            split(RoiImage,rgb_planes);//将rgb图分解到rgb_planes各个分量中
            calcHist(&rgb_planes[0],1,0,Mat(),rhist,1,&histsize1,&ranges1);
            normalize(rhist,rhist,0,DRAW_H,NORM_MINMAX);//进行最大最小值归一化
            calcHist(&rgb_planes[1],1,0,Mat(),ghist,1,&histsize1,&ranges1);
            normalize(ghist,ghist,0,DRAW_H,NORM_MINMAX);
            calcHist(&rgb_planes[2],1,0,Mat(),bhist,1,&histsize1,&ranges1);
            normalize(bhist,bhist,0,DRAW_H,NORM_MINMAX);
            if(nFrame_num==1)
            {
//                preselectROI=selectROI;
                preselectROI.x=selectROI.x;
                preselectROI.y=selectROI.y;
                preselectROI.width=selectROI.width;
                preselectROI.height=selectROI.height;
                staRoiHist=RoiHist.clone();//第一次选定目标，作为标准模板目标
            }
            else if(nFrame_num>1&&comp==true)
            {
            //---------------------------------------------------------------------------------------------------------------------
			//	compareHist():比较2个直方图的相似度
            //	compareHist()，参数1为比较相似度的直方图1；参数2为比较相似度的直方图2；参数3为相似度的计算方式。有四种，
            //	分别为CV_COMP_CORREL,CV_COMP_CHISQR,CV_COMP_INTERSECT,CV_COMP_BHATTACHARYYA   
            //---------------------------------------------------------------------------------------------------------------------
                double distence=compareHist(staRoiHist,RoiHist,CV_COMP_INTERSECT);//计算后面选定的与这次选定的相似度，使用INTERSECT，值越大越相似
                printf("\n\t>与第1次选定的图像区域相似度为：%f\n",distence);//数组越大，相似度越大

                //显示直方图
                for(int i=1;i<histsize1;i++)
                {
                    //画直线中要注意2点，因为图片的原点在左上角，而直方图坐标系的原点在左下角，所以高度值都需要被直方图图纸高度减掉，另外取一维直方图时只能用at运算符
                    line(draw,Point(DRAW_BIN_W*(i-1),DRAW_H-cvRound(rhist.at<float>((i-1)))),Point(DRAW_BIN_W*(i),DRAW_H-cvRound(rhist.at<float>(i))),Scalar(255,0,0),2,8,0);
                    line(draw,Point(DRAW_BIN_W*(i-1),DRAW_H-cvRound(ghist.at<float>((i-1)))),Point(DRAW_BIN_W*(i),DRAW_H-cvRound(ghist.at<float>(i))),Scalar(0,255,0),2,8,0);
                    line(draw,Point(DRAW_BIN_W*(i-1),DRAW_H-cvRound(bhist.at<float>((i-1)))),Point(DRAW_BIN_W*(i),DRAW_H-cvRound(bhist.at<float>(i))),Scalar(0,0,255),2,8,0);
                }
                imshow("rgb_hist",draw);
                draw=Mat::zeros(DRAW_W,DRAW_H,CV_8UC3);//每画完一次直方图后都进行一次清0操作
                comp=false;
            }
            rectangle(frame,selectROI,Scalar(0,255,0),2,8);//手动选定一次就显示一次
        }//end tracking
        rectangle(frame,preselectROI,Scalar(0,0,255),5,8);//初始的选定目标一直不变
        imshow("camera",frame);

        //键盘响应
        char c = (char)waitKey(10);
        if( c == 27 )
            break;
        switch(c)
        {
        case 'p'://暂停键
            pause = !pause;
            break;
        default:
            ;
        }
    }//end while;
    return 0;
}