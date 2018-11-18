#include<opencv2\opencv.hpp>
#include "opencv2/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"
#include"iostream"
#include"ctime"
#include<vector>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include"windows.h"    //Sleep(5*1000);
#include"Camera.h"
#include<fstream>
int Camera:: Camerarun() {

	if (!Lcap.isOpened()) return 1;
	if (!Rcap.isOpened()) return 1;
	//Determine if the camera is on

	cv::Mat Lpic, Rpic;
	cv::Mat ImpropicL, ImpropicR, grayMatL, outMat;
	while (cv::waitKey(1) != 27 )
	{
		Lcap.read(Lpic);
		Rcap.read(Rpic);

	/*	cv::GaussianBlur(Lpic,ImpropicL,cv::Size(3,3),0,0);
		cv::cvtColor(ImpropicL, grayMatL,cv::COLOR_RGB2GRAY);
		cv::Laplacian(grayMatL, outMat,CV_16S,3,1,0);
		cv::convertScaleAbs(outMat,ImpropicL);*/

		cv::imshow("Left Image",Lpic);
		//cv::imshow("LOG", ImpropicL);

		//cv::blur(Rpic, ImpropicR, cv::Size(3, 3));
		//cv::Canny(Rpic,ImpropicR,3,5,3);
		cv::imshow("Right Image",Rpic);
		//cv::imshow("Canny", ImpropicR);
	}
	return 0;
}

Camera::Camera() {
	std::cout<<"Camera start"<<std::endl;

	Lcap.open(1);
	Lcap.set(CV_CAP_PROP_FRAME_WIDTH, 320);
	Lcap.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
	//Lcap.set(CV_CAP_PROP_FRAME_WIDTH, 960);
	//Lcap.set(CV_CAP_PROP_FRAME_HEIGHT, 720);    //双目摄像头时不能设置太高分辨率
	//open  left camera
	Rcap.open(0);
	Rcap.set(CV_CAP_PROP_FRAME_WIDTH, 320);
	Rcap.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
	//open right camera
	std::cout << "Camera start  Over" << std::endl;
	
}

Camera::~Camera() {
	std::cout << "Camera close" << std::endl;
}
