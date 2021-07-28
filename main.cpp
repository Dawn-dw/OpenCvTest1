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
	char CasSrc_eyes[200] = "G:/opencv3.4/opencv/sources/data/haarcascades/haarcascade_eye_tree_eyeglasses.xml";//�۾��ķ���������
	/*
	//���´�����ͼƬʶ��Ĳ���
	printf("��������Ƭ·��:");
	cin >> src;
	face_img(src, CasSrc);*/
	face_video(CasSrc, CasSrc_eyes);
	
	return 0;
}

int face_img(char * src,char *Cas) {//src:ͼƬ·�� Cas:ģ��·��
	Mat face_img = imread(src);
	Mat face_gray = imread(src, IMREAD_GRAYSCALE);//ת�Ҷ�ͼ

	equalizeHist(face_gray, face_gray);//����ͼƬ�Աȶȣ����Ǳ����

	CascadeClassifier faceCascade;//���������
	if (!faceCascade.load(Cas)) {//���ط�����
		cout << "������⼶��������û�ҵ�" << endl;
		return -1;
	}
	vector<Rect> faces;//����һ������
	faceCascade.detectMultiScale(face_gray,faces,1.2,5,0,Size(30,30));//���Ŀ��
	for (auto b : faces) {//����Ŀ��
		cout << "����λ��:(x,y)" << "(" << b.x << "," << b.y << "),\n(width,heigh)" << b.width << "," << b.height<<")\n";
		cout << "=================================================" << endl;
	}
	cout << "count:" << faces.size() << endl;
	if (faces.size() > 0) {
		for (size_t i = 0; i < faces.size(); i++) {
			putText(face_img, "Face ", Point(faces[i].x+(faces[i].width/2), faces[i].y-10), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 255), 1, 8);
			rectangle(face_img, Point(faces[i].x,faces[i].y), Point(faces[i].x+faces[i].width,faces[i].y+faces[i].height), Scalar(0, 0, 255), 1, 8);//������

		}
	
	}
	imshow("images",face_img);
	waitKey();
	return 0;
}

int face_video(char* src, char* Cas) {
	VideoCapture cap(0);//������ͷ

	if (!face_Cascade.load(src)) { cout << "�޷�������������������" << endl; return -1; }
	if (!eyes_Cascade.load(Cas)) { cout << "�޷������۲�����������" << endl; return -1; }
	if (!cap.isOpened()) { cout << "�޷�������ͷ" << endl; return -1; }

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
		cvtColor(img, imgGray, CV_BGR2GRAY);//ת�Ҷ�
		
		t = (double)getTickCount();
		//����ͼƬ ���Ч��
		#ifdef VERSION_2_4	
				resize(imgGray, smallImg, Size(), 4, 4, INTER_LINEAR);
		#else
				resize(imgGray, smallImg, Size(),1,1, INTER_LINEAR_EXACT);
		#endif
		
		equalizeHist(imgGray, smallImg);//��ֵ��
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

	//for (auto b : faces) {//����
	//	cout << "λ��(x,y)"<<b.x<<","<<b.y<<"\n Data(w,h)"<< b.width<<","<<b.height <<"\n=============="<< endl;
	//}
	if (faces.size()) {//���������
		for (size_t i = 0; i < faces.size(); i++)
		{
			putText(img, "Face", Point(faces[i].x + (faces[i].width / 2), faces[i].y - 10), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 255), 1, 8);
			//�������һ������
			rectangle(img, Point(faces[i].x, faces[i].y), Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height), Scalar(0, 0, 255), 1, 8);
			
			Mat face_eyes_Gray = imgGray(faces[i]);
			//�ж��۾�������Բ��
			eyes_Cascade.detectMultiScale(face_eyes_Gray, eyes,1.1,5,0,Size(10,10));
			for (size_t j = 0; j < eyes.size(); j++)
			{
				Point eys_center(faces[i].x+eyes[j].x+eyes[j].width/2, faces[i].y + eyes[j].y+eyes[j].height / 2);//Բ��x,y

				circle(img, eys_center, (float)(eyes[j].width + eyes[j].height) * 0.25,Scalar(0,0,255),4,8,0);

			}
		}
	}
	imshow("video", img);
}


void BackUp() {
	//Mat src = imread("C:/Users/Administrator/Desktop/haianxian.png",IMREAD_COLOR);
	//putText(src,"Hello,World",Point(200,200),FONT_HERSHEY_COMPLEX,5.0,Scalar(0,0,0),10);

	/*ͼƬ�˲�����
	//Mat ker = (Mat_<char>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);//��
	//Mat ker = Mat::ones(5, 5, CV_32F/(float)25);//ģ������
	Mat ker = (Mat_<int>(2, 2) << 1, 0, 0, -1);//��Ե����
	filter2D(src, dst,src.depth(),ker);
	imshow("��ʾһ��ͼƬ", dst);
	*/

	/*�ϲ�ͼƬ����
	* 	Mat src2= imread("C:/Users/Administrator/Desktop/����.png", IMREAD_COLOR);
	* if (src.cols != src2.cols || src.rows != src2.rows || src.type() != src.type()) {
		resize(src, src, Size(500, 500));
		resize(src2, src2, Size(500, 500));
	}
	addWeighted(src, 0.5, src2, 1 - 0.5, 0.0, dst);
	*/

	/*Э����������
	* ����ͼƬ����
	*
	Mat means;//�����ֵ;
	Mat stddev;//�����׼��;
	meanStdDev(src, means,stddev);//����һ��ͼƬ��Э�������ֵ
	cout << "means row: " << means.rows << " " << "means cols:" << means.cols << endl;
	cout << "stddev row:" << stddev.rows << " " << "stddev cols:" << stddev.cols << endl;
	//����ͼƬ����ά����Э����(RGB)
	for (int row = 0; row < means.rows; row++) {
		printf("means %d=%3.f\n", row, means.at<double>(row));//�����ֵ
		printf("stddev %d=%3.f\n", row, stddev.at<double>(row));//�����ֵ
	}
	*/
	/*����ֵ����������

		Mat data = (Mat_<double>(2, 2) <<
			2, 1,
			1, 2);//����һ�����Եľ���
		Mat eigen_values, eigen_vector;//����ֵ����������

		eigen(data, eigen_values, eigen_vector);//��ȡ����ֵ����������
		for (int i = 0; i < eigen_values.rows; i++) {
			printf("eigen values %d: %.3f\n", i, eigen_values.at<double>(i));
		}
		cout << "eigen vector :" << endl << eigen_vector << endl;
	*/
	/*PCA�ӿ���ʾ

	*/
	//��ɫ�ռ�ת������ cvtcolor()
	//Mat gray, binary;//�Ҷȡ���ֵ��
	//cvtColor(src, gray, COLOR_BGR2GRAY);
	//imshow("�Ҷ�", gray);
	//��ֵ��ͼƬ 
	/*double threshold(1,2,3,4,5)
		��ͼƬ��ֵ�������ǽ�ͼƬ�����ص�ĻҶ�ֵ���ó�0 or 255 ,���ֳ������Եİ���ڵ�Ч��
		1 src ԭͼ
		2 dst ת��������
		3 thresh �趨����ֵ
		4 maxval ���Ҷ�ֵ����/С����ֵ�� ���ûҶȸ��ɵ�ֵ
		5 type ��ǰ��ֵ���ķ�ʽ
	*/
	//threshold(gray,binary,0,255,THRESH_BINARY|THRESH_OTSU);
	//imshow("��ֵ��", binary);
	//�������
	/*void findContours(1,2,3,4,5,6)
	* 1 image : һ���ǻҶ�ͼ���߶�ֵ��ͼ
	* 2 contours : ����Ϊ "vector<vector<Point>> contours" ��һ��˫������,
		ÿһ��㼯����һ���������ж������� contours���ж���Ԫ��
	  3 hierarchy: ����Ϊ"vector<Vec4i> hierarchy" ,hierarchy��һ������,������ÿ��Ԫ��
	  ������һ��4��int�͵�����
	  4 mode :���������ļ���ģʽ
				CV_RETR_EXTERNAL: ֻ�������Χ����,��������Χ�������ڵ���������������
				CV_RETR_LIST������������������������̳�(����)��ϵ��
				CV_RETR_TREE������������������ҽ������еļ̳�(����)��ϵ��Ҳ����˵��CV_RETR_EXTERNAL��CV_RETR_LIST������ʱ��hierarchy���������û�õģ���Ϊǰ��û�а�����ϵ���ҵ��Ķ��������������߽������ҵ���Ӵ�������������Ѱ�����ϵ���֡���TREE���ּ�ⷽ����ʱ�����ǵ�hierarchy�����������������ġ���ʵ�ϣ�Ӧ��ǰ���ַ�����ʱ�����Ǿ���findContours��������ĵڶ��������ˡ�
				CV_RETR_CCOMP������������������ǽ����������������ϵ���������ŵ����㣬�����������ĵ�һ���������ŵ��ײ㣬����������������������ǾͰ���Щ�������ŵ�����ȥ��
	*  5 mothod �����������Ľ��Ʒ���
	*			  CV_CHAIN_APPROX_NONE�������������еĵ�洢��
				  CV_CHAIN_APPROX_SIMPLE��ֻ�洢ˮƽ����ֱ���Խ�ֱ�ߵ���ʼ�㡣��drawContours������˵�������ַ���û������
	   6 offset :Pointƫ���������е�������Ϣ�����ԭʼͼƬ��Ӧ��ƫ�������൱����ÿһ�����������ĵ��ϼ��ϸ�ƫ����������Ppint�������Ǹ�ֵ

	*
	*/
	//vector<vector<Point>>contours;//������������Ϣ
	//vector<Vec4i> hierarchy;
	//findContours(binary,contours,hierarchy,RETR_LIST,CHAIN_APPROX_NONE);
	//imshow("����", src);
	//imshow("ԭͼ", src);
	//waitKey(0);
}