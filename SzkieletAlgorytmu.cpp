#include "stdafx.h"
#include "SzkieletAlgorytmu.h"

using namespace std;
using namespace cv;

SzkieletAlgorytmu::SzkieletAlgorytmu()
{
}


SzkieletAlgorytmu::~SzkieletAlgorytmu()
{
}

cv::Mat SzkieletAlgorytmu::clip(cv::Mat &m) {
	unsigned int line_nr = 9999999;
	bool out = false;
	for (unsigned int Ri = m.rows-1; Ri > 0; Ri--) {
		for (unsigned int Ci = 0; Ci < m.cols; Ci++) {
			if (m.at<cv::uint8_t>(Ri, Ci) != 0) {
				line_nr = Ri;
				out = true;
				break;
			}
		}
		if (out) break;
	}
	std::cout << "line_nr:    " << line_nr << std::endl;

	cv::Mat ret_mat(line_nr,m.cols,CV_8UC1);

	cv::Rect srcRect1 = cv::Rect(cv::Point(0, 0), cv::Size(m.cols, line_nr));
	cv::Rect dstRect1 = cv::Rect(cv::Point(0, 0), srcRect1.size());
	m(srcRect1).copyTo(ret_mat(dstRect1));
	cv::Mat RGB = imread(panorama_jpg_str, CV_LOAD_IMAGE_COLOR);
	panoramaRGB = cv::Mat(line_nr, m.cols, CV_8UC3).clone();;
	RGB(srcRect1).copyTo(panoramaRGB(dstRect1));
	return ret_mat;
}

void SzkieletAlgorytmu::loadData()
{
	std::cout << "** wczytywanie danych" << std::endl;
	panoramaRGB_bw = clip(imread(panorama_jpg_str, 0));
	wczyanieObrazuZGlebokosci(ptx_intensity_str, panoramaDepth, panoramaIntensity);
	//wczyanieObrazuZGlebokosciV2(ptx_intensity_str, panoramaDepth, panoramaIntensity);
	cv::resize(panoramaRGB_bw, panoramaRGB_bw, cv::Size(panoramaDepth.cols, panoramaDepth.rows), 0, 0, 1);
	cv::resize(panoramaRGB, panoramaRGB, cv::Size(panoramaDepth.cols, panoramaDepth.rows), 0, 0, 1);

	std::cout << "              rows     cols" << std::endl;
	std::cout << "RGB bw SIZE:" << panoramaRGB_bw.rows << "," << panoramaRGB_bw.cols << std::endl;
	std::cout << "INT SIZE:   " << panoramaIntensity.rows << "," << panoramaIntensity.cols << std::endl;
	std::cout << "DEP SIZE:   " << panoramaDepth.rows << "," << panoramaDepth.cols << std::endl;

	if (!panoramaRGB_bw.data)                              // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl;
		exit(1);
	}
	if (!panoramaIntensity.data)                              // Check for invalid input
	{
		cout << panoramaIntensity.rows << "," << panoramaIntensity.cols << std::endl;
		cout << "Could not open or find the image" << std::endl;
		getchar();
		exit(2);
	}
	if (!panoramaDepth.data)                              // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl;
		exit(3);
	}
	std::cout << "** dane wczytane" << std::endl;
}



void SzkieletAlgorytmu::detectEdges()
{
	/*
		czesc dla glebokosci
	*/

	kombajnDoMieleniaKrawêdziDEPTH();

	/*
		czesc dla RGB
	*/
	
	//kombajnDoMieleniaKrawêdziRGB();
	rgb_edges_detection2();

	/*
		czesc dla intensity
	*/

	//cv::Canny(panoramaIntensity, panoramaIntensity_edges, 170, 200, 3);
		

	std::cout << "po            rows     cols" << std::endl;
	std::cout << "RGB SIZE:   " << panoramaRGB_bw.rows << "," << panoramaRGB_bw.cols << std::endl;
	std::cout << "INT SIZE:   " << panoramaIntensity.rows << "," << panoramaIntensity.cols << std::endl;
	std::cout << "DEP SIZE:   " << panoramaDepth.rows << "," << panoramaDepth.cols << std::endl;
}

void SzkieletAlgorytmu::displayPartOfImages(what_to_display what)
{
	cv::Mat small = cv::Mat::zeros(cv::Size(2048 / m, 1024 / m), CV_8UC1);
	switch(what) {
	case(IMAGES):
		displayPartOfImage(panoramaRGB_bw_edges, "part RGB bw edges");
		displayPartOfImage(panoramaIntensity_edges, "part INT edges");
		displayPartOfImage(panoramaDepth_edges, "part DEP edges");
		break;
	case(EDGES):
		displayPartOfImage(panoramaRGB_bw, "part RGB bw");
		displayPartOfImage(panoramaIntensity, "part INT");
		displayPartOfImage(panoramaDepth, "part DEP");
		break;
	case(ROTATED_EDGES):
		displayPartOfImage(panoramaDepth_edges, "part DEP");
		displayPartOfImage(panoramaRGB_bw_rotated_edges, "part RGB bw edges rotated");
		panoramaRGB_bw_rotated(srcRect1).copyTo(small(dstRect1));
		imshow("panorama rgb bw rotated", small);
		cvWaitKey();
		break;
	default:
		break;
	}

}

void SzkieletAlgorytmu::displayPartOfImage(cv::Mat & img, std::string label)
{
	cv::Mat small = cv::Mat::zeros(cv::Size(2048 / m, 1024 / m), CV_8UC1);
	img(srcRect1).copyTo(small(dstRect1));
	imshow(label, small);
	cvWaitKey();
}

void SzkieletAlgorytmu::resizeImages()
{

}

void SzkieletAlgorytmu::displaySmaller(cv::Mat &m, std::string label) {
	cv::Mat smaller;
	cv::resize(m, smaller, cv::Size(2048, 1024), 0, 0, 1);
	imshow(label, smaller);
	cvWaitKey();
}

void SzkieletAlgorytmu::displayImages(what_to_display what)
{
	cv::Mat small;
	switch (what) {
	case BW:
		displaySmaller(panoramaRGB_bw, "RGB bw");
		displaySmaller(panoramaDepth, "DEP");
		displaySmaller(panoramaIntensity, "INT");
		break;
	case ALL:
		displaySmaller(panoramaRGB, "RGB");
		displaySmaller(panoramaRGB_bw, "RGB bw");
		displaySmaller(panoramaDepth, "DEP");
		displaySmaller(panoramaIntensity, "INT");
		break;
	case EDGES:
		displaySmaller(panoramaRGB_bw_edges, "RGB bw edges");
		displaySmaller(panoramaDepth_edges, "DEP edges");
		break;
	case ROTATED_EDGES:
		displaySmaller(panoramaRGB_bw_rotated_edges, "RGB bw edges rotated");
		displaySmaller(panoramaIntensity_edges, "INT edges");
		break;
	case ROTATION_COMPARE:
		displaySmaller(panoramaRGB_bw_rotated, "R1 RGB");
		displaySmaller(panoramaIntensity, "R2 INT");
	case(10):
		small = cv::Mat(panoramaDepth_edges.size(), CV_8UC3);
		for (int i = 0; i < small.rows; i++)
		{
			auto Mi1 = panoramaDepth_edges.ptr<unsigned char>(i);
			auto Mi2 = panoramaRGB_bw_rotated_edges.ptr<unsigned char>(i);
			unsigned char * outptr = (unsigned char*)(small.data);
			for (int j = 0; j < small.cols; j++) 
				small.at<cv::Vec3b>(i, j) = cv::Vec3b(Mi1[j],Mi2[j],0);
		}
		displaySmaller(small, "KOLOROWY");
		break;
	default:
		break;
	}
}

void SzkieletAlgorytmu::detectAndPerformRotation()
{

	panoramaRGB_bw_rotated_edges = cv::Mat::zeros(panoramaRGB_bw_edges.rows, panoramaRGB_bw_edges.cols, CV_8UC1);
	panoramaRGB_bw_rotated = cv::Mat::zeros(panoramaRGB_bw_edges.rows, panoramaRGB_bw_edges.cols, CV_8UC1);
	panoramaRotation(
		panoramaRGB_bw_edges,
		panoramaDepth_edges,
		panoramaRGB_bw_rotated_edges,
		panoramaRGB_bw,
		panoramaRGB_bw_rotated);
	
}



void SzkieletAlgorytmu::kombajnDoMieleniaKrawêdziRGB() {
//	cv::Mat tes = panoramaRGB.clone();
	cv::Mat tes;
	cv::resize(panoramaRGB, tes,panoramaRGB.size()/2, 0, 0, 0);
	std::cout << "poczatek" << std::endl;
	GaussianBlur(tes, tes, Size(5, 5), 0, 0, BORDER_DEFAULT);
	unsigned int s = 13;
	float h = 3; int w1 = 5; int w2 = 10;

	//for (int i = 0; i < 10; i++)
	//	fastNlMeansDenoising(tes, tes, h, w1, w2);

	//cvtColor(tes, tes, cv::COLOR_BGR2HSV);
	//fastNlMeansDenoisingColored(tes, tes, h, w1, w2);
	std::cout << "rodzielanie kanalow" << std::endl;
	cv::Mat ch1small = cv::Mat::zeros(cv::Size(2048, 1024), CV_8UC1);
	cv::Mat ch2small = cv::Mat::zeros(cv::Size(2048, 1024), CV_8UC1);
	cv::Mat ch3small = cv::Mat::zeros(cv::Size(2048, 1024), CV_8UC1); 
	vector<Mat> channels(3);
	split(tes, channels);
	cv::Mat ch1 = channels[0];
	cv::Mat ch2 = channels[1];
	cv::Mat ch3 = channels[2];
	
	/*std::cout << "odszumianie" << std::endl;
	fastNlMeansDenoising(ch1, ch1, h, w1, w2);
	fastNlMeansDenoising(ch2, ch2, h, w1, w2);
	fastNlMeansDenoising(ch3, ch3, h, w1, w2);*/
	std::cout << "rozmycie" << std::endl;
	for (int i = 0; i <3; i++) {
		GaussianBlur(ch1, ch1, Size(s, s), 0, 0, BORDER_DEFAULT);
		GaussianBlur(ch2, ch2, Size(s, s), 0, 0, BORDER_DEFAULT);
		GaussianBlur(ch3, ch3, Size(s, s), 0, 0, BORDER_DEFAULT);
	}
	std::cout << "canny" << std::endl;
	Canny(ch1, ch1, 15, 20, 3);
	Canny(ch2, ch2, 15, 20, 3);
	Canny(ch3, ch3, 15, 20, 3);

	/*for (int i = 0; i < 5; i++) {
		fastNlMeansDenoising(ch1, ch1, h, w1, w2);
		fastNlMeansDenoising(ch2, ch2, h, w1, w2);
		fastNlMeansDenoising(ch3, ch3, h, w1, w2);
	}*/
	cv::Mat tm = cv::Mat::zeros(panoramaRGB.size(), CV_8UC1);
	cv::Mat result = cv::Mat::zeros(panoramaRGB.size() , CV_8UC1); 
	cv::addWeighted(ch2 ,0.33333, (ch3), 0.33333, 0.0, tm);
	cv::addWeighted(tm, 0.6666666, (ch1), 0.33333, 0.0, panoramaRGB_bw_edges);
	for (int i = 0; i < 10; i++)
		fastNlMeansDenoising(panoramaRGB_bw_edges, panoramaRGB_bw_edges, h, w1, w2);

	displaySmaller(panoramaRGB_bw_edges, "test polaczonej");
	cv::resize(panoramaRGB_bw_edges, panoramaRGB_bw_edges, panoramaRGB.size(), 0, 0, 2);
}


void SzkieletAlgorytmu::kombajnDoMieleniaKrawêdziDEPTH() {
	cv::Canny(panoramaDepth, panoramaDepth_edges,9,10, 3);
}

void SzkieletAlgorytmu::krawedziV1(cv::Mat &base)
{
	std::cout << "krawedziV1" << std::endl;
	cv::Mat src = cv::Mat(cv::Size(2048, 1024), CV_8UC1);
	base(srcRect1).copyTo(src(dstRect1));
	imshow("src", src);

	std::cout << "dst, cdst" << std::endl;
	cv::Mat dst, cdst;
	//Canny(src, dst, 50, 200, 3);
	dst = src.clone();
	cvtColor(dst, cdst, CV_GRAY2BGR);
	
	std::cout << "hough" << std::endl;
	vector<Vec4i> lines;
	HoughLinesP(dst, lines, 1, CV_PI / 180, 20 , 20, 10);
	for (size_t i = 0; i < lines.size(); i++)
	{
		Vec4i l = lines[i];
		line(cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(rand() % 2 ? 255 : 0, rand() % 2 ? 255 : 0, rand() % 2 ? 255 : 0), 2, CV_AA);
	}
	std::cout << "efekty" << std::endl;
	imshow("source", src);
	cvWaitKey();
	imshow("detected lines", cdst);
	cvWaitKey();
}

void SzkieletAlgorytmu::rgb_edges_detection1()
{
	cv::Mat tes, tes_flip, tes_s1, tes_s2, abs_tes1, abs_tes2, out;
	//cv::resize(panoramaRGB, tes, panoramaRGB.size() , 0, 0, 1);
	tes = panoramaRGB.clone();

	for(int i = 0; i < 10; i++)
		GaussianBlur(tes, tes, Size(11,11), 0, 0, BORDER_DEFAULT);
	cvtColor(tes, tes, cv::COLOR_BGR2GRAY);
	cv::flip(tes, tes_flip, -1);
	Sobel(tes, tes_s1, CV_16S, 1, 0, 3);
	Sobel(tes_flip, tes_s2, CV_16S, 1, 0, 3);
	convertScaleAbs(tes_s1, abs_tes1);
	convertScaleAbs(tes_s2, abs_tes2);
	cv::flip(abs_tes2, abs_tes2, -1);
	addWeighted(abs_tes1, 1, abs_tes2, 1, 0, out);
	out *= 5;
	displaySmaller(out,"koncowy");
	displayPartOfImage(out, "koncowy part");
	panoramaRGB_bw_edges = out.clone();
	cvWaitKey();
}

void SzkieletAlgorytmu::rgb_edges_detection2()
{
	Mat tes = panoramaRGB.clone();
	Mat out;

	float h = 3; int w1 = 5; int w2 = 10;
	//for (int i = 0; i < 1; i++)
	//	fastNlMeansDenoisingColored(tes, tes, h, w1, w2);

	for (int i = 0; i < 8; i++)
		GaussianBlur(tes, tes, Size(5, 5), 0, 0, BORDER_DEFAULT);



	Canny(tes, out, 30, 31, 3);

	displaySmaller(out, "koncowy");
	displayPartOfImage(out, "koncowy part");
	panoramaRGB_bw_edges = out.clone();
}