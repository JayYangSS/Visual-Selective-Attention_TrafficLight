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
#define BIASX 50  //矩形框偏移


#define PosSamNO    523 //正样本个数
//#define PosSamNO    28 //正样本个数
#define HORZ_PosSamNO    36 //正样本个数
//#define PosSamNO 10    //正样本个数
//#define NegSamNO 2  //负样本个数
#define NegSamNO 2574   //负样本个数
#define HORZ_NegSamNO 2416
#define HardExampleNO 334
#define HORZ_HardExampleNO 23

//HardExample：负样本个数。如果HardExampleNO大于0，表示处理完初始负样本集后，继续处理HardExample负样本集。
//不使用HardExample时必须设置为0，因为特征向量矩阵和特征类别矩阵的维数初始化时用到这个值


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

//继承自CvSVM的类，因为生成setSVMDetector()中用到的检测子参数时，需要用到训练好的SVM的decision_func参数，
//但通过查看CvSVM源码可知decision_func参数是protected类型变量，无法直接访问到，只能继承之后通过函数访问

class MySVM : public CvSVM
{
public:
  //获得SVM的决策函数中的alpha数组
  double * get_alpha_vector()
  {
    return this->decision_func->alpha;
  }

  //获得SVM的决策函数中的rho参数,即偏移量
  float get_rho()
  {
    return this->decision_func->rho;
  }
};


#endif