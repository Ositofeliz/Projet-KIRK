#ifndef OBSTACLES_H
#define OBSTACLES_H

#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QPainter>
#include <cmath>
#include "ObstacleItem.h"

#include <iostream>
using namespace std;

/*!
 *\namespace
 * Espace de nommage des options des obstacles
 */
namespace OBST
{
    static const QString FilePath = "OBSTACLES.txt"; /*!< Chemin vers le fichier contenant les obstacles*/
    static const QColor Color = QColor( 245, 12, 35);
	static const QBrush FillColor = QBrush ( QColor( Color.red(), Color.green(), Color.blue(), 100) );
	static const QPen BorderColor = QPen ( Color);
	// Extension dans le header "Obstacle.h" -> Ajout des types d'obstacles
}


/*!
 * \brief The Obstacle class
 *
 * La classe Obstacle permet de définir les obstacles sur CarteItem et dans la carte utilisée
 * par l'algorithme A*.
 */
class Obstacle
{
    QString _id; /*!< identifiant de l'obstacle */
    char _type = OBST::INCONNU; /*!< Type d'obstacle */
    double _centLat; /*!< Latitude du centre de l'obstacle */
    double _centLon; /*!< Longitude du centre de l'obstacle */
    double _latTail; /*!< Latitude de l'extrémité de l'obstacle */
    double _lonTail; /*!< Longitude de l'extrémlité de l'obstacle */

public:
    /*!
      * \brief Destructeur
      * Destructeur de la classe Obstacle
      */
    ~Obstacle() {}

    /*!
     * \brief Obstacle
     *
     * Constructeur par défaut de la classe Obstacle
     */
    Obstacle() {}

    /*!
     * \brief Obstacle
     * \param obst
     *
     * Constructeur de la classe Obstacle prenant en paramètre les options de la classe obstacle.
     */
	Obstacle( QString obst )
	{
		QStringList obstDet = obst.remove(' ').split(';');
		if ( obstDet.size() == 6 )
		{
			setID( obstDet.at(0) );
			if ( obstDet.at( 1 ) == "rect" )
			{
				setType( OBST::RECTANGLE );
			}
			else if ( obstDet.at( 1 ) == "ell" )
			{
				setType( OBST::ELLIPSE );
			}
			setCentLat( obstDet.at(2).toDouble() );
			setCentLon( obstDet.at(3).toDouble() );
			setLatTail( obstDet.at(4).toDouble() );
			setLonTail( obstDet.at(5).toDouble() );
		}
    }

    /*!
     * \brief setID
     * \param id
     *
     * Mutateur de l'ID de l'obstacle
     */
    void setID( QString id ) { _id = id; }

    /*!
     * \brief setType
     * \param type
     *
     * Mutateur du type d'obstacle
     */
	void setType( QString type )
	{
		if ( type.startsWith("rect") )
		{
			_type = OBST::RECTANGLE;
		}
		else
		{
			_type = OBST::ELLIPSE;
		}
    }

    /*!
     * \brief setType
     * \param type
     *
     * Mutateur du type d'obstacle.
     */
    void setType( char type ) { _type = type; }

    /*!
     * \brief setCentLat
     * \param centLat
     * Mutateur de la latitude du centre de l'obstacle.
     */
    void setCentLat( double centLat ) { _centLat = centLat; }

    /*!
     * \brief setCentLon
     * \param centLon
     * Mutateur de la longititude du centre de l'obstacle.
     */
    void setCentLon( double centLon ) { _centLon = centLon; }

    /*!
     * \brief setLatTail
     * \param latTail
     * Mutateur de la latitude de l'extrémité de l'obstacle.
     */
    void setLatTail( double latTail ) { _latTail = latTail; }

    /*!
     * \brief setLonTail
     * \param lonTail
     * Mutateur de la longitude de l'extrémité de l'obstacle.
     */
    void setLonTail( double lonTail ) { _lonTail = lonTail; }

    /*!
     * \brief ID
     * \return Retourne l'identifiant de l'obstacle
     * Accesseur de l'identifiant de l'obstacle
     */
    QString ID() const { return _id; }

    /*!
     * \brief type
     * \return Retourne le type de l'obstacle
     * Accesseur du type de l'obstacle
     */
    char type() { return _type ; }

    /*!
     * \brief centLat
     * \return Retourne la latitude du centre de l'obstacle
     * Accesseur de la latitude du centre de l'obstacle
     */
    double centLat() { return _centLat; }

    /*!
     * \brief centLon
     * \return Retourne la longititude du centre de l'obstacle
     * Accesseur de la longitude du centre de l'obstacle
     */
    double centLon() { return _centLon; }

    /*!
     * \brief latTail
     * \return Retourne la latitude de l'extrémité de l'obstacle
     * Accesseur de la latitude de l'extrémité de l'obstacle
     */
    double latTail() { return _latTail; }

    /*!
     * \brief lonTail
     * \return Retourne la longitude de l'extrémité de l'obstacle
     * Accesseur de la longitude de l'extrémité de l'obstacle
     */
    double lonTail() { return _lonTail; }

    /*!
     * \brief sauvegarder
     * \return
     *
     * Permet de sauvegarder un obstacle.
     */
	QString sauvegarder()
	{
		QString local = ID() + ";";
		if ( type() == OBST::RECTANGLE )
		{
			local += "rect";
		}
		else if ( type()  == OBST::ELLIPSE )
		{
			local += "ell";
		}
		else
		{
			local = "";
		}
		if ( local != "" )
		{
			local = local + ";" + QString::number( centLat() ) + ";" + QString::number( centLon() ) + ";" + QString::number( latTail() ) + ";" + QString::number( lonTail() );
		}
		return local;
	}
};

// **********************************
/*!
 * \brief The Obstacles class
 *
 * La classe Obstacles est une liste d'objet Obstacle.
 */
class Obstacles
{
private:
    QList<Obstacle> _obstacles;
public:
	// Destructeur
    /*!
      *\brief Destrcuteur
      * Destructeur de la classe Obstacles
      */
    ~Obstacles(){}

    /*!
     * \brief Obstacles
     * Constructeur par défaut de la classe obstacle à partir du fichier OBSTACLES.TXT
     */
	Obstacles()
	{
		lectureFichierObstacle();
    }

    /// Getters & Setters
    /*!
     * \brief liste
     * \return Retourne la liste des obstacles
     * Accesseur de la classe Obstacles.
     */
	QList<Obstacle> liste()
	{
		return _obstacles;
    }

	// Méthodes
    /*!
     * \brief lectureFichierObstacle
     * Méthode qui permet de lire le fichier OBSTACLES.TXT
     */
	void lectureFichierObstacle()
	{
		clear();
		QStringList local;
		QFile file( OBST::FilePath );
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			QMessageBox::warning(0, "Error", file.errorString());
			return;
		}
		QTextStream stream( &file );
		while ( !stream.atEnd() )
		{
			QString line = stream.readLine().remove(" ");
			if ( line != "" && !line.contains("//") )
				_obstacles.append( Obstacle(line) );
		}
		file.close();
		return;
    }

    /*!
     * \brief sauvegarder
     * Méthode qui permet de sauvegarder la liste des obstacles.
     */
	void sauvegarder()
	{
		QFile file( OBST::FilePath );
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			QMessageBox::warning(0, "Error", file.errorString());
			return;
		}
		QTextStream out( &file );
		for ( auto elm : liste() )
			out << elm.sauvegarder() << "\n";
		file.close();
    }

    /*!
     * \brief ajouter
     * \param obst
     * Méthode qui permet d'ajouter un obstacle à la liste.
     */
	void ajouter( Obstacle obst )
	{
		_obstacles.append( obst );
    }

    /*!
     * \brief modifier
     * \param obst
     * \param it
     * \return
     * Méthode qui permet de modifier l'obstacle à l'index it.
     */
	bool modifier( Obstacle obst, int it )
	{
		if ( it >= 0 && it < liste().size() )
		{
			_obstacles.replace( it, obst );
			return true;
		}
		return false;
    }

    /*!
     * \brief supprimer
     * \param it
     * \return
     * Méthode qui permet de supprimer l'obstacle à l'index it.
     */
	bool supprimer( int it )
	{
		if ( it >= 0 && it < liste().size() )
		{
			_obstacles.removeAt( it );
			return true;
		}
		return false;
    }

    /*!
     * \brief existeDeja
     * \param obst
     * \return
     * Méthode qui permet de vérifier si l'obstacle existe déjà dans la liste avec son identifiant.
     */
	bool existeDeja( Obstacle obst )
	{
		for ( auto elm : liste() )
			if ( elm.ID() == obst.ID() )
				return true;
		return false;
    }

    /*!
     * \brief clear
     * Méthode qui permet de nettoyer la liste d'obstacles.
     */
	void clear()
	{
		_obstacles.clear();
    }

    /*!
     * \brief ajouterDansFichier
     * \param obst
     * Méthode qui permet d'ajouter un obstacle dans le fichier OBSTACLES.TXT.
     */
	static void ajouterDansFichier( Obstacle obst )
	{
		QFile file( OBST::FilePath );
		if (!file.open( QIODevice::Append ))
		{
			QMessageBox::warning(0, "Error", file.errorString());
			return;
		}
		QTextStream out( &file );
		out << obst.sauvegarder() << "\n";
		file.close();
    }

    /*!
     * \brief dejaDansFichier
     * \param id
     * \return
     * Méthode qui permet de vérifier si un obstacle est déjà dans le fichier, en fonction de son identifiant.
     */
	static bool dejaDansFichier( QString id )
	{
		QStringList local;
		QFile file( OBST::FilePath );
		if (!file.open( QIODevice::ReadOnly | QIODevice::Text ))
		{
		}
		QTextStream stream( &file );
		while ( !stream.atEnd() )
		{
			QString line = stream.readLine().remove(" ");
			if ( line != "" && !line.contains("//") )
			{
				QString obstDet = line.remove(' ').split(';').at(0);
				if ( obstDet == id )
				{
					return true;
				}
			}
		}
		file.close();
		return false;
    }
};

#endif // OBSTACLES_H
