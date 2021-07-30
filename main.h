

#pragma once
#include<iostream>
#include<opencv/cv.h>
#include<opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/imgcodecs/imgcodecs.hpp>
#include<opencv2/face.hpp> 


using namespace std;
using namespace cv;
using namespace cv::face;

static char CasSrc[200] = "G:/opencv3.4/opencv/sources/data/haarcascades/haarcascade_frontalface_alt2.xml";//人脸分类器
static char CasSrc_eyes[200] = "G:/opencv3.4/opencv/sources/data/haarcascades/haarcascade_eye_tree_eyeglasses.xml";//眼部分类器
static char FaceCasSrc[200] = "G:/opencv3.4/opencv/sources/data/haarcascades/haarcascade_frontalface_alt_tree.xml";//人脸分类器2
