#include "traffic.h"
void fft2(IplImage *src, IplImage *dst);

IplImage* GetSaliencyMap(IplImage *src)//src_input为单通道图像
{
	IplImage *Fourier,*Inverse,*ImageRe,*ImageIm,*LogAmplitude,*Sine,*Cosine,*Residual,*Saliency,
			 *tmp1,*tmp2,*tmp3;

	int nRow,nCol;
	double minNum = 0, maxNum = 0, scale, shift;

	//cvSmooth(src,src,CV_GAUSSIAN,1,1);
    //注意Fourier是一个两通道的图像，一个通道为实部，一个为虚部
    Fourier = cvCreateImage(cvGetSize(src), IPL_DEPTH_64F, 2);
    Inverse = cvCreateImage(cvGetSize(src), IPL_DEPTH_64F, 2);
    //频谱的实部
    ImageRe = cvCreateImage(cvGetSize(src), IPL_DEPTH_64F, 1);
    //频谱的虚部
    ImageIm = cvCreateImage(cvGetSize(src), IPL_DEPTH_64F, 1);
    //log振幅谱
    LogAmplitude = cvCreateImage(cvGetSize(src), IPL_DEPTH_64F, 1);
    //正弦谱
    Sine = cvCreateImage(cvGetSize(src), IPL_DEPTH_64F, 1);
    //余弦谱
    Cosine = cvCreateImage(cvGetSize(src), IPL_DEPTH_64F, 1);

    //频谱冗余（spectral residual）
    Residual = cvCreateImage(cvGetSize(src), IPL_DEPTH_64F, 1);
    //特征map(Saliency map)
    Saliency = cvCreateImage(cvGetSize(src), src->depth, src->nChannels);

    //临时的空间
    tmp1 = cvCreateImage(cvGetSize(src), IPL_DEPTH_64F, 1);
    tmp2 = cvCreateImage(cvGetSize(src), IPL_DEPTH_64F, 1);
    tmp3 = cvCreateImage(cvGetSize(src), IPL_DEPTH_64F, 1);

    nRow = src->height;
    nCol = src->width;

    //归一化一下
    scale = 1.0/255.0;
    cvConvertScale(src, tmp1, scale, 0);

    //傅里叶变换，得到的Fourier有两个通道，一个是实部，一个是虚部
    fft2(tmp1, Fourier);

    //将傅里叶谱的实部和虚部存放到对应的图像中去。
    cvSplit(Fourier, ImageRe, ImageIm, 0, 0);

    //计算傅里叶振幅谱，实部和虚部平方和再开方，得到振幅谱存到tmp3中
    cvPow( ImageRe, tmp1, 2.0);
    cvPow( ImageIm, tmp2, 2.0);
    cvAdd( tmp1, tmp2, tmp3);
    cvPow( tmp3, tmp3, 0.5 );


    //计算正弦谱和余弦谱和自然对数谱
    cvLog( tmp3, LogAmplitude );
    cvDiv(ImageIm, tmp3, Sine);
    cvDiv(ImageRe, tmp3, Cosine);

    //对LogAmplitude做3*3均值滤波
    cvSmooth(LogAmplitude, tmp3, CV_BLUR, 3, 3);

    //计算出剩余普
    cvSub(LogAmplitude, tmp3, Residual);

    /************************************************************************/
    /*
    对exp(Residual+i*Phase)作傅里叶反变换
    由欧拉公式：
    exp(r+i*Θ) = exp(r)*(cos(Θ) + i*sin(Θ)) = exp(r)*cos(Θ) + i*exp(r)*sin(Θ)
    其中Θ是相位。

    而sin(Θ) = ImageIm/Amplitude; cos(Θ) = ImageRe/Amplitude;
    */
    /************************************************************************/
    cvExp(Residual, Residual);
    cvMul(Residual, Cosine, tmp1);
    cvMul(Residual, Sine, tmp2);


    //将剩余普Residual作为实部，相位谱Phase作为虚部
    cvMerge(tmp1, tmp2, 0, 0, Fourier);
    
    //实现傅里叶逆变换
    cvDFT(Fourier, Inverse, CV_DXT_INV_SCALE);
    cvSplit(Inverse ,tmp1, tmp2,0,0);


    //求出对应的实部虚部平方和
    cvPow(tmp1, tmp1, 2);
    cvPow(tmp2, tmp2, 2);
    cvAdd(tmp1, tmp2, tmp3, NULL);

    //高斯滤波
    cvSmooth(tmp3, tmp3, CV_GAUSSIAN, 3, 3);

    cvMinMaxLoc(tmp3, &minNum, &maxNum, NULL, NULL);
    scale = 255/(maxNum - minNum);
    shift = -minNum * scale;

    //将shift加在ImageRe各元素按比例缩放的结果上，存储为ImageDst
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
//傅里叶正变换
void fft2(IplImage *src, IplImage *dst)
{   //实部、虚部
    IplImage *image_Re = 0, *image_Im = 0, *Fourier = 0;
    //   int i, j;
    image_Re = cvCreateImage(cvGetSize(src), IPL_DEPTH_64F, 1);  //实部
    //Imaginary part
    image_Im = cvCreateImage(cvGetSize(src), IPL_DEPTH_64F, 1);  //虚部
    //2 channels (image_Re, image_Im)
    Fourier = cvCreateImage(cvGetSize(src), IPL_DEPTH_64F, 2);

    //实部的值初始设为源图像，虚部的值初始设为0
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