#include<vector>
#include"ctime"
#include"windows.h"    //Sleep(5*1000);
#include<iostream>
#include"fstream"
#include"string"
#include"sstream"
#include"cv.h"
#include <algorithm>
#include <iterator>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "opencv2/calib3d/calib3d.hpp"
#include"opencv2/imgproc/imgproc.hpp"
#include<opencv2/opencv.hpp>
#include "opencv2/highgui.hpp"
#include"time.h"
class CP {

public:
	cv::VideoCapture Lcap;   //正视图的左相机
	cv::Mat Intrinsic, Discoeff; //相机内外参数  
	cv::Mat binary;
	const int imageWidth = 320;
	const int imageHeight = 240; 
	int paitingPoint[240 * 2] = { 0 }; int therehold = 0;  int pointsums; int LinePoint[240 * 2][2] = { 0 };
	int picNums = 10; std::string imageName = "E:\\Code_zoom\\OpenCVcode\\CV\\CV\\test\\test_";
	void LoadMaritx();
	void readImage();
	void GetPic();
	void start();
	void run();
	void FindPoint(cv::Mat src);
	void paiting(cv::Mat src);

};
