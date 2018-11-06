#include"iostream"
#include<opencv2\opencv.hpp>
#include "opencv2/highgui.hpp"
#include"OpenCamera.h"
#include"cv.h" 
int main()
{
	Camera d_camera;
	//d_camera.Getpic();
	d_camera.CameraInit();

	std::cin.get();
	return 0;

}
