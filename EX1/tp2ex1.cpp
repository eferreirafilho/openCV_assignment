/*TP 2 DISCPLINA ANÁLISE E DETECÇÃO DE IMAGENS


EDSON BERNARDES FERREIRA FILHO
EX1 - SUBTRAÇÃO DE BACKGROUND

*/

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;



    int process(VideoCapture& capture) {
    
         
	int thre=25,blur=1,fps=20;
	Mat frame,frame2,bg,bg_1,bg_2,output,bg_down,frame_down,out,out2, bg01,bg02,bg03,bg04,bg05,bg06,bg07,bg08,bg09,bg10;

	CvSize size = cvSize(640,480);
	double res=2;


	cout<<"Fator de multiplicacao da multiresolucao (1(original),2,4,8,16,32,80 ou 160): ";
	cin>>res;
	
		sleep(1);
		//CAPTURANDO BACKGROUND:
		for(int i=1;i<100;i++)			//Capturando e descartando várias fotos para inicializar a camera
		{
			capture >> bg_1;
			usleep(100);
		}
			
		//Capturando 10 backgrounds e fazendo a média para reduzir problemas na variação da iluminação
		capture >> bg01;usleep(100);
		capture >> bg02;usleep(100);
		capture >> bg03;usleep(100);
		capture >> bg04;usleep(100);	
		capture >> bg05;usleep(100);
		capture >> bg06;usleep(100);
		capture >> bg07;usleep(100);
		capture >> bg08;usleep(100);
		capture >> bg09;usleep(100);	
		capture >> bg10;usleep(100);

		bg_1=0.1*bg01+0.1*bg02+0.1*bg03+0.1*bg04+0.1*bg05+0.1*bg06+0.1*bg07+0.1*bg08+0.1*bg09+0.1*bg10;
		
		

		cvtColor(bg_1, bg_2, CV_BGR2GRAY);			//rgb -> grayscale
   		GaussianBlur(bg_2, bg, Size(3,3),blur,blur);		//filtro "blur" para reduzir rúido
		//imshow("Com Blur",bg);				//Imagem Background Resultante
		cout<<" BG Salvo!"<<endl;
		
		//imshow("bg_2",bg_2);
		
		resize(bg, bg,Size(),1/res,1/res);			//bg original perdido
		//imshow("bg",bg);					//bg downscaled
   
		VideoWriter oVideoWriter ("VideoTP2ex1.avi", CV_FOURCC('P','I','M','1'), 20, size,true);//inicia o VideoWriter 		

        for (int i=1;i<200;i++) 
	{      

		//CAPTURANDO VIDEO

		capture >> frame;					
		cvtColor(frame, frame2, CV_BGR2GRAY);			//rgb -> grayscale
		resize(frame2, frame_down, Size(),1/res,1/res );
		
		imshow("frame_down", frame_down);			//imagem original downscaled

		if( waitKey (30) >= 0) break;

		namedWindow("PARAMETROS", 1);

		createTrackbar( "Threadshold", "PARAMETROS", &thre, 255);	//Barra para regular o threadshold

		output=(frame_down - bg);					//Subtração do background
		output=output+(-frame_down + bg);					
		

		threshold(output,output,thre,255,THRESH_BINARY);
		//imshow("output", output);				//imagem original downscaled - bg downcaled	

		out=output;
		

		resize(out,out, Size(),res,res,INTER_NEAREST);		//Redimensiona com Interpolação simples(Nearest Neighbor)

		createTrackbar( "Blur", "PARAMETROS", &blur, 10);
		Canny(out, out, 100, 100, 3);

		//imshow("out", out);					//somente as bordas tamanho original

		
		cvtColor(out, out, CV_GRAY2BGR);
		
		out2=frame + out;					
		imshow("PARAMETROS", out2);  				//bordas em cima da imagem original

		oVideoWriter.write(out2);

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
