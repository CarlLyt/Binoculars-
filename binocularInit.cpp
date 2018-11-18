#include"Camera.h"
//#include"calibrateCamera.cpp"

cv::Mat mLeft(3, 3, CV_64FC1);
cv::Mat dLeft(5, 1, CV_64FC1);
cv::Mat mRight(3, 3, CV_64FC1);
cv::Mat dRight(5, 1, CV_64FC1);
cv::Mat camerNum(20, 1, CV_64FC1);
//std::vector<cv::Mat> imageLeft; std::vector<cv::Mat> imageRight;
void LOG(const std::string TAG, const std::string info)
{
	std::cout << TAG << ":" << info << std::endl;
}

template  <class type>
type stringtoNum(const std::string& str)
{
	std::istringstream matrixnum(str);
	type num;
	matrixnum >> num;
	return num;
}

void Camera::readMatirxData(const std::string fileDirs)
{
	std::string lineStirng;
	std::fstream fin;
	fin.open(fileDirs, std::ios::in);
	int i = 0; int j = 0; int cout = 0;
	while (std::getline(fin, lineStirng))
	{
		if (lineStirng[0] != '=')
		{
			camerNum.at<double>(i++, 0) = stringtoNum<double>(lineStirng);
		}
	}
	//左相机参数
	mLeft.at<double>(0, 0) = camerNum.at<double>(0, 0);
	mLeft.at<double>(1, 1) = camerNum.at<double>(1, 0);
	mLeft.at<double>(0, 2) = camerNum.at<double>(2, 0);
	mLeft.at<double>(1, 2) = camerNum.at<double>(3, 0);

	dLeft.at<double>(0, 0) = camerNum.at<double>(4, 0);
	dLeft.at<double>(1, 0) = camerNum.at<double>(5, 0);
	dLeft.at<double>(2, 0) = camerNum.at<double>(6, 0);
	dLeft.at<double>(3, 0) = camerNum.at<double>(7, 0);
	dLeft.at<double>(4, 0) = camerNum.at<double>(8, 0);

	cameraMatrix[0] = mLeft.clone();
	distortionMatrix[0] = dLeft.clone();//深拷贝
	//右相机参数
	mRight.at<double>(0, 0) = camerNum.at<double>(9, 0);
	mRight.at<double>(1, 1) = camerNum.at<double>(10, 0);
	mRight.at<double>(0, 2) = camerNum.at<double>(11, 0);
	mRight.at<double>(1, 2) = camerNum.at<double>(12, 0);

	dRight.at<double>(0, 0) = camerNum.at<double>(13, 0);
	dRight.at<double>(1, 0) = camerNum.at<double>(14, 0);
	dRight.at<double>(2, 0) = camerNum.at<double>(15, 0);
	dRight.at<double>(3, 0) = camerNum.at<double>(16, 0);
	dRight.at<double>(4, 0) = camerNum.at<double>(17, 0);

	cameraMatrix[1] = mRight.clone();
	distortionMatrix[1] = dRight.clone();//深拷贝
	LOG(DCAMERA,"Init successed");
	fin.close();
}

void Camera::DloadImages(const std::string imgdirs, const std::string imgdirs2, std::vector<std::vector<cv::Point2f>> &lcorners, \
	std::vector<std::vector<cv::Point2f>> &rcorners)
{
	int goodFeatNums = 0;
	cv::namedWindow("chessboard");
	std::string filename,filename2;
	int i = 0; int cout = 0;
	while (goodFeatNums < imageNums)
	{
		cout = 0;
		filename = imgdirs + std::to_string(i) + ".jpg";
		filename2 = imgdirs2 + std::to_string(i) + ".jpg";
		i++;
		rgbImage = cv::imread(filename, CV_LOAD_IMAGE_COLOR);	
		cv::cvtColor(rgbImage, grayImage, CV_RGB2GRAY);
		cv::imshow("Reload leftImage", grayImage);
		bool found = cv::findChessboardCorners(rgbImage, boardSize, dcorner[0], 0);
		if (found == true) //所有角点都被找到 说明这幅图像是可行的  
		{
			cv::cornerSubPix(grayImage, dcorner[0], cv::Size(5, 5), cv::Size(-1, -1), cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 20, 0.1));
			cv::drawChessboardCorners(rgbImage, boardSize, dcorner[0], found);
			cv::imshow("chessboard", rgbImage);
			cout++;
		}

		rgbImage2 = cv::imread(filename2, CV_LOAD_IMAGE_COLOR);
		cv::cvtColor(rgbImage2, grayImage2, CV_RGB2GRAY);
		cv::imshow("Reload rightImage", grayImage2);
		bool found2 = cv::findChessboardCorners(rgbImage2, boardSize, dcorner[1], 0);
		if (found2 == true) //所有角点都被找到 说明这幅图像是可行的  
		{
			/*
			Size(5,5) 搜索窗口的一半大小
			Size(-1,-1) 死区的一半尺寸
			TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 20, 0.1)迭代终止条件
			*/
			cv::cornerSubPix(grayImage2, dcorner[1], cv::Size(5, 5), cv::Size(-1, -1), cv::TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 20, 0.1));
			cv::drawChessboardCorners(rgbImage2, boardSize, dcorner[1], found2);
			cv::imshow("chessboard2", rgbImage2);
			cout++;
		}
	
		if (cout == 2)
		{
			lcorners.push_back(dcorner[0]);
			rcorners.push_back(dcorner[1]);
			goodFeatNums++;
			std::cout << "GOOD IMAGE" << std::endl;
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
	cv::destroyWindow("chessboard2");
	cv::destroyWindow("Reload rightImage");
	cv::destroyWindow("Reload leftImage");
}
void Camera::calCameraMatrix(std::vector<std::vector<cv::Point2f>> &lcorners, std::vector<std::vector<cv::Point2f>> &rcorners)
{
	DloadImages("E:\\Code_zoom\\OpenCVcode\\CV\\CV\\Camera_pic\\Lpic\\Lpic_","E:\\Code_zoom\\OpenCVcode\\CV\\CV\\Camera_pic\\Rpic\\Rpic_", lcorners,  rcorners);
	objRealPoint.resize(imageNums);
	
	for (int i = 0; i < imageNums; i++)
	{
		for (int j = 0; j < boardSize.height; j++)
			for (int k = 0; k < boardSize.width; k++)
				objRealPoint[i].push_back(cv::Point3f(k*squareSize, j*squareSize, 0));
	}

	lcorners.resize(imageNums);
	rcorners.resize(imageNums);
	/*cameraMatrix[0] = cv::initCameraMatrix2D(objRealPoint, lcorners, imageSize, 0);
	cameraMatrix[1] = cv::initCameraMatrix2D(objRealPoint, rcorners, imageSize, 0);*/
	std::cout << "Running stereo calibration ...\n";

	double rms = stereoCalibrate(objRealPoint, lcorners, rcorners,
		cameraMatrix[0], distortionMatrix[0],
		cameraMatrix[1], distortionMatrix[1],
		imageSize, R, T, E, F,
		cv::CALIB_FIX_ASPECT_RATIO +
		cv::CALIB_ZERO_TANGENT_DIST +
		cv::CALIB_USE_INTRINSIC_GUESS +
		cv::CALIB_SAME_FOCAL_LENGTH +
		cv::CALIB_RATIONAL_MODEL +
		cv::CALIB_FIX_K3 + cv::CALIB_FIX_K4 + cv::CALIB_FIX_K5,
		cv::TermCriteria(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 100, 1e-5));

	std::cout << "done with RMS error=" << rms <<std:: endl;

	double err = 0;
	int npoints = 0;
	std::vector<cv::Vec3f> lines[2];
	for (int i = 0; i < imageNums; i++)
	{
		int npt = (int)lcorners[i].size();
		cv::Mat imgpt[2];
		for (int k = 0; k < 2; k++)
		{
			imgpt[k] = cv::Mat(imagePoints[k][i]);
			cv::undistortPoints(imgpt[k], imgpt[k], cameraMatrix[k], distortionMatrix[k], cv::Mat(), cameraMatrix[k]);
			cv::computeCorrespondEpilines(imgpt[k], k + 1, F, lines[k]); //计算极线 imgpt输入角点  k + 1包含点的图像（必须为1或者2）
			//F基础矩阵  Line【k】极线参数
		}
		for (int j = 0; j < npt; j++)
		{
			double errij = fabs(lcorners[i][j].x*lines[1][j][0] +lcorners[i][j].y*lines[1][j][1] + lines[1][j][2]) +  //图1的极线方程
				fabs(rcorners[i][j].x*lines[0][j][0] +
					rcorners[i][j].y*lines[0][j][1] + lines[0][j][2]);
			err += errij;
		}
		npoints += npt;
	}
	std::cout << "average epipolar err = " << err / npoints << std::endl;

	cv::FileStorage fs("intrinsics.yml", cv::FileStorage::WRITE);
	if (fs.isOpened())
	{
		fs << "M1" << cameraMatrix[0] << "D1" << distortionMatrix[0]<<
			"M2" << cameraMatrix[1] << "D2" << distortionMatrix[1];
		fs.release();
	}
	else
		std::cout << "Error: can not save the intrinsic parameters\n";

}

int Camera::binocularInit()
{
	readMatirxData("E:\\Code_zoom\\OpenCVcode\\CV\\matrixFile.txt");
	calCameraMatrix(imagePoints[0], imagePoints[1]);
	
	return 0;
}

//=================================================================================
