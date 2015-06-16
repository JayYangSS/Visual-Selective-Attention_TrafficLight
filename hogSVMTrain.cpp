#include"traffic.h"
void hogSVMTrain( HOGDescriptor &myHOG,bool TRAIN,bool HORZ)
{
	
	//��ⴰ��(64,128),��ߴ�(16,16),�鲽��(8,8),cell�ߴ�(8,8),ֱ��ͼbin����9
  HOGDescriptor hog_vertical(Size(15,30),Size(5,10),Size(5,5),Size(5,5),9,1,-1.0,0,0.2,true,32);//HOG���������������HOG�����ӵ�
  HOGDescriptor hog_horz(Size(30,15),Size(10,5),Size(5,5),Size(5,5),9,1,-1.0,0,0.2,true,32);
  int DescriptorDim;//HOG�����ӵ�ά������ͼƬ��С����ⴰ�ڴ�С�����С��ϸ����Ԫ��ֱ��ͼbin��������
  MySVM svm;//SVM������
  int WinStride;
  //��TRAINΪtrue������ѵ��������



  if(!HORZ)
  {
	  if(TRAIN)
  {
    string ImgName;//ͼƬ��(����·��)
   ifstream finPos("D:\\JY\\JY_TrainingSamples\\positive\\positivePath.txt");//������ͼƬ���ļ����б�
   //ifstream finPos("D:\\JY\\JY_TrainingSamples\\Traffic Light Benchmark\\Positive\\pos.txt");//������ͼƬ���ļ����б�
   //ifstream finNeg("D:\\Traffic Light Detection\\JY_TrainingSamples\\negetive1\\negetive1.txt");//������ͼƬ���ļ����б�
   ifstream finNeg("D:\\JY\\JY_TrainingSamples\\negetive1\\negetive1.txt");//������ͼƬ���ļ����б�
   

   Mat sampleFeatureMat;//����ѵ������������������ɵľ��������������������ĸ�������������HOG������ά��	
   Mat sampleLabelMat;//ѵ����������������������������������ĸ�������������1��1��ʾ���ˣ�-1��ʾ����
	


    //���ζ�ȡ������ͼƬ������HOG������

    for(int num=0; num<PosSamNO && getline(finPos,ImgName); num++)
    {
		cout<<"����"<<ImgName<<endl;
		Mat src = imread(ImgName);//��ȡͼƬ

 
		resize(src,src,Size(15,30));
		vector<float> descriptors;//HOG����������
		hog_vertical.compute(src,descriptors,Size(8,8));//����HOG�����ӣ���ⴰ���ƶ�����(8,8)
		cout<<"������ά����"<<descriptors.size()<<endl;

      //�����һ������ʱ��ʼ�����������������������Ϊֻ��֪��������������ά�����ܳ�ʼ��������������
      if( 0 == num )
      {
        DescriptorDim = descriptors.size();//HOG�����ӵ�ά��
        //��ʼ������ѵ������������������ɵľ��������������������ĸ�������������HOG������ά��sampleFeatureMat
		sampleFeatureMat = Mat::zeros(PosSamNO+NegSamNO+HardExampleNO, DescriptorDim, CV_32FC1);
        sampleLabelMat = Mat::zeros(PosSamNO+NegSamNO+HardExampleNO, 1, CV_32FC1);
		//sampleLabelMat = Mat::zeros(PosSamNO+5742*NegSamNO+HardExampleNO, 1, CV_32FC1);
      }

      //������õ�HOG�����Ӹ��Ƶ�������������sampleFeatureMat
      for(int i=0; i<DescriptorDim; i++)
		sampleFeatureMat.at<float>(num,i) = descriptors[i];//��num�����������������еĵ�i��Ԫ��
      sampleLabelMat.at<float>(num,0) = 1;//���������Ϊ1������
    }
	
    //���ζ�ȡ������ͼƬ������HOG������

    for(int num=0; num<NegSamNO && getline(finNeg,ImgName); num++)
    {
      cout<<"����"<<ImgName<<endl;
      Mat src = imread(ImgName);//��ȡͼƬ
	 
      //resize(src,src,Size(64,128));
	 resize(src,src,Size(15,30));
	//   resize(src,src,Size(10,20));
	  //cvtColor(src,gray,CV_RGB2GRAY);
      vector<float> descriptors;//HOG����������
	  hog_vertical.compute(src,descriptors,Size(8,8));//����HOG�����ӣ���ⴰ���ƶ�����(8,8)
      cout<<"������ά����"<<descriptors.size()<<endl;

      //������õ�HOG�����Ӹ��Ƶ�������������sampleFeatureMat
/*	 WinStride=descriptors.size()/DescriptorDim;
	  for(int j=0;j<WinStride;j++)
	  {
		  for(int i=0; i<DescriptorDim; i++)
				sampleFeatureMat.at<float>(j+num*WinStride+PosSamNO,i) = descriptors[i+DescriptorDim*j];//��PosSamNO+num�����������������еĵ�i��Ԫ��
		  sampleLabelMat.at<float>(j+num*WinStride+PosSamNO,0) = -1;//���������Ϊ-1������
	  }*/


    for(int i=0; i<DescriptorDim; i++)
        sampleFeatureMat.at<float>(num+PosSamNO,i) = descriptors[i];//��PosSamNO+num�����������������еĵ�i��Ԫ��
      sampleLabelMat.at<float>(num+PosSamNO,0) = -1;//���������Ϊ-1������
    }

    //����HardExample������
    if(HardExampleNO > 0)
    {
      ifstream finHardExample("D:\\JY\\JY_TrainingSamples\\hardexample\\hardexample.txt");//HardExample���������ļ����б�
      //���ζ�ȡHardExample������ͼƬ������HOG������
      for(int num=0; num<HardExampleNO && getline(finHardExample,ImgName); num++)
      {
        cout<<"����"<<ImgName<<endl;
       // ImgName = ImgName;//����HardExample��������·����
        Mat src = imread(ImgName);//��ȡͼƬ

		
		//resize(src,src,Size(64,128));
		resize(src,src,Size(15,30));
		//resize(src,src,Size(10,20));
		//cvtColor(src,gray,CV_RGB2GRAY);
        vector<float> descriptors;//HOG����������
       hog_vertical.compute(src,descriptors,Size(8,8));//����HOG�����ӣ���ⴰ���ƶ�����(8,8)
		//hog_vertical.compute(src,descriptors,Size(5,5));//����HOG�����ӣ���ⴰ���ƶ�����(8,8)
        //cout<<"������ά����"<<descriptors.size()<<endl;

        //������õ�HOG�����Ӹ��Ƶ�������������sampleFeatureMat
        for(int i=0; i<DescriptorDim; i++)
          sampleFeatureMat.at<float>(num+PosSamNO+NegSamNO,i) = descriptors[i];//��PosSamNO+num�����������������еĵ�i��Ԫ��
        sampleLabelMat.at<float>(num+PosSamNO+NegSamNO,0) = -1;//���������Ϊ-1������
      }
    }

    ////���������HOG�������������ļ�
    //ofstream fout("SampleFeatureMat.txt");
    //for(int i=0; i<PosSamNO+NegSamNO; i++)
    //{
    //	fout<<i<<endl;
    //	for(int j=0; j<DescriptorDim; j++)
    //		fout<<sampleFeatureMat.at<float>(i,j)<<"  ";
    //	fout<<endl;
    //}

    //ѵ��SVM������
    //������ֹ��������������1000�λ����С��FLT_EPSILONʱֹͣ����
    CvTermCriteria criteria = cvTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 1000, FLT_EPSILON);
    //SVM������SVM����ΪC_SVC�����Ժ˺������ɳ�����C=0.01
    CvSVMParams param(CvSVM::C_SVC, CvSVM::LINEAR, 0, 1, 0, 0.01, 0, 0, 0, criteria);
    cout<<"��ʼѵ��SVM������"<<endl;
    svm.train(sampleFeatureMat, sampleLabelMat, Mat(), Mat(), param);//ѵ��������
    cout<<"ѵ�����"<<endl;
    svm.save("src//SVM_HOG_BenchMark.xml");//��ѵ���õ�SVMģ�ͱ���Ϊxml�ļ�

  }
  else //��TRAINΪfalse����XML�ļ���ȡѵ���õķ�����
  {
    svm.load("src//SVM_HOG_BenchMark.xml");//��XML�ļ���ȡѵ���õ�SVMģ��
  }


  /*************************************************************************************************
  ����SVMѵ����ɺ�õ���XML�ļ����棬��һ�����飬����support vector������һ�����飬����alpha,��һ��������������rho;
  ��alpha����ͬsupport vector��ˣ�ע�⣬alpha*supportVector,���õ�һ����������֮���ٸ���������������һ��Ԫ��rho��
  ��ˣ���õ���һ�������������ø÷�������ֱ���滻opencv�����˼��Ĭ�ϵ��Ǹ���������cv::HOGDescriptor::setSVMDetector()����
  �Ϳ����������ѵ������ѵ�������ķ������������˼���ˡ�
  ***************************************************************************************************/
  DescriptorDim = svm.get_var_count();//����������ά������HOG�����ӵ�ά��
  int supportVectorNum = svm.get_support_vector_count();//֧�������ĸ���
 // cout<<"֧������������"<<supportVectorNum<<endl;

  Mat alphaMat = Mat::zeros(1, supportVectorNum, CV_32FC1);//alpha���������ȵ���֧����������
  Mat supportVectorMat = Mat::zeros(supportVectorNum, DescriptorDim, CV_32FC1);//֧����������
  Mat resultMat = Mat::zeros(1, DescriptorDim, CV_32FC1);//alpha��������֧����������Ľ��

  //��֧�����������ݸ��Ƶ�supportVectorMat������
  for(int i=0; i<supportVectorNum; i++)
  {
    const float * pSVData = svm.get_support_vector(i);//���ص�i��֧������������ָ��
    for(int j=0; j<DescriptorDim; j++)
    {
      //cout<<pData[j]<<" ";
      supportVectorMat.at<float>(i,j) = pSVData[j];
    }
  }

  //��alpha���������ݸ��Ƶ�alphaMat��
  double * pAlphaData = svm.get_alpha_vector();//����SVM�ľ��ߺ����е�alpha����
  for(int i=0; i<supportVectorNum; i++)
  {
    alphaMat.at<float>(0,i) = pAlphaData[i];
  }

  //����-(alphaMat * supportVectorMat),����ŵ�resultMat��
  //gemm(alphaMat, supportVectorMat, -1, 0, 1, resultMat);//��֪��Ϊʲô�Ӹ��ţ�
  resultMat = -1 * alphaMat * supportVectorMat;

  //�õ����յ�setSVMDetector(const vector<float>& detector)�����п��õļ����
  vector<float> myDetector;
  //��resultMat�е����ݸ��Ƶ�����myDetector��
  for(int i=0; i<DescriptorDim; i++)
  {
    myDetector.push_back(resultMat.at<float>(0,i));
  }
  //������ƫ����rho���õ������
  myDetector.push_back(svm.get_rho());
 // cout<<"�����ά����"<<myDetector.size()<<endl;
  //����HOGDescriptor�ļ����
 // HOGDescriptor myHOG_vertical(Size(15,30),Size(5,10),Size(5,5),Size(5,5),9,1,-1.0,0,0.2,true,34);//�˴������������еĲ��������opencv��Ĭ�ϵ�HOG����Ӳ���������֮ǰ�趨�Ĳ���ì�ܣ�������һ���磡����
  myHOG.setSVMDetector(myDetector);//����hog_vertical.cpp�е�setSVMDetector�����е�svmDetector��
  //myDetector.push_back(svm.get_rho());/////////////////////////////////daigai
  //myHOG_vertical.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

  //�������Ӳ������ļ�
  ofstream fout("HOGDetectorForOpenCV_BenchMark.txt");
  for(int i=0; i<myDetector.size(); i++)
  {
    fout<<myDetector[i]<<endl;
  }
  }





  else
{
	if(TRAIN)
  {
    string horz_ImgName;//ͼƬ��(����·��)
    ifstream horz_finPos("D:\\Traffic Light Detection\\JY_TrainingSamples\\horz_positive\\horz_Pos.txt");
    ifstream horz_finNeg("D:\\Traffic Light Detection\\JY_TrainingSamples\\horz_Neg\\horz_Neg.txt");
	// ifstream finNeg("C:\\Users\\JY\\Desktop\\test\\test.txt");//������ͼƬ���ļ����б�
   
	
	Mat Horz_sampleFeatureMat;//����ѵ������������������ɵľ��������������������ĸ�������������HOG������ά��	
    Mat Horz_sampleLabelMat;//ѵ����������������������������������ĸ�������������1��1��ʾ���ˣ�-1��ʾ����

    //���ζ�ȡ������ͼƬ������HOG������

    for(int num=0; num<HORZ_PosSamNO && getline(horz_finPos,horz_ImgName); num++)
    {
		cout<<"����"<<horz_ImgName<<endl;
      Mat horz_src = imread(horz_ImgName);//��ȡͼƬ

   
	  resize(horz_src,horz_src,Size(30,15));
	//resize(horz_src,horz_src,Size(50,20));
	

      vector<float> horz_descriptors;//HOG����������
      hog_horz.compute(horz_src,horz_descriptors,Size(8,8));//����HOG�����ӣ���ⴰ���ƶ�����(8,8)
	 // hog_vertical.compute(src,descriptors,Size(5,5));//����HOG�����ӣ���ⴰ���ƶ�����(8,8)
      cout<<"������ά����"<<horz_descriptors.size()<<endl;

      //�����һ������ʱ��ʼ�����������������������Ϊֻ��֪��������������ά�����ܳ�ʼ��������������
      if( 0 == num )
      {
        DescriptorDim = horz_descriptors.size();//HOG�����ӵ�ά��
        //��ʼ������ѵ������������������ɵľ��������������������ĸ�������������HOG������ά��sampleFeatureMat
        Horz_sampleFeatureMat = Mat::zeros(HORZ_PosSamNO+HORZ_NegSamNO+HORZ_HardExampleNO, DescriptorDim, CV_32FC1);
		//sampleFeatureMat = Mat::zeros(PosSamNO+5742*NegSamNO+HardExampleNO, DescriptorDim, CV_32FC1);
        //��ʼ��ѵ����������������������������������ĸ�������������1��1��ʾ���ˣ�0��ʾ����
        Horz_sampleLabelMat = Mat::zeros(HORZ_PosSamNO+HORZ_NegSamNO+HORZ_HardExampleNO, 1, CV_32FC1);
		//sampleLabelMat = Mat::zeros(PosSamNO+5742*NegSamNO+HardExampleNO, 1, CV_32FC1);
      }

      //������õ�HOG�����Ӹ��Ƶ�������������sampleFeatureMat
      for(int i=0; i<DescriptorDim; i++)
		Horz_sampleFeatureMat.at<float>(num,i) = horz_descriptors[i];//��num�����������������еĵ�i��Ԫ��
        Horz_sampleLabelMat.at<float>(num,0) = 1;//���������Ϊ1������
    }
	
    //���ζ�ȡ������ͼƬ������HOG������

    for(int num=0; num<HORZ_NegSamNO && getline(horz_finNeg,horz_ImgName); num++)
    {
      cout<<"����"<<horz_ImgName<<endl;
      Mat horz_src = imread(horz_ImgName);//��ȡͼƬ
	 
      //resize(src,src,Size(64,128));
	 resize(horz_src,horz_src,Size(30,15));
	   //resize(horz_src,horz_src,Size(50,20));
	  //cvtColor(src,gray,CV_RGB2GRAY);
      vector<float> horz_descriptors;//HOG����������
	  hog_horz.compute(horz_src,horz_descriptors,Size(8,8));//����HOG�����ӣ���ⴰ���ƶ�����(8,8)
      cout<<"������ά����"<<horz_descriptors.size()<<endl;

      

    for(int i=0; i<DescriptorDim; i++)
        Horz_sampleFeatureMat.at<float>(num+HORZ_PosSamNO,i) = horz_descriptors[i];//��PosSamNO+num�����������������еĵ�i��Ԫ��
		Horz_sampleLabelMat.at<float>(num+HORZ_PosSamNO,0) = -1;//���������Ϊ-1������
    }

    //����HardExample������
    if(HORZ_HardExampleNO > 0)
    {
      ifstream horz_finHardExample("D:\\Traffic Light Detection\\JY_TrainingSamples\\horz_HardExample\\hardexample.txt");//HardExample���������ļ����б�
      //���ζ�ȡHardExample������ͼƬ������HOG������
      for(int num=0; num<HORZ_HardExampleNO && getline(horz_finHardExample,horz_ImgName); num++)
      {
        cout<<"����"<<horz_ImgName<<endl;
       // ImgName = ImgName;//����HardExample��������·����
        Mat horz_src = imread(horz_ImgName);//��ȡͼƬ

		
		//resize(src,src,Size(64,128));
		resize(horz_src,horz_src,Size(30,15));
		//resize(src,src,Size(10,20));
		//cvtColor(src,gray,CV_RGB2GRAY);
        vector<float> horz_descriptors;//HOG����������
        hog_horz.compute(horz_src,horz_descriptors,Size(8,8));//����HOG�����ӣ���ⴰ���ƶ�����(8,8)
		//hog_vertical.compute(src,descriptors,Size(5,5));//����HOG�����ӣ���ⴰ���ƶ�����(8,8)
        //cout<<"������ά����"<<descriptors.size()<<endl;

        //������õ�HOG�����Ӹ��Ƶ�������������sampleFeatureMat
        for(int i=0; i<DescriptorDim; i++)
          Horz_sampleFeatureMat.at<float>(num+HORZ_PosSamNO+HORZ_NegSamNO,i) = horz_descriptors[i];//��PosSamNO+num�����������������еĵ�i��Ԫ��
		  Horz_sampleLabelMat.at<float>(num+HORZ_PosSamNO+HORZ_NegSamNO,0) = -1;//���������Ϊ-1������
      }
    }

    ////���������HOG�������������ļ�
    //ofstream fout("SampleFeatureMat.txt");
    //for(int i=0; i<PosSamNO+NegSamNO; i++)
    //{
    //	fout<<i<<endl;
    //	for(int j=0; j<DescriptorDim; j++)
    //		fout<<sampleFeatureMat.at<float>(i,j)<<"  ";
    //	fout<<endl;
    //}

    //ѵ��SVM������
    //������ֹ��������������1000�λ����С��FLT_EPSILONʱֹͣ����
    CvTermCriteria criteria = cvTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 1000, FLT_EPSILON);
    //SVM������SVM����ΪC_SVC�����Ժ˺������ɳ�����C=0.01
    CvSVMParams param(CvSVM::C_SVC, CvSVM::LINEAR, 0, 1, 0, 0.01, 0, 0, 0, criteria);
    cout<<"��ʼѵ��SVM������"<<endl;
    svm.train(Horz_sampleFeatureMat, Horz_sampleLabelMat, Mat(), Mat(), param);//ѵ��������
    cout<<"ѵ�����"<<endl;
    svm.save("src//SVM_HOG_Horz.xml");//��ѵ���õ�SVMģ�ͱ���Ϊxml�ļ�

  }
  else //��TRAINΪfalse����XML�ļ���ȡѵ���õķ�����
  {
    svm.load("src//SVM_HOG_Horz.xml");//��XML�ļ���ȡѵ���õ�SVMģ��
  }


  /*************************************************************************************************
  ����SVMѵ����ɺ�õ���XML�ļ����棬��һ�����飬����support vector������һ�����飬����alpha,��һ��������������rho;
  ��alpha����ͬsupport vector��ˣ�ע�⣬alpha*supportVector,���õ�һ����������֮���ٸ���������������һ��Ԫ��rho��
  ��ˣ���õ���һ�������������ø÷�������ֱ���滻opencv�����˼��Ĭ�ϵ��Ǹ���������cv::HOGDescriptor::setSVMDetector()����
  �Ϳ����������ѵ������ѵ�������ķ������������˼���ˡ�
  ***************************************************************************************************/
  DescriptorDim = svm.get_var_count();//����������ά������HOG�����ӵ�ά��
  int supportVectorNum = svm.get_support_vector_count();//֧�������ĸ���
  cout<<"֧������������"<<supportVectorNum<<endl;

  Mat alphaMat = Mat::zeros(1, supportVectorNum, CV_32FC1);//alpha���������ȵ���֧����������
  Mat supportVectorMat = Mat::zeros(supportVectorNum, DescriptorDim, CV_32FC1);//֧����������
  Mat resultMat = Mat::zeros(1, DescriptorDim, CV_32FC1);//alpha��������֧����������Ľ��

  //��֧�����������ݸ��Ƶ�supportVectorMat������
  for(int i=0; i<supportVectorNum; i++)
  {
    const float * pSVData = svm.get_support_vector(i);//���ص�i��֧������������ָ��
    for(int j=0; j<DescriptorDim; j++)
    {
      //cout<<pData[j]<<" ";
      supportVectorMat.at<float>(i,j) = pSVData[j];
    }
  }

  //��alpha���������ݸ��Ƶ�alphaMat��
  double * pAlphaData = svm.get_alpha_vector();//����SVM�ľ��ߺ����е�alpha����
  for(int i=0; i<supportVectorNum; i++)
  {
    alphaMat.at<float>(0,i) = pAlphaData[i];
  }

  //����-(alphaMat * supportVectorMat),����ŵ�resultMat��
  //gemm(alphaMat, supportVectorMat, -1, 0, 1, resultMat);//��֪��Ϊʲô�Ӹ��ţ�
  resultMat = -1 * alphaMat * supportVectorMat;

  //�õ����յ�setSVMDetector(const vector<float>& detector)�����п��õļ����
  vector<float> myDetector;
  //��resultMat�е����ݸ��Ƶ�����myDetector��
  for(int i=0; i<DescriptorDim; i++)
  {
    myDetector.push_back(resultMat.at<float>(0,i));
  }
  //������ƫ����rho���õ������
  myDetector.push_back(svm.get_rho());
  cout<<"�����ά����"<<myDetector.size()<<endl;
  //����HOGDescriptor�ļ����
 // HOGDescriptor myHOG_vertical(Size(15,30),Size(5,10),Size(5,5),Size(5,5),9,1,-1.0,0,0.2,true,34);//�˴������������еĲ��������opencv��Ĭ�ϵ�HOG����Ӳ���������֮ǰ�趨�Ĳ���ì�ܣ�������һ���磡����
  myHOG.setSVMDetector(myDetector);//����hog_vertical.cpp�е�setSVMDetector�����е�svmDetector��
  //myDetector.push_back(svm.get_rho());/////////////////////////////////daigai
  //myHOG_vertical.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

  //�������Ӳ������ļ�
  ofstream fout("HOGDetectorForOpenCV_horz.txt");
  for(int i=0; i<myDetector.size(); i++)
  {
    fout<<myDetector[i]<<endl;
  }
}

  

}