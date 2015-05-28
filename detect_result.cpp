#include"traffic.h"

int detect_result(Mat src_test,vector<Rect> &found_filtered)
{
	int result=10;//记录检测结果，用于向上位机传输
	int flag=10;//颜色检测结果标志
	if(found_filtered.size()==0)
	{
		printf("无交通灯\n");
		result=0;
	}
	
	else
	{
		  for(int i=0; i<found_filtered.size(); i++)
		{
			  IplImage *src_Img=(&(IplImage)src_test);
			  Rect r = found_filtered[i];
	  
			  cvSetImageROI(src_Img,found_filtered[i]);
			  IplImage *dst=cvCreateImage(found_filtered[i].size(),8,3);//////////////////////此处没必要进行Mat和IplImage转换。。。。。优化一下
			  cvCopy(src_Img,dst);
			  cvResetImageROI(src_Img);
			  flag=colorSegmentation(dst,r.area());

			  src_Img=NULL;
			//  dst=NULL;
			  cvReleaseImage(&dst);
			  if(flag==RESULT_G)
			  {
				  printf("绿色交通灯\n");

				  Rect r = found_filtered[i];
				  r.x += cvRound(r.width*0.1);
				  r.width = cvRound(r.width*0.8);
				  r.y += cvRound(r.height*0.07);
				  r.height = cvRound(r.height*0.8);
				  rectangle(src_test, r.tl(), r.br(), Scalar(0,255,0), 2);//tl()返回左上角点坐标,br()返回右下角点坐标
				  result=1;
				  break;
			  }
			   if(flag==RESULT_R)
			  {
				  printf("红色交通灯\n");
				  Rect r = found_filtered[i];
				  r.x += cvRound(r.width*0.1);
				  r.width = cvRound(r.width*0.8);
				  r.y += cvRound(r.height*0.07);
				  r.height = cvRound(r.height*0.8);
				  rectangle(src_test, r.tl(), r.br(), Scalar(0,0,255), 2);//tl()返回左上角点坐标,br()返回右下角点坐标
				  result=2;
				  break;
			  }
			  //Seg=NULL;

			if(flag==RESULT_NON)
			{
				printf("无交通灯\n");
				result=0;
			}
			
		}
	  
	}
	imshow("src_Result",src_test);
	waitKey(1);//注意：imshow之后必须加waitKey，否则无法显示图像
	return result;
}



