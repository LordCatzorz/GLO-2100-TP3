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
	m_listeVoisin.resize(n);
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

//! \brief		Obtient le nombre de sommet
//! \return		Le nombre de sommet
template<typename T,typename N>
size_t Graphe<T,N>::reqNbSommets() const
{
	return m_nbSommets;
}

//! \brief		Obtient le nom d'un sommet
//! \param[in] 	i L'index du sommet
//! \return 	le nom du sommet
template<typename T,typename N>
T Graphe<T,N>::reqNom(unsigned int i) const
{
	return m_noms[i];
}

//! \brief 		Obtient le poid entre deux sommet selon l'algorithme originale
//! \param[in]	i Index sommet origine
//! \param[in] 	j Index sommet destination
//! \return		Le poids entre ces deux sommets
template<typename T,typename N>
const N & Graphe<T,N>::reqPoids(unsigned int i, unsigned int j) const
{
	PRECONDITION( i< m_nbSommets && j < m_nbSommets);
	return m_matrice[i][j];
}


//! \brief 		Defini le nom d'un sommet
//! \param[in]	i Index sommet
//! \param[in] 	p_nom Référence vers le nom du sommet
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
	m_listeVoisin[i].push_back(voisin(j, poids));


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

//! \brief Algorithme de Dijkstra permettant de trouver le plus court chemin entre p_origine et p_destination
//! \pre p_origine et p_destination doivent être des sommets du graphe
//! \param[out] la longueur du plus court chemin est retournée
//! \param[out] le chemin est retourné
//! \return la longueur du chemin (= numeric_limits<N>::max() si p_destination n'est pas atteignable)
template<typename T,typename N>
N Graphe<T,N>::dijkstraV2(const unsigned int & p_origine, const unsigned int & p_destination,
		std::vector< std::pair<unsigned int, T> > & p_chemin)
{
	PRECONDITION( p_origine < m_nbSommets && p_destination < m_nbSommets);

	typename Graphe<T,N>::solution solutionTrouve;
	typename std::map<unsigned int, Graphe<T,N>::solution>::const_iterator iter = this->m_solutions.find(p_origine);
	if (iter != this->m_solutions.end())
	{
		solutionTrouve=iter->second;
	}
	else
	{
		solutionTrouve = this->DijkstraCalculerChemins(p_origine);
		typename std::pair<unsigned int, Graphe<T,N>::solution> pair;
		pair = make_pair(p_origine, solutionTrouve);
		this->m_solutions.insert(pair);
	}


	std::stack<unsigned int> pileDuChemin;
	for (unsigned int sommetPrecedent = p_destination;sommetPrecedent != std::numeric_limits<unsigned int>::max(); sommetPrecedent =  solutionTrouve.predecesseurs[sommetPrecedent])
	{
		pileDuChemin.push(sommetPrecedent);
	}

	while(!pileDuChemin.empty())
	{
		p_chemin.push_back( pair<unsigned int, T>(pileDuChemin.top(), reqNom(pileDuChemin.top())) );
		pileDuChemin.pop();
	}

	//cas où l'on n'a pas de solution
	if (solutionTrouve.predecesseurs[p_destination] == numeric_limits<unsigned int>::max()
	       && p_destination != p_origine)
		return numeric_limits<N>::max();

	return solutionTrouve.distance_minimum[p_destination];
}


//! \brief Algorithme de Dijkstra permettant de trouver tous les plus petits chemins d'un point d'origine p_origine.
//! \param[in] p_origine l'index d'un sommet du graphe.
//! \param[out] p_distance_minimum Un vecteur de taille qui représente la longueur du plus petit chemin du sommet d'origine au sommet à l'index.
//! \param[out] p_predecesseur Un vecteur d'index qui représente l'index du sommet précédent l'index dans son chemin le plus court du point d'origine
//! \pre p_origine doit être un sommet du graph
//! \note C'est algorithme est une version franciser de celui disponible au http://rosettacode.org/wiki/Dijkstra's_algorithm#C.2B.2B
template<typename T,typename N>
typename Graphe<T,N>::solution Graphe<T,N>::DijkstraCalculerChemins(unsigned int p_origine) const
{
	PRECONDITION( p_origine < m_nbSommets);
	int tailleListeVoisin = m_listeVoisin.size();
	typename Graphe<T,N>::solution laSolution;
	laSolution.distance_minimum.clear();
	laSolution.distance_minimum.resize(tailleListeVoisin, std::numeric_limits<N>::max());
	laSolution.distance_minimum[p_origine] = 0;

	laSolution.predecesseurs.clear();
	laSolution.predecesseurs.resize(tailleListeVoisin, std::numeric_limits<unsigned int>::max());
	std::set< std::pair<N, unsigned int> > lesArcsARegarder;
	lesArcsARegarder.insert(std::make_pair(laSolution.distance_minimum[p_origine], p_origine));
	
	while (!lesArcsARegarder.empty())
	{
		//Obtenir le sommet du première arcs.
		N distance = lesArcsARegarder.begin()->first;
		unsigned int sommet = lesArcsARegarder.begin()->second;
		lesArcsARegarder.erase(lesArcsARegarder.begin());
		
		//Visite chaque arc sortant du sommet;
		const std::vector<voisin>& vecteurVoisins = m_listeVoisin[sommet];

		for(typename std::vector<voisin>::const_iterator iter_voisin = vecteurVoisins.begin(); iter_voisin != vecteurVoisins.end(); iter_voisin++)
		{
			unsigned int unVoisin = iter_voisin->destination;
			N poidsVoisin = iter_voisin->poids;
			N poidsTotalVoisin = distance+poidsVoisin;
			if (poidsTotalVoisin < laSolution.distance_minimum[unVoisin])
			{
				lesArcsARegarder.erase(std::make_pair(laSolution.distance_minimum[unVoisin], unVoisin));

				laSolution.distance_minimum[unVoisin] = poidsTotalVoisin;
				laSolution.predecesseurs[unVoisin] = sommet;
				lesArcsARegarder.insert(std::make_pair(laSolution.distance_minimum[unVoisin], unVoisin));
			}
		}
	}
	return laSolution;
}

