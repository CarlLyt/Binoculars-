#include"CP.h"
/*
clock_t start, finish;
	double totaltime;
start = clock();
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	std::cout << "\n此程序的运行时间为" << totaltime << "秒！" << std::endl;
	*/
void CP::readImage() {
	LoadMaritx();
	std::cout << Intrinsic << std::endl << Discoeff << std::endl;
	cv::Mat testImage = cv::imread("test.jpg");
	cv::Mat ImpropicL;
	while(cv::waitKey(1)!=32)    //27===ESC   32==空格 
	{ 
		cv::imshow("TestImage", testImage);
		cv::undistort(testImage, ImpropicL, Intrinsic, Discoeff);   //显示修正过后的图像
		cv::imshow("undistort Image", ImpropicL);
	}
}
void CP::LoadMaritx() {
	
	Intrinsic.create(3, 3, CV_64FC1);
	Discoeff.create(5, 1, CV_64FC1);

	Intrinsic.at<double>(0, 0) = 239.0560;     
	Intrinsic.at<double>(0, 1) = 0;     
	Intrinsic.at<double>(0, 2) = 164.7223;
	Intrinsic.at<double>(1, 0) = 0;		
	Intrinsic.at<double>(1, 1) = 239.2198;   
	Intrinsic.at<double>(1, 2) = 116.5985;
	Intrinsic.at<double>(2, 0) = 0;
	Intrinsic.at<double>(2, 1) = 0;
	Intrinsic.at<double>(2, 2) = 1;

	Discoeff.at<double>(0, 0) = -0.4358;  //k1  
	Discoeff.at<double>(1, 0) = 0.1722;  //k2  
	Discoeff.at<double>(2, 0) = 0;   //p1  
	Discoeff.at<double>(3, 0) = 0;   //p2  
	Discoeff.at<double>(4, 0) = 0;   //p3  
}
void CP::GetPic() {
	Lcap.open(1);
	Lcap.set(CV_CAP_PROP_FRAME_WIDTH, 320);
	Lcap.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
	if (!Lcap.isOpened()) return ;
	cv::Mat Lpic;
	while (cv::waitKey(1) != 27)
	{
	while (cv::waitKey(1) != 32)    //空格控制
	{
		Lcap.read(Lpic);
		imageName += std::to_string(picNums++) + ".jpg";
		cv::imshow("Inital Image", Lpic);	
	}
	cv::imwrite(imageName, Lpic);
	}
	return ;
}

//void maxLine(int *a,int nums,int &num1,int &num2)
//{
//	num1 = 0;
//	num2 = *a;
//	int maxlens = num2 - num1;
//	for (int i = 0; i < nums-1; i++)
//	{
//		std::cout << maxlens << std::endl;
//		if ((*a + i + 1) - (*a + i) > maxlens)
//		{num1 = *a + i; num2 = *a + i + 1;}
//	}
//}
void CP::FindPoint(cv::Mat src)
{   //遍历一遍图片需要0.014s
	binary = cv::Mat::zeros(cv::Size(imageWidth, imageHeight), CV_32FC1);//全0矩阵
	int colsPoint[240 * 2] = { 0 }; 
	int Sumlens = 0;int position = 0; int cols, rows, pointNums = 0;
	cv::Point paiting1, paiting2;
	for (rows = 0; rows < imageHeight; rows++)
	{
		int sePoint = 0, fPoint = 0, PointNums = 0;
		int maxLine = 0; 
		for (cols = 0; cols < imageWidth; cols++)   //列遍历
		{
			
			if (float(src.at<cv::Vec3b>(rows, cols)[0]) > 100)
			{
				//paiting1.x = cols; paiting1.y = rows; cv::circle(src, paiting1, 0.1, cv::Scalar(255, 0, 0));
				colsPoint[PointNums++] = cols; 
			}
		}
		for (int k = 0; k < PointNums-1; k++)
			if (colsPoint[k+1] - colsPoint[k] > maxLine)
			{
				maxLine = colsPoint[k+1] - colsPoint[k];
				Sumlens += maxLine;
				sePoint = colsPoint[k + 1]; fPoint = colsPoint[k];
			}
		paitingPoint[pointNums++] = fPoint; paitingPoint[pointNums++] = sePoint;
	}
	therehold = Sumlens / 240;

	//把每行黑线最长线段端点画出来，每行只保留两个端点，然后按行遍历
	for (int i = 0; i < imageHeight; i++)
	{
		if((paitingPoint[position+1]- paitingPoint[position])> therehold)
		{
			/*paiting1.x = paitingPoint[position]; paiting1.y = i; cv::circle(src, paiting1, 1, cv::Scalar(255, 0, 0));
			paiting2.x = paitingPoint[position+1]; paiting2.y = i; cv::circle(src, paiting2, 1, cv::Scalar(255, 0, 0));*/
			LinePoint[pointsums][0] = paitingPoint[position]; LinePoint[pointsums +1][0] = paitingPoint[position+1];
			LinePoint[pointsums][1] = i; LinePoint[pointsums +1][1] = i;
			pointsums+=2;
		}
		position += 2; 
	}
	
	//cv::imshow("绘制点",src);
	//std::cout <<std::endl<< therehold << std::endl;
}

void CP::paiting(cv::Mat src)
{
	//int cols, rows,pointNums=0;
	pointsums = pointsums - 2;
	cv::Point paiting;
	cv::Point c1, c2, c3, c4;
	std::cout << LinePoint[pointsums][0] << "	" << LinePoint[pointsums - 1][0] << std::endl;
	std::cout << LinePoint[pointsums][1] << "	" << LinePoint[pointsums - 1][1] << std::endl;
	std::cout << LinePoint[0][0] << "	" << LinePoint[1][0] << std::endl;
	std::cout << LinePoint[0][1] << "	" << LinePoint[1][1] << std::endl;
	c1.x = LinePoint[pointsums][0]; c2.x = LinePoint[pointsums - 1][0];
	c3.x = LinePoint[0][0]; c4.x = LinePoint[1][0];

	c1.y = LinePoint[pointsums][1]; c2.y = LinePoint[pointsums - 1][1];
	c3.y = LinePoint[0][1]; c4.y = LinePoint[1][1];

	cv::line(src, c1, c2, cv::Scalar(0, 0, 255), 4);
	cv::line(src, c3, c4, cv::Scalar(0, 255, 0), 4);
	cv::line(src, c2, c4, cv::Scalar(255, 0, 0), 4);
	cv::line(src, c1, c3, cv::Scalar(255, 255, 255), 4);
	cv::imshow("绘制点", src);

	/*int colsgather[320*240] = { 0 }; int rowsgather[320*240] = {0};*/
	//for (rows = 0; rows < imageHeight; rows++)   //imageHeight=240
	//{
	//	for (cols = 0; cols < imageWidth; cols++)    //imageWidth =320
	//	{
	//		if (int(src.at<cv::Vec3b>(rows, cols)[0]) > 100)
	//		{
	//			
	//			paiting.x = cols; paiting.y = rows;
	//			cv::circle(src, paiting, 0.1, cv::Scalar(255, 0, 0));
	//		}
	//	}
	//}
	/*for (int i = 0; i < pointNums; i++)
	{
		paiting.x = colsgather[i]; paiting.y = rowsgather[i];
		cv::circle(src, paiting, 1, cv::Scalar(255, 0, 0)); 
	}*/
	/*paiting.x = 300; paiting.y = 100;
	cv::circle(src, paiting, 4, cv::Scalar(255, 0, 0)); */
	
}
void CP::start() {
	cv::Mat testImage = cv::imread("undisImage.jpg");
	//cv::Mat copyImage, grayMat, edgeImage;
	//cv::cvtColor(testImage, grayMat, cv::COLOR_RGB2GRAY);
	//cv::blur(grayMat,grayMat,cv::Size(7,7));
	//cv::Canny(grayMat,edgeImage,0,30,3);
	//cv::imshow("Canny Image", edgeImage);      //edgeImage  是图片边缘
	cv::Mat edgeImage = cv::imread("edgeImage.jpg");  //3通道

	cv::Mat copyImage;
	copyImage = edgeImage.clone();

	FindPoint(copyImage);
	paiting(testImage);
	while (cv::waitKey(1) != 32);
}

