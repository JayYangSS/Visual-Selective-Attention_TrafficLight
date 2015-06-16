/***********************************************************************
 * ��������ͼ���Ӿ�ѡ��ע��ģ��
 ***********************************************************************/
#include"traffic.h"
using namespace cv;
using namespace std;




int VSA(IplImage *frame,vector<Rect> &found_filtered)
{
    
	IplImage *src_lab,*src_a,*gray;
    IplImage *Saliency,*Saliency_gray,*Saliency_a;
	IplImage *resize_frame;
	IplImage *src_HSV,*src_S;
	CvSize size;
    double minNum = 0, maxNum = 0, scale, shift;
    int nRow, nCol;
	
 
	int blockW=5; //��saliencyMap�ϵļ�ⴰ��С
	int blockH=5;
	int DetcW=15;
	int DetcH=30;//��ⴰ�ڵĳߴ�



	//��ԭͼ������Ϊԭ�����֮һ
	size.height=frame->height*0.2;
	size.width=frame->width*0.2;
	resize_frame=cvCreateImage(size,frame->depth,frame->nChannels);
	cvResize(frame,resize_frame);
	
   //�õ�ͼ��Aͨ��
	src_lab = cvCreateImage(cvGetSize(resize_frame),resize_frame->depth,resize_frame->nChannels);
	src_a=cvCreateImage(cvGetSize(src_lab),src_lab->depth,1);
	cvCvtColor(resize_frame,src_lab,CV_BGR2Lab);
	cvSplit(src_lab,0,src_a,0,0);


	//�õ�ͼ��Sͨ��
	/*src_HSV = cvCreateImage(cvGetSize(resize_frame),resize_frame->depth,resize_frame->nChannels);
	src_S=cvCreateImage(cvGetSize(src_HSV),src_lab->depth,1);
	cvCvtColor(resize_frame,src_HSV,CV_BGR2HSV);
	cvSplit(src_HSV,0,src_S,0,0);*/


	//�õ�ͼ��Ҷ�ͨ��
	gray=cvCreateImage(cvGetSize(resize_frame),resize_frame->depth,1);
	cvCvtColor(resize_frame,gray,CV_BGR2GRAY);
	
	
	//Saliency_S=cvCreateImage(cvGetSize(src_a),src_a->depth,src_a->nChannels);
	//Saliency_S=GetSaliencyMap(src_a);
	Saliency_a=cvCreateImage(cvGetSize(src_a),src_a->depth,src_a->nChannels);
	Saliency_a=GetSaliencyMap(src_a);
	Saliency_gray=cvCreateImage(cvGetSize(gray),gray->depth,gray->nChannels);
	Saliency_gray=GetSaliencyMap(gray);

	cvShowImage("A_Saliency",Saliency_a);
	cvWaitKey(1);

	Saliency=cvCreateImage(cvGetSize(gray),src_a->depth,src_a->nChannels);
	cvAdd(Saliency_a,Saliency_gray,Saliency);


	//cvErode(Saliency_gray,Saliency_gray,NULL,1);
	//cvDilate(Saliency_gray,Saliency_gray,NULL,1);
	cvNamedWindow("Saliency", 1);
	cvShowImage("Saliency",Saliency);
	cvWaitKey(1);
	//��ʾAͨ����Ҷ�ͨ������ͼ����Ч��

	
	/*************************************************************************
	*           �����ڲ����м��
	**************************************************************************/
	nRow=Saliency->height;
	nCol=Saliency->width;

	IplImage *copySaliency;
	int result=0;
	copySaliency=cvCreateImage(Size(blockW,blockH),Saliency->depth,Saliency->nChannels);
	 for (int i=0;i<nCol-3*blockW;i=i+blockW)
	 {
		 for (int j=0;j<nRow-3*blockH;j=j+blockH)
		 {
			 CvRect rect=cvRect(i,j,blockW,blockH);
			 cvSetImageROI(Saliency,rect);
			 cvCopy(Saliency,copySaliency);
			 if (MarkROI(copySaliency,90))
			 {
				 CvRect DetcRect=cvRect(i*5,j*5,DetcW*3,DetcH*3);
				 cvSetImageROI(frame,DetcRect);

				 //��ROI�����ڽ���HOG+SVM���
				 Mat Saliency_ROI(frame);
				 Saliency_HOG(Saliency_ROI,found_filtered);//�ҳ����������ڵĽ�ͨ��λ�ã�Rect��ʽ���أ�

				 cvResetImageROI(frame);


				 //�˴���Ҫ��������任��������
				 for (int m=0;m<found_filtered.size();m++)
				 {
					 found_filtered[m].x=found_filtered[m].x+5*i;
					 found_filtered[m].y=found_filtered[m].y+5*j;
				 }
				 result=detect_result(frame,found_filtered);
				  
			 }

			 cvResetImageROI(Saliency);//clear ROI
		 }
	 }

	cvReleaseImage(&copySaliency);
	cvReleaseImage(&src_a);
	cvReleaseImage(&src_lab);
	cvReleaseImage(&gray);
	cvReleaseImage(&resize_frame);
	cvReleaseImage(&Saliency);
	cvReleaseImage(&Saliency_a);
	cvReleaseImage(&Saliency_gray);
    return 0;
}


int main()
{
	
	#ifdef VIDEO
		CvCapture* g_capture=NULL;
		IplImage* frame=NULL;
		vector<Rect> found_filtered;

		g_capture=cvCreateFileCapture("D:\\JY\\JY_TrainingSamples\\TrafficSignVideo\\trafficSign6.avi");
		while (1)
		{
		
				clock_t start=clock();
				frame=cvQueryFrame(g_capture);
				if(!frame)
			{
				cout<<"No file"<<endl;
				break;
			}
			VSA(frame,found_filtered);
			//cvReleaseImage(&res);
			clock_t end=clock();
			cout<<"Run time:"<<static_cast<double>(end-start)/CLOCKS_PER_SEC*1000<<"ms"<<endl;
		
		}

	#else
	
		char buff[100];
		vector<Rect> found_filtered;
		IplImage *frame;
		IplImage *resize_tmp=cvCreateImage(Size(640,480),8,3);

		for(int i=5200;i<5500;i++)
		{
			int Start=cvGetTickCount();
			sprintf(buff,"D:\\JY\\JY_TrainingSamples\\Traffic Light Benchmark\\Lara_UrbanSeq1_JPG\\Lara3D_UrbanSeq1_JPG\\%d.jpg",i);
			frame=cvLoadImage(buff);
			cvResize(frame,resize_tmp);
			VSA(resize_tmp,found_filtered);
	
		
			int End=cvGetTickCount();
			float time=(float)(End-Start)/(cvGetTickFrequency()*1000000);
			cout<<"ʱ�䣺"<<time<<endl;
			//frame = cvQueryFrame(capture);
		}

	#endif

	return 0;
}