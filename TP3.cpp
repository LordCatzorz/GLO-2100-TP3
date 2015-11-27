//============================================================================
// Name        : TP3.cpp
// Author      : Mario
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <cmath>
#include <ctime>
#include <sys/time.h>

#include "Graphe.h"
#include "ContratException.h"

using namespace std;

//détermine le temps d'exécution (en microseconde) entre tv2 et tv2
long tempsExecution(const timeval& tv1, const timeval& tv2) {
	const long unMillion = 1000000;
	long dt_usec = tv2.tv_usec - tv1.tv_usec;
	long dt_sec = tv2.tv_sec - tv1.tv_sec;
	long dtms = unMillion * dt_sec + dt_usec;
	POSTCONDITION(dtms >= 0);
	return dtms;
}

Graphe<string,unsigned int> chargerGraphe(ifstream & p_fichierEntree)
{
	PRECONDITION(p_fichierEntree.is_open());

	unsigned int nbStations, noStation, nbLiens;
	p_fichierEntree >> nbStations >> nbLiens;

	Graphe<string,unsigned int> metro(nbStations);

	//Lecture du nom des stations
	for (unsigned int i = 0; i < nbStations; i++)
	{
		p_fichierEntree >> noStation;
		p_fichierEntree.ignore();
		string s;
		getline(p_fichierEntree, s);
		metro.nommer(noStation, s);
	}

	//Lecture des arc et placement des arcs dans le graphe en mémoire
	string s;
	getline(p_fichierEntree, s); //ignorer la ligne ne contenant que '$'
	int s1, s2, l_cout;
	for (unsigned int i = 0; i < nbLiens; i++)
	{
		p_fichierEntree >> s1 >> s2 >> l_cout;
		metro.ajouteArc(s1, s2, l_cout+20); //on ajoute un 20 secondes de délais d'attente par arc
		//std::cout << s1 << " " << s2 << " " << l_cout << std::endl;
	}

	return metro;
}

int executionUnePaire()
{
	timeval tv1;
	timeval tv2;

	ifstream fichier("Metro.txt");
	Graphe<string,unsigned int> metro = chargerGraphe(fichier);

	unsigned int numOrigine;
	unsigned int numDestination;
	unsigned int duree;

	cout << "Entrez le numéro de la station de départ" << endl;
	cin >> numOrigine;

	cout << "Entrez le numéro de la station d'arrivée" << endl;
	cin >> numDestination;

	if (gettimeofday(&tv1, 0) != 0)
			throw logic_error("gettimeofday() a échoué");

	vector< pair<unsigned int, string> > chemin;
	duree = metro.dijkstra(numOrigine, numDestination, chemin);
	if (duree == numeric_limits<unsigned int>::max())
		throw logic_error("Graphe<T,N>::dijkstra(): pas de solution pour cette paire origine/destination");

	if (gettimeofday(&tv2, 0) != 0)
			throw logic_error("gettimeofday() a échoué");

	POSTCONDITION(duree >= 0);
	cout << "Le plus court chemin trouvé par Dijkstra est: " << endl;
	for (unsigned int i = 0; i < chemin.size(); ++i)
	{
		cout << chemin[i].first << " " << chemin[i].second << endl;
	}
	cout << "avec un temps estimé de " << duree << " secondes" << endl << endl;

	cout << "Temps d'exécution = " << tempsExecution(tv1, tv2)
				<< " microsecondes" << endl << endl;

	return 0;
}

//exécute un algorithme de plus court chemin sur toutes les paires possibles
int moyenneToutesLesPaires()
{
	timeval tv1;
	timeval tv2;

	ifstream fichier("Metro.txt");
	Graphe<string,unsigned int> metro = chargerGraphe(fichier);

	unsigned int duree;
	vector< pair<unsigned int, string> > chemin;

	const unsigned int nbSt = 376;
	cout
			<< "Détermination du temps moyen d'exécution de l'algorithme de plus court chemin, moyenné sur toutes les paires origne/destination..."
			<< endl;

	long double sum_avg = 0;
	for (unsigned int i = 0; i < nbSt; ++i) {
		unsigned long total_ms = 0;
		for (unsigned int j = 0; j < nbSt; ++j) {
			if (j != i) {
				if (gettimeofday(&tv1, 0) != 0)
					throw logic_error("gettimeofday() a échoué");
				duree = metro.dijkstra(i, j, chemin);
				if (gettimeofday(&tv2, 0) != 0)
					throw logic_error("gettimeofday() a échoué");
				POSTCONDITION(duree >= 0);
				long tExec = tempsExecution(tv1, tv2);
				total_ms += tExec;
			}
		}
		sum_avg += (long double) total_ms / (long double) (nbSt - 1);
	}

	cout << "Temps d'exécution moyen de l'algorithme de plus court chemin = "
			<< sum_avg / (long double) nbSt << " microsecondes" << endl << endl;

	return 0;
}


int main()
{
	return executionUnePaire();
//	return moyenneToutesLesPaires();
}

