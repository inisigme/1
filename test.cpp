#include "stdafx.h"
#include "SzkieletAlgorytmu.h"

#include <iostream>
#include <fstream>
#include <sstream> 
#include <conio.h> 

using namespace std;
using namespace cv;


int main(int argc, char *argv[])
{
	try {
		SzkieletAlgorytmu alg;
		alg.panorama_jpg_str = "download/PANO_JPG/350.jpg";
		alg.panorama_intensity_str = "download/PANO_SKANER/350.pano360rf.png";
		alg.ptx_intensity_str = "download/350.ptx";
		std::cout << "ladowanie danych" << std::endl;
		alg.loadData();
		//alg.displayImages(ALL);
		std::cout << "wykrywanie krawedzi" << std::endl;
		alg.detectEdges();
		//std::cout << "wyswietlanie krawedzi" << std::endl;
		//alg.displayImages(EDGES);
		std::cout << "wykrywqnaie rotacji" << std::endl;
		alg.detectAndPerformRotation();
		alg.displayImages(COS);
		//std::cout << "rotated" << std::endl;
		//alg.displayPartOfImage(alg.panoramaRGB_bw_rotated_edges, "UDALO SIE");
		alg.displayPartOfImages(ROTATED_EDGES);
		std::cout << "KRAWEDZI" << std::endl;
		alg.displayImages(ALL);
	
		alg.krawedziV1(alg.panoramaRGB_bw_rotated_edges);



		alg.krawedziV1(alg.panoramaDepth_edges);


		/*matcher*/
		/*generate ptx*/
		//std::cout << "KONIEC" << std::endl;
		//getchar();
	} catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
		getchar();
	}
	return 0;
}


