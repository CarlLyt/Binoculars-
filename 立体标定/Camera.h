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
#define DCAMERA "Binocular"
#define SCAMERA "SimpleCamera"
#define PICNUMS 38

class Camera 
{
public:
	int Camerarun();
	void Getpic();  //获取棋盘照片
	void CameraInit();//相机标定
	void LOG(const std::string TAG, const std::string info);
	void print(const std::string info);
	 cv::VideoCapture Lcap;   //正视图的左相机
	 cv::VideoCapture Rcap;	  //正视图的右相机
	 Camera();
	 ~Camera();
	 
	//图片分辨率
	const int imageWidth = 320;
	const int imageHeight = 240;
	//角点个数
	const int boardWidth = 9;
	const int boardHeight = 6;
	const int boardCorner = boardHeight * boardWidth;
	const int imageNums = 15;		//标定图片数量
	const int squareSize = 29;//单个正方形大小
	const cv::Size boardSize = cv::Size(boardWidth,boardHeight);//标定尺寸
	cv::Size imageSize;
	cv::Mat rgbImage, grayImage, rgbImage2, grayImage2;
	cv::Mat lIntrinsic,rIntrinsic;                                          //相机内参数  
	cv::Mat lDistortion_coeff,rDistortion_coeff;                                   //相机畸变参数  
	std::vector<cv::Mat> lRvecs,rRvecs;                                        //旋转向量  
	std::vector<cv::Mat> lTvecs, rTvecs;                                        //平移向量  
	std::vector<std::vector<cv::Point2f>> lCorners,rCorners;                        //各个图像找到的角点的集合 和objRealPoint 一一对应  
	std::vector<std::vector<cv::Point3f>> lObjRealPoint, rObjRealPoint;
	std::vector<std::vector<cv::Point3f>> objRealPoint;
	std::vector<cv::Point2f> corner;   //某一副图像找到的角点  
	/*		双目标定      */
	cv::Mat cameraMatrix[2], distortionMatrix[2];			//相机内参数 畸变系数
	std::vector<std::vector<cv::Point2f> >imagePoints[2];
	cv::Mat R, T, E, F;
	std::vector<cv::Point2f> dcorner[2];
	int binocularInit();

private:
	void initCameraparam(cv::Mat &camMatrix, cv::Mat &distortion);
	void outputCameraParam(cv::Mat &lcamMatrix, cv::Mat &ldistortion, cv::Mat &rcamMatrix,\
		cv::Mat &rdistortion, std::vector<cv::Mat> rvecs, std::vector<cv::Mat> tvecs);

	void loadImages(const std::string imgdirs, std::vector<std::vector<cv::Point2f>> &corners);

	void calCameraMatrix(const std::string imgdirs, cv::Mat &camMatrix, cv::Mat &distortion,std::vector<std::vector<cv::Point2f>> &corners,\
		std::vector<std::vector<cv::Point3f>> objRealPoint, std::vector<cv::Mat> rvecs, std::vector<cv::Mat> tvecs);
	void readMatirxData(const std::string fileDirs);

	void calCameraMatrix(std::vector<std::vector<cv::Point2f>> &lcorners, std::vector<std::vector<cv::Point2f>> &rcorners);
	void DloadImages(const std::string imgdirs, const std::string imgdirs2, std::vector<std::vector<cv::Point2f>> &corners, std::vector<std::vector<cv::Point2f>> &rcorners);
	void calRealPoint(std::vector<std::vector<cv::Point3f>>& obj, int boardwidth, int boardheight, \
		int imgNumber, int squaresize);
};
