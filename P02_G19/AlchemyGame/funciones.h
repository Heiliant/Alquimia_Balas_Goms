#pragma once

void leerComando(std::string &comandoJugador);
void add(int numero, std::vector<std::string> &currentList);
void addBasics(std::vector <std::string> &currentList, std::vector<std::string> &elements_basics);
void BorrarElement(std::vector<std::string> &currentList, int &numero);
void informasao(int &indice, std::vector<std::string> &currentList);
void clean(std::vector<std::string> &currentList);
void Tutorial();