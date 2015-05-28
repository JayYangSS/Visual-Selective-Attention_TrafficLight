#include"traffic.h"


static int high = 155;//红色
static int low = 113;//绿色


int colorSegmentation(IplImage* inputImage,int RectArea)//其实该回调函数根本没有使用lown，只是为了满足cvCreateTrackbar中的回调函数声明类型才设置的
{
	IplImage *IMG_LAB, *IMG_A,*IMG_OUT;
	IMG_LAB = cvCreateImage(cvGetSize(inputImage), inputImage->depth, 3);
	IMG_A = cvCreateImage(cvGetSize(inputImage), inputImage->depth, 1);
	IMG_OUT = cvCreateImage(cvGetSize(inputImage), inputImage->depth, 1);
	

	cvCvtColor(inputImage, IMG_LAB, CV_BGR2Lab);
	cvSplit(IMG_LAB, 0, IMG_A, 0, 0);

	cvShowImage("A",IMG_A);
	cvWaitKey(20);

	if (!IMG_A)
		exit(EXIT_FAILURE);

	int iHeight = IMG_A->height;
	int iWidth = IMG_A->width;
	int iOutWidthStep = IMG_A->widthStep;//widthStep 表示存储一行像素所需的字节数
	int Green_num = 0, Red_num = 0;
	int value;//存储单通道图像中的像素值



	unsigned char* in = (unsigned char*)IMG_A->imageData;
	unsigned char* out = (unsigned char*)IMG_OUT->imageData;

	for (int j = 0; j<iHeight; j++)
	{
		in = (unsigned char*)IMG_A->imageData + j*iOutWidthStep;
		out = (unsigned char*)IMG_OUT->imageData + j*iOutWidthStep;
		for (int i = 0; i<iWidth; i++){
			value = in[i];//存储的是A通道的值


			if (value<low)
			{
				out[i] = GREEN_PIXEL_LABEL;
				Green_num++;
			}

			else if (value>high)//原来程序的阈值
			{
				out[i] = RED_PIXEL_LABEL;
				Red_num++;
			}

			else
				out[i] = NON_BLOB_PIXEL_LABEL;
		}
	}

	cvShowImage("MARK",IMG_OUT);
	cvWaitKey(20);
	cvReleaseImage(&IMG_A);
	cvReleaseImage(&IMG_LAB);
	cvReleaseImage(&IMG_OUT);

	if((Green_num>Red_num)/*&&(Green_num>0.1*RectArea)*/)
		return RESULT_G;
	else if((Green_num<Red_num)/*&&(Red_num>0.1*RectArea)*/)
		return RESULT_R;
	else
		return RESULT_NON;
	//return imageSeg;

}

/*int main()
{
	CvCapture* g_capture=NULL;
	IplImage* frame=NULL;

	//g_capture=cvCreateFileCapture("huanhu_clip2.avi");
	g_capture=cvCreateFileCapture("pt.avi");

	while (1)
	{
		clock_t start=clock();
		frame=cvQueryFrame(g_capture);

		if(!frame)
		{
			cout<<"No file"<<endl;
			break;
		}
		cvShowImage("src",frame);
		cvWaitKey(10);
		colorSegmentation(frame,1);
		clock_t end=clock();
		cout<<"Run time:"<<static_cast<double>(end-start)/CLOCKS_PER_SEC*1000<<"ms"<<endl;
	}
	return 0;
}*/