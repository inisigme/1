#pragma once
void panoramaRotation(
	cv::Mat & panoramaRGB_edges,
	cv::Mat & panoramaDepth_edges,
	cv::Mat & panoramaRGB_rotated_edges,
	cv::Mat & panoramaRGB,
	cv::Mat & panoramaRGB_rotated);
unsigned int *calcRotation(cv::Mat m);
void calcOffsetError(unsigned int* t1, unsigned int* t2, unsigned int* errorsTab, unsigned int offset, unsigned int cols);
void detectPoints(cv::Mat img1, cv::Mat img2);
void wykrywanieRotacji();
