#include "Train.h"

/*
* 本文件为素材处理以及样本训练
* 配置文件格式: 图片路径;样本标签;样本命名
* 文件为txt,ascll编码
*/
int StartAnalysis(string  ConfigFilePath, string Source) {//配置文件路径,分析源
	vector<Mat> images;//图片
	vector<int> labels;//模型标记
	vector<string> names;//名字

	string filename = ConfigFilePath;

	ifstream file(filename.c_str(), ifstream::in);//读入样本文件

	if (!file) {
		cout << "没有找到路径样本文件" << endl;
		return -1;
	}

	//分析每一行样本数据
	string line, path, classlabel, name;
	char spilt = ';';//作为分割符
	while (getline(file, line)) {
		stringstream linesSub(line);
		getline(linesSub, path, spilt);//读出一行,默认结束符定义为';'
		getline(linesSub, classlabel, spilt);//读出标记
		getline(linesSub, name);//名字

		//判断是否存在数据
		if (!path.empty() && !classlabel.empty()) {
			images.push_back(imread(path));
			labels.push_back(atoi(classlabel.c_str()));//atoi:string转Int
		}
		//为空就赋值为unknown
		if (name.empty()) {
			name = "unknown";
		}
		names.push_back(name);
	}

	if (images.size()<1 || labels.size() < 1 ) {
		cout << "初始化错误,检查文件是否有误" << endl;
		return -1;
	}
	//读出测试用的数据
	cout << "配置文件加载完成" << endl;
	Mat testImg = images[images.size() - 1];
	int testLabel = labels[labels.size() - 1];
	string testName = names[names.size() - 1];

	StartTrain(Source, images, labels, names, testImg);

	return 0;
}

void StartTrain(string Src, vector<Mat>images, vector<int>labels, vector<string>name, Mat testImg) {//分析源路径,
	//加载字体 freetype环境有问题 暂时先不用
	/*Ptr<freetype::FreeType2> ft2;
	ft2 = freetype::createFreeType2();*/
	//ft2->loadFontData("c:/windows/fonts/msyh.ttf", 0);//微软雅黑

	//3.0写法变了CreateEigenFaceRecognizer() = = 》 EigenFaceRecognizer::create()
	Ptr<BasicFaceRecognizer> model = EigenFaceRecognizer::create();
	model->train(images, labels);//训练后打上标签

	CascadeClassifier faceDetector;
	faceDetector.load(FaceCasSrc);//加载分类器

	VideoCapture cap(Src);

	if (!cap.isOpened()) {
		cout << "打开视频文件失败" << endl;
		return;
	}
	vector<Rect> faces;
	Mat dst, frame;

	while (cap.read(frame)) {
		//分析人脸,这里数据要跟material.cpp下的参数一样
		faceDetector.detectMultiScale(frame, faces, 1.1, 1, 0, Size(100, 100), Size(1080, 1920));
		for (size_t i = 0; i < faces.size(); i++)
		{
			imshow("Train-window", frame);
			
			Mat tmp = frame(faces[i]);
			//灰度
			cvtColor(tmp, dst, COLOR_BGR2GRAY);
			//设定大小保持统一
			resize(dst, testImg, testImg.size());
			//测试
			int lab = model->predict(testImg);
			//画矩形标记人脸
			rectangle(frame, faces[i], Scalar(255, 0, 0), 2, 8, 0);
			string text="unknow";
			//遍历找标记，把标记和name联系起来
			/*for (size_t i = 0; i < labels.size(); i++)
			{
				if (lab == labels[i]) {
					text = name[i].c_str();
					break;
				}

			}*/
			//ft2->putText(frame, text, faces[i].tl(), 40, Scalar(0, 255, 0), -1, 8, true);
			
			putText(frame, "BeiShiGuNiang", Point(faces[i].x + (faces[i].width / 2), faces[i].y - 10), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 255), 1, 8);
			if (waitKey(100) == 27) {
				break;
			}
			
			
		}
		
	}


}


