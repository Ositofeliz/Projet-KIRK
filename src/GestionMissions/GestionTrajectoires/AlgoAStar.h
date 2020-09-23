#ifndef ALGOASTAR_H
#define ALGOASTAR_H

// **********************************
#include <QApplication>
#include <QObject>
#include <QEventLoop>
#include <QMessageBox>
#include <cmath>
#include <iostream>
#include <fstream>
#include <map>
using namespace std;
#include <QList>
#include <QFileDialog>
#include "GestionCarte/CarteOpts.h"
#include "GestionCarte/GestionObstacles/Obstacles.h"
// **********************************
// **********************************
// Options de l'algo
//namespace ASTAR
//{
//}
// **********************************
// **********************************
// Structures utilisées par l'algo

typedef  pair<int, int> CleNoeud;
/*!
 * \brief The Noeud struct
 * Structures utilsées par l'algorithme A*.
 */
struct Noeud
{
	double cout_DEP;
	double cout_CIB;
	double cout_TOT;
	CleNoeud parent;
};

typedef  map<CleNoeud, Noeud> ListeNoeuds;
// **********************************
// **********************************
/*!
 * \brief The AlgoAStar class
 *
 * Implémentaiton de l'algorithme A*, un algorithme de recherche de chemin
 * dans un graphe entre un noeud initial et un noeud final donnés.
 *
 * Description de l'algorithme :
 * "A Formal Basis for the Heuristic Determination of Minimum Cost Paths"
 * https://ieeexplore.ieee.org/document/4082128
 * Peter E. Hart; Nils J. Nilsson; Bertram Raphael;
 */
class AlgoAStar : public QObject
{
	Q_OBJECT
private:
	QList<char> donnee;
	// Taille de la carte
    int w = 100;  /*!< Largeur de la carte */
    int h = 100; /*!< Hauteur de la carte */

	// Position correspondante à la taille de la carte
	double minLat = -50;//85.051129;
	double minLon = 90;//-180;
	double maxLat = 50;//-85.051129;
	double maxLon =- 90;//180;
	// Position correspondante à la taille de la carte
    PosCarte depart = { -1, -1 }; /*!< Position du point de départ */
    PosCarte cible = { -1, -1 }; /*!< Position de la cible */
    QList<PosCarte> checkpoints;  /*!< Liste des positions des checkpoints */
	// Donnees locales
    PosCarte p1 = { -1, -1 };  /*!<  */
    PosCarte p2 = { -1, -1 }; /*!<  */
    Coordonnees co1 = { 0, 0 }; /*!<  */
    Coordonnees co2 = { 0, 0 }; /*!<  */
    ListeNoeuds noeudsOuverts;  /*!< Liste des noeuds ouverts */
    ListeNoeuds noeudsFermes; /*!< Liste des noeuds fermés */
    QList<PosCarte> trajectoire;  /*!< Liste des noeuds de la trajectoire */
    QList<Coordonnees> trajectoireReel;  /*!< Liste des coordonnées de la trajectoire réelle */
    bool arretCalcul = false;  /*!<  */
	bool calculLance = false;
	bool overlap = false;
    double _distanceTot = 0; /*!< Distance totale de la trajectoire */


	// Méthodes privées
    /*!
     * \brief distance
     * \param x1 Coordonnées X du point 1
     * \param y1 Coordonnées Y du point 1
     * \param x2 Coordonnées X du point 2
     * \param y2 Coordonnées Y du point 2
     * \return Retourne la distance entre deux points.
     *
     * Méthode qui permet de calculer la distance entre deux points.
     */
	double distance( int x1, int y1, int x2, int y2)
    {
		return CarteMaths::distance( versCoord( { x1, y1 } ), versCoord( { x2, y2 } ) );
    }

    /*!
     * \brief presentDansListe
     * \param cle Clé d'un noeud
     * \param lns Liste de noeuds
     * \return Retourne vrai ou faux
     * Méthode qui permet de vérfier si la clé d'un noeud est contenue dans une liste de noeuds.
     */
	bool presentDansListe( const CleNoeud &cle, const ListeNoeuds &lns)
	{
		for ( auto elm : lns )
		{
			QApplication::processEvents();
			if ( arretCalcul ) return false;
			if ( elm.first == cle )
				return true;
		}
		return false;
    }

    /*!
     * \brief ajouterCasesAdjacentes
     * \param cle Clé du noeud
     * Méthode qui permet d'ajouter dans a lsite des noeuds ouverts les
     * cases voisines du noeyd ayant pour clé "cle".
     */
	void ajouterCasesAdjacentes( const CleNoeud &cle )
	{
		Noeud tmp;
		for ( int it = cle.first - 1; it <= cle.first + 1 ; it+=1 )
		{
			if ( it < 0 || it > w )
				continue;

            int newX = it;
			for ( int jt = cle.second - 1; jt <= cle.second + 1; jt+=1)
			{
				QApplication::processEvents();
				if ( arretCalcul ) return;

				if ( jt < 0 || jt > h )
					continue;
				if ( newX == cle.first && jt == cle.second )
					continue;
				// Condition de non passage
				if ( getDonnee( newX, jt ) == CarteOpts::INTERDIT )
					continue;
				// Création de la cle du nouveau noeud
				CleNoeud ncle( newX, jt );
				// Vérification qu'il ne se trouve pas dans la liste des noeuds fermés
				if ( !presentDansListe( ncle, noeudsFermes ) )
				{
					// Calcul de la distance depuis le point de départ
					tmp.cout_DEP = noeudsFermes[cle].cout_DEP + distance( newX, jt, cle.first, cle.second );
					// Calcul de la distance vers la cible
					tmp.cout_CIB = distance( newX, jt, p2.x(), p2.y() );
					tmp.cout_TOT = tmp.cout_DEP + tmp.cout_CIB;
					tmp.parent = cle;
					if ( presentDansListe( ncle, noeudsOuverts ) )
					{
						/* le noeud est déjà présent dans la liste ouverte, il faut comparer les couts */
						if (tmp.cout_TOT < noeudsOuverts[ncle].cout_TOT){
							/* si le nouveau chemin est meilleur, on met à jour */
							noeudsOuverts[ncle]=tmp;
						}
						/* le noeud courant a un moins bon chemin, on ne change rien */
					}
					else
					{
						/* le noeud n'est pas présent dans la liste ouverte, on l'y ajoute */
						noeudsOuverts[CleNoeud(newX,jt)]=tmp;
					}
				}
			}
		}
    }


    /*!
     * \brief meilleurNoeud
     * \param lns Liste de noeud
     * \return Retourne la clé du noeud
     * Méthode qui permet de récupérer la clé du noeud
     * ayant leplus faible coût total entre le départ
     * et la cible.
     */
	CleNoeud meilleurNoeud( const ListeNoeuds &lns )
	{
		double m_coutTOT = lns.begin()->second.cout_TOT;
		CleNoeud m_noeud = lns.begin()->first;
		for ( auto elm : lns)
		{
			QApplication::processEvents();
			if ( arretCalcul ) return CleNoeud();
			if ( elm.second.cout_TOT < m_coutTOT )
			{
				m_coutTOT = elm.second.cout_TOT;
				m_noeud = elm.first;
			}
		}
		return m_noeud;
    }


    /*!
     * \brief ajouterListeFermee
     * \param cle Clé du noeud à ajouter
     * Méthode qui permet d'ajouter à la liste des noeuds fermés
     * le noeyd ayant pour clé "cle".
     */
	void ajouterListeFermee( const CleNoeud& cle )
	{
		Noeud& no = noeudsOuverts[cle];
		noeudsFermes[cle] = no;
		/* il faut le supprimer de la liste ouverte, ce n'est plus une solution explorable */
        noeudsOuverts.erase( cle );
		return;
    }


    /*!
     * \brief tracerTrajectoire
     * Méthode qui permet de récupérer la trajectoire optimale.
     */
	void tracerTrajectoire()
	{
		/* l'arrivée est le dernier élément de la liste fermée */
		Noeud& tmp = noeudsFermes[ CleNoeud( p2.x(), p2.y() ) ];
		_distanceTot += tmp.cout_TOT;
		PosCarte n;
		CleNoeud prec;
		n.setX( p2.x() );
		n.setY( p2.y() );
		prec.first  = tmp.parent.first;
		prec.second = tmp.parent.second;
		QList<PosCarte> trajTMP;
		trajTMP.push_front(n);

		while ( prec != CleNoeud( p1.x(), p1.y() ) )
		{
			QApplication::processEvents();
			if ( arretCalcul ) return;
			n.setX( prec.first );
			n.setY( prec.second );
			trajTMP.push_front( n );

			tmp = noeudsFermes[tmp.parent];
			prec.first  = tmp.parent.first;
			prec.second = tmp.parent.second;
		}

		for ( int it = 0; it < trajTMP.size(); it++ )
		{
			QApplication::processEvents();
			if ( arretCalcul ) return;
			Coordonnees newCoord = versCoord(trajTMP.at(it));
			if ( it == 0 && newCoord != co1   )
			{
				trajectoireReel.push_front( co1 );
			}
			//trajectoire.push_front( trajTMP.at(it) );
			trajectoireReel.push_front( newCoord );
			if ( it == trajTMP.size()-1 && newCoord != co2   )
			{
				trajectoireReel.push_front( co2 );
			}
		}
    }

    /*!
     * \brief nettoyageListeNoeuds
     * Méthode qui permet de nettoyer la liste des noeuds.
     */
	void nettoyageListeNoeuds()
	{
		noeudsOuverts.clear();
		noeudsFermes.clear();
    }

    /*!
     * \brief nettoyageTrajectoires
     * Méthode qui permet de nettoyer les trajectoires.
     */
	void nettoyageTrajectoires()
	{
		trajectoire.clear();
		trajectoireReel.clear();
    }

public:
    /*!
     *\brief Destructeur
     * Destructeur de la classe AlgoAStar.
     */
    ~AlgoAStar() {}

    // Constructeurs
    /*!
     * \brief AlgoAStar
     * Constructeur de la classe AlgoAStar.
     */
	AlgoAStar()
	{
		majDonnees();
		// TOUT OK !!!
		connect( this, SIGNAL(demandeArretCalcul()), this, SLOT(arret()));
		connect( this, SIGNAL(demandeArretCalcul()), this, SLOT(arreteCalcul()));
    }


	// Méthodes
	// Mise à jour de la carte - Initialisé à AUTORISE
    /*!
     * \brief majDonnees
     * Méthode qui permet de mettre à jour la carte.
     * Initialisé à AUTORISE.
     */
	void majDonnees()
	{
		donnee.clear();
		for ( int it = 0; it <= (w+1)*(h+1); it++ )
			donnee.push_back( CarteOpts::AUTORISE );
    }

    /*!
     * \brief majBornes
     * \param co1 Coordonnées au point 1
     * \param co2 Coordonnées au point 2
     * Méthode qui permet de mettre à jour les bornes de la carte.
     */
	void majBornes( const Coordonnees &co1, const Coordonnees &co2 )
	{
		double cLat = 0.5 * ( co2.lat() + co1.lat() );
		double cLon = 0.5 * ( co2.lon() + co1.lon() );
		double rayon = sqrt( pow( co1.lat() - cLat, 2 ) + pow( co1.lon() - cLon, 2) );
		minLat = cLat + rayon; if ( minLat > 86 ) minLat = 86;
		minLon = cLon - rayon;// if ( minLon < -180 ) minLon = -180;
		maxLat = cLat - rayon; if ( maxLat < -86 ) maxLat = -86;
        maxLon = cLon + rayon;// if ( maxLon > 180 ) maxLon = 180;
    }


    /*!
     * \brief majPoints
     * \param dep Point de départ
     * \param cib Cible
     * \param checkps Checkpoints
     * Méthode qui permet de mettre à jour le point de départ, la cible, et les
     * checkpoints dans la carte courante.
     */
	void majPoints( Coordonnees dep, Coordonnees cib, QList<Coordonnees> checkps )
	{
		depart = versCarte( dep );
        cible = versCarte( cib );
		checkpoints.clear();
		for ( auto elm : checkps )
		{
            checkpoints.append( versCarte(elm) );
		}
    }


    /*!
     * \brief majObstacles
     * \param obstacles
     * Méthode qui permet de mettre à jour les obstacles dans la carte courante.
     */
	void majObstacles( QList<Obstacle> obstacles )
	{
		//clearObstacleItems();
		for( auto elm : obstacles )
		{
			double lat1 =  elm.centLat() + 0.5 * elm.latTail();
			double lat2 =  elm.centLat() - 0.5 * elm.latTail();
			double lon1 =  elm.centLon() - 0.5 * elm.lonTail();
			double lon2 =  elm.centLon() + 0.5 * elm.lonTail();
			if ( overlap )
			{
//				cout << "----------" << endl;
//				cout << lon1 << " / " << lon2 << endl;
				if ( lon1 < 0 || lon2 < 0 ) { lon1 += 360; lon2 += 360; }
//				cout << lon1 << " / " << lon2 << endl;
				//OK
			}
			PosCarte pos1 = versCarte( { lat1, lon1 });
			PosCarte pos2 = versCarte( { lat2, lon2});
			int x1 = pos1.x(), x2 = pos2.x();
			if ( x1 > x2 ) { x1 = pos2.x(); x2 = pos1.x(); }
			int y1 = pos1.y(), y2 = pos2.y();
			if ( y1 > y2 ) { y1 = pos2.y(); y2 = pos1.y(); }
//				cout << "----------" << endl;
//				cout <<x1 << " / " << y1 << endl;
//				cout << x2 << " / " << y2 << endl;
			//
			// Ajout de l'obstacle dans la carte
			// Ajout d'un obstacle Rectangulaire
			if ( elm.type() == OBST::RECTANGLE )
			{
				for ( int it = x1; it <= x2; it++ )
				{
					if ( it >= 0 && it < w )
					{
						for ( int jt =y1; jt <= y2; jt++ )
						{
							if ( jt >= 0 && it < h )
							{
								setDonne( CarteOpts::INTERDIT, it, jt );
							}
						}
					}
				}
			}
			// Ajout d'un obstacle Ellipique
			else if ( elm.type() == OBST::ELLIPSE )
			{
				for ( int it = x1; it <= x2; it++ )
				{
					if ( it >= 0 && it < w )
					{
						for ( int jt =y1; jt <= y2; jt++ )
						{
							if ( jt >= 0 && it < h )
							{
								double centlon =  elm.centLon();
								if ( overlap && centlon < 0  ) centlon += 360;
								Coordonnees coord = versCoord( { it, jt });
								if (
									pow( 2*( coord.lat() - elm.centLat()) / elm.latTail(), 2 )  +
									pow( 2*(coord.lon() - centlon) / elm.lonTail(), 2 ) <= 1 )
								{
									setDonne( CarteOpts::INTERDIT, it, jt );
								}
							}
						}
					}
				}
			}
		}
    }



	// Fonction SUPER importante, car structurant le calcul !!
	// Permet de calculer la trajectoire entre deux points de la carte
    /*!
     * \brief rechercheTrajectoire
     * \param dep Départ
     * \param cib Cible
     * Méthode capitale qui permet de structurer le calcul. Elle permet de
     * calculer la trajectoire entre deux points de la carte.
     */
	void rechercheTrajectoire( const PosCarte &dep, const PosCarte &cib )
	{
		nettoyageListeNoeuds();
		// Définition du point de départ et d'arrivé;
		p1 = dep;
		p2 = cib;

		if ( getDonnee( p1.x(), p1.y() ) != CarteOpts::INTERDIT &&
			 p1.x() >= 0 && p1.x() <= w &&
			 p1.y() >= 0 && p1.y() <= h &&
			 getDonnee( p2.x(), p2.y() ) != CarteOpts::INTERDIT &&
			 p2.x() >= 0 && p2.x() <= w &&
			 p2.y() >= 0 && p2.y() <= h )
		{
			CleNoeud courant;
			/* déroulement de l'algo A* */
			/* initialisation du noeud courant */
			courant.first  = p1.x();
			courant.second = p1.y();

			/* ajout de courant dans la liste ouverte */
			Noeud nDep;
			nDep.parent = { p1.x(), p1.y() };
			noeudsOuverts[courant] = nDep;
			ajouterListeFermee( courant );
			ajouterCasesAdjacentes( courant );

			/* tant que la destination n'a pas été atteinte et qu'il reste des noeuds à explorer dans la liste ouverte */
			int itt = 0;
			while( !( ( courant.first == p2.x() ) && ( courant.second == p2.y() ) ) &&
				   ( !noeudsOuverts.empty() ) )
			{
				//				if ( itt % 20 == 0)
				//				{
				//					double dDep = distance( depart.x(), depart.y(), courant.first, courant.second );
				//					double dCib = distance( cible.x(), cible.y(), courant.first, courant.second );
				//					emit envoiDistances( dDep, dCib );
				//				}
				itt++;
				QApplication::processEvents();
				if ( arretCalcul ) return;
				/* on cherche le meilleur noeud de la liste ouverte, on sait qu'elle n'est pas vide donc il existe */
				courant = meilleurNoeud(noeudsOuverts);
				/* on le passe dans la liste fermée, il ne peut pas déjà y être */
				ajouterListeFermee(courant);
				/* on recommence la recherche des noeuds adjacents */
				ajouterCasesAdjacentes(courant);
			}
			/* si la destination est atteinte, on remonte le chemin */
			if ( (courant.first == p2.x()) && (courant.second == p2.y()) )
			{
				tracerTrajectoire();
			}
			else
			{
				/* pas de solution */
				QMessageBox msg;
				msg.addButton( QMessageBox::Ok );
				msg.setWindowTitle( "Information" );
				msg.setText( "Aucune trajectoire n'a pu être calculé !" );
				msg.setIcon( QMessageBox::Information );
				msg.exec();
			}
		}
		else
		{
			QMessageBox msg;
			msg.addButton( QMessageBox::Ok );
			msg.setWindowTitle( "Erreur ! " );
			msg.setText( "Des points que vous avez choisi se trouvent dans une zone interdite !!" );
			msg.setIcon( QMessageBox::Critical );
			msg.exec();
		}
    }

    /*!
     * \brief setDonne
     * \param val
     * \param it Position
     * Méthode qui permet de changer la donnée de la carte à la position  it
     */
	void setDonne( char val, int it )
	{
		if ( it >= 0 && it <= (w+1)*(h+1) )
			donnee.replace( it, val);
    }


    /*!
     * \brief setDonne
     * \param val
     * \param x
     * \param y
     * Méthode qui permet de changer la donnée de la carte aux coordonnées X et Y.
     */
	void setDonne( char val, int x, int y )
	{
		if ( ( y * (w+1) ) + x >= 0 &&
			 ( y * (w+1) ) + x  <= (w+1)*(h+1) )
			donnee.replace( ( y * (w+1) ) + x, val);
    }

	// ----------------
	// Récupère la donnée de la carte à la position it
    /*!
     * \brief getDonnee
     * \param it
     * \return
     * Méthode qui permet de récupérer la donnée de la carte à la position it.
     */
	char getDonnee( int it )
	{
		if ( it >= 0 && it <= (w+1)*(h+1) )
			return donnee.at( it );
		return -1;
    }


    /*!
     * \brief getDonnee
     * \param x Coordonnée x
     * \param y Coordonnée y
     * \return
     * Méthode qui permet de récupérer la donnée de la carte aux
     * coordonnées x et y.
     */
	char getDonnee( int x, int y )
	{
		if ( ( y * (w+1) ) + x >= 0 &&
			 ( y * (w+1) ) + x  <= (w+1)*(h+1) )
			return donnee.at( ( y * (w+1) ) + x );
		return -1;
    }

    /*!
     * \brief versCoord
     * \param pc
     * \return
     * Méthode qui permet de convertir PosCart en Coordonnees.
     */
	Coordonnees versCoord( const PosCarte &pc )
	{
		double f = 0;
		double aLon = ( maxLon - minLon ) / ( (1.0-2*f)*w );
		double bLon = minLon - f*aLon*w;
		double aLat = ( maxLat - minLat ) / ( (1.0-2*f)*h );
		double bLat = minLat - f*aLat*h;
		return Coordonnees( aLat*pc.y() + bLat, aLon*pc.x() + bLon );
    }

	// ----------------
	// Conversion Coordonnees vers PosCarte
    /*!
     * \brief versCarte
     * \param co
     * \return
     * Méthode qui permet la conversion de Coordonnées vers PosCarte.
     */
	PosCarte versCarte( const Coordonnees &co )
	{
		double f = 0;
		double ax =  (1.0-2*f) * w / ( maxLon - minLon );
		double bx = f * w - ax * minLon;
		double ay = (1.0-2*f) * h / ( maxLat - minLat );
		double by = f*h - ay * minLat;
		return PosCarte( ax*co.lon() + bx, ay*co.lat() + by );
    }


	PosCarte versCarteEtendue( const Coordonnees &co )
	{
		double f = 0;
		double ax =  1.0 * w / ( maxLon - minLon );
		double bx = f * w - ax * minLon;
		double ay = (1.0-2*f) * h / ( maxLat - minLat );
		double by = f*h - ay * minLat;
		return PosCarte( ax*co.lon() + bx, ay*co.lat() + by );
    }


    /*!
     * \brief sauvegarderTrajectoire
     * Méthode qui permet de sauvegarder la trajectoire.
     */
	void sauvegarderTrajectoire()
	{
		//	QString fileName = QFileDialog::getSaveFileName(nullptr, "Save File", "trajec.txt", "Fichier Trajectoire (*.txt)" );
		//	cout << fileName.toStdString() << endl;
		ofstream myfile;
		myfile.open("trajec.txt");
		if (!myfile.is_open())
		{
			cout << "Unable to open file";
		}
		for ( auto elm : trajectoireReel )
		{
			myfile << elm.lat() << "; " << elm.lon() << "\n";
		}
		myfile.close();
    }

    /*!
     * \brief sauvegarderCarte
     * Méthode qui permet d'afficher et d'enregistrer la carte.
     */
	void sauvegarderCarte()
	{
		// Enregistrement
		ofstream myfile;
		myfile.open ("map.txt");
		if (!myfile.is_open())
		{
			cout << "Unable to open file";
		}
		// Ajout de la trajectoire à la carte
		for ( int it = 0; it < trajectoire.size(); it++ )
		{
			setDonne( '*', trajectoire.at(it).x(), trajectoire.at(it).y() );
		}
		// AJout de la trajectoire à la carte
		for ( int it = 0; it < h; it++)
		{
			for ( int jt = 0; jt < w ; jt++)
			{
				myfile << getDonnee( jt, it ) ;
			}
			myfile <<"\n";
		}
		myfile.close();
    }


    /*!
     * \brief trajectoireCalculee
     * \return Trajectoire
     * Méthode qui permet de récupérer la trajectoire calculée
     */
	QList<Coordonnees> trajectoireCalculee()
	{
		return trajectoireReel;
    }

    /*!
     * \brief distanceTot
     * \return Distance
     * Méthode qui permet de récupérer la distance totale.
     */
	double distanceTot()
	{
		return _distanceTot;
    }

    /*!
     * \brief lissageTrajectoire
     * \param traj
     * \return
     * Méthode qui permet de lisser la trajectoire.
     */
	QList<PosCarte> lissageTrajectoire( QList<PosCarte> traj )
	{
		QList<PosCarte> trajTMP = traj;
		int tt = 1;
		//int tMax = trajTMP.size();
		while ( tt+1 < trajTMP.size() )
		{
			//	cout << "Iteration : " << trajTMP.size() << " - " << tt << endl;
			PosCarte p0 = trajTMP.at(tt-1);
			PosCarte p1 = trajTMP.at(tt);
			PosCarte p2 = trajTMP.at(tt+1);
			double d02 = distance( p0.x(), p0.y(), p2.x(), p2.y() );
			double d012 = distance( p0.x(), p0.y(), p1.x(), p1.y() ) + distance( p1.x(), p1.y(), p2.x(), p2.y() );
			//cout << "d02 : " << d02 << " - d012 : " << d012 << endl;
			if ( d02 < d012 )
			{
				bool obstacleTrouve = false;
				int minIT = p0.x() < p2.x() ? p0.x() : p2.x();
				int maxIT = p0.x() > p2.x() ? p0.x() : p2.x();
				int minJT = p0.y() < p2.y() ? p0.y() : p2.y();
				int maxJT = p0.y() > p2.y() ? p0.y() : p2.y();
				//				cout << "minIT : " << minIT << " - maxIT : " << maxIT << endl;
				//				cout << "minJT : " << minJT << " - maxJT : " << maxJT << endl;
				for ( int it = minIT; it <= maxIT; it++ )
				{
					for ( int jt = minJT; jt <= maxJT; jt++ )
					{
						if ( getDonnee( it, jt ) == CarteOpts::INTERDIT )
						{
							obstacleTrouve = true;
							break;
						}
					}
					if ( obstacleTrouve ) break;
				}
				if ( !obstacleTrouve )
				{
					//cout << "it : " << tt << endl;
					trajTMP.removeAt( tt );
				}
				else
				{
					tt++;
				}
			}
			else
			{
				tt++;
			}
		}
		return trajTMP;
    }


public slots:

    /*!
     *\brief calculerTrajectoire
     * Méthode qui permet de calculer la trajectoire.
     */
	void calculerTrajectoire( Coordonnees dep, Coordonnees cib, QList<Coordonnees> checkps, QList<Obstacle> obstacles = {} )
	{
		nettoyageTrajectoires();
		arretCalcul = false;
		calculLance = true;

//		for ( auto elm : checkps)
//		{
//			cout << elm.lat() <<  " / " << elm.lon() << endl;
//		}

		if ( ( dep.lat() > 86 && dep.lat() < -86 ) ||
			 (cib.lat() > 86 && cib.lat() < -86 ) )
		{
			QMessageBox msg;
			msg.addButton( QMessageBox::Ok );
			msg.setWindowTitle( "Information" );
			msg.setText( "Départ ou Cible non défini !" );
			msg.setIcon( QMessageBox::Information );
			msg.exec();
			emit arretPremature();
		}
		else
		{
			if ( checkps.size() == 0 )
			{
				_distanceTot = 0;
				co1 = dep;
				co2 = cib;
				if ( abs(co1.lon() - co2.lon()) > 180 )
				{
					co1.lon()  < 0 ? co1.setLon( co1.lon()  + 360) : co2.setLon( co2.lon()  + 360);
					overlap = true;
				}
				majDonnees();
				majBornes( co1, co2 );
				majObstacles( obstacles );
				rechercheTrajectoire( versCarte( co1 ),  versCarte( co2 ) );
				overlap = false;
			}
			else
			{
				co1 = dep;
				co2 = checkps.at(0) ;
				if ( abs(co1.lon() - co2.lon()) > 180 )
				{
					co1.lon()  < 0 ? co1.setLon( co1.lon()  + 360) : co2.setLon( co2.lon()  + 360);
					overlap = true;
				}
				majDonnees();
				majBornes( co1, co2 );
				majObstacles( obstacles );
				rechercheTrajectoire( versCarte( co1 ),  versCarte( co2 ) );
				overlap = false;
				for ( int it = 0; it < checkps.size()-1; it++)
				{
					QApplication::processEvents();
					if ( arretCalcul ) return;
					co1 = checkps.at(it);
					co2 = checkps.at(it+1) ;
					if ( abs(co1.lon() - co2.lon()) > 180 )
					{
						co1.lon()  < 0 ? co1.setLon( co1.lon()  + 360) : co2.setLon( co2.lon()  + 360);
						overlap = true;
					}
					majDonnees();
					majBornes( co1, co2 );
					majObstacles( obstacles );
					rechercheTrajectoire( versCarte( co1 ),  versCarte( co2 ) );
					overlap = false;
				}
				co1 = checkps.last();
				co2 = cib ;
				if ( abs(co1.lon() - co2.lon()) > 180 )
				{
					co1.lon()  < 0 ? co1.setLon( co1.lon()  + 360) : co2.setLon( co2.lon()  + 360);
					overlap = true;
				}
				majDonnees();
				majBornes( co1, co2 );
				majObstacles( obstacles );
				rechercheTrajectoire( versCarte( co1 ),  versCarte( co2 ) );
				overlap = false;
			}
		}
		if ( !arretCalcul ) emit calculTermine();
		calculLance = false;
    }
	// ---------------------
	// Demande l'arret du calcul
    /*!
     * \brief arreteCalcul
     * Méthode qui permet d'arrêter le calcul de la trajectoire
     */
	void arreteCalcul()
	{
		arretCalcul = true;
    }

    /*!
     * \brief calculIsComplete
     * \return
     * Méthode qui permet d'afficher quand le calcul de la trajectoire
     * est terminé.
     */
	bool calculIsComplete()
	{
		return !arretCalcul;
    }

    /*!
     * \brief arret
     * Méthode qui affiche un message lorsque le calcul est arrêté.
     */
	void arret()
	{
		if ( !arretCalcul && calculLance )
			QMessageBox::warning(0, "Info", "Calcul arrêté !!" );
    }

signals:
	void demandeArretCalcul();
	void envoiDistances( double, double );
	void calculTermine();
	void arretPremature();
};
// **********************************
// **********************************
#endif // ALGOASTAR_H
