#include "material.h"
/*
* ����ǻ�ȡ��Ƶ���ݶ�Ŀ��ԴΪ����web��ƵԴ�������زĵĽ�������
* ��ȡ�󰴿ո��������ʶ���ɸѡ����N����һ֡ ��ESC�˳�
* 
*/
int GetMat(char * src) {
	
	int count = 0;
	VideoCapture cap(src);
	if (!cap.isOpened()) {
		printf("�޷����ļ�\n");
		return -1;

	}
	//������⼶��������
	CascadeClassifier faceDetector;
	//��ʼ��������ȫ�ֱ���������������·��
	faceDetector.load(FaceCasSrc);
	Mat frame, img,dst;
	vector<Rect> faces;
	//Ϊ�����䶶������Ƶ�ķֱ���
	cap.set(CAP_PROP_FRAME_WIDTH, 1920);
	cap.set(CAP_PROP_FRAME_HEIGHT, 1080);
	//��ȡ����
	while (cap.read(frame))
	{
		//���λ��
		
		faceDetector.detectMultiScale(frame,faces,1.1,1,0,Size(100,100),Size(1080,1920));

		for (size_t i = 0; i < faces.size(); i++)
		{
			img = frame;
			rectangle(frame, faces[i], Scalar(0, 0, 255), 2, 8, 0);
			imshow("Material", frame);

			//���ո񱣴�,��N��һ��
			char c = waitKey(0);
			//�ո񱣴�
			if (c == 32) {
				resize(img(faces[i]), dst, Size(100, 100));
				imwrite(format("images/face_%d.jpg", ++count), dst);
				cout << "save_:face_" << count << endl;
			}
			else if (c == 27) {//ESC�˳�
				
				cap.release();
				return 0;
			}
			else if (c == 'n') {//��һ��
				break;
			
			}

		}

	}

}