#include "Lazy-Snapping.h"

#include <opencv2/imgproc/imgproc.hpp>

Mat g_imgSrc,g_imgShow;
vector<Point> g_forePts,g_backPts;
bool IsLeftDown=false,IsRightDown=false;
Point currentPoint,nextPoint;

static void OnMouse(int even,int x,int y,int,void*)
{
	if (even==CV_EVENT_LBUTTONDOWN)
	{
		currentPoint=Point(x,y);
		g_forePts.push_back(currentPoint);
		IsLeftDown=true;
		return;
	}
	if (IsLeftDown&&even==CV_EVENT_MOUSEMOVE)
	{   
		nextPoint=Point(x,y);
		g_forePts.push_back(nextPoint);
		line(g_imgShow,currentPoint,nextPoint,Scalar(255,0,0),5);  //绘制蓝色前景
		currentPoint=nextPoint;
		imshow("原图像",g_imgShow);
		return;
	}
	if (IsLeftDown&&even==CV_EVENT_LBUTTONUP)
	{
		IsLeftDown=false;
		return;
	}

	if (even==CV_EVENT_RBUTTONDOWN)
	{   
		currentPoint=Point(x,y);
		g_backPts.push_back(currentPoint);
		IsRightDown=true;
		return;
	}
	if (IsRightDown&&even==CV_EVENT_MOUSEMOVE)
	{
		nextPoint=Point(x,y);
		g_backPts.push_back(nextPoint);
		line(g_imgShow,currentPoint,nextPoint,Scalar(0,0,255),5);
		currentPoint=nextPoint;
		imshow("原图像",g_imgShow);
		return;
	}
	if (IsRightDown&&even==CV_EVENT_RBUTTONUP)
	{
		IsRightDown=false;
		return;
	}
}

//缩小图像处理
//取消背景笔的使用  

int main()
{  
	g_forePts.clear();
	g_backPts.clear();
	g_imgSrc=imread("D:\\My Pictures\\111\\2344.jpg");
	g_imgSrc.copyTo(g_imgShow);
	namedWindow("原图像");
	setMouseCallback("原图像",OnMouse);
	Mat mask,lastImg;
	while(1)
	{
		int t=waitKey();
		if (t==27) break;
		imshow("原图像",g_imgShow);
		char c=(char)t;
		if (c=='s')
		{   
			LazySnapping_Self lazySnapping;
			lazySnapping.InitLazySnapping(g_forePts,g_backPts,g_imgSrc);
			double _time=static_cast<double>(getTickCount());
			lazySnapping.Run();
			_time=1000*(static_cast<double>(getTickCount())-_time)/getTickFrequency();
			printf("%f\n",_time);
			lazySnapping.GetImageMask(mask);
			g_imgSrc.copyTo(lastImg,mask);
			imshow("目标",lastImg);
		}
	}
	mask.release();
	lastImg.release();
	g_imgSrc.release();
	g_imgShow.release();
	destroyAllWindows();
	return 0;
}