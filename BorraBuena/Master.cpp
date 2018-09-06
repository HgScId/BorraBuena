#include<opencv2/core/core.hpp> // Librer�as necesarias para usar OpenCV.
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>


#include<iostream>
#include<conio.h>     
#include<vector>
#include<math.h>
#include<queue>
#include <filesystem>
#include "Agujero.h"

using namespace std;
using namespace cv;
using namespace experimental::filesystem::v1;

int main()
{
	path ruta;
	ruta= "ImagenEntrada/";
	int num_img = 0;

	for (directory_entry p : recursive_directory_iterator(ruta)) // iteraci�n en carpetas y subcarpetas de la ruta
	{
		path ruta_archivo = p; // Cada archivo o subcarpeta localizado se utiliza como clase path
		string direc = ruta_archivo.string(); // convertir la ruta de clase path a clase string
		string nombre = ruta_archivo.filename().string(); // convertir el nombre del archivo de clase path a string	
		if (nombre.compare(nombre.size()-3,3,"bmp")==0) // Para im�genes .BMP SOLAMENTE!!!
		{
			num_img++;
			cv::Mat imagen = cv::imread(direc, CV_LOAD_IMAGE_GRAYSCALE); // Abre la imagen y la transforma en escala de grises
			//cv::imwrite("ImagenSalida/"+to_string(num_img) + "_Egris_"+nombre, imagen);

			cv::Mat imagenUmbral;
			cv::threshold(imagen, imagenUmbral, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU); // Umbralizar con Otsu
			//cv::imwrite("ImagenSalida/" + to_string(num_img) + "_ImgUmbral_" + nombre, imagenUmbral);

			std::vector<agujero> Agujeros; // Aqu� se almacenan todos los agujeros de la imagen
			cv::Mat conect(imagenUmbral.size(), CV_8UC1, cv::Scalar(0)); // Matriz de conectores de la imagen

			for (int i = 0; i < imagenUmbral.rows; i++)
			{
				for (int j = 0; j < imagenUmbral.cols; j++)
				{

					if (imagenUmbral.at<unsigned char>(i, j) == 255 && conect.at<unsigned char>(i, j) != 255) // Si el p�xel es un agujero y todav�a no ha sido conectado:
					{
						agujero agujerotemp; // Agujero temporal que se introducir� (o no) al vector Agujeros
						agujerotemp.AnalizaAguj(i, j, imagenUmbral, conect);

						if (agujerotemp.bordeimg==false) Agujeros.emplace_back(agujerotemp); // Si el agujero no pertenece al borde, introd�celo en el vector Agujeros
					}
				}
			}

			// Se calcula la media de todos los agujeros detectados en la imagen
			int mediaAgu = 0;

			for (int i = 0; i < Agujeros.size(); i++)
			{
				mediaAgu += Agujeros[i].tamano;
			}

			mediaAgu /= int(Agujeros.size());


			// Analizamos si los agujeros son m�s grandes que la media y si no lo son se eliminan de la lista. Si son m�s grandes
			for (int i = int(Agujeros.size())-1; i >=0; i--)
			{
				if (Agujeros[i].tamano > mediaAgu)
				{
					for (int j = 0; j < Agujeros[i].PosAgu.size(); j++)
					{
						imagenUmbral.at<unsigned char>(Agujeros[i].PosAgu[j].val[0], Agujeros[i].PosAgu[j].val[1]) = 63;
					}
				}
				else
				{
					Agujeros.erase(Agujeros.begin()+i);
				}
			}
			// EROSI�N Y DILATACI�N DE LA IMAGEN PARA LIMPIAR LA IMAGEN INICIAL. SE ELIMIN� ESTA PARTE DEL C�DIGO.
			/*
			bool mejora = true; // La erosi�n mejora la imagen
			while (mejora)
			{
				mejora = false;
				cv::Mat kerneldilat = cv::getStructuringElement(0, cv::Size(3, 3), cv::Point(-1, -1)); // El 0 es matriz de convoluci�n cuadrada. Punto ancla (-1,-1) en el centro del kernel (matriz de convoluci�n)
				cv::Mat kerneleros = cv::getStructuringElement(1, cv::Size(3, 3), cv::Point(-1, -1)); // El 1 es matriz de convoluci�n en forma de cruz. Punto ancla (-1,-1) en el centro del kernel (matriz de convoluci�n)

				dilate(imagenUmbral, imagenUmbral, kerneldilat); // Dilata la imagen
				//cv::imwrite("prueba/cosa1.bmp", Salida);

				erode(imagenUmbral, imagenUmbral, kerneleros); // Erosiona la imagen.
				//cv::imwrite("prueba/cosa2.bmp", Salida);

				std::vector<agujero> AgujerosTemp;
				AgujerosTemp = Agujeros;
				int diferencia = 0; // Indica la diferencia de p�xeles entre agujeros
				for (int i = 0; i < Agujeros.size(); i++)
				{
					AgujerosTemp[i].ExtiendeAgujero(imagenUmbral);
					diferencia= AgujerosTemp[i].tamano - Agujeros[i].tamano;
					if (diferencia > 40) mejora = true;
				}
				if (mejora == true)
				{
					Agujeros = AgujerosTemp;
				}
			}
			*/
			
			cv::imwrite("ImagenSalida/" + to_string(num_img) + "_ImgFinal_" + nombre, imagenUmbral);
		}
		

	}
	return 0;
	
}