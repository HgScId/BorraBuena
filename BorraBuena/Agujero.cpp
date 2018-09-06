#include "Agujero.h"
#include<queue>


void agujero::AnalizaAguj(int posfil, int poscol, cv::Mat & imagenUmbral, cv::Mat & conect)
{
	std::queue<cv::Vec2i> Extension;
	Extension.push({ posfil,poscol }); // introducimos el primer contenido detectado como último elemento (está vacío, se coloca el primero)
	conect.at<unsigned char>(posfil, poscol) = 255; // Conectar el píxel inicial. Ya analizado.
	tamano++;
	PosAgu.push_back({ posfil,poscol });

	while (!Extension.empty())
	{
		// Comprobación para evitar salidas de la imagen
		int testfil1 = 1, testfil2 = 1, testcol1 = 1, testcol2 = 1;
		if (Extension.front().val[0] == 0) testfil1 = 0;
		if (Extension.front().val[0] == conect.rows-1) testfil2 = 0;
		if (Extension.front().val[1] == 0) testcol1 = 0;
		if (Extension.front().val[1] == conect.cols-1) testcol2 = 0;
		
		if (testcol1 == 0 || testcol2 == 0 || testfil1 == 0 || testfil2 == 0) bordeimg = true;

		for (int i = Extension.front().val[0] - testfil1; i <= Extension.front().val[0] + testfil2; i++)
		{
			for (int j = Extension.front().val[1] - testcol1; j <= Extension.front().val[1] + testcol2; j++)
			{
				if (imagenUmbral.at<unsigned char>(i, j) == 255 && conect.at<unsigned char>(i, j) != 255)
				{
					conect.at<unsigned char>(i, j) = 255;
					tamano++;
					PosAgu.push_back({ i,j });

					Extension.push({ i,j });
				}
			}
		}
		Extension.pop(); // Se elimina de la queue el primer elemento.
	}
	



}

void agujero::ExtiendeAgujero(cv::Mat & imagensalida)
{
	tamano = 0;
	std::queue<cv::Vec2i> Extension;
	cv::Mat conect(imagensalida.size(), CV_8UC1, cv::Scalar(0)); // Matriz de conectores

	int cosa = 0;
	while (Extension.empty()) // Por si se da el caso que al dilatar y erosionar (por este orden) la primera posición introducida de un agujero deja de ser gris.
	{
		if (imagensalida.at<unsigned char>(PosAgu[cosa].val[0], PosAgu[cosa].val[1]) == 63)
		{
			Extension.push({ PosAgu[cosa].val[0],PosAgu[cosa].val[1] });
			tamano++;
			conect.at<unsigned char>(PosAgu[cosa].val[0], PosAgu[cosa].val[1]) = 255;
		}
		else
		{
			cosa++;
		}		
	}
	
	PosAgu.clear(); // Se va a limpiar las posiciones del agujero y se introducirán los nuevos valores.
	PosAgu.push_back({ Extension.front().val[0] ,Extension.front().val[1] }); // Se introduce la posición de partida

	while (!Extension.empty())
	{
		for (int i = Extension.front().val[0] - 1; i <= Extension.front().val[0] + 1; i++)
		{
			for (int j = Extension.front().val[1] - 1; j <= Extension.front().val[1] + 1; j++)
			{
				if (imagensalida.at<unsigned char>(i, j) == 63 && conect.at<unsigned char>(i, j) != 255)
				{
					conect.at<unsigned char>(i, j) = 255;
					tamano++;
					PosAgu.push_back({ i,j });

					Extension.push({ i,j });
				}
				else if (imagensalida.at<unsigned char>(i, j) == 255 && conect.at<unsigned char>(i, j) != 255) // ASUNCIÓN PELIGROSA! Se supone que siempre que se extiende por puntos blancos al final habrá mejora == true en master.
				{
					conect.at<unsigned char>(i, j) = 255;
					imagensalida.at<unsigned char>(i, j) = 63;
					tamano++;
					PosAgu.push_back({ i,j });

					Extension.push({ i,j });
				}
			}
		}
		Extension.pop(); // Se elimina de la queue el primer elemento.
	}
}
