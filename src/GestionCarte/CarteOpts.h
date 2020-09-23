#ifndef CARTEOPTS_H
#define CARTEOPTS_H
// *********************************************
// *********************************************
#include <QPoint>
#include <QString>
// *********************************************
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;
// *********************************************
// *********************************************

/*!
 *\namespace CarteMaths
 * Espace de nommage regroupant les formules utilisés pour le calcul dans les cartes.
 */
namespace CarteMaths
{
    static const double PI = 3.141592653589793;  /*!< Valeur de Pi */

    /*!
     * \brief toRad
     * \param deg
     * \return
     * Conversion degrés en radians.
     */
	static double toRad( double deg )
	{
		return ( PI * deg ) / 180;
	};

    /*!
     * \brief toDeg
     * \param rad
     * \return
     * Conversion radians en degrés.
     */
	static double toDeg( double rad )
	{
		return ( 180 * rad ) / PI;
	};

	static double gd( double y )
	{
		return ( 180 / PI ) * ( 2 * atan( exp(y) ) - PI / 2 );
	};

	static double agd( double lat )
	{
		return 0.5*log( ( 1 + sin( toRad( lat ) ) )/( 1 - sin( toRad( lat ) ) ) );
	};
};
using namespace CarteMaths;


/*!
 * \brief The PosCarte class
 *
 * \ingroup GestionCarte
 *
 * La classe PosCarte permet de définir la position dans la carte
 * utilisée dans l'algorithme A*
 */
class PosCarte
{
private:
    int _x; /*!<  Coordonnées au point x */
    int _y; /*!<  Coordonnées au point y */

public:

    /*!
     * \brief PosCarte
     *
     * Constructeur par défaut
     */
    PosCarte() {}

    /*!
     * \brief PosCarte
     * \param x Position X
     * \param y Position Y
     *
     * Constructeur avec les points de coordonnées X et Y.
     */
    PosCarte( int x, int y )
	{
		setX( x ); setY( y );
    }

    /*!
     * \brief PosCarte
     * \param pos Référence à la position
     *
     * Constructeur avec des QPoints
     */
    PosCarte( const QPoint &pos )
	{
		setX( pos.x() ); setY( pos.y() );
    }

    /*!
     * \brief setX Mutateur de X
     * \param x Position au point X
     */
    void setX( int x ) { _x = x; }
    /*!
     * \brief x Accesseur de X
     * \return Retourne la valeur de X
     */
    int x() const { return _x; }

    /*!
     * \brief setY Mutateur de Y
     * \param y Position au point Y
     */
    void setY( int y ) { _y = y; }
    /*!
     * \brief y Accesseur de Y
     * \return Retourne la valeur de Y
     */
    int y() const { return _y; }


	PosCarte& operator *= ( int it )
	{
		_x *= it;
		_y *= it;
		return *this;
    }

	PosCarte& operator /= ( int it )
	{
		_x /= it;
		_y /= it;
		return *this;
    }

	void move( int dx, int dy )
	{
		_x += dx;
		_y += dy;
    }

	void move( QPoint delta )
	{
		_x += delta.x();
		_y += delta.y();
    }
};


/*!
 * \brief The Coordonnees class
 *
 * \ingroup GestionCarte
 *
 * La classe Coordonnées définit les coordonnées réelles (latitude et longitude)
 */
class Coordonnees
{
	private:
    double _lon; /*!< Longitude */
    double _lat; /*!< Latitude */
public:
    /*!
     * \brief Coordonnees
     * \param lat Latitude
     * \param lon Longitude
     *
     * Constructeur de la classe coordonnées.
     */
	Coordonnees( double lat, double lon )
	{
		 setLat( lat ); setLon( lon );
    }

    /*!
     * \brief setLon Mutateur de la Longitude
     * \param lon Longitude
     */
    void setLon( double lon ) { _lon = lon; }
    /*!
     * \brief lon Accesseur de la Longitude
     * \return Retourne la longitude
     */
    double lon() const { return _lon; }

    /*!
     * \brief setLat Mutateur de la Latitude
     * \param lat Latitude
     */
    void setLat( double lat ) { _lat = lat; }
    /*!
     * \brief lat Accesseur de la Latitude
     * \return Retourne la Latitude
     */
    double lat() const { return _lat; }

    ///Opérateurs

    /*!
     * \brief operator ==
     * \param co
     * \return
     */
	bool operator == ( Coordonnees &co )
	{
		if ( this->_lat  == co.lat() && this->_lon  == co.lon() )
			return true;
		return false;
    }

    /*!
     * \brief operator !=
     * \param co
     * \return
     */
	bool operator != ( Coordonnees &co )
	{
		if ( this->_lat  != co.lat() && this->_lon  != co.lon() )
			return true;
		return false;
    }

    /*!
     * \brief toString
     * \return
     */
	QString toString() const
	{
		return QString::number(lat())+'/'+QString::number(lon()) ;
	}
};


/*!
 * \namespace CarteOpts
 *
 * Espace de nommage des options de la carte utilisées dans l'algorithme A*
 */
namespace CarteOpts
{
	// Options des cases
	// Accès interdit
	static constexpr char INTERDIT = '1';
	// Accès autorisé
	static constexpr char AUTORISE = '0';
	//  Chemin
	static constexpr char CHEMIN = '*';
}

// *********************************************
// *********************************************
/*!
 * \brief The Carte class
 *
 * La classe Carte utilisée dans l'algorithme A*
 */
class Carte
{
private:
	vector<char> _donnee;
	// Taille de la carte
    int _w; /*!< Largeur de la carte*/
    int _h; /*!< Hauteur de la carte*/

	// Position correspondante à la taille de la carte
	// 0/0 en coord Carte correspond à _lon1/_lat1 en coord réelle
	// _w/_h en coord Carte correspond à _lon2/_lat2 en coord réelle
	double _lon1 = -180;
	double _lon2 = 180;
	double _lat1 = 85;
	double _lat2 = -85;

public:
	// Destructeur
    /*!
     *\brief Destructeur
     *
     * Destructeur de la classe Carte
     */
    ~Carte(){}

    /*!
     * \brief Carte
     * \param w Largeur
     * \param h Hauteur
     *
     * Constructeur de la classe Carte
     */
    Carte( int w, int h )
	{
		maj( w, h );
    }

    /// Accesseurs & Mutateurs
    /*!
     * \brief w Accesseur de la Largeur
     * \return Retourne la largeur
     */
    int w() { return _w; }
    /*!
     * \brief setW Mutateur de la Largeur
     * \param w Largeur
     */
    void setW( int w ) { _w = w; }

    /*!
     * \brief h Accesseur de la hauteur
     * \return Retourne la hauteur
     */
    int h() { return _h; }
    /*!
     * \brief setH Mutateur de la hauteur
     * \param h Hauteur
     */
    void setH( int h ) { _h = h; }

    /*!
     * \brief setTaille Définit la taille de la carte
     * \param w Largeur
     * \param h Hauteur
     */
	void setTaille( int w, int h )
	{
		setW( w );
		setH( h );
    }

    double lon1() { return _lon1; }
    void setLon1( double lon ) { _lon1 = lon; }

    double lon2() { return _lon2; }
    void setLon2( double lon ) { _lon2 = lon; }

    double lat1() { return _lat1; }
    void setLat1( double lat ) { _lat1 = lat; }

    double lat2() { return _lat2; }
    void setLat2( double lat ) { _lat2 = lat; }

	void setCoordonnes( double lon1, double lon2, double lat1, double lat2 )
	{
		setLon1( lon1 ); setLon2( lon2 );
		setLat1( lat1 ); setLat2( lat2 );
    }

    /// Méthodes
    /*!
     * \brief maj Mise à jour de la carte
     * \param w Largeur
     * \param h Hauteur
     *
     * Méthode qui permet la mise à jour de la carte.
     */
	void maj( int w, int h )
	{
		setTaille( w, h );
		_donnee.clear();
		for ( int it = 0; it < this->w()*this->h(); it++ )
			_donnee.push_back( CarteOpts::AUTORISE );
    }

	// Change la donnée de la carte à la position it
    /*!
     * \brief setDonne Change les données de la carte à la position it
     * \param val
     * \param it
     */
	void setDonne( char val, int it )
	{
		if ( it >= 0 && it < (int)_donnee.size() )
			_donnee.at( it ) = val;
    }

	// Change la donnée de la carte à la coord x/y
    /*!
     * \brief setDonne
     * \param val
     * \param x
     * \param y
     *
     * Change les données de la carte aux coordonnées x et y
     */
	void setDonne( char val, int x, int y )
	{
		if ( ( y * this->w() ) + x >= 0 &&
			( y * this->w() ) + x  < (int)_donnee.size() )
			_donnee.at( ( y * this->w() ) + x ) = val;
    }

    /*!
     * \brief getDonnee
     * \param it
     * \return
     *
     * Récupère les données de la carte à la position it
     */
	char getDonnee( int it )
	{
		if ( it >= 0 && it < (int)_donnee.size() )
			return _donnee.at( it );
		return -1;
    }

	// Récupère la donnée de la carte à la coord x/y
    /*!
     * \brief getDonnee
     * \param x
     * \param y
     * \return
     *
     * Récup_re les données de la carte aux coordonnées x et y
     */
	char getDonnee( int x, int y )
	{
		if ( ( y * this->w() ) + x >= 0 &&
			( y * this->w() ) + x  < (int)_donnee.size() )
			return _donnee.at( ( y * this->w() ) + x );
		return -1;
    }

	// Conversion PosCarte vers Coordonnees
    /*!
     * \brief CarteVersCoord Conversion PosCarte en Coordonnees
     * \param pc
     * \return
     *
     * Méthode de conversion PosCarte en Coordonnées
     */
	Coordonnees CarteVersCoord( const PosCarte &pc )
	{
		double aLon = ( lon2() - lon1() ) / w();
		double bLon = lon1();
		double aLat = ( lat2() - lat1() ) / h();
		double bLat = lat1();
		return Coordonnees( aLat*pc.y() + bLat, aLon*pc.x() + bLon );
    }

	// Conversion Coordonnees vers PosCarte
    /*!
     * \brief CoordVersCarte Conversion Coordonnees en PosCarte
     * \param co
     * \return
     *
     * Méthode de conversion Coordonnees en PosCarte
     */
	PosCarte CoordVersCarte( const Coordonnees &co )
	{
		double ax = w() / ( lon2() - lon1() );
		double bx = -ax * lon1();
		double ay = h() / ( lat2() - lat1() );
		double by = -ay * lat1();
		return PosCarte( ax*co.lon() + bx, ay*co.lat() + by );
    }

    /*!
     * \brief getCoordReel
     * \param pc
     * \return
     *
     * Méthode d'obtention des coordonnées réelles.
     */
	Coordonnees getCoordReel( const PosCarte &pc )
	{
		double n = w() / ( lon2() - lon1() );
		double lo = lon1() - 0/n;
		double a = h() / ( gd(lat2()) - gd(lat1()) );
		double b = 0 - a*gd(lat1());
		return Coordonnees( gd( (pc.y()-b)/a), pc.x()/n + lo );
    }

    /*!
     * \brief getCoordVirt
     * \param co
     * \return
     *
     * Méthode d'obtention des coordonées virtuelles.
     */
	PosCarte getCoordVirt( const Coordonnees &co )
	{
		double n = w() / ( lon2() - lon1() );
		double lo = lon1() - 0/n;
		double a = h() / ( agd(lat2()) - agd(lat1()) );
		double b = 0 - a*agd(lat1());
		return PosCarte( n*(co.lon() + lo), a*agd(co.lat()) + b );
    }

};
// *********************************************
// *********************************************

/*!
 *\namespace CarteMaths
 *
 * Espace de Nommage des opérations sur la carte
 */
namespace CarteMaths
{
	static const double RTERRE = 6371;

	static double distanceAngulaire( double phi1, double lam1, double phi2, double lam2 )
	{
		double dphi = phi2 - phi1;
		double dlam = lam2 - lam1;
		double dpsi = log( tan( 0.5*phi2 + 0.25*PI ) /  tan( 0.5*phi1 + 0.25*PI ) );
		double q = abs( dpsi ) > pow( 10, -12 ) ? dphi / dpsi : cos ( phi1 );
		return sqrt( dphi*dphi + q*q * dlam*dlam );
	};

	static double distanceAngulaire( const Coordonnees &co1, const Coordonnees &co2 )
	{
		double phi1 = toRad( co1.lat() );
		double phi2 = toRad( co2.lat() );
		double dphi = phi2 - phi1;
		double lam1 = toRad( co1.lon() );
		double lam2 = toRad( co2.lon() );
		double dlam = lam2 - lam1;
		double dpsi = log( tan( 0.5*phi2 + 0.25*PI ) /  tan( 0.5*phi1 + 0.25*PI ) );
		double q = abs( dpsi ) > pow( 10, -12 ) ? dphi / dpsi : cos ( phi1 );
		return sqrt( dphi*dphi + q*q * dlam*dlam );
	};

	static double distance( double phi1, double lam1, double phi2, double lam2 )
	{
		double a = pow( sin( 0.5*(phi2-phi1) ), 2 ) + cos(phi1) * cos(phi2) * pow( sin( 0.5*(lam2-lam1) ), 2 );
		return 2*RTERRE*atan2( sqrt( a ), sqrt( 1-a ) );// * cos(phi1);
	};

	static double distance( const Coordonnees &co1, const Coordonnees &co2 )
	{
		double lon1 = co1.lon(), lon2 = co2.lon();
		if ( abs( lon1 - lon2 ) > 180 )
		{
			lon1 < 0 ? lon1 += 360 : lon2 += 360;
		}
		double phi1 = toRad( co1.lat() );
		double phi2 = toRad( co2.lat() );
		double lam1 = toRad( lon1 );
		double lam2 = toRad( lon2 );
		return distance( phi1, lam1, phi2, lam2 );
	};

	static Coordonnees coordInterm( const Coordonnees &co1, const Coordonnees &co2, double f )
	{
		double lon1 = co1.lon(), lon2 = co2.lon();
		if ( abs( lon1 - lon2 ) > 180 )
		{
			lon1 < 0 ? lon1 += 360 : lon2 += 360;
		}
		double phi1 = toRad( co1.lat() );
		double phi2 = toRad( co2.lat() );
		double lam1 = toRad( lon1 );
		double lam2 = toRad( lon2 );
		double d = distanceAngulaire( phi1, lam1, phi2, lam2 );
		double a = sin( (1-f)*d ) / sin( d );
		double b = sin( f*d ) / sin( d );
		double x = a * cos( phi1 ) * cos( lam1 ) + b * cos( phi2 ) * cos( lam2 );
		double y = a * cos( phi1 ) * sin( lam1 ) + b * cos( phi2 ) * sin( lam2 );
		double z = a * sin( phi1 ) + b * sin( phi2 );
		double lat = toDeg( atan2( z, sqrt( x*x + y*y ) ) );
		double lon = toDeg( atan2( y, x ) );
		return Coordonnees( lat, lon );
	};
};

#endif // CARTEOPTS_H
