#include "cvtest.h"
#include <opencv2/opencv.hpp>
using namespace std;

int cvtest()
{
	const char* imagename = "./Resources/test.jpg";

	//���ļ��ж���ͼ��
	cv::Mat img = cv::imread(imagename);

	//�������ͼ��ʧ��
	if (img.empty())
	{
		fprintf(stderr, "Can not load image %s\n", imagename);
		return -1;
	}

	//��ʾͼ��
	cv::imshow("image", img);

	//�˺����ȴ�������������������ͷ���
	cv::waitKey();

	return 0;
}