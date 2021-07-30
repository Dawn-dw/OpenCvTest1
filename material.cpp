#include "material.h"
/*
* 这个是获取视频（暂定目标源为抖音web视频源）人脸素材的解析函数
* 获取后按空格进行人脸识别的筛选，按N到下一帧 按ESC退出
* 
*/
int GetMat(char * src) {
	
	int count = 0;
	VideoCapture cap(src);
	if (!cap.isOpened()) {
		printf("无法打开文件\n");
		return -1;

	}
	//人脸检测级联分类器
	CascadeClassifier faceDetector;
	//初始化，导入全局变量人脸级联数据路径
	faceDetector.load(FaceCasSrc);
	Mat frame, img,dst;
	vector<Rect> faces;
	//为了适配抖音短视频的分辨率
	cap.set(CAP_PROP_FRAME_WIDTH, 1920);
	cap.set(CAP_PROP_FRAME_HEIGHT, 1080);
	//读取数据
	while (cap.read(frame))
	{
		//检测位置
		
		faceDetector.detectMultiScale(frame,faces,1.1,1,0,Size(100,100),Size(1080,1920));

		for (size_t i = 0; i < faces.size(); i++)
		{
			img = frame;
			rectangle(frame, faces[i], Scalar(0, 0, 255), 2, 8, 0);
			imshow("Material", frame);

			//按空格保存,按N下一个
			char c = waitKey(0);
			//空格保存
			if (c == 32) {
				resize(img(faces[i]), dst, Size(100, 100));
				imwrite(format("images/face_%d.jpg", ++count), dst);
				cout << "save_:face_" << count << endl;
			}
			else if (c == 27) {//ESC退出
				
				cap.release();
				return 0;
			}
			else if (c == 'n') {//下一个
				break;
			
			}

		}

	}

}