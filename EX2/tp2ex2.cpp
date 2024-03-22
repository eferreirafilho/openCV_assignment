/*TP 2 DISCPLINA ANÁLISE E DETECÇÃO DE IMAGENS


EDSON BERNARDES FERREIRA FILHO
EX2 - DETECÇÃO DE OBJETOS COLORIDOS

*/
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;



    int process(VideoCapture& capture) {
    
    	char frame0_image[100],out2_im[100],filename[100];
        
	int x=0,y=0,xant=0,yant=0,x2=0,y2=0,x2ant=0,y2ant=0,thre=20,blur=30,cor_low1=89,cor_up1=130,cor_low2=80,cor_up2=255,cor_low3=1,cor_up3=255,sat=1,pt1=0,pt2=0,
dp=2,hough1=200,hough2=100,minrad=1,maxrad=200,dif=5;
	Mat frame0,frame,frame2,cor;


Point P;

	Mat img;
	Mat ch1, ch2, ch3;


	capture >> frame;
 	Mat *rastro = new Mat(frame.rows,frame.cols,CV_8UC3); //Mat que vai conter as informações do rastro 

	CvSize size = cvSize(640,480);
	int fps=20;
	


	VideoWriter oVideoWriter ("VideoTP2ex2.avi", CV_FOURCC('P','I','M','1'), 30, size,true);//inicia o VideoWriter 		

        for (int k=1;k<200;k++) 
	{      

		//CAPTURANDO VIDEO
		capture >> frame;
	

		cvtColor(frame, frame2, CV_BGR2HSV);	//rgb -> HSV
		
		inRange(frame2,Scalar(cor_low1,cor_low2,cor_low3),Scalar(cor_up1,cor_up2,cor_up3),cor);	//Default para detecção de azul
		GaussianBlur(cor, cor, Size(9,9), blur, blur);

		
		if( waitKey (30) >= 0) break;


		//Parametros para mudar cor da detecção, parametros do hough circles
		namedWindow("PARAMETROS", 1);

		createTrackbar( "BLUR", "PARAMETROS", &blur, 100);
		createTrackbar( "HUE LOW", "PARAMETROS", &cor_low1, 179);	//Minimo valor de Hue
		createTrackbar( "HUE HIGH", "PARAMETROS", &cor_up1, 255);	//Maximo valor de Hue
		createTrackbar( "SATUR LOW", "PARAMETROS", &cor_low2, 255);	//Minimo valor de Saturation
		createTrackbar( "SATUR HIGH", "PARAMETROS", &cor_up2, 255);	//Maximo valor de Saturation
		createTrackbar( "VALUE LOW", "PARAMETROS", &cor_low3, 255);	//Minimo valor de Value
		createTrackbar( "VALUE HIGH", "PARAMETROS", &cor_up3, 255);	//Maximo valor de Value
		createTrackbar( "HOUGH1", "PARAMETROS", &hough1, 255);		//50	->Canny1 chamado pelo Hough Cirles
		createTrackbar( "HOUGH2", "PARAMETROS", &hough2, 255);		//100	->Canny2 chamado pelo Hough Circles
		createTrackbar( "minrad", "PARAMETROS", &minrad, 255);		//20	->Minimo Raio
		createTrackbar( "maxrad", "PARAMETROS", &maxrad, 500);		//300	->Maximo Raio

		

    vector<Vec3f> circles;
 
    HoughCircles(cor,circles,CV_HOUGH_GRADIENT,dp,cor.rows/4,hough1,hough2,minrad,maxrad);	//Detecta circulos e salva na mat COR
    

   // Draw the circles detected (Parte do código retirado de docs.opencv.org)
    for(int i = 0; i < circles.size(); i++ )
    {
         Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
         int radius = cvRound(circles[i][2]);
	
	//Desenha centro do círculo
        circle(frame,center, 3, Scalar(255,0,0), -2, 8, 0 );
         


	cout<<"\nX do objeto 1:  "<<circles[0][0];
	cout<<"  Y do objeto 1:  "<<circles[0][1];
	cout<<"\nX do objeto 2:  "<<circles[1][0];
	cout<<"  Y do objeto 2:  "<<circles[1][1];
	cout<<"\nRaio do objeto 1:  "<<circles[0][2];
	cout<<"  Raio do objeto 2:  "<<circles[1][2];


	//Desenhando rastro do primeiro objeto
	if(i==0 && (cvRound(circles[0][0])-xant)^2<dif && (cvRound(circles[0][1])-yant)^2<dif && cvRound(circles[0][2]>60)){
	
	x=cvRound(circles[0][0]); 		//X atual
	y=cvRound(circles[0][1]); 		//Y atual
	

	   if(x>1 && y>1 && xant>1 && yant>1 && (x-xant)^2<dif && (y-yant)^2<dif){	//Se Objeto não mudou muito de posição, desenha linha 
		line(*rastro, Point(x,y), Point(xant,yant),Scalar(255,0,0),3,8,0);
					
	}
	xant=x;			//X anterior
	yant=y;			//Y anterior
	}


	//Desenhando rastro do segundo objeto
	if(i==1 && (cvRound(circles[1][0])-x2ant)^2<dif && (cvRound(circles[1][0])-y2ant)^2<dif && cvRound(circles[1][2]<60)){
	x2=cvRound(circles[1][0]);		//X atual
	y2=cvRound(circles[1][1]);		//Y atual

	
	   if(x2>1 && y2>1 && x2ant>1 && y2ant>1 && (x2-x2ant)^2<dif && (y2-y2ant)^2<dif){  	//Se Objeto não mudou muito de posição, desenha linha 
		line(*rastro, Point(x2,y2), Point(x2ant,y2ant),Scalar(0,255,255),3,8,0);
						
	}

	x2ant=x2;
	y2ant=y2;
	}
	
    }
  
		if(circles.size()>0){
		cout<<"\nCircles:  "<<circles.size();		//Quantidade de objetos
		}

		add(frame,*rastro,frame,noArray(),-1);		//Junta rastros com a imagem em tempo real
		
		imshow("frame",frame);
		imshow("cor",cor);

		


		

		oVideoWriter.write(frame);

      }

}




int main() {


    VideoCapture capture(0); 					// Abrir a câmera padrão
	sleep(1);

    if (!capture.isOpened()) {					// Checa se a câmera foi aberta 

     	 cout << "Falha ao abrir a câmera, certifique-se que a câmera é a padrão e tente novamente!\n" << endl;
      return 1;
   }

    return process(capture);

}
