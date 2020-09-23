#ifndef DRONE_H
#define DRONE_H

#include <QString>
#include <QList>
#include <QFile>
#include <QDebug>
#include <QtWidgets/QMessageBox>
#include <QTextStream>

/*!
 *\namespace UtilOpts
 * Espace de nommage du fichier des drones
 */
namespace UtilOpts {
    static const QString DFILEPATH="DRONES.txt"; /*!< Chemin vers le fichier des drones */
}

/*!
 * \brief The TypeDrone enum
 * Énumération des types de drones
 */
enum class TypeDrone : char{hale='h',male='m'};

/*!
 * \brief The Drone class
 * La classe drone définit les drones selon les paramètres suivants :
 *  Identifiant
 *  Type
 *  Vitesse Maximale
 *  Charge Maximale
 *  Poids du drone
 *  Autonomie
 */
class Drone
{
private: //TODO ajouter un attribut pour la disponibilité du drone
    int id; /*!< Identifiant du drone */
    TypeDrone type;  /*!< Type de drone */
    double vitesseMax;  /*!< Vitesse maximale du drone (km/h)*/
    double chargeMax;  /*!< Charge maximale pouvant être portée par le drone (kg)*/
    double poids;  /*!< Poids du drone (kg)*/
    double autonomie;  /*!< Autonomie du drone (km) */
    int altitudeMax; /*!< Altitude de vol maximale du drone (m) */
    int altitudeMin; /*!< Altitude de vol minimale du drone (m) */
    QString nom; /*!< Nom du drone */

public:
    /*!
     * \brief Drone
     * Constructeur par défaut.
     */
    Drone();

    /*!
     * \brief Drone
     * Constructeur avec identifiant
     */
    Drone(int);

    /*!
     * \brief Drone
     * Constructeur avec type de drone, nom, altitude et vitesse max.
     */
    Drone(TypeDrone, QString="", int=0, int=0);

    /*!
     * \brief Drone
     * Constructeur avec toutes les options.
     */
    Drone(int,TypeDrone,QString,double, double, double,double,int,int);
    //~Drone();

    /// DRONE : GETTERS & SETTERS

    /*!
     * \brief getTypeQS
     * \return
     */
    QString getTypeQS();

    /*!
     * \brief getType
     * \return Retourne le type de drone
     *
     * Accesseur du type de drone.
     */
    TypeDrone getType() const;

    /*!
     * \brief setTypeQS
     */
    void setTypeQS(QString);

    /*!
     * \brief setType
     * \param value
     * Mutateur du type de drone.
     */
    void setType(const TypeDrone &value);

    /*!
     * \brief getNom
     * \return Retourne le nom du drone
     * Accesseur du nom du drone.
     */

    QString getNom() const;
    /*!
     * \brief setNom
     * \param value
     * Mutateur du nom du drone.
     */
    void setNom(const QString &value);

    /*!
     * \brief getVitesseMax
     * \return Retourne la vitesse maximale du drone
     * Accesseur de la vitesse maximale du drone.
     */
    double getVitesseMax() const;

    /*!
     * \brief setVitesseMax
     * \param value
     * Mutateur de la vitesse maximale du drone.
     */
    void setVitesseMax(double value);

    /*!
     * \brief getChargeMax
     * \return Retourne la charge maximale du drone
     * Accesseur de la charge maximale du drone.
     */
    double getChargeMax() const;

    /*!
     * \brief setChargeMax
     * \param value
     * Mutateur de la charge maximale du drone.
     */
    void setChargeMax(double value);

    /*!
     * \brief getPoids
     * \return Retourne le poids du drone
     * Accesseur du poids du drone.
     */
    double getPoids() const;

    /*!
     * \brief setPoids
     * \param value
     * Mutateur du poids du drone.
     */
    void setPoids(double value);

    /*!
     * \brief getAutonomie
     * \return Retourne l'autonomie du drone
     * Accesseur de l'autonomie du drone.
     */
    double getAutonomie() const;

    /*!
     * \brief setAutonomie
     * \param value
     * Mutateur de l'autonomie du drone
     */
    void setAutonomie(double value);

    /*!
     * \brief getAltitudeMax
     * \return Retourne l'altitude maximale du drone
     * Accesseur de l'altitude maximale du drone.
     */
    int getAltitudeMax() const;

    /*!
     * \brief setAltitudeMax
     * \param value
     * Mutateur de l'altitude maximale du drone.
     */
    void setAltitudeMax(int value);

    /*!
     * \brief getAltitudeMin
     * \return Retourne l'altitude minimale du drone
     * Accesseur de l'altitude minimale du drone.
     */
    int getAltitudeMin() const;

    /*!
     * \brief setAltitudeMin
     * \param value
     * Mutateur de l'altitude minimale du drone.
     */
    void setAltitudeMin(int value);

    /*!
     * \brief getId
     * \return Retourne l'identifiant du drone
     * Accesseur de l'identifiant du drone.
     */
    int getId();

    /*!
     * \brief setId
     * \param value
     * Mutateur de l'identifiant du drone.
     */
    void setId(int value);
};

/*!
 * \brief The ListDrone class
 * La classe ListDrone est une liste composée de drones.
 */
class ListDrone{
private:
    QList<Drone> _drones;  /*!< Liste de drones */

public:

    /*!
     * \brief ListDrone
     * Constructeur de la liste des drones à partir du fichier DRONES.txt
     */
    ListDrone(){
        importer();
    }

    ListDrone(QString){}

    /*!
      * \brief Destructeur
      * Destructeur de la classe ListDrone.
      */
    ~ListDrone(){}

    /*!
     * \brief liste
     * \return Retourne les drones de la liste
     * Accesseur de la liste des drones.
     */
    QList<Drone> liste()
        {
            return _drones;
        }

public slots:

    /*!
     * \brief importer
     * Méthode qui permet d'importer la liste des drones du fichier
     * DRONES.txt
     */
    void importer()
    {
        _drones.clear();
        QFile file( UtilOpts::DFILEPATH );
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox::warning(nullptr, "Error", file.errorString());
            return;
        }
        QTextStream stream( &file );
        while ( !stream.atEnd() )
        {
            QString line = stream.readLine();
            if ( line != "" )
            {
                QStringList details = line.split( ";" );
                if ( details.size() > 7 )
                {
                    TypeDrone type;
                    if(details.at(1)=="hale"){
                        type = TypeDrone::hale;
                    }else{
                        type = TypeDrone::male;

                    }
                    Drone d(details.at(0).toInt(), type, details.at(2), details.at(3).toDouble(),
                            details.at(4).toDouble(), details.at(5).toDouble(), details.at(6).toDouble(), details.at(7).toInt(), details.at(8).toInt());
                    _drones.append(d);
                }
            }
        }
        file.close();
    }

    /*!
     * \brief sauvegarder
     * Méthode qui permet de sauvegarder la liste des drones dans le
     * fichier DRONES.txt
     */
    void sauvegarder()
    {
        QFile file( UtilOpts::DFILEPATH );
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMessageBox::warning(nullptr, "Error", file.errorString());
            return;
        }
        QTextStream out( &file );
        for ( auto elm : _drones )
        {
            out << elm.getId() << ";" << elm.getTypeQS() << ";" << elm.getNom()<< ";" << elm.getVitesseMax()<< ";"<<
                   elm.getChargeMax()<< ";" <<elm.getPoids()<< ";" <<elm.getAutonomie()<< ";" <<
                   elm.getAltitudeMax()<< ";" <<elm.getAltitudeMin();
            out << "\n";
        }
        file.close();
    }

    /*!
     * \brief ajouter
     * \param d Drone
     * Méthode qui permet d'ajouter un drone à la liste.
     */
    void ajouter( Drone d )
    {
        _drones.append( d );
    }
    // ----------------
	//Modification d'un drone

    /*!
     * \brief modifier
     * \param d Drone
     * \param i Index
     * Méthode qui permet de modifier le drone d à l'index i.
     */
	void modifier(Drone d, int i)
	{
		_drones.replace(i,d);
	}

    /*!
     * \brief modifier
     * \param d Drone
     * \param name Nom du drone
     * Méthode qui permet de modifier un drone à partir de son nom.
     */
	void modifier(Drone d, QString name)
	{
		for ( int it = 0; it < _drones.size(); ++it )
		{
			if ( _drones[it].getNom() == name )
			{
				_drones[it] = d;
				return;
			}
		}
	}

    /*!
     * \brief supprimer
     * \param index
     * Méthode qui permet de supprimer un drone situé à
     * l'index indiqué.
     */
    void supprimer( int index )
    {
        if ( index >= 0 && index < _drones.size() )
            _drones.removeAt( index );
    }

    /*!
     * \brief supprimer
     * \param d Drone
     * Méthode qui permet de supprimer un drone à partir de son
     * identifiant.
     */
    void supprimer( Drone d )
    {
        if ( d.getId() >= 0 && d.getId() < _drones.size() )
            _drones.removeAt( d.getId() );
    }

    /*!
     * \brief droneByName
     * \param nom
     * \return Retourne un drone.
     * Méthode qui permet de retrouver un drone dans la liste à partir
     * de son nom.
     */
	Drone droneByName( QString nom )
	{
		for ( auto elm : liste() )
		{
			if ( elm.getNom() == nom )
				return elm;
		}
		return Drone();
    }
};




#endif // DRONE_H
