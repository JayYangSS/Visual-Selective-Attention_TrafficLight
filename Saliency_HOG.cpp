#include"traffic.h"

//该函数实现在显著图上的显著区域内进行HOG+SVM检测目标
void Saliency_HOG(Mat src,vector<Rect> &found_filtered)
{


	//vector<Rect> found_filtered;
	bool TRAIN=false;
	bool HORZ=false;

	HOGDescriptor myHOG_vertical(Size(15,30),Size(5,10),Size(5,5),Size(5,5),9,1,-1.0,0,0.2,true,10);
	HOGDescriptor myHOG_horz(Size(30,15),Size(10,5),Size(5,5),Size(5,5),9,1,-1.0,0,0.2,true,10);
	
	if(HORZ)
		hogSVMTrain(myHOG_horz,TRAIN,HORZ);
	else
		hogSVMTrain(myHOG_vertical,TRAIN,HORZ);
	
	    Mat temp;		
		found_filtered.clear();
		if(HORZ)
			BoxDetect(src,myHOG_horz,found_filtered,HORZ);
		BoxDetect(src,myHOG_vertical,found_filtered,HORZ);
		//int result=detect_result(src,found_filtered);

}