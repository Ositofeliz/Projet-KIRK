// **********************************
// **********************************
// Fichier contenant la classe Mission
// et la classe ListeMissions
// **********************************
// **********************************
#ifndef MISSION_H
#define MISSION_H
// **********************************
#include <QString>
#include <QTime>
#include <QList>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <iostream>
using namespace std;
#include "GestionUtilisateurs/Utilisateur.h"
#include "GestionDrones/Drone.h"
#include "GestionCarte/CarteOpts.h"


/*!
 *\namespace MissionOpts
 * Espace de nommage des options de mission.
 */
namespace MissionOpts
{
	static constexpr char UNKNOWN = -2;
	static constexpr char UNAUTHORIZED = -1;
	static constexpr char ADMIN = 0;
	static constexpr char USER = 1;
	static const QString FILEPATH = "MISSIONS.txt";
};

/*!
 * \brief The Mission class
 * La classe Mission décrit les caractérisitques d'une mission.
 *
 * Une mission est définie par un identifiant, une date et heure de départ,
 * ses coordonnées de départ et d'arrivée, ses checkponts, la charge allouée,
 * le drone sélectionné,et l'utilisateur qui a créé la mission.
 */
class Mission
{
private:
    QString _id; /*!< Identitfiant de la mission */
    QTime _time; /*!< Heure de la mission */
    QDate _date; /*!< Date de la mission */
    Coordonnees _depart = Coordonnees(-1000, 0); /*!< Coordonnées du point de départ */
    Coordonnees _cible = Coordonnees(-1000, 0); /*!< Coordonnées de la cible */
    QList<Coordonnees> _checkpoints; /*!< Liste des coordonnées des checkpoints */
    double _charge = 0; /*!< Charge attribuée au drone pour la mission */
    QString _drone; /*!< Drone sélectionné pour la mission */
    QString _util; /*!< Utilisateur qui a lancé la mission */

public:

    /*!
     *\brief Destructeur
     * Destructeur de la classe Mission
     */
    ~Mission() {}

    /*!
     * \brief Mission
     * Constructeur par défaut de la classe Mission.
     */
	Mission(){}


    /*!
     * \brief Mission
     * \param id Identifiant de la mission
     * Constructeur de mission avec le paramètre Identifiant.
     */
	Mission( QString id )
	{
		setID( id );
    }

    /*!
     * \brief Mission
     * \param id
     * \param data
     */
	Mission( QString id, QString data )
	{
		setID( id );
    }

	// ----------------
    /// Mission : Setters & Getters

    /*!
     * \brief setID
     * \param id Identifiant de la mission
     * Mutateur du paramètre identifiant.
     */
    void setID( QString id ) { _id = id; }

    /*!
     * \brief id
     * \return Retourne l'identifiant de la mission
     * Accesseur de l'identifiant de la mission.
     */
    QString id() const { return _id; }

    /*!
     * \brief setTime
     * \param time Heure de la mission
     * Mutateur du paramètre time (heure) de la mission.
     */
    void setTime( QTime time ) { _time = time; }

    /*!
     * \brief time
     * \return time
     * Accesseur du paramètre time (heure) de la mission.
     */
    QTime time() const { return _time; }

    /*!
     * \brief setDate
     * \param date Date de la mission
     * Mutateur du paramètre date de la mission.
     */
    void setDate( QDate date ) { _date = date; }

    /*!
     * \brief date
     * \return Retourne la date de la mission
     * Accesseur du paramètre date de la misison.
     */
    QDate date() const { return _date; }

    /*!
     * \brief setDepart
     * \param depart Coordonées du point de départ
     * Mutateur du point de départ de la mission.
     */
    void setDepart( Coordonnees depart ) { _depart = depart; }

    /*!
     * \brief setDepart
     * \param depart
     * Mutateur du point de départ à partir d'une QString (fichier texte).
     */
	void setDepart( QString depart )
	{
		if ( depart.contains('/') )
		{
			QStringList tmp = depart.split( '/' );
			_depart.setLat( tmp.at(0).toDouble() );
			_depart.setLon( tmp.at(1).toDouble() );
		}
    }

    /*!
     * \brief depart
     * \return Retourne les coordonnées du point de départ
     * Accesseurs des coordonnées du point de départ.
     */
    Coordonnees depart() const { return _depart; }

    /*!
     * \brief setCible
     * \param cible Coordonées de la cible
     * Mutateur des coordonées de la cible.
     */
    void setCible( Coordonnees cible ) { _cible = cible; }

    /*!
     * \brief setCible
     * \param cible Coordonées de la cible contenu dans un texte
     * Mutateur des coordonnées de la cible contenus dans un fichier texte.
     */
	void setCible( QString cible )
	{
		if ( cible.contains('/') )
		{
			QStringList tmp = cible.split( '/' );
			_cible.setLat( tmp.at(0).toDouble() );
			_cible.setLon( tmp.at(1).toDouble() );
		}
    }

    /*!
     * \brief cible
     * \return Retourne les coordonnées de la cible
     * Accesseur des coordonées de la cible de la mission.
     */
    Coordonnees cible() const { return _cible; }

    /*!
     * \brief setCheckpoints
     * \param checks Coordonnées des checkpoints
     * Mutateur des coordonnées des checkpoints.
     */
    void setCheckpoints( QList<Coordonnees> checks ) { _checkpoints = checks; }

    /*!
     * \brief setCheckpoints
     * \param checks Coordonnées des checkpoints (texte)
     * Mutateur des coordonnées des checkpoints à partir d'un texte.
     */
	void setCheckpoints( QString checks )
	{
		QStringList checksList = checks.split( '+' );
		for ( auto elm : checksList )
		{
			if ( elm.contains('/') )
			{
				QStringList tmp = elm.split( '/' );
				Coordonnees tmpCO = Coordonnees( tmp.at(0).toDouble(), tmp.at(1).toDouble() );
				_checkpoints.append( tmpCO );
			}
		}
    }

    /*!
     * \brief checkpoints
     * \return Liste des coordonnées des checkpoints.
     * Accesseur des coordonnées des checkpoints.
     */
    QList<Coordonnees> checkpoints() const { return _checkpoints; }

    /*!
     * \brief setCharge
     * \param charge Charge embarquée par le drone
     * Mutateur de la charge embarquée par le drone pour la mission.
     */
    void setCharge( double charge ) { _charge = charge; }

    /*!
     * \brief charge
     * \return Retourne la charge embarquée.
     * Accesseur de la charge embarquée par le drone pour la mission.
     */
    double charge() const { return _charge; }

    /*!
     * \brief setDrone
     * \param drone Drone sélectionné pour la mission
     * Mutateur du drone sélectionné pour la mission.
     */
    void setDrone( QString drone ) { _drone = drone; }

    /*!
     * \brief drone
     * \return Retourne le drone sélectionné pour une mission.
     */
    QString drone() const { return _drone; }

    /*!
     * \brief setUtilisateur
     * \param util Utilisateur
     * Mutateur pour l'utilisateur qui a lancé une mission.
     */
    void setUtilisateur( QString util ) { _util = util; }

    /*!
     * \brief utilisateur
     * \return Utilisateur qui a lancé une mission
     * Accesseur à l'utilisateur qui a lancé une mission.
     */
    QString utilisateur() const { return _util; }
};

/*!
 * \brief The ListeMissions class
 * La classe ListeMission est une liste composée de missions enregistrées.
 */
class ListeMissions
{
private:
	QList<Mission> _liste;

public:
	// Destructeur
    /*!
     *\brief Destructeur
     * Destructeur de la liste des missions.
     */
    ~ListeMissions() {}

    /*!
     * \brief ListeMissions
     * Constructeur de la liste des missions à partir du fichier MISSIONS.txt
     */
	ListeMissions()
	{
		importer();
    }

    /// Getters & Setters

    /*!
     * \brief liste
     * \return Retourne la liste des missions
     * Accesseur de la liste des missions.
     */
	QList<Mission> liste()
	{
		return _liste;
	}
	// Méthodes
    /*!
     * \brief existeDeja
     * \param id Identifiant de la mission
     * \return
     * Méthode qui permet de vérifier si une mission existe déjà
     * à partir de son identifiant.
     */
	int existeDeja( QString id )
	{
		for ( int it = 0; it < liste().size(); it++ )
		{
			if ( liste().at(it).id() == id )
			{
				return it;
			}
		}
		return -1;
	}


public slots:
    /*!
     * \brief importer
     * Méthode qui permet d'importer la liste des missions
     * à partir du fichier MISSIONS.txt
     */
	void importer()
	{
		_liste.clear();
		QFile file( MissionOpts::FILEPATH );
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			QMessageBox::warning(0, "Error", file.errorString());
			return;
		}
		QTextStream stream( &file );
		while ( !stream.atEnd() )
		{
			QString line = stream.readLine();
			if ( line != "" )
			{
				QStringList details = line.split( ";" );
				if ( details.size() >= 9 )
				{
					Mission tmpMis;
					tmpMis.setID( details.at(0) );
					tmpMis.setTime( QTime::fromString( details.at(1), "HH:mm" ) );
					tmpMis.setDate( QDate::fromString( details.at(2), "dd/MM/yyyy" )  );
					tmpMis.setDrone( details.at(3) );
					tmpMis.setUtilisateur( details.at(4) );
					tmpMis.setDepart( details.at(5) );
					tmpMis.setCible( details.at(6) );
					tmpMis.setCheckpoints( details.at(7) );
					tmpMis.setCharge( details.at(8).toDouble() );
					_liste.append( tmpMis );
				}
			}
		}
		file.close();
    }

    /*!
     * \brief sauvegarder
     * Méthode qui permet de sauvegarder la liste des missions
     * dans le fichier MISSIONS.txt.
     */
	void sauvegarder()
	{
		QFile file( MissionOpts::FILEPATH );
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			QMessageBox::warning(0, "Error", file.errorString());
			return;
		}
		QTextStream out( &file );
		for ( auto elm : liste() )
		{
			out << elm.id() << ";"  <<
				   elm.time().toString("HH:mm") << ";"  <<
				   elm.date().toString("dd/MM/yyyy") << ";"  <<
				   elm.drone() << ";"  <<
				   elm.utilisateur() << ";"  <<
				   elm.depart().toString() << ";"  <<
				   elm.cible().toString() << ";";
				   for ( int it = 0; it < elm.checkpoints().size(); ++it )
				   {
					   out << elm.checkpoints().at(it).toString();
					   if ( it+1 != elm.checkpoints().size() ) out << "+";
				   }
				   out << ";" << QString::number( elm.charge() ) << "\n";
		}
		file.close();
    }

    /*!
     * \brief sauvegarder
     * \param mis Mission
     * Méthode qui permet de sauvegarder une mission dans la
     * liste des missions.
     */
	void sauvegarder( Mission mis )
	{
		int ind = existeDeja( mis.id() ) ;
		if ( ind == -1 )
		{
			_liste.append( mis );
		}
		else
		{
			_liste.replace( ind, mis );
		}
		sauvegarder();
    }

    /*!
     * \brief supprimer
     * \param mis Mission
     * Méthode qui permet de supprimer une mission
     * de la liste des mission.
     */
	void supprimer( Mission mis )
	{
		int ind = existeDeja( mis.id() ) ;
		if ( ind != -1 )
			_liste.removeAt( ind );
		sauvegarder();
    }
};
// **********************************
// **********************************
#endif // MISSION_H
// **********************************
// **********************************
