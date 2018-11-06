#include<opencv2\opencv.hpp>
#include "opencv2/highgui.hpp"
#include<iostream>	
#include <fstream>  
class Camera 
{
public:
	int Camerarun();
	void Getpic();  //��ȡ������Ƭ
	void CameraInit();//����궨
	 cv::VideoCapture Lcap;   //����ͼ�������
	 cv::VideoCapture Rcap;	  //����ͼ�������
	 Camera();
	 ~Camera();

	//ͼƬ�ֱ���
	const int imageWidth = 320;
	const int imageHeight = 240;
	//�ǵ����
	const int boardWidth = 9;
	const int boardHeight = 6;
	const int boardCorner = boardHeight * boardWidth;
	const int imageNums = 15;//�궨ͼƬ����
	const int squareSize = 20;//���������δ�С
	const cv::Size boardSize = cv::Size(boardWidth,boardHeight);//�궨�ߴ�
	cv::Mat rgbImage, grayImage;
	cv::Mat lIntrinsic,rIntrinsic;                                          //����ڲ���  
	cv::Mat lDistortion_coeff,rDistortion_coeff;                                   //����������  
	std::vector<cv::Mat> lRvecs,rRvecs;                                        //��ת����  
	std::vector<cv::Mat> lTvecs, rTvecs;                                        //ƽ������  
	std::vector<std::vector<cv::Point2f>> lCorners,rCorners;                        //����ͼ���ҵ��Ľǵ�ļ��� ��objRealPoint һһ��Ӧ  
	std::vector<std::vector<cv::Point3f>> lObjRealPoint, rObjRealPoint;
	std::vector<cv::Point2f> corner;   //ĳһ��ͼ���ҵ��Ľǵ�  

private:
	void initCameraparam(cv::Mat &camMatrix, cv::Mat &distortion);
	void outputCameraParam(cv::Mat &lcamMatrix, cv::Mat &ldistortion, cv::Mat &rcamMatrix,\
		cv::Mat &rdistortion, std::vector<cv::Mat> rvecs, std::vector<cv::Mat> tvecs);

	void loadImages(const std::string imgdirs, std::vector<std::vector<cv::Point2f>> &corners);

	void calCameraMatrix(const std::string imgdirs, cv::Mat &camMatrix, cv::Mat &distortion,std::vector<std::vector<cv::Point2f>> &corners,\
		std::vector<std::vector<cv::Point3f>> objRealPoint, std::vector<cv::Mat> rvecs, std::vector<cv::Mat> tvecs);
};
