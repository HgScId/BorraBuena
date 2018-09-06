#pragma once
#include<opencv2/core/core.hpp> // Librer�as necesarias para usar OpenCV
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>


// Esta clase recopilar� las propiedades de los agujeros que se detecten en la imagen
class agujero
{

public: // Constructor de la clase


	
public: // Miembros p�blicos de la clase
	
	int tamano=0;
	bool bordeimg=false;
	std::vector<cv::Vec2i> PosAgu;

public: // Funciones p�blicas de la clase

	void AnalizaAguj(int posfil, int poscol, cv::Mat & imagenUmbral, cv::Mat & conect);
	void ExtiendeAgujero(cv::Mat & imagensalida);

private: // Funciones privadas de la clase

};