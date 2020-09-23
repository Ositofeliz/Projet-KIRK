#ifndef CARTEITEM_H
#define CARTEITEM_H
#include <QObject>
#include <QIcon>
#include <QGraphicsPixmapItem>
#include "Options.h"
#include "Tuile.h"
#include "CarteOpts.h"
#include "GestionObstacles/Obstacles.h"
#include "GestionObstacles/ObstacleItem.h"
#include "GestionMissions/GestionTrajectoires/AlgoAStar.h"

/*!
  \class CarteItem
  \brief La classe CarteItem est l'élément qui affiche la carte.

  \ingroup GestionCarte

  La classe CarteItem hérite de QGraphicsPixmapItem (et QObject).
  La carte est une simple image.

  La classe centralise la définition de la carte, l'algorithme A*,
  ainsi que les obstacles
*/
class CarteItem  : public QObject, public QGraphicsPixmapItem
{
	Q_OBJECT
private:
    QString _theme = "black/"; /*!< Thème sombre */
    QString _theme2 = "white/"; /*!< Thème clair  */
    Carte *_carte = nullptr; /*!< Carte */
    QList<QGraphicsItem*> _tuiles; /*!< Tuiles */
    QGraphicsRectItem *_tuilesCont = nullptr; /*!< Conteneur des tuiles */
    Obstacles _obstacles; /*!< Obstacle */
    AlgoAStar *_astar = new AlgoAStar(); /*!< Algorithme A* */
    QGraphicsPathItem *_trajecItem = nullptr; /*!< Trajectoire calculée */
    QGraphicsPathItem *_trajecItem2 = nullptr; /*!<  */
    QGraphicsPixmapItem *_departItem = nullptr; /*!< Point de départ sur la carte */
    QGraphicsPixmapItem *_cibleItem = nullptr; /*!< Cible sur la carte */
    QList<QGraphicsPixmapItem*> _checkpointItems; /*!<  */
    QList<ObstacleItem*> _obstacleItems; /*!< Liste des obstacles */
    char _z = 6; /*!<  */
    int _x = 30; /*!<  */
    int _y = 21; /*!<  */
    short _tailleTuile = 256; /*!< Taille d'une tuile */
    int _nbTuiles = pow( 2, 6 ); /*!< Nombre de tuiles */
    int _tailleTot = 256*_nbTuiles; /*!< Taille totale de la carte */
    int _firstIT = 30; /*!<  */
    int _firstJT = 21; /*!<  */
    PosCarte _centre = { (int)((_x+2.5)*_tailleTuile), (int)((_y+1.5)*_tailleTuile) }; /*!<  */
    PosCarte _offset = { 0, 0 }; /*!<  */
    QPoint _oldPos = {-1, -1}; /*!<  */
    QSize _tailleVue = {259, 259}; /*!<  */
    QSize _tailleCarteVisible = {256, 256}; /*!< Taille de la carte visible */
    double _lonMin = -180; /*!<  */
    double _lonMax = 180; /*!<  */
    double _latMin = 85.051129; /*!<  */
    double _latMax = -85.051129; /*!<  */
    Coordonnees _depart = {-1000, 0}; /*!< Coordonnées du point de départ */
    Coordonnees _cible = {-1000, 0}; /*!< Coordonnées de la cible */
    QList<Coordonnees> _checkpoints; /*!< Liste des coordonnées des checkpoints */
    QList<Coordonnees> _trajectoire; /*!< Coordonnées de la trajectoire */
    QList<Coordonnees> _trajectoire2; /*!<  */
    double _distanceTot = 0; /*!< Distance totale de la trajectoire */

public:
	// Destructor
    /*!
     * \brief Destructeur
     * Destructeur de la classe CarteItem.
     */
	~CarteItem()
	{
		clearTuiles();
		delete _tuilesCont;
		delete _carte;
		delete _astar;
		delete _trajecItem;
		delete _trajecItem2;
		delete _departItem;
		delete _cibleItem;
		clearCheckpointItems();
    }

	// Constructeurs
    /*!
     * \brief CarteItem
     * \param parent
     * Constructeur de la classe CarteItem.
     */
	CarteItem( QGraphicsItem *parent = nullptr ) : QGraphicsPixmapItem( parent )
	{
		// Initialisation du conteneur des tuiles
		_tuilesCont = new QGraphicsRectItem( this );
		//
		// Initialisation de la carte à la taille de MapItem
		_carte = new Carte( _tailleTot, _tailleTot );
		// Initialisation des obstacles
		_obstacles = Obstacles();
		//
		// Initialisation de _trajec
		// Cet Item permet d'affiche la trajectoire calculée
		_trajecItem = new QGraphicsPathItem( this );
		// Changer la couleur de la trajectoire
		QPen pen( QColor( 0, 80, 130), 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin );
		_trajecItem->setPen( pen );
		//
		// Initialisation des Item _depart et _cible
		// Items affichant le point de départ et la cible
		_departItem = new QGraphicsPixmapItem(this);
		QPixmap pix = QIcon( ICONS::PATH + _theme2 + ICONS::DEPART ).pixmap(QSize(20,20));
		_departItem->setPixmap( pix );
		_departItem->setOffset(-10, -20);
		_departItem->hide();
		_cibleItem = new QGraphicsPixmapItem(this);
		pix = QIcon( ICONS::PATH + _theme2 + ICONS::CIBLE ).pixmap(QSize(20,20));
		_cibleItem->setPixmap( pix );
		_cibleItem->setOffset(-10, -10);
		_cibleItem->hide();

		// AStarAlgo connexions
		connect( this, SIGNAL(demandeArretCalcul()), _astar, SIGNAL( demandeArretCalcul() ) );
		connect( _astar, &AlgoAStar::calculTermine, this, &CarteItem::calculTermine );
		connect( _astar, &AlgoAStar::arretPremature, this, &CarteItem::arretPremature );
    }

public slots:
    // Méthodes Publiques
    /*!
     * \brief majCarte
     *
     * Mise-à-jour de la carte.
     */
	void majCarte()
	{
		// Nettoyage des anciennes tuiles
		clearTuiles();
		//  Mise à jour de l'offset de la carte
		_offset.setX( 0.5*_tailleVue.width() - _centre.x() );
		_offset.setY( 0.5*_tailleVue.height() - _centre.y() );
		// Défintion du nombre de tuiles
		_nbTuiles = pow( 2, _z );
		// Défintion de la taille totale de la carte
		_tailleTot = _nbTuiles * _tailleTuile;
		// Définition de la première tuile à afficher
		_firstIT = 0;
		if ( _nbTuiles*_tailleTuile >= _tailleVue.width() && _offset.x() < 0 ) {
			_firstIT = -_offset.x() / _tailleTuile; }
		_firstJT = 0;
		if ( _nbTuiles*_tailleTuile >= _tailleVue.height() && _offset.y() < 0 ) {
			_firstJT = -_offset.y() / _tailleTuile; }
		//
		// Mise à jour de la taille de la carte visible.
		// NECESSAIRE pour majDepart, majCible, majCheckpoints !!!!
		_tailleCarteVisible = { (_tailleVue.width()/_tailleTuile+2)*256, (_tailleVue.height()/_tailleTuile+2)*256 };
		// Mise à jour du Départ
		majDepart();
		// Mise à jour de la Cible
		majCible();
		// Mise à jour des Checkpoints
		majCheckpoints();
		// Mise à jour de l'affichage de la trajectoire
		majTrajectoireItem();
		// Mise à jour de l'affichage des obstacles
		majObstacles();
		//
		// Mise à jour des tuiles de la carte
		int it, jt;
		for ( it = 0; it <= 1+_tailleVue.width()/_tailleTuile; it++)
		{
			if ( it+_firstIT < _nbTuiles )
			{
				for ( jt = 0; jt <= 1+_tailleVue.height()/_tailleTuile; jt++)
				{
					if ( jt+_firstJT < _nbTuiles )
					{
						// Ajout de la tuile
						Tuile *pixItem = new Tuile( _z,  it+_firstIT, jt+_firstJT, _tuilesCont );
						pixItem->setPos(  (it+_firstIT)*_tailleTuile + _offset.x(), (jt+_firstJT)*_tailleTuile + _offset.y() );
						_tuiles.append( pixItem );
						// Ajout du cadre de la tuile
						QGraphicsRectItem *rec = new QGraphicsRectItem( (it+_firstIT)*_tailleTuile + _offset.x(), (jt+_firstJT)*_tailleTuile + _offset.y(), 256, 256, _tuilesCont );
						rec->setPen( QPen(Qt::red));
						_tuiles.append( rec );
					}
					else
					{
						break;
					}
				}
			}
			else
			{
				break;
			}
		}
		// Mise à jour de la taille de la carte visible. TAILLE VRAI !
        _tailleCarteVisible = { (it+1)*256, (jt+1)*256 };
    }

    /*!
     * \brief majDepart
     * Méthode qui permet de mettre à jour l'affichage du point de départ.
     */
	void majDepart()
	{
		_departItem->hide();
		if ( (_depart.lat() >= _latMin && _depart.lat() <= _latMax) ||
			 (_depart.lat() <= _latMin && _depart.lat() >= _latMax) )
		{
			PosCarte departPos = getPosCarte( _depart );
			int newX = departPos.x() + _offset.x();
			int newY = departPos.y() + _offset.y();
			if ( newX > 0 && newX < _tailleCarteVisible.width() && newY > 0 && newY < _tailleCarteVisible.height() )
			{
				_departItem->setPos( newX, newY );
				_departItem->show();
			}
		}
    }

    /*!
     * \brief majCible
     * Méthode qui permet de mettre à jour l'affichage de la cible.
     */
	void majCible()
	{
		_cibleItem->hide();
		if ( (_cible.lat() >= _latMin && _cible.lat() <= _latMax) ||
			 (_cible.lat() <= _latMin && _cible.lat() >= _latMax) )
		{
			PosCarte ciblePos = getPosCarte( _cible );
			int newX = ciblePos.x() + _offset.x();
			int newY = ciblePos.y() + _offset.y();
			if ( newX > 0 && newX < _tailleCarteVisible.width() && newY > 0 && newY < _tailleCarteVisible.height() )
			{
				_cibleItem->setPos( newX, newY );
				_cibleItem->show();
			}
		}
    }

	// Mise à jour de l'affichage des checkpoints
    /*!
     * \brief majCheckpoints
     * Méthode qui permet de mettre à jour l'affichage des checkpoints.
     */
	void majCheckpoints()
	{
		// Nettoyage de l'affichage des checkpoints
		clearCheckpointItems();
		//
		for ( auto elm : _checkpoints )
		{
			if ( (elm.lat() >= _latMin && elm.lat() <= _latMax) ||
				 (elm.lat() <= _latMin && elm.lat() >= _latMax) )
			{
				PosCarte checkpoint = getPosCarte( elm );
				int newX = checkpoint.x() + _offset.x();
				int newY = checkpoint.y() + _offset.y();
				if ( newX > 0 && newX < _tailleCarteVisible.width() && newY > 0 && newY < _tailleCarteVisible.height() )
				{
					addCheckpointItem( QPoint(newX, newY) );
				}
			}
		}
    }
	// ----------------
    /*!
     * \brief majObstacles
     * Méthode qui permet de mettre à jour les obstacles.
     */
	void majObstacles()
	{
		clearObstacleItems();
		_obstacles.lectureFichierObstacle();
		//
		for( auto elm : _obstacles.liste() )
		{
			// Définition de la position du coin haut-gauche et bas-droit
			// sur l'affichage de la carte
			PosCarte pos = getPosCarte( { elm.centLat() - 0.5 * elm.latTail(), elm.centLon() - 0.5 * elm.lonTail() } );
			int newX = pos.x() + _offset.x();
			int newY = pos.y() + _offset.y();
			PosCarte pos2 = getPosCarte(  { elm.centLat() + 0.5 * elm.latTail(), elm.centLon() + 0.5 * elm.lonTail() } );
			// Ajout de l'obstacle en cours
			ObstacleItem *obst = new ObstacleItem( elm.type(), newX, newY, pos2.x()-pos.x(), pos2.y()-pos.y(), 0, this );
			obst->setPen( OBST::BorderColor );
			obst->setBrush( OBST::FillColor );
			_obstacleItems.append( obst );
		}
    }

    /*!
     * \brief majTrajectoireItem
     * Méthode qui permet de mettre à jour la trajectoire.
     */
	void majTrajectoireItem()
	{
		_trajecItem->hide();
		QPainterPath path;
		int oldX = 0;
		int oldY = 0;
		if ( !_trajectoire.empty() )
		{
			if ( _trajectoire.first() != _depart )
			{
				_trajectoire.push_back( _depart );
			}
			if ( _trajectoire.last() != _cible )
			{
				_trajectoire.push_front( _cible );
			}
		}
		for ( auto elm : _trajectoire )
		{
			PosCarte elmPos = getPosCarte( elm );
			int newX = elmPos.x();
			int newY = elmPos.y() + _offset.y();
			if ( elm == _trajectoire.first() )
			{
				int newXX = newX > _tailleTot ? newX - _tailleTot : newX;
				newXX +=  _offset.x();
				path.moveTo( newXX, newY );
			}
			else
			{
				int newXX = newX > _tailleTot ? newX - _tailleTot : newX;
				newXX +=  _offset.x();
				if ( (newX <= _tailleTot && oldX > _tailleTot ) ||
					 (newX > _tailleTot && oldX <= _tailleTot ) )
				{
					int oldXX = oldX > _tailleTot ? oldX - _tailleTot : oldX;
					path.moveTo( oldXX, oldY );
					path.closeSubpath();
					path.moveTo( newXX, newY );

				}
				else
				{
					path.lineTo( newXX, newY );
				}
			}
			oldX = newX;
			oldY = newY;
		}
		_trajecItem->show();
		_trajecItem->setPath( path );
    }

    /*!
     * \brief resetTrajectoire
     * Méthode qui permet de reset la trajectoire
     */
	void resetTrajectoire()
	{
		_trajecItem->setPath( QPainterPath() ); // reset de la traj
		_trajectoire.clear();
    }

    /*!
     * \brief majTailleVue
     * \param taille
     * Mise à jour de la taille du cadre (ou Viewport)
     */
	void majTailleVue( QSize taille )
	{
		_tailleVue = taille;
		majCarte();
    }

    /*!
     * \brief zoomIn
     * Méthode qui permet de zoomer dans la carte.
     */
	void zoomIn()
	{
		if ( _z < 19 )
		{
			_z++;
			_centre *= 2;
			majCarte();
		}
    }

    /*!
     * \brief zoomOut
     * Méthode qui permet de dézoomer dans la carte.
     */
	void zoomOut()
	{
		if ( _z > 0 )
		{
			_z--;
			_centre /= 2;
			majCarte();
		}
    }

    /*!
     * \brief move
     * \param dx
     * \param dy
     * Méthode qui permet de déplacer la carte.
     */
	void move( int dx, int dy )
	{
		_centre.move( dx, dy );
		majCarte();
    }

    /*!
     * \brief majTheme
     * \param theme
     * Méthode qui permet de mettre à jour le thème sélectionné.
     */
	void majTheme( QString theme )
	{
		_theme = theme;
		QPixmap pix = QIcon( ICONS::PATH + _theme2 + ICONS::DEPART ).pixmap(QSize(20,20));
		_departItem->setPixmap( pix );
		pix = QIcon( ICONS::PATH + _theme2 + ICONS::CIBLE ).pixmap(QSize(20,20));
		_cibleItem->setPixmap( pix );
		for ( auto elm : _checkpointItems )
		{
			pix = QIcon( ICONS::PATH + _theme2 + ICONS::CHECKPOINT ).pixmap(QSize(10,10));
			elm->setPixmap( pix );
		}
    }


    /*!
     * \brief calculTrajectoire
     * Méthode qui permet de calculer la trajectoire avec l'algorithme A*.
     */
	void calculTrajectoire()
	{
		_distanceTot = 0;
		resetTrajectoire();
		majObstacles();
		_astar->calculerTrajectoire( _depart, _cible, _checkpoints, _obstacles.liste() );
		if ( _astar->calculIsComplete() )
		{
			_distanceTot = ceil( _astar->distanceTot() );
            emit afficherFaisabilite( _distanceTot );
			_trajectoire.clear();
			_trajectoire =_astar->trajectoireCalculee();
			majTrajectoireItem();
		}
		else
		{
			emit afficherFaisabilite(-1);
		}
    }
	// Affiche le départ à la position pos
    /*!
     * \brief setDepart
     * \param pos Position de départ
     * Méthode qui permet d'afficher le départ à une position donnée.
     */
	void setDepart( QPointF pos )
	{
		QPoint local ( pos.x()-_offset.x(), pos.y()-_offset.y() );
		_depart = getCoordReel( local );
		_departItem->setPos( pos ); // Maj de la pos de Item
		_departItem->show();
		resetTrajectoire(); // reset de la traj
			emit afficherFaisabilite(-1);
    }

    /*!
     * \brief setDepart
     * \param co Coordonnées de départ
     * Méthode qui permet d'afficher le départ aux coordonnées saisies.
     */
	void setDepart( Coordonnees co )
	{
		_depart = co;
		majDepart(); // Maj de la pos de Item
		resetTrajectoire(); // reset de la traj
			emit afficherFaisabilite(-1);
    }

    /*!
     * \brief setCible
     * \param pos Position
     * Méthode qui permet d'afficher la cible à la position saisie.
     */
	void setCible( QPointF pos )
	{
		QPoint local ( pos.x()-_offset.x(), pos.y()-_offset.y() );
		_cible = getCoordReel( local );
		_cibleItem->show();
		_cibleItem->setPos( pos ); // Maj de la pos de Item
		resetTrajectoire(); // reset de la traj
			emit afficherFaisabilite(-1);
    }

    /*!
     * \brief setCible
     * \param co Coordonnées
     * Méthode qui permet d'afficher la cible aux coordonnées saisies.
     */
	void setCible( Coordonnees co )
	{
		_cible = co;
		majCible(); // Maj de la pos de Item
		resetTrajectoire(); // reset de la traj
    }

    /*!
     * \brief setCheckpoints
     * \param list Liste des checkpoints
     * Méthode qui permet d'afficher la liste des checkpoints.
     */
	void setCheckpoints( QList<Coordonnees> list )
	{
		clearCheckpoints();
		_checkpoints = list;
		majCheckpoints(); // Maj de la pos de Item
		resetTrajectoire(); // reset de la traj
			emit afficherFaisabilite(-1);
    }

    /*!
     * \brief addCheckpoint
     * \param pos Position
     * Méthode qui permet d'ajouter un checkpoint.
     */
	void addCheckpoint( QPointF pos )
	{
		QPoint local ( pos.x()-_offset.x(), pos.y()-_offset.y() );
		_checkpoints.append( getCoordReel( local ) );
		// Ajout à la liste _checkpointItems
		addCheckpointItem( pos );
		resetTrajectoire(); // reset de la traj
			emit afficherFaisabilite(-1);
    }


    /*!
     * \brief addCheckpointItem
     * \param pos
     * Méthode qui permet d'ajouter l'affichage d'un checkpoint
     */
	void addCheckpointItem( QPointF pos )
	{
		QGraphicsPixmapItem *newCP = new QGraphicsPixmapItem( this );
		newCP->setPos( pos ); //. maj de la position
		// Ajouter l'image
		char taille = 16;
		QPixmap pix = QIcon( ICONS::PATH + _theme2 + ICONS::CHECKPOINT ).pixmap(QSize(taille,taille));
		pix.scaledToWidth(12);
		newCP->setPixmap( pix );
		newCP->setOffset(-0.5*taille, -0.5*taille);
		// Ajout à la liste _checkpoints
		_checkpointItems.append( newCP );
    }

    /*!
     * \brief clearCheckpoints
     * Méthode qui permet d'effacer les checkpoints affichés sur la carte.
     */
	void clearCheckpoints()
	{
		_checkpoints.clear();
		clearCheckpointItems();
		resetTrajectoire(); // reset de la traj
			emit afficherFaisabilite(-1);
    }

    /*!
     * \brief clearCheckpointItems
     * Méthode qui permet de nettoyer l'affichage des checkpoints sur la carte.
     */
	void clearCheckpointItems()
	{
		for ( auto elm : _checkpointItems )
		{
			delete elm;
		}
		_checkpointItems.clear();
    }

    /*!
     * \brief clearObstacleItems
     * Méthode qui permet de nettoyer l'affichage des obstacles sur la carte.
     */
	void clearObstacleItems()
	{
		for ( auto elm : _obstacleItems )
		{
			delete elm;
		}
		_obstacles.clear();
		_obstacleItems.clear();
    }

    /*!
     * \brief clearTuiles
     * Méthode qui permet de nettoyer les tuiles.
     */
	void clearTuiles()
	{
		for ( auto elm : _tuiles )
		{
			delete elm;
		}
		_tuiles.clear();
    }


    /*!
     * \brief getDepartCoord
     * \return Coordonnées point de départ
     * Accesseur des coordonnées du point de départ.
     */
	Coordonnees getDepartCoord()
	{
		return _depart;
    }

    /*!
     * \brief getCibleCoord
     * \return Coordonnées cible
     * Accesseur des coordonnées de la cible
     */
	Coordonnees getCibleCoord()
	{
		return _cible;
    }

    /*!
     * \brief getCheckpointsCoord
     * \return Coordonnées checpoint
     * Accesseur des coordonnées du checkpoint
     */
	QList<Coordonnees> getCheckpointsCoord()
	{
		return _checkpoints;
    }

	// ----------------
	//  Permet d'obtenir les coordonnées réelles (lat, lon) d'un point de la carte 2D
	// Projection de MERCATOR - Modèle Sphérique
    /*!
     * \brief getCoordReel
     * \param pc
     * \return
     *
     * Méthode qui permet d'obtenir les coordonnées réelles (latitude, longitude)
     * d'un point de la carte 2D.
     *
     * Projection de MERCATOR - Modèle Sphérique.
     */
	Coordonnees getCoordReel( const PosCarte &pc )
	{
		double n = _tailleTot / ( _lonMax - _lonMin);
		double lo = _lonMin;
		double a = _tailleTot / ( agd( _latMax ) - agd( _latMin ) );
		double b =  - a * agd( _latMin );
		return Coordonnees( gd( ( pc.y() - b ) / a ), (pc.x() / n) + lo );
    }

    /*!
     * \brief versCoordReel
     * \param pc
     * \return
     * Méthode qui permet la conversion des coordonnées vers des coordonnées réelles.
     */
	Coordonnees versCoordReel( const QPointF &pc )
	{
		QPoint local ( pc.x()-_offset.x(), pc.y()-_offset.y() );
		return getCoordReel( local );
    }


    /*!
     * \brief getPosCarte
     * \param co
     * \return
     * Méthode qui permet d'obtenir le point de la carte 2D à partir de coordonnées réelles.
     * Projection de MERCATOR - Modèle Sphérique.
     */
	PosCarte getPosCarte( const Coordonnees &co )
	{
		double n = _tailleTot / ( _lonMax - _lonMin);
		double lo = _lonMin;
		double a = _tailleTot / ( agd( _latMax ) - agd( _latMin ) );
		double b =  - a * agd( _latMin );
		return PosCarte( n * ( co.lon() - lo ), a * agd( co.lat() ) + b );
	};

    /*!
     * \brief sauvegarderTrajectoire
     * Méthode qui permet de sauvegarder la trajectoire.
     */
	void sauvegarderTrajectoire()
	{
		QString fileName = QFileDialog::getSaveFileName(nullptr, "Sauvegarde de la trajectoire", "", "Fichier Trajectoire (*.txt)" );
		QFile file(fileName);
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			QMessageBox::warning(0, "Error", file.errorString());
			return;
		}
		QTextStream out( &file );
		for ( auto elm : _trajectoire )
		{
			out << elm.lat() << "; " << elm.lon() << "\n";
		}
		file.close();
	};

protected:

    /*!
     * \brief mouseDoubleClickEvent
     * Permet de détecter le double clic.
     */
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *) override
	{
		if (Qt::LeftButton)
		{
			// émission du signal doubleClicked(), si le button de la souris est le gauche !
			emit doubleClicked();
		}
	};

signals:
	void doubleClicked();
	void demandeArretCalcul();
	void envoiDistances( double, double );
	void calculTermine();
	void arretPremature();
	void afficherFaisabilite( int );
};


#endif // CARTEITEM_H
