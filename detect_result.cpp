#include"traffic.h"

int detect_result(Mat src_test,vector<Rect> &found_filtered)
{
	int result=10;//��¼���������������λ������
	int flag=10;//��ɫ�������־
	if(found_filtered.size()==0)
	{
		printf("�޽�ͨ��\n");
		result=0;
	}
	
	else
	{
		  for(int i=0; i<found_filtered.size(); i++)
		{
			  IplImage *src_Img=(&(IplImage)src_test);
			  Rect r = found_filtered[i];
	  
			  cvSetImageROI(src_Img,found_filtered[i]);
			  IplImage *dst=cvCreateImage(found_filtered[i].size(),8,3);//////////////////////�˴�û��Ҫ����Mat��IplImageת�������������Ż�һ��
			  cvCopy(src_Img,dst);
			  cvResetImageROI(src_Img);
			  flag=colorSegmentation(dst,r.area());

			  src_Img=NULL;
			//  dst=NULL;
			  cvReleaseImage(&dst);
			  if(flag==RESULT_G)
			  {
				  printf("��ɫ��ͨ��\n");

				  Rect r = found_filtered[i];
				  r.x += cvRound(r.width*0.1);
				  r.width = cvRound(r.width*0.8);
				  r.y += cvRound(r.height*0.07);
				  r.height = cvRound(r.height*0.8);
				  rectangle(src_test, r.tl(), r.br(), Scalar(0,255,0), 2);//tl()�������Ͻǵ�����,br()�������½ǵ�����
				  result=1;
				  break;
			  }
			   if(flag==RESULT_R)
			  {
				  printf("��ɫ��ͨ��\n");
				  Rect r = found_filtered[i];
				  r.x += cvRound(r.width*0.1);
				  r.width = cvRound(r.width*0.8);
				  r.y += cvRound(r.height*0.07);
				  r.height = cvRound(r.height*0.8);
				  rectangle(src_test, r.tl(), r.br(), Scalar(0,0,255), 2);//tl()�������Ͻǵ�����,br()�������½ǵ�����
				  result=2;
				  break;
			  }
			  //Seg=NULL;

			if(flag==RESULT_NON)
			{
				printf("�޽�ͨ��\n");
				result=0;
			}
			
		}
	  
	}
	imshow("src_Result",src_test);
	waitKey(1);//ע�⣺imshow֮������waitKey�������޷���ʾͼ��
	return result;
}



