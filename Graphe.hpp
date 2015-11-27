//
//  Graphe.h
//  Classe pour graphes orientés pondérés avec matrice d'adjacence
//
//  Mario Marchand automne 2015.
//

#include "Graphe.h"

using namespace std;

//! \brief		Constructeur sans paramètre
//! \post		Un graphe vide est créé
template<typename T,typename N>
Graphe<T,N>::Graphe() : m_nbSommets(0)
{
}

//! \brief		Constructeur avec un paramètre n = nombre de sommets
//! \brief		initialise le vecteur m_noms avec n éléments
//! \brief		initialise la matrice de valuation: 0 dans la diagonale et infini ailleurs
//! \post		Un graphe vide est créé de n sommets
template<typename T,typename N>
Graphe<T,N>::Graphe(size_t n) : m_nbSommets(n)
{
	m_nbSommets = n;
	m_noms.resize(n);
	m_matrice.resize(n);
	for (unsigned int i = 0; i < n; ++i)
	{
		m_matrice[i].resize(n);
		for (unsigned int j = 0; j < n; ++j)
		{
			if (i == j)
				m_matrice[i][j] = 0;
			else
				m_matrice[i][j] = numeric_limits<N>::max();
		}
	}

}
//! \brief		Destructeur
//! \post		Le graphe est détruit
template<typename T,typename N>
Graphe<T,N>::~Graphe()
{
}

template<typename T,typename N>
size_t Graphe<T,N>::reqNbSommets() const
{
	return m_nbSommets;
}

template<typename T,typename N>
T Graphe<T,N>::reqNom(unsigned int i) const
{
	return m_noms[i];
}

template<typename T,typename N>
const N & Graphe<T,N>::reqPoids(unsigned int i, unsigned int j) const
{
	PRECONDITION( i< m_nbSommets && j < m_nbSommets);
	return m_matrice[i][j];
}

template<typename T,typename N>
void Graphe<T,N>::nommer(unsigned int i, const T & p_nom)
{
	m_noms[i] = p_nom; // operator= doit exister pour le type T
}

//! \brief ajoute un arc d'un poids donné par longueur
template<typename T,typename N>
void Graphe<T,N>::ajouteArc(unsigned int i, unsigned int j, N poids)
{
	PRECONDITION( i< m_nbSommets && j < m_nbSommets);
	m_matrice[i][j] = poids;
}

//! \brief Algorithme de Dijkstra permettant de trouver le plus court chemin entre p_origine et p_destination
//! \pre p_origine et p_destination doivent être des sommets du graphe
//! \param[out] la longueur du plus court chemin est retournée
//! \param[out] le chemin est retourné
//! \return la longueur du chemin (= numeric_limits<N>::max() si p_destination n'est pas atteignable)
template<typename T,typename N>
N Graphe<T,N>::dijkstra(const unsigned int & p_origine, const unsigned int & p_destination,
		std::vector< std::pair<unsigned int, T> > & p_chemin) const
{
	PRECONDITION( p_origine < m_nbSommets && p_destination < m_nbSommets);

	vector<N> distance(m_nbSommets);
	vector<unsigned int> predecesseur(m_nbSommets);
	vector<bool> estNonSolutionne(m_nbSommets);
	for (unsigned int i = 0; i < m_nbSommets; ++i)
	{
		distance[i] = numeric_limits<N>::max(); //une distance infinie a priori pour rejoindre ce noeud
		predecesseur[i] = numeric_limits<unsigned int>::max(); //indique l'absence d'un prédécesseur
		estNonSolutionne[i] = true;
	}
	distance[p_origine] = 0;

	list<unsigned int> sommetsNonSolutionnes;
	for (unsigned int i = 0; i < m_nbSommets; ++i)
		sommetsNonSolutionnes.push_back(i);

	//Boucle principale: touver distance[] et predecesseur[]
	for (unsigned int cpt = 0; cpt < m_nbSommets; ++cpt)
	{
		list<unsigned int>::iterator uStarItr;
		//trouver le sommet uStar de distance minimale
		if (!sommetsNonSolutionnes.empty())
		{
			uStarItr = sommetsNonSolutionnes.begin();
			for (list<unsigned int>::iterator itr = ++sommetsNonSolutionnes.begin(); itr != sommetsNonSolutionnes.end(); ++itr)
			{
				if (distance[*itr] < distance[*uStarItr])
					uStarItr = itr;
			}
		}
		else
		{
			throw(logic_error("Graphe<T,N>::disjkstra(): La liste des sommets non solutionnés ne devrait pas être vide"));
		}
		//enlever ce numéro de sommet de la liste sommetsNonSolutionnes
		unsigned int uStar = *uStarItr;
		sommetsNonSolutionnes.erase(uStarItr);
		estNonSolutionne[uStar] = false;

		if(distance[uStar]==numeric_limits<N>::max())
			break; //terminer dijkstra car p_destination n'est pas accessible de p_source

		if(uStar==p_destination)
			break; //terminer dijkstra car on a solutionné p_destination

		for(unsigned int u=0; u<m_nbSommets; ++u)
		{
			if(m_matrice[uStar][u]!=numeric_limits<N>::max() && u!=uStar && estNonSolutionne[u])
			{
				N temp = distance[uStar] + m_matrice[uStar][u];
				if (temp < distance[u])
				{
					distance[u] = temp;
					predecesseur[u] = uStar;
				}
			}
		}
	}

	//Construire le plus court chemin à l'aide de predecesseur[]
	p_chemin.clear();
	stack<unsigned int> pileDuChemin;
	unsigned int numero = p_destination;
	pileDuChemin.push(numero);
	while(predecesseur[numero]!= numeric_limits<unsigned int>::max())
	{
		numero = predecesseur[numero];
		pileDuChemin.push(numero);
	}
	while(!pileDuChemin.empty())
	{
		p_chemin.push_back( pair<unsigned int, T>(pileDuChemin.top(), reqNom(pileDuChemin.top())) );
		pileDuChemin.pop();
	}

    //cas où l'on n'a pas de solution
	if (predecesseur[p_destination] == numeric_limits<unsigned int>::max()
            && p_destination != p_origine)
        return numeric_limits<N>::max();

	return distance[p_destination];
}
