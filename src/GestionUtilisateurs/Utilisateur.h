// **********************************
// **********************************
// Fichier contenant la classe utilisatuer
// et la classe ListeUtilisateur
// **********************************
// **********************************
#ifndef UTILISATEUR_H
#define UTILISATEUR_H
// **********************************
#include <QString>
#include <QList>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <iostream>
using namespace std;
// **********************************
// **********************************
// Options des utilisateurs
/*!
 *\namespace UtilOpts
 * Espace de nommage des options utilisateurs.
 */
namespace UtilOpts
{
	static constexpr char UNKNOWN = -2;
	static constexpr char UNAUTHORIZED = -1;
	static constexpr char ADMIN = 0;
	static constexpr char USER = 1;
	static const QString FILEPATH = "UTILISATEURS.txt";
};
// **********************************
// **********************************
/*!
 * \brief The Utilisateur class
 * La classe Utilisateur définit les utilisateurs selon
 * un identifiant, un mot de passe et un statut.
 * Le statut leur confère des droits d'accès à des fonctions
 * spécifiques.
 */
class Utilisateur
{
private:
    QString _id; /*!< Identifiant de l'utilisateur */
    mutable QString _mdp; /*!< Mot de passe de l'utilisateur */
    mutable char _statut = UtilOpts::USER; /*!< Statut de l'utilisateur */
	// ----------------
	// ----------------
public:
	// Destructeur
    /*!
     *\brief Destructeur
     * Destructeur de la classe utilisateur.
     */
    ~Utilisateur() {}

	// Constructeurs
    /*!
     * \brief Utilisateur
     * \param id Identifiant de l'utilisateur
     * \param mdp Mot de passe de l'utilisateur
     * \param statut Statut de l'utilisateur
     * Constructeur de la classe utilisateur.
     */
	Utilisateur( QString id, QString mdp, char statut = UtilOpts::USER )
	{
		setID( id );
		setMDP( mdp );
		setStatut( statut );
    }

    /// Utlisateur : Setters & Getters

    /*!
     * \brief setID
     * \param id Identifiant de l'utilisateur
     * Mutateur de l'identifiant utilisateur.
     */
    void setID( QString id ) { _id = id; }

    /*!
     * \brief id
     * \return Retourne l'identifiant de l'utilisateur
     * Accesseur de l'identifiant utilisateur.
     */
    QString id() const { return _id; }

    /*!
     * \brief setMDP
     * \param mdp Mot de passe
     * Mutateur du mot de passe utilisateur.
     */
    void setMDP( QString mdp ) const { _mdp = mdp; }

    /*!
     * \brief mdp
     * \return Retourne le mot de passe utilisateur.
     */
    QString mdp() const { return _mdp; }

    /*!
     * \brief setStatut
     * \param statut Statut utilisateur
     * Mutateur du statut utilisateur.
     */
    void setStatut( char statut ) const { _statut = statut; }

    /*!
     * \brief statut
     * \return Retourne le statut utilisateur
     * Accesseur du statut utilisateur.
     */
    char statut() const { return _statut; }
};


/*!
 * \brief The ListeUtilisateur class
 * La classe ListeUtilisateur est une liste des Utilisateurs.
 */
class ListeUtilisateur
{
private:
    QList<Utilisateur> _utilisateurs;  /*!< Liste des utilisateurs */

public:

    /*!
     *\brief Destructeur
     * Destructeur de la classe ListeUtilisateur
     */
    ~ListeUtilisateur(){}

    // Constructeurs
    /*!
     * \brief ListeUtilisateur
     * Constructeur de la classe ListeUtilisateur à partir
     * du fichier UTILISATEURS.txt.
     */
    ListeUtilisateur()
	{
		importer();
    }


    /// ListeUtilisateur : Getters & Setters

    /*!
     * \brief liste
     * \return Retourne la liste des utilisateurs
     */
	QList<Utilisateur> liste()
	{
		return _utilisateurs;
	}
    /// Méthodes ListeUtilisateur

    /*!
     * \brief existeDeja
     * \param id Identifiant utilisateur
     * \return
     * Méthode qui permet de savoir si un utilisateur existe
     * dans la liste avec son identifiant.
     */
	int existeDeja( QString id )
	{
		for ( int it = 0; it < _utilisateurs.size(); it++ )
		{
			if ( _utilisateurs.at(it).id() == id )
			{
				return it;
			}
		}
		return -1;
	}

    /*!
     * \brief autorise
     * \param id Identifiant
     * \param mdp Mot de passe
     * \return
     * Méthode qui permet de donner les droits d'accès
     * au différentes fonctions à un utilisateur en
     * fonction de son statut.
     */
	char autorise( QString id, QString mdp )
	{
		for ( int it = 0; it < _utilisateurs.size(); it++ )
		{
			if ( _utilisateurs.at(it).id() == id )
			{
				if ( _utilisateurs.at(it).mdp() == mdp  )
				{
					return _utilisateurs.at(it).statut();
				}
				else
				{
					return UtilOpts::UNAUTHORIZED;
				}
			}
		}
		return UtilOpts::UNKNOWN;
	}


	char autorisation( int index, QString mdp )
	{
		if ( _utilisateurs.at(index).mdp() == mdp  )
		{
			return _utilisateurs.at(index).statut();
		}
		return UtilOpts::UNAUTHORIZED;
	}
	int verification( QString id )
	{
		for ( int it = 0; it < _utilisateurs.size(); it++ )
		{
			if ( _utilisateurs.at(it).id() == id  )
			{
				return it;
			}
		}
		return -1;
	}
	// ----------------
	// ----------------
public slots:
	// Importation de la liste des utilisateurs
    /*!
     * \brief importer
     * Méthode qui permet d'importer la liste des utilsateurs
     * à partir du fichier UTILISATEURS.txt.
     */
	void importer()
	{
		_utilisateurs.clear();
		QFile file( UtilOpts::FILEPATH );
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
				if ( details.size() == 3 )
				{
					char statut = UtilOpts::USER;
					if ( details.at( 2 ).contains("ADMIN") ) statut = UtilOpts::ADMIN;
					_utilisateurs.append( Utilisateur( details.at(0), details.at(1), statut ) );
				}
			}
		}
		file.close();
    }



    /*!
     * \brief sauvegarder
     * Méthode qui permet de sauvegarder la liste des utlisateurs dans le fichier
     * UTILISATEURS.txt.
     */
	void sauvegarder()
	{
		QFile file( UtilOpts::FILEPATH );
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			QMessageBox::warning(0, "Error", file.errorString());
			return;
		}
		QTextStream out( &file );
		for ( auto elm : _utilisateurs )
		{
			out << elm.id() << ";" << elm.mdp() << ";";
			if ( elm.statut() == UtilOpts::USER ) out << "USER";
			else if ( elm.statut() == UtilOpts::ADMIN ) out << "ADMIN";
			out << "\n";
		}
		file.close();
    }


    /*!
     * \brief ajouter
     * \param util Utilisateur
     * Méthode qui permet d'ajouter un utilisateur à la liste des utilisateurs.
     */
	void ajouter( Utilisateur util )
	{
		_utilisateurs.append( util );
    }

    /*!
     * \brief modifier
     * \param mdp Mot de passe
     * \param statut Statut
     * \param index
     * Méthode qui permet de modifier les paramètres d'un utilisateur.
     */
	void modifier( QString mdp, char statut, int index)
	{
		_utilisateurs.at(index).setMDP( mdp );
		_utilisateurs.at(index).setStatut( statut );
    }

    /*!
     * \brief modifier
     * \param id Identifiant
     * \param mdp Mot de passe
     * Méthode qui permet de modifier l'identifiant et le mot de passe
     * d'un utilisateur.
     */
	void modifier( QString id, QString mdp )
	{
		for ( int it = 0; it < _utilisateurs.size(); it++ )
		{
			if ( _utilisateurs.at(it).id() == id  )
			{
				_utilisateurs.at(it).setMDP( mdp );
				return;
			}
		}
    }
	// ----------------
	// Suppression d'un utilisateur
    /*!
     * \brief supprimer
     * \param index
     * Méthode qui permet de supprimer un utilisateur de la liste.
     */
	void supprimer( int index )
	{
		if ( index >= 0 && index < _utilisateurs.size() )
			_utilisateurs.removeAt( index );
    }
};
// **********************************
// **********************************
#endif // UTILISATEUR_H
// **********************************
// **********************************
