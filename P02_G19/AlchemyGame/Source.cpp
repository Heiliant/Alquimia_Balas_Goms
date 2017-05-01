#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <set>
#include <windows.h>
#include <cstdlib>
#include <algorithm>
#include "funciones.h"

struct pairHash {

	std::pair<std::string, std::string> a;

	size_t operator ()(const std::pair<std::string, std::string> &b) const {
		return ((std::hash<std::string>()(b.first)) ^ ((std::hash<std::string>()(b.second)) << 1)) >> 1;
	}
	bool operator==(const std::pair<std::string, std::string> &c) const {
		return ((a.first == c.first) && (a.second == c.second));
	}

};

bool myfunction(std::string &a, std::string &b) {
	return (a < b);
}

void dataLoad()
{

	status componente = status::_Value;

	std::string linea;
	std::ifstream elements;
	elements.open("elements.dat");

	std::vector <std::string> formulas;
	std::unordered_map <std::pair<std::string, std::string>, std::string, pairHash> MapaFormulas;

	while (getline(elements, linea))
	{
		formulas.push_back(linea);
	}
	elements.close();


	std::string key;
	std::string key2;
	std::string content;
	for (std::string e : formulas) //recorrerá el vector formulas y en cada iteracion e será la string de esa posicion
	{
		for (char i : e) //recorrera la string e y en cada iteracion i sera una letra de esta
		{
			switch (i) { //mientras recorro la string, los chars pueden valer '=', '+', ' ' o una letra cualquiera.
			case ('='):
				componente = status::_Clave1; //si he leído un igual, se que estoy empezando a leer la key1, así que actualizo el 
											  //status a _Clave1. Como lo primero que hay en la línea siempre es el value, el 
											  //valor de status es _Value por defecto.
				break;
			case('+'):
				componente = status::_Clave2; //si he leído el '+', lo mismo que antes pero a _Clave2
				break;
			case(' '): //este caso me asegura que las claves no se encontrarán rodeadas por espacios en blanco.
					   //cosa importante para poder después acceder a los valores.
				break;
			default:
				switch (componente) {  //dependiendo del valor de status guardo los chars en un container distinto
				case(status::_Value):
					content.push_back(i);
					break;
				case(status::_Clave1):
					key.push_back(i);
					break;
				case(status::_Clave2):
					key2.push_back(i);
					break;
				}
				break;
			}
		} //una vez se acaba el foreach y he leído todos los chars de la línea, actualizo el status para que en la siguiente
		  //iteración vuelva a guardar los chars en el container "content" hasta que se encuentr el '='
		componente = status::_Value;
		//Meto el pair con el pair de strings (keys) y el string (value)
		MapaFormulas.insert(std::pair <std::pair <std::string, std::string>, std::string>(std::pair <std::string, std::string>(key, key2), content));
		//y reseteo los contenedores donde se guardan las keys y el value.
		content.erase();
		key.erase();
		key2.erase();
	}

	componente = status::_Value;

	for (std::string i : formulas) {
		std::string valueTemp = "";
		std::string keyOneTemp = "";
		std::string keyTwoTemp = "";
		for (int z = 0; z < i.size(); ++z) {
			switch (i.at(z)) {
			case ' ':
				break;
			case '+': componente = status::_Clave2;
				break;
			case '=': componente = status::_Clave1;
				break;
			default: switch (componente) {
			case status::_Clave1: keyOneTemp.push_back(i.at(z));
				break;
			case status::_Clave2: keyTwoTemp.push_back(i.at(z));
				break;
			case status::_Value: valueTemp.push_back(i.at(z));
				break;
			}
					 break;
			}
		}
		std::string valTemp_mymap = MapaFormulas.at(std::pair<std::string, std::string>(keyOneTemp, keyTwoTemp));
		if (valTemp_mymap != valueTemp) {
			system("pause");
		}
		componente = status::_Value;
		if (i == formulas.back()) {
			valueTemp.clear();
			keyOneTemp.clear();
			keyTwoTemp.clear();
		}
	}

	formulas.clear();
	formulas.shrink_to_fit();
}

void leerComando(std::string &comandoJugador, std::vector<std::string> &currentList, std::vector<std::string> &elementsBasics) //esta a medias, la ire haciendo a medida que tengamos las funciones
{
	std::getline(std::cin, comandoJugador); //el getline mete todo lo que habia en cin en comandoJugador IGNORANDO el '\n'
	 //buscamos "add"
	
	if (comandoJugador.find("add") != std::string::npos) //si lo encontramos...
	{
		if (comandoJugador.find("basics") == std::string::npos) {//pero no encontramos la palabra basics, significa que el player
			//quiere duplicar un elemento de la lista.
				std::string addon = "add ";
				std::string elem1;
				elem1 = comandoJugador.substr(comandoJugador.find("add") + addon.length(),
					comandoJugador.find(',') - (comandoJugador.find("add") + addon.length()));
				std::cout << elem1 << std::endl;
				int elem1I = atoi(elem1.c_str()); //atoi ignora los espacios que pueda haber delante y/o detrás de los números

				add(elem1I, currentList);
			}
		else {// si SI encontramos basics, llamamos a la función addbasics y añadimos los 4 elementos básicos a la currentList
			addBasics(currentList, elementsBasics);
			}
		} //add done (+add basics)

	else if (comandoJugador.find("delete") != std::string::npos) {
		std::string addon = "delete ";
		std::string elem1 = comandoJugador.substr(comandoJugador.find("delete")+addon.length());
		int elem1I = atoi(elem1.c_str());
		_delete(elem1I, currentList);
	} //delete done

	else if (comandoJugador.find("info") != std::string::npos) {
		std::string addon = "info ";
		std::string elem1 = comandoJugador.substr(comandoJugador.find("info") + addon.length());
		int elem1I = atoi(elem1.c_str());

		informasao(elem1I, currentList);
	} //info done

	else if (comandoJugador.find("clean") != std::string::npos) {
		clean(currentList);
	} //clean done

	else if (comandoJugador.find("sort") != std::string::npos) {
		std::sort(currentList.begin(), currentList.end(), myfunction);
	} //sort done

	else if (comandoJugador.find("help") != std::string::npos) {
		Tutorial();
	}

	else {
		std::string::iterator w;
		for (char i : comandoJugador) {
			static int z = 0;
			if (i != ' ') {
				w = comandoJugador.begin()+(z);
				break;
			}
			++z;
		}
		std::string elem1 = comandoJugador.substr((comandoJugador.begin()+int(w)), comandoJugador.find(' '));
	}

}

void add(int numero, std::vector<std::string> &currentList) //duplica un elemento de la lista
{
	if ((numero-1)<currentList.size() && (numero-1)>=0) 
		currentList.push_back(currentList.at(numero - 1));
		
}

void _delete(int numero, std::vector<std::string> &currentList) {
	if((numero - 1)<currentList.size() && (numero - 1)>=0)
		currentList.erase(currentList.begin()+(numero-1));
}

void addBasics(std::vector <std::string> &currentList, std::vector<std::string> &elements_basics)
{
	for (std::string a : elements_basics) {
		currentList.push_back(a);
	}

}

void informasao(int &indice, std::vector<std::string> &currentList)
{
	std::string concatenacion = {"https://en.wikipedia.org/wiki/" + currentList.at(indice - 1) };

	ShellExecuteA(NULL, "open", concatenacion.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

void clean(std::vector<std::string> &currentList)
{
	std::set <std::string> currentListPurifier;
	for (std::string i : currentList) {
		currentListPurifier.insert(i);
	}
	currentList.clear();

	for (std::string i : currentListPurifier) {
		currentList.push_back(i);
	}
	currentList.shrink_to_fit();
}

void Tutorial()
{
	std::cout << "El jugador empieza con los 4 elementos basicos: Air, Fire, Water y Earth​" << std::endl;
	std::cout << "Cuando se combinan 2 elementos y estos producen un resultado, se suma 1 a la	puntuacion " << std::endl;
	std::cout << "si el nuevo elemento no se encuentra en la lista de elementos del jugador." << std::endl;
	std::cout << "Para combinar dos elementos introduzca el numero en lista del primero '', '' el numero en lista del segundo." << std::endl;
	std::cout << "No se pueden combinar 2 elementos que ocupen la misma posicion en la lista.​" << std::endl;
	std::cout << std::endl;
	std::cout << "Si el jugador escribe \"add\" y el numero de un elemento disponible en la lista, se añade una copia del elemento al que hace referencia dentro de la lista.​" << std::endl;
	std::cout << std::endl;
	std::cout << "Si el jugador escribe \"add basics​\" se añaden nuevas instancias de los 4 elementos basicos dentro de la lista de elementos.​" << std::endl;
	std::cout << std::endl;
	std::cout << "Si el jugador escribe \"delete​\" y el numero de un elemento disponible en la lista, se elimina el elemento al que hace referencia.​" << std::endl;
	std::cout << std::endl;
	std::cout << "Si el jugador escribe \"info​\" y el numero de un elemento disponible en la lista, se abre en el navegador la página de Wikipedia" << std::endl;
	std::cout << "con la información acerca del elemento.​" << std::endl;
	std::cout << std::endl;
	std::cout << "Si el jugador escribe \"sort​\" se ordenan todos los elementos por orden alfabetico." << std::endl;
	std::cout << std::endl;
	std::cout << "Si el jugador escribe \"clean​\" se eliminan todos los elementos que esten repetidos en la lista." << std::endl;
	std::cout << std::endl;
	std::cout << "Si el jugador escribe \"help​\" se muestra un tutorial con todas las acciones previamente mencionadas que puede realizar el jugador durante la partida." << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;

}

void listPrint(std::vector<std::string> currentList) {
	int w = 0;
	for (std::string i : currentList) {
		
		std::cout << ++w << ".- " << i << std::endl;
	}
}

void main() {
	dataLoad();
	std::vector<std::string> elements_basics = { "Fire", "Water", "Earth", "Air" };
	std::vector <std::string> currentList(elements_basics);
	Tutorial();

	do {
		std::string prueba = "";
		listPrint(currentList);
		leerComando(prueba, currentList, elements_basics);
		system("cls");
	} while (true);
}