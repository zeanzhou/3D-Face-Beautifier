#include "cvtest.h"
#include <opencv2/opencv.hpp>
using namespace std;

int cvtest()
{
	const char* imagename = "./Resources/test.jpg";

	//从文件中读入图像
	cv::Mat img = cv::imread(imagename);

	//如果读入图像失败
	if (img.empty())
	{
		fprintf(stderr, "Can not load image %s\n", imagename);
		return -1;
	}

	//显示图像
	cv::imshow("image", img);

	//此函数等待按键，按键盘任意键就返回
	cv::waitKey();

	return 0;
}