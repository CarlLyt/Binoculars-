#include<opencv2\opencv.hpp>
#include "opencv2/highgui.hpp"
#include<iostream>	
#include <fstream>  
class Camera 
{
public:
	int Camerarun();
	void Getpic();  //获取棋盘照片
	void CameraInit();//相机标定
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
	const int imageNums = 15;//标定图片数量
	const int squareSize = 20;//单个正方形大小
	const cv::Size boardSize = cv::Size(boardWidth,boardHeight);//标定尺寸
	cv::Mat rgbImage, grayImage;
	cv::Mat lIntrinsic,rIntrinsic;                                          //相机内参数  
	cv::Mat lDistortion_coeff,rDistortion_coeff;                                   //相机畸变参数  
	std::vector<cv::Mat> lRvecs,rRvecs;                                        //旋转向量  
	std::vector<cv::Mat> lTvecs, rTvecs;                                        //平移向量  
	std::vector<std::vector<cv::Point2f>> lCorners,rCorners;                        //各个图像找到的角点的集合 和objRealPoint 一一对应  
	std::vector<std::vector<cv::Point3f>> lObjRealPoint, rObjRealPoint;
	std::vector<cv::Point2f> corner;   //某一副图像找到的角点  

private:
	void initCameraparam(cv::Mat &camMatrix, cv::Mat &distortion);
	void outputCameraParam(cv::Mat &lcamMatrix, cv::Mat &ldistortion, cv::Mat &rcamMatrix,\
		cv::Mat &rdistortion, std::vector<cv::Mat> rvecs, std::vector<cv::Mat> tvecs);

	void loadImages(const std::string imgdirs, std::vector<std::vector<cv::Point2f>> &corners);

	void calCameraMatrix(const std::string imgdirs, cv::Mat &camMatrix, cv::Mat &distortion,std::vector<std::vector<cv::Point2f>> &corners,\
		std::vector<std::vector<cv::Point3f>> objRealPoint, std::vector<cv::Mat> rvecs, std::vector<cv::Mat> tvecs);
};
