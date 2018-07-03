#pragma once
#include "stdafx.h"
#include "rotacja.h"
#include <iostream>
#include <fstream>
#include <sstream> 
#include <conio.h> 

void wczyanieObrazuZGlebokosci(std::string fileName, cv::Mat & glebokosci, cv::Mat & intensity);
void wczyanieObrazuZGlebokosciV2(std::string fileName, cv::Mat & glebokosci, cv::Mat & intensity);