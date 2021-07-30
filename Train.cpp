#include "Train.h"

/*
* ���ļ�Ϊ�زĴ����Լ�����ѵ��
* �����ļ���ʽ: ͼƬ·��;������ǩ;��������
* �ļ�Ϊtxt,ascll����
*/
int StartAnalysis(string  ConfigFilePath, string Source) {//�����ļ�·��,����Դ
	vector<Mat> images;//ͼƬ
	vector<int> labels;//ģ�ͱ��
	vector<string> names;//����

	string filename = ConfigFilePath;

	ifstream file(filename.c_str(), ifstream::in);//���������ļ�

	if (!file) {
		cout << "û���ҵ�·�������ļ�" << endl;
		return -1;
	}

	//����ÿһ����������
	string line, path, classlabel, name;
	char spilt = ';';//��Ϊ�ָ��
	while (getline(file, line)) {
		stringstream linesSub(line);
		getline(linesSub, path, spilt);//����һ��,Ĭ�Ͻ���������Ϊ';'
		getline(linesSub, classlabel, spilt);//�������
		getline(linesSub, name);//����

		//�ж��Ƿ��������
		if (!path.empty() && !classlabel.empty()) {
			images.push_back(imread(path));
			labels.push_back(atoi(classlabel.c_str()));//atoi:stringתInt
		}
		//Ϊ�վ͸�ֵΪunknown
		if (name.empty()) {
			name = "unknown";
		}
		names.push_back(name);
	}

	if (images.size()<1 || labels.size() < 1 ) {
		cout << "��ʼ������,����ļ��Ƿ�����" << endl;
		return -1;
	}
	//���������õ�����
	cout << "�����ļ��������" << endl;
	Mat testImg = images[images.size() - 1];
	int testLabel = labels[labels.size() - 1];
	string testName = names[names.size() - 1];

	StartTrain(Source, images, labels, names, testImg);

	return 0;
}

void StartTrain(string Src, vector<Mat>images, vector<int>labels, vector<string>name, Mat testImg) {//����Դ·��,
	//�������� freetype���������� ��ʱ�Ȳ���
	/*Ptr<freetype::FreeType2> ft2;
	ft2 = freetype::createFreeType2();*/
	//ft2->loadFontData("c:/windows/fonts/msyh.ttf", 0);//΢���ź�

	//3.0д������CreateEigenFaceRecognizer() = = �� EigenFaceRecognizer::create()
	Ptr<BasicFaceRecognizer> model = EigenFaceRecognizer::create();
	model->train(images, labels);//ѵ������ϱ�ǩ

	CascadeClassifier faceDetector;
	faceDetector.load(FaceCasSrc);//���ط�����

	VideoCapture cap(Src);

	if (!cap.isOpened()) {
		cout << "����Ƶ�ļ�ʧ��" << endl;
		return;
	}
	vector<Rect> faces;
	Mat dst, frame;

	while (cap.read(frame)) {
		//��������,��������Ҫ��material.cpp�µĲ���һ��
		faceDetector.detectMultiScale(frame, faces, 1.1, 1, 0, Size(100, 100), Size(1080, 1920));
		for (size_t i = 0; i < faces.size(); i++)
		{
			imshow("Train-window", frame);
			
			Mat tmp = frame(faces[i]);
			//�Ҷ�
			cvtColor(tmp, dst, COLOR_BGR2GRAY);
			//�趨��С����ͳһ
			resize(dst, testImg, testImg.size());
			//����
			int lab = model->predict(testImg);
			//�����α������
			rectangle(frame, faces[i], Scalar(255, 0, 0), 2, 8, 0);
			string text="unknow";
			//�����ұ�ǣ��ѱ�Ǻ�name��ϵ����
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


