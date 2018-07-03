#pragma once
#include "glebokosci.h"

enum what_to_display {
	IMAGES = 0,
	EDGES = 1,
	ROTATED_EDGES = 2,
	ROTATED_RGB = 3,
	BW = 4,
	ROTATION_COMPARE = 5,
	ALL = 6,
	COS = 10
};

class SzkieletAlgorytmu
{
public:

	std::string panorama_jpg_str;
	std::string panorama_intensity_str;
	std::string ptx_intensity_str;

	cv::Mat panoramaRGB;
	cv::Mat panoramaRGB_bw;
	cv::Mat panoramaIntensity;
	cv::Mat panoramaDepth;

	cv::Mat panoramaRGB_bw_edges;
	cv::Mat panoramaIntensity_edges;
	cv::Mat panoramaDepth_edges;
	cv::Mat panoramaRGB_to_edge_detection;

	cv::Mat panoramaRGB_bw_rotated;
	cv::Mat panoramaRGB_bw_rotated_edges;

	cv::Size bazowaRodzielczosc;
	unsigned int m = 1; // dzielnik rozdieczosci do wyswietlania
	cv::Rect srcRect1 = cv::Rect(cv::Point(5000, 2000), cv::Size(2048, 1024));
	cv::Rect dstRect1 = cv::Rect(cv::Point(0, 0), cv::Size(2048, 1024));

	SzkieletAlgorytmu();
	~SzkieletAlgorytmu();
	void loadData();
	void displayImages(what_to_display what);
	void detectAndPerformRotation();
	void detectEdges();
	void displayPartOfImages(what_to_display what);
	void displayPartOfImage(cv::Mat & img, std::string label);
	void resizeImages();
	void displaySmaller(cv::Mat &m, std::string label);
	cv::Mat clip(cv::Mat &m);
	void kombajnDoMieleniaKrawêdziRGB();
	void kombajnDoMieleniaKrawêdziDEPTH();
	void krawedziV1(cv::Mat &src);
	void rgb_edges_detection1();
	void rgb_edges_detection2();
};

