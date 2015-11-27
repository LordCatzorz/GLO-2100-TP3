//
//  Graphe.h
//  pour graphes orientés valués avec matrice de valuation
//
// Mario Marchand, automne 2015
//

#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <list>
#include <stack>
#include <limits>
#include <iostream>
#include "ContratException.h"

//! \brief Patron de classe pour graphes orientés pondérés utilisant une matrice de valuation
//! \brief les numéros de sommets débutent à 0
//! \brief T est le type pour les noms de sommets
//! \brief N est le type pour les poids
template <typename T,typename N>
class Graphe
{
public:
    
	Graphe();
	Graphe(size_t p_nombre);
	~Graphe();

	const N & reqPoids(unsigned int i, unsigned int j) const;
	void ajouteArc(unsigned int i, unsigned int j, N poids);
	size_t reqNbSommets() const;
	T reqNom(unsigned int i) const;
	void nommer(unsigned int i, const T & p_nom);

	N dijkstra(const unsigned int & p_origine, const unsigned int & p_destination,
			std::vector< std::pair<unsigned int, T> > & p_chemin) const;
    
private:
	size_t m_nbSommets;
	std::vector<T> m_noms;  /*! les noms donnés aux sommets */
    std::vector< std::vector<N> > m_matrice; /*!< la matrice d'adjacence */
    
    
};

#include "Graphe.hpp"

#endif
