#include "traffic.h"
//所有的以新风格命名的函数都在 cv 命名空间中
//如果希望不要每次都输入 cv:: ，则可使用下面语句
using namespace cv;
using namespace std;

bool MarkROI(IplImage *src,double Thresh)
{
	int iwidth=src->width; 
	int iheight=src->height;
	int iwidthStep=src->widthStep;//每一行像素所占的字节数
	int count=0;//统计举行区域内大于阈值的像素数目
	unsigned char* row=(unsigned char*)src->imageData;
	for(int j=0;j<iheight;j++)
	{
		row=(unsigned char*)src->imageData+j*iwidthStep;
		for (int i=0;i<iwidth;i++)
		{
			if(row[i]>Thresh)count++;
		}
	}

	if (count>10)
	{
		return true;
	} 
	else
	{
		return false;
	}
}