#include"iostream"
#include<opencv2\opencv.hpp>
#include "opencv2/highgui.hpp"
#include"Camera.h"
#include"cv.h" 
int main()
{
	Camera d_camera;
	d_camera.binocularInit();
	//d_camera.Getpic();
	//d_camera.CameraInit();
	std::cin.get();
	return 0;

}
