#include<iostream>
#include<opencv/cv.h>
#include<opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/imgcodecs/imgcodecs.hpp>

using namespace std;
using namespace cv;

int face_img(char* src, char *Cas);
int face_video(char* src, char* Cas);
void Rect_Tag_FaceAndEyes(Mat img, Mat imgGray);

CascadeClassifier face_Cascade;
CascadeClassifier eyes_Cascade;
int main()
{
	char src[200] = {};
	char CasSrc[200] = "G:/opencv3.4/opencv/sources/data/haarcascades/haarcascade_frontalface_alt2.xml";
	char CasSrc_eyes[200] = "G:/opencv3.4/opencv/sources/data/haarcascades/haarcascade_eye_tree_eyeglasses.xml";//眼睛的分类器数据
	/*
	//以下代码是图片识别的测试
	printf("请输入照片路径:");
	cin >> src;
	face_img(src, CasSrc);*/
	face_video(CasSrc, CasSrc_eyes);
	
	return 0;
}

int face_img(char * src,char *Cas) {//src:图片路径 Cas:模型路径
	Mat face_img = imread(src);
	Mat face_gray = imread(src, IMREAD_GRAYSCALE);//转灰度图

	equalizeHist(face_gray, face_gray);//提升图片对比度，不是必须的

	CascadeClassifier faceCascade;//定义分类器
	if (!faceCascade.load(Cas)) {//加载分类器
		cout << "人脸检测级联分类器没找到" << endl;
		return -1;
	}
	vector<Rect> faces;//创建一个矩形
	faceCascade.detectMultiScale(face_gray,faces,1.2,5,0,Size(30,30));//检测目标
	for (auto b : faces) {//遍历目标
		cout << "人脸位置:(x,y)" << "(" << b.x << "," << b.y << "),\n(width,heigh)" << b.width << "," << b.height<<")\n";
		cout << "=================================================" << endl;
	}
	cout << "count:" << faces.size() << endl;
	if (faces.size() > 0) {
		for (size_t i = 0; i < faces.size(); i++) {
			putText(face_img, "Face ", Point(faces[i].x+(faces[i].width/2), faces[i].y-10), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 255), 1, 8);
			rectangle(face_img, Point(faces[i].x,faces[i].y), Point(faces[i].x+faces[i].width,faces[i].y+faces[i].height), Scalar(0, 0, 255), 1, 8);//画矩形

		}
	
	}
	imshow("images",face_img);
	waitKey();
	return 0;
}

int face_video(char* src, char* Cas) {
	VideoCapture cap(0);//打开摄像头

	if (!face_Cascade.load(src)) { cout << "无法读入人脸级联分类器" << endl; return -1; }
	if (!eyes_Cascade.load(Cas)) { cout << "无法读入眼部级联分类器" << endl; return -1; }
	if (!cap.isOpened()) { cout << "无法打开摄像头" << endl; return -1; }

	Mat img, imgGray,smallImg;
	double t;
	cap.set(CAP_PROP_FPS, 60);
	cap.set(CAP_PROP_FRAME_WIDTH, 640);
	cap.set(CAP_PROP_FRAME_HEIGHT, 480);
	while (true) {

		if (!cap.read(img)) {
			cap.set(CV_CAP_PROP_POS_FRAMES, 0);
			continue;
		}
		cvtColor(img, imgGray, CV_BGR2GRAY);//转灰度
		
		t = (double)getTickCount();
		//缩放图片 提高效率
		#ifdef VERSION_2_4	
				resize(imgGray, smallImg, Size(), 4, 4, INTER_LINEAR);
		#else
				resize(imgGray, smallImg, Size(),1,1, INTER_LINEAR_EXACT);
		#endif
		
		equalizeHist(imgGray, smallImg);//均值化
		Rect_Tag_FaceAndEyes(img, smallImg);
		t = (double)getTickCount()-t;
		printf("detection time = %g ms\n", t * 1000 / getTickFrequency());
		if (waitKey(1)==27) {
		
			break;
		}

		
	}
	return 0;
}

void Rect_Tag_FaceAndEyes(Mat img,Mat imgGray) {
	vector<Rect> faces,eyes;

	face_Cascade.detectMultiScale(imgGray, faces, 1.1, 5, 0, Size(30, 30));

	//for (auto b : faces) {//遍历
	//	cout << "位置(x,y)"<<b.x<<","<<b.y<<"\n Data(w,h)"<< b.width<<","<<b.height <<"\n=============="<< endl;
	//}
	if (faces.size()) {//如果有数据
		for (size_t i = 0; i < faces.size(); i++)
		{
			putText(img, "Face", Point(faces[i].x + (faces[i].width / 2), faces[i].y - 10), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 255), 1, 8);
			//脸部标记一个矩形
			rectangle(img, Point(faces[i].x, faces[i].y), Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height), Scalar(0, 0, 255), 1, 8);
			
			Mat face_eyes_Gray = imgGray(faces[i]);
			//判断眼睛并画出圆心
			eyes_Cascade.detectMultiScale(face_eyes_Gray, eyes,1.1,5,0,Size(10,10));
			for (size_t j = 0; j < eyes.size(); j++)
			{
				Point eys_center(faces[i].x+eyes[j].x+eyes[j].width/2, faces[i].y + eyes[j].y+eyes[j].height / 2);//圆心x,y

				circle(img, eys_center, (float)(eyes[j].width + eyes[j].height) * 0.25,Scalar(0,0,255),4,8,0);

			}
		}
	}
	imshow("video", img);
}


void BackUp() {
	//Mat src = imread("C:/Users/Administrator/Desktop/haianxian.png",IMREAD_COLOR);
	//putText(src,"Hello,World",Point(200,200),FONT_HERSHEY_COMPLEX,5.0,Scalar(0,0,0),10);

	/*图片滤波操作
	//Mat ker = (Mat_<char>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);//锐化
	//Mat ker = Mat::ones(5, 5, CV_32F/(float)25);//模糊处理
	Mat ker = (Mat_<int>(2, 2) << 1, 0, 0, -1);//边缘处理
	filter2D(src, dst,src.depth(),ker);
	imshow("显示一张图片", dst);
	*/

	/*合并图片操作
	* 	Mat src2= imread("C:/Users/Administrator/Desktop/海关.png", IMREAD_COLOR);
	* if (src.cols != src2.cols || src.rows != src2.rows || src.type() != src.type()) {
		resize(src, src, Size(500, 500));
		resize(src2, src2, Size(500, 500));
	}
	addWeighted(src, 0.5, src2, 1 - 0.5, 0.0, dst);
	*/

	/*协方差矩阵计算
	* 单个图片计算
	*
	Mat means;//保存均值;
	Mat stddev;//保存标准差;
	meanStdDev(src, means,stddev);//计算一个图片的协方差矩阵值
	cout << "means row: " << means.rows << " " << "means cols:" << means.cols << endl;
	cout << "stddev row:" << stddev.rows << " " << "stddev cols:" << stddev.cols << endl;
	//计算图片的三维矩阵协方差(RGB)
	for (int row = 0; row < means.rows; row++) {
		printf("means %d=%3.f\n", row, means.at<double>(row));//计算差值
		printf("stddev %d=%3.f\n", row, stddev.at<double>(row));//计算差值
	}
	*/
	/*特征值和特征向量

		Mat data = (Mat_<double>(2, 2) <<
			2, 1,
			1, 2);//创建一个测试的矩阵
		Mat eigen_values, eigen_vector;//特征值、特征向量

		eigen(data, eigen_values, eigen_vector);//获取特征值和特征向量
		for (int i = 0; i < eigen_values.rows; i++) {
			printf("eigen values %d: %.3f\n", i, eigen_values.at<double>(i));
		}
		cout << "eigen vector :" << endl << eigen_vector << endl;
	*/
	/*PCA接口演示

	*/
	//颜色空间转换函数 cvtcolor()
	//Mat gray, binary;//灰度、二值化
	//cvtColor(src, gray, COLOR_BGR2GRAY);
	//imshow("灰度", gray);
	//二值化图片 
	/*double threshold(1,2,3,4,5)
		将图片二值化，就是将图片的像素点的灰度值设置成0 or 255 ,呈现出最明显的白与黑的效果
		1 src 原图
		2 dst 转化后的输出
		3 thresh 设定的阈值
		4 maxval 当灰度值到达/小于阈值后 将该灰度赋成的值
		5 type 当前二值化的方式
	*/
	//threshold(gray,binary,0,255,THRESH_BINARY|THRESH_OTSU);
	//imshow("二值化", binary);
	//轮廓检测
	/*void findContours(1,2,3,4,5,6)
	* 1 image : 一般是灰度图或者二值化图
	* 2 contours : 定义为 "vector<vector<Point>> contours" 是一个双重向量,
		每一组点集就是一个轮廓，有多少轮廓 contours就有多少元素
	  3 hierarchy: 定义为"vector<Vec4i> hierarchy" ,hierarchy是一个向量,向量内每个元素
	  都包含一个4个int型的数组
	  4 mode :定义轮廓的检索模式
				CV_RETR_EXTERNAL: 只检测最外围轮廓,包含在外围轮廓的内的内轮轮廓被忽略
				CV_RETR_LIST：检测所有轮廓，但不建立继承(包含)关系。
				CV_RETR_TREE：检测所有轮廓，并且建立所有的继承(包含)关系。也就是说用CV_RETR_EXTERNAL和CV_RETR_LIST方法的时候hierarchy这个变量是没用的，因为前者没有包含关系，找到的都是外轮廓，后者仅仅是找到所哟的轮廓但并不把包含关系区分。用TREE这种检测方法的时候我们的hierarchy这个参数才是有意义的。事实上，应用前两种方法的时候，我们就用findContours这个函数的第二种声明了。
				CV_RETR_CCOMP：检测所有轮廓，但是仅仅建立两层包含关系。外轮廓放到顶层，外轮廓包含的第一层内轮廓放到底层，如果内轮廓还包含轮廓，那就把这些内轮廓放到顶层去。
	*  5 mothod ：定义轮廓的近似方法
	*			  CV_CHAIN_APPROX_NONE：把轮廓上所有的点存储。
				  CV_CHAIN_APPROX_SIMPLE：只存储水平，垂直，对角直线的起始点。对drawContours函数来说，这两种方法没有区别。
	   6 offset :Point偏移量，所有的轮廓信息相对于原始图片对应的偏移量，相当于在每一个检测出轮廓的点上加上该偏移量，而且Ppint还可以是负值

	*
	*/
	//vector<vector<Point>>contours;//轮廓点向量信息
	//vector<Vec4i> hierarchy;
	//findContours(binary,contours,hierarchy,RETR_LIST,CHAIN_APPROX_NONE);
	//imshow("轮廓", src);
	//imshow("原图", src);
	//waitKey(0);
}