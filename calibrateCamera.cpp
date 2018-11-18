#include"Camera.h"

void Cout_time(int time);
void Camera::LOG(const std::string TAG, const std::string info)
{
	std::cout << TAG<<":" << info << std::endl;
}
void Camera::print(const std::string info)
{
	std::cout << info  << std::endl;
}
void Camera::CameraInit()
{
	calCameraMatrix("E:\\Code_zoom\\OpenCVcode\\CV\\CV\\Camera_pic\\Lpic\\Lpic_", lIntrinsic, lDistortion_coeff, lCorners, lObjRealPoint, lRvecs, lTvecs);
	print("按任意键进行右摄像头标定");
	std::cin.get();
	calCameraMatrix("E:\\Code_zoom\\OpenCVcode\\CV\\CV\\Camera_pic\\Rpic\\Rpic_", rIntrinsic, rDistortion_coeff, rCorners, rObjRealPoint, rRvecs, rTvecs);
	print("标定结束");
	outputCameraParam(lIntrinsic, lDistortion_coeff, rIntrinsic, rDistortion_coeff, lRvecs, lTvecs);
}

void Camera::Getpic()
{
	//相机拍照保存部分
	cv::Mat Get_Lpic, Get_Rpic;
	char choose;
	while (1)
	{
		std::cout << "S:start the camera" << std::endl;
		std::cout << "Q:quite the camera" << std::endl;
		choose = std::getchar();
		if (choose = 's')
		{
			std::cout << "Please input blankborad to get picture" << std::endl;
			int i = 0;
			while (1)
			{
				Lcap >> Get_Lpic;
				Rcap >> Get_Rpic;
				cv::imshow("Showing left picture", Get_Lpic);
				cv::imshow("Showing right picture", Get_Rpic);
				if (cv::waitKey(1) == 32)
				{
					Cout_time(0);
					std::string LimageName = "E:\\Code_zoom\\OpenCVcode\\CV\\CV\\Camera_pic\\Lpic\\Lpic_" + std::to_string(i) + ".jpg";
					std::string RimageName = "E:\\Code_zoom\\OpenCVcode\\CV\\CV\\Camera_pic\\Rpic\\Rpic_" + std::to_string(i) + ".jpg";
					Lcap >> Get_Lpic;
					Rcap >> Get_Rpic;
					//cv::imshow("showingpic", Get_Lpic);
					cv::imwrite(LimageName, Get_Lpic);
					cv::imwrite(RimageName, Get_Rpic);
					std::cout << "Successed" << std::endl;
					i++;
				}
				if (cv::waitKey(1) == 27)
					break;
			}
		}
		if (choose = 'q')
			break;
	}
	//相机拍照保存部分
	std::cout << "End!" << std::endl;
}

void Camera::loadImages(const std::string imgdirs, std::vector<std::vector<cv::Point2f>> &corners)
{
	int goodFeatNums = 0;
	cv::namedWindow("chessboard");
	std::string filename;
	int i = 0;
	while (goodFeatNums < imageNums)
	{
		filename = imgdirs + std::to_string(i) + ".jpg";
		i++;
		rgbImage = cv::imread(filename, CV_LOAD_IMAGE_COLOR);
		cv::cvtColor(rgbImage, grayImage, CV_RGB2GRAY);
		cv::imshow("Reload Image", grayImage);
		bool isFind = cv::findChessboardCorners(rgbImage, boardSize, corner, 0);
		if (isFind == true) //所有角点都被找到 说明这幅图像是可行的  
		{
			/*
			Size(5,5) 搜索窗口的一半大小
			Size(-1,-1) 死区的一半尺寸
			TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 20, 0.1)迭代终止条件
			*/
			cv::cornerSubPix(grayImage, corner, cv::Size(5, 5), cv::Size(-1, -1), cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 20, 0.1));
			cv::drawChessboardCorners(rgbImage, boardSize, corner, isFind);
			cv::imshow("chessboard", rgbImage);
			corners.push_back(corner);
			goodFeatNums++;
			std::cout << "The image is good" << std::endl;
		}
		else
		{
			std::cout << "The image is bad feature" << std::endl;
		}
		if (cv::waitKey(10) == 'q')
			break;
	}
	imageSize = rgbImage.size();
	Sleep(1000);
	cv::destroyWindow("chessboard");
	cv::destroyWindow("Reload Image");
}
void Camera::calRealPoint(std::vector<std::vector<cv::Point3f>>& obj, int boardwidth, int boardheight, \
	int imgNumber, int squaresize)
{
	//  Mat imgpoint(boardheight, boardwidth, CV_32FC3,Scalar(0,0,0));  
	std::vector<cv::Point3f> imgpoint;
	for (int rowIndex = 0; rowIndex < boardheight; rowIndex++)
	{
		for (int colIndex = 0; colIndex < boardwidth; colIndex++)
		{
			imgpoint.push_back(cv::Point3f(rowIndex * squaresize, colIndex * squaresize, 0));
		}
	}
	for (int imgIndex = 0; imgIndex < imgNumber; imgIndex++)
	{
		obj.push_back(imgpoint);
	}
}
void Camera::initCameraparam(cv::Mat &camMatrix, cv::Mat &distortion)
{

	camMatrix.create(3, 3, CV_64FC1);
	distortion.create(5, 1, CV_64FC1);

	camMatrix.at<double>(0, 0) = 256.8093262;   //fx         
	camMatrix.at<double>(0, 2) = 160.2826538;   //cx  
	camMatrix.at<double>(1, 1) = 254.7511139;   //fy  
	camMatrix.at<double>(1, 2) = 127.6264572;   //cy  

	camMatrix.at<double>(0, 1) = 0;
	camMatrix.at<double>(1, 0) = 0;
	camMatrix.at<double>(2, 0) = 0;
	camMatrix.at<double>(2, 1) = 0;
	camMatrix.at<double>(2, 2) = 1;

	/*
	k1 k2 p1 p2 p3
	*/
	distortion.at<double>(0, 0) = -0.193740;  //k1  
	distortion.at<double>(1, 0) = -0.378588;  //k2  
	distortion.at<double>(2, 0) = 0.028980;   //p1  
	distortion.at<double>(3, 0) = 0.008136;   //p2  
	distortion.at<double>(4, 0) = 0;          //p3  
}

void Camera::outputCameraParam(cv::Mat &lcamMatrix, cv::Mat &ldistortion, cv::Mat &rcamMatrix, cv::Mat &rdistortion, \
	std::vector<cv::Mat> rvecs, std::vector<cv::Mat> tvecs)
{
	std::ofstream matrixFile;
	matrixFile.open("E:\\Code_zoom\\OpenCVcode\\CV\\matrixFile.txt");
	if (!matrixFile.is_open())
	{
		print("当前参数矩阵无法正常保存，是否继续");
		std::cin.get();
	}
	/*std::cout << "fx :" << intrinsic.at<double>(0, 0) << std::endl << "fy :" << intrinsic.at<double>(1, 1) << std::endl;
	std::cout << "cx :" << intrinsic.at<double>(0, 2) << std::endl << "cy :" << intrinsic.at<double>(1, 2) << std::endl;
	std::cout << "k1 :" << distortion_coeff.at<double>(0, 0) << std::endl;
	std::cout << "k2 :" << distortion_coeff.at<double>(1, 0) << std::endl;
	std::cout << "p1 :" << distortion_coeff.at<double>(2, 0) << std::endl;
	std::cout << "p2 :" << distortion_coeff.at<double>(3, 0) << std::endl;
	std::cout << "p3 :" << distortion_coeff.at<double>(4, 0) << std::endl;
*/
//matrixFile << "Left matrix data" << std::endl;
	matrixFile << lcamMatrix.at<double>(0, 0) << std::endl << rcamMatrix.at<double>(1, 1) << std::endl;
	matrixFile << lcamMatrix.at<double>(0, 2) << std::endl << rcamMatrix.at<double>(1, 2) << std::endl;

	matrixFile << ldistortion.at<double>(0, 0) << std::endl;
	matrixFile << ldistortion.at<double>(1, 0) << std::endl;
	matrixFile << ldistortion.at<double>(2, 0) << std::endl;
	matrixFile << ldistortion.at<double>(3, 0) << std::endl;
	matrixFile << ldistortion.at<double>(4, 0) << std::endl;
	print("左参数保存成功");
	matrixFile << "================================" << std::endl;

	matrixFile << rcamMatrix.at<double>(0, 0) << std::endl << rcamMatrix.at<double>(1, 1) << std::endl;
	matrixFile << rcamMatrix.at<double>(0, 2) << std::endl << rcamMatrix.at<double>(1, 2) << std::endl;
	matrixFile << rdistortion.at<double>(0, 0) << std::endl;
	matrixFile << rdistortion.at<double>(1, 0) << std::endl;
	matrixFile << rdistortion.at<double>(2, 0) << std::endl;
	matrixFile << rdistortion.at<double>(3, 0) << std::endl;
	matrixFile << rdistortion.at<double>(4, 0) << std::endl;
	print("右参数保存成功");
	matrixFile << "================================" << std::endl;


	matrixFile.close();
	print("参数保存成功");
}

void Cout_time(int time) {
	for (int i = 0; i < time; i++)
	{
		std::cout << i << "~~~" << std::endl;
		Sleep(1000);
	}
}
void Camera::calCameraMatrix(const std::string imgdirs, cv::Mat &camMatrix, cv::Mat &distortion, \
	std::vector<std::vector<cv::Point2f>> &corners, std::vector<std::vector<cv::Point3f>> objRealPoint, \
	std::vector<cv::Mat> rvecs, std::vector<cv::Mat> tvecs)
{

	loadImages(imgdirs, corners);
	initCameraparam(camMatrix, distortion);
	print("initCamera successed");
	calRealPoint(objRealPoint, boardWidth, boardHeight, imageNums, squareSize);//计算世界坐标值
	print("cal real successed");
	cv::calibrateCamera(objRealPoint, corners, cv::Size(imageWidth, imageHeight), \
		camMatrix, distortion, rvecs, tvecs, 0);
	//相机标定计算标定矩阵和偏移矩阵
	print("calibrateCamera successed");
	cv::Mat uImage;
	cv::undistort(rgbImage, uImage, camMatrix, distortion);
	//undistort包含了两个步骤  1.cvinitUndistortMap  计算畸变矩阵    2.reMap在图像上应用这个映射
	//rgbImage 转换成一个无偏移的图像uImage，intriinsic是内参数，distortion_coeff 为相机外参数（畸变）
	cv::imshow("undistort image", uImage);
	cv::imshow("initial image", rgbImage);
	print("imshow OK");
	cv::waitKey(1000);
	cv::destroyWindow("undistort image");
	cv::destroyWindow("initial image");
}

