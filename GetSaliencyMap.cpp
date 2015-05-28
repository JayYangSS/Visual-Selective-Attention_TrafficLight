#include "traffic.h"
void fft2(IplImage *src, IplImage *dst);

IplImage* GetSaliencyMap(IplImage *src)//src_inputΪ��ͨ��ͼ��
{
	IplImage *Fourier,*Inverse,*ImageRe,*ImageIm,*LogAmplitude,*Sine,*Cosine,*Residual,*Saliency,
			 *tmp1,*tmp2,*tmp3;

	int nRow,nCol;
	double minNum = 0, maxNum = 0, scale, shift;

	//cvSmooth(src,src,CV_GAUSSIAN,1,1);
    //ע��Fourier��һ����ͨ����ͼ��һ��ͨ��Ϊʵ����һ��Ϊ�鲿
    Fourier = cvCreateImage(cvGetSize(src), IPL_DEPTH_64F, 2);
    Inverse = cvCreateImage(cvGetSize(src), IPL_DEPTH_64F, 2);
    //Ƶ�׵�ʵ��
    ImageRe = cvCreateImage(cvGetSize(src), IPL_DEPTH_64F, 1);
    //Ƶ�׵��鲿
    ImageIm = cvCreateImage(cvGetSize(src), IPL_DEPTH_64F, 1);
    //log�����
    LogAmplitude = cvCreateImage(cvGetSize(src), IPL_DEPTH_64F, 1);
    //������
    Sine = cvCreateImage(cvGetSize(src), IPL_DEPTH_64F, 1);
    //������
    Cosine = cvCreateImage(cvGetSize(src), IPL_DEPTH_64F, 1);

    //Ƶ�����ࣨspectral residual��
    Residual = cvCreateImage(cvGetSize(src), IPL_DEPTH_64F, 1);
    //����map(Saliency map)
    Saliency = cvCreateImage(cvGetSize(src), src->depth, src->nChannels);

    //��ʱ�Ŀռ�
    tmp1 = cvCreateImage(cvGetSize(src), IPL_DEPTH_64F, 1);
    tmp2 = cvCreateImage(cvGetSize(src), IPL_DEPTH_64F, 1);
    tmp3 = cvCreateImage(cvGetSize(src), IPL_DEPTH_64F, 1);

    nRow = src->height;
    nCol = src->width;

    //��һ��һ��
    scale = 1.0/255.0;
    cvConvertScale(src, tmp1, scale, 0);

    //����Ҷ�任���õ���Fourier������ͨ����һ����ʵ����һ�����鲿
    fft2(tmp1, Fourier);

    //������Ҷ�׵�ʵ�����鲿��ŵ���Ӧ��ͼ����ȥ��
    cvSplit(Fourier, ImageRe, ImageIm, 0, 0);

    //���㸵��Ҷ����ף�ʵ�����鲿ƽ�����ٿ������õ�����״浽tmp3��
    cvPow( ImageRe, tmp1, 2.0);
    cvPow( ImageIm, tmp2, 2.0);
    cvAdd( tmp1, tmp2, tmp3);
    cvPow( tmp3, tmp3, 0.5 );


    //���������׺������׺���Ȼ������
    cvLog( tmp3, LogAmplitude );
    cvDiv(ImageIm, tmp3, Sine);
    cvDiv(ImageRe, tmp3, Cosine);

    //��LogAmplitude��3*3��ֵ�˲�
    cvSmooth(LogAmplitude, tmp3, CV_BLUR, 3, 3);

    //�����ʣ����
    cvSub(LogAmplitude, tmp3, Residual);

    /************************************************************************/
    /*
    ��exp(Residual+i*Phase)������Ҷ���任
    ��ŷ����ʽ��
    exp(r+i*��) = exp(r)*(cos(��) + i*sin(��)) = exp(r)*cos(��) + i*exp(r)*sin(��)
    ���Ц�����λ��

    ��sin(��) = ImageIm/Amplitude; cos(��) = ImageRe/Amplitude;
    */
    /************************************************************************/
    cvExp(Residual, Residual);
    cvMul(Residual, Cosine, tmp1);
    cvMul(Residual, Sine, tmp2);


    //��ʣ����Residual��Ϊʵ������λ��Phase��Ϊ�鲿
    cvMerge(tmp1, tmp2, 0, 0, Fourier);
    
    //ʵ�ָ���Ҷ��任
    cvDFT(Fourier, Inverse, CV_DXT_INV_SCALE);
    cvSplit(Inverse ,tmp1, tmp2,0,0);


    //�����Ӧ��ʵ���鲿ƽ����
    cvPow(tmp1, tmp1, 2);
    cvPow(tmp2, tmp2, 2);
    cvAdd(tmp1, tmp2, tmp3, NULL);

    //��˹�˲�
    cvSmooth(tmp3, tmp3, CV_GAUSSIAN, 3, 3);

    cvMinMaxLoc(tmp3, &minNum, &maxNum, NULL, NULL);
    scale = 255/(maxNum - minNum);
    shift = -minNum * scale;

    //��shift����ImageRe��Ԫ�ذ��������ŵĽ���ϣ��洢ΪImageDst
    cvConvertScale(tmp3, Saliency, scale, shift);
	//cvReleaseImage(&src);
	cvReleaseImage(&ImageIm);
	cvReleaseImage(&ImageRe);
	cvReleaseImage(&Fourier);
	cvReleaseImage(&Inverse);
	cvReleaseImage(&LogAmplitude);
	cvReleaseImage(&Sine);
	cvReleaseImage(&Cosine);
	//cvReleaseImage(&Saliency);
	cvReleaseImage(&Residual);
	cvReleaseImage(&tmp1);
	cvReleaseImage(&tmp2);
	cvReleaseImage(&tmp3);
	//cvReleaseImage(&copySaliency);
	//cvReleaseImage(&resize_frame);

	return Saliency;
}

/**************************************************************************
//src IPL_DEPTH_8U
//dst IPL_DEPTH_64F
**************************************************************************/
//����Ҷ���任
void fft2(IplImage *src, IplImage *dst)
{   //ʵ�����鲿
    IplImage *image_Re = 0, *image_Im = 0, *Fourier = 0;
    //   int i, j;
    image_Re = cvCreateImage(cvGetSize(src), IPL_DEPTH_64F, 1);  //ʵ��
    //Imaginary part
    image_Im = cvCreateImage(cvGetSize(src), IPL_DEPTH_64F, 1);  //�鲿
    //2 channels (image_Re, image_Im)
    Fourier = cvCreateImage(cvGetSize(src), IPL_DEPTH_64F, 2);

    //ʵ����ֵ��ʼ��ΪԴͼ���鲿��ֵ��ʼ��Ϊ0
    // Real part conversion from u8 to 64f (double)
    cvConvertScale(src, image_Re, 1, 0);
    // Imaginary part (zeros)
    cvZero(image_Im);
    // Join real and imaginary parts and stock them in Fourier image
    cvMerge(image_Re, image_Im, 0, 0, Fourier);
    // Application of the forward Fourier transform
   // cvDFT(Fourier, dst, CV_DXT_FORWARD);
	cvDFT(Fourier, dst,DFT_COMPLEX_OUTPUT+DFT_SCALE);
    cvReleaseImage(&image_Re);
    cvReleaseImage(&image_Im);
    cvReleaseImage(&Fourier);
}