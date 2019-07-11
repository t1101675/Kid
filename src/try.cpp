#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
	cv::Mat image = cv::imread("hello.png");
	cv::namedWindow("test image");
	cv::imshow("test image", image);
	cv::waitKey(0);
	return 0;

}

