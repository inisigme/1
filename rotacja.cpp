#include "stdafx.h"
#include "rotacja.h"

using namespace cv;
using namespace std;

void panoramaRotation(
	cv::Mat & panoramaRGB_bw_edges,
	cv::Mat & panoramaDEP_edges,
	cv::Mat & panoramaRGB_bw_rotated_edges,
	cv::Mat & panoramaRGB_bw,
	cv::Mat & panoramaRGB_bw_rotated)
{
	if (panoramaRGB_bw_edges.cols != panoramaDEP_edges.cols ||
		panoramaRGB_bw_edges.cols <= 0 || panoramaDEP_edges.cols <= 0) {
		std::cout << panoramaRGB_bw_edges.cols << std::endl;
		std::cout << panoramaDEP_edges.cols << std::endl;
		getchar();
		exit(2314);
	}
		
	std::cout << "poprawne rozmiary" << std::endl;

	unsigned int *t1 = calcRotation(panoramaRGB_bw_edges);
	unsigned int *t2 = calcRotation(panoramaDEP_edges);

	unsigned int *errorsTab = new unsigned int[panoramaRGB_bw_edges.cols];
	for (int i = 0; i < panoramaRGB_bw_edges.cols; i++) {
		calcOffsetError(t1,t2, errorsTab, i, panoramaRGB_bw_edges.cols);
	}
	unsigned int optOffset = 99999;
	unsigned int minDiff = 99999999;
	for (unsigned int i = 0; i < panoramaRGB_bw_edges.cols; i++) {
		if (errorsTab[i] < minDiff) {
			minDiff = errorsTab[i];
			optOffset = i;
		}
	}
	
	/*std::cout << "policzone, teraz rysowanie wykresow" << std::endl;
	cv::Mat chart = cv::Mat::zeros(panoramaRGB_bw_edges.rows, panoramaRGB_bw_edges.cols, CV_8UC3);
	for (unsigned int i = 2; i < panoramaRGB_bw_edges.cols-2; i++) {
		auto w_nr = panoramaRGB_bw_edges.rows - t1[i] * 4 - 1;
		if (w_nr < 0) w_nr = 0;
		chart.at<cv::Vec3b>(w_nr, i) = cv::Vec3b(255, 0, 0);
		w_nr = panoramaRGB_bw_edges.rows - t2[(i + optOffset) % panoramaRGB_bw_edges.cols] * 4 - 1;
		if (w_nr < 0) w_nr = 0;
		chart.at<cv::Vec3b>(w_nr, i) = cv::Vec3b(0 , 0, 255);
	//	chart.at<cv::Vec3b>(panoramaRGB_edges.rows - t2[i]*2-1, i) = cv::Vec3b(0, 0, 255);
	}
	
	std::cout << "pomniejszanie itp" << std::endl;
	cv::resize(chart, chart, cv::Size(2048, 1024), 0, 0, INTER_LANCZOS4);
	for (int i = 0; i < chart.rows; i++)
		for (int j = 0; j < chart.cols; j++) {
			if (chart.at<cv::Vec3b>(i, j)[0] > 0) 
				chart.at<cv::Vec3b>(i, j)[0] = 255;
			if (chart.at<cv::Vec3b>(i, j)[2] > 0) 
				chart.at<cv::Vec3b>(i, j)[2] = 255;
		}


	imshow("ROTACJE", chart);
	cvWaitKey();*/

	//2
	cv::Rect srcRect1(cv::Point(0, 0), cv::Size((int)panoramaRGB_bw_edges.cols - (int)optOffset, panoramaRGB_bw_edges.cols));
	cv::Rect dstRect1(cv::Point(optOffset, 0), srcRect1.size()); 

	dstRect1 = dstRect1 & cv::Rect(cv::Point(0, 0), panoramaRGB_bw_edges.size());
	srcRect1 = cv::Rect(srcRect1.tl(), dstRect1.size()); 
	panoramaRGB_bw_edges(srcRect1).copyTo(panoramaRGB_bw_rotated_edges(dstRect1));
	panoramaRGB_bw(srcRect1).copyTo(panoramaRGB_bw_rotated(dstRect1));
	
	//1
	cv::Rect srcRect2(cv::Point((int)panoramaRGB_bw_edges.cols - (int)optOffset, 0.0), cv::Size((int)optOffset, (int)panoramaRGB_bw_edges.cols));
	cv::Rect dstRect2(cv::Point(0, 0), srcRect2.size());

	dstRect2 = dstRect2 & cv::Rect(cv::Point(0, 0), panoramaRGB_bw_edges.size());
	srcRect2 = cv::Rect(srcRect2.tl(), dstRect2.size());
	panoramaRGB_bw_edges(srcRect2).copyTo(panoramaRGB_bw_rotated_edges(dstRect2));
	panoramaRGB_bw(srcRect2).copyTo(panoramaRGB_bw_rotated(dstRect2));

}

unsigned int *calcRotation(cv::Mat M) {
	unsigned int *tab = new unsigned int[M.cols];
	for (int k = 0; k < M.cols; k++) tab[k] = 0;
	for (int i = 0; i < M.rows; i++)
	{
		const unsigned char* Mi = M.ptr<unsigned char>(i);
		for (int j = 0; j < M.cols; j++)
			tab[j] += Mi[j] ? 1 : 0;
	}
	return tab;
}

void calcOffsetError(unsigned int* t1, unsigned int*t2, unsigned int * errorsTab, unsigned int offset, unsigned int cols)
{
	unsigned int sum = 0;
	for (unsigned int i = 0; i < cols; i++) {
		int diff = t1[i] - t2[(i + offset) % cols];
		sum += abs(diff);
	}
	errorsTab[offset] = sum;
}

// do not use
void wykrywanieRotacji()
{
	unsigned int m = 1;
	cv::Mat img1 = cv::imread("download/doTestu/110.jpg", IMREAD_GRAYSCALE);
	cv::resize(img1, img1, cv::Size(2048/m, 1024 / m), 0, 0, 1);
	cv::Mat img1org = img1.clone();
	cv::equalizeHist(img1, img1);
	blur(img1, img1, Size(5,5));
	cv::Canny(img1, img1, 120, 200, 3);

	cv::Mat img2 = cv::imread("download/doTestu/110.pano360rf.png", IMREAD_GRAYSCALE);
	cv::resize(img2, img2, cv::Size(2048 / m, 1024 / m), 0, 0, 1);
	cv::Mat img2org = img2.clone();
	cv::Mat backup = img2.clone();
	blur(img2, img2, Size(5,5));
	cv::Canny(img2, img2, 120, 200, 3);

	cv::hconcat(img1, img2, img1);

	int r = 600;
	cv::Mat okno1(r, r, CV_8UC1);
	cv::Mat okno2(r, r, CV_8UC1);

	cv::Rect srcRect1(cv::Point(300,300), cv::Size(r, r));
	cv::Rect dstRect1(cv::Point(0, 0), cv::Size(r, r));

	img1(srcRect1).copyTo(okno1(dstRect1));
	img2(srcRect1).copyTo(okno2(dstRect1));

	imshow("1", okno1);
	imshow("2", okno2);

	detectPoints(okno1, okno2);

	cvWaitKey();
}



