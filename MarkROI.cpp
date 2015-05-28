#include "traffic.h"
//���е����·�������ĺ������� cv �����ռ���
//���ϣ����Ҫÿ�ζ����� cv:: �����ʹ���������
using namespace cv;
using namespace std;

bool MarkROI(IplImage *src,double Thresh)
{
	int iwidth=src->width; 
	int iheight=src->height;
	int iwidthStep=src->widthStep;//ÿһ��������ռ���ֽ���
	int count=0;//ͳ�ƾ��������ڴ�����ֵ��������Ŀ
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