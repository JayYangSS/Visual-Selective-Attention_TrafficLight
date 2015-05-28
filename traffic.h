#ifndef TRAFFIC_H
#define TRAFFIC_H

#include <iostream>
#include <fstream>
/*#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/ml/ml.hpp>*/

#include "opencv2/opencv.hpp"
#include <string>
#include <time.h>
//#include <socket_manager.h>

#define GREEN_PIXEL_LABEL 255
#define RED_PIXEL_LABEL 128
#define NON_BLOB_PIXEL_LABEL 0
//#define ROIHeight 300
#define ROIHeight 300
#define	ROIWidth 600
#define PI 3.1415
#define RESULT_G 0
#define RESULT_R 1
#define RESULT_NON 2
#define BIASX 50  //���ο�ƫ��


#define PosSamNO    523 //����������
//#define PosSamNO    28 //����������
#define HORZ_PosSamNO    36 //����������
//#define PosSamNO 10    //����������
//#define NegSamNO 2  //����������
#define NegSamNO 2574   //����������
#define HORZ_NegSamNO 2416
#define HardExampleNO 334
#define HORZ_HardExampleNO 23

//HardExample�����������������HardExampleNO����0����ʾ�������ʼ���������󣬼�������HardExample����������
//��ʹ��HardExampleʱ��������Ϊ0����Ϊ������������������������ά����ʼ��ʱ�õ����ֵ


using namespace std;
using namespace cv;

struct DetecResult{
	int LightResult[8];
	//int LightPos[8];
	Rect LightPos[8];
};


int colorSegmentation(IplImage* inputImage,int RectArea);
int detect_result(Mat src_test,vector<Rect> &found_filtered);
int SortRect(Mat src_test,int num,DetecResult *Rst,char Direct);

void rgb2hsi(int red, int green, int blue, int& hue, int& saturation, int& intensity );
void hogSVMTrain( HOGDescriptor &myHOG,bool TRAIN,bool HORZ);
void BoxDetect(Mat src_test,HOGDescriptor &myHOG,vector<Rect> &found_filtered,bool HORZ);
void Saliency_HOG(Mat src,vector<Rect> &found_filtered);
bool MarkROI(IplImage *src,double Thresh);

IplImage* GetSaliencyMap(IplImage *src_input);

//�̳���CvSVM���࣬��Ϊ����setSVMDetector()���õ��ļ���Ӳ���ʱ����Ҫ�õ�ѵ���õ�SVM��decision_func������
//��ͨ���鿴CvSVMԴ���֪decision_func������protected���ͱ������޷�ֱ�ӷ��ʵ���ֻ�ܼ̳�֮��ͨ����������

class MySVM : public CvSVM
{
public:
  //���SVM�ľ��ߺ����е�alpha����
  double * get_alpha_vector()
  {
    return this->decision_func->alpha;
  }

  //���SVM�ľ��ߺ����е�rho����,��ƫ����
  float get_rho()
  {
    return this->decision_func->rho;
  }
};


#endif