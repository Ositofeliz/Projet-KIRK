#ifndef TUILE_H
#define TUILE_H
// **********************************
#include <QApplication>
#include <QObject>
#include <QGraphicsPixmapItem>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
// **********************************
// **********************************
//
//
//
// **********************************
class Tuile : public QObject, public QGraphicsPixmapItem
{
	Q_OBJECT
	// ----------------
	// ----------------
private:
	//	char _z;
	//	int _x;
	//	int _y;
	// ----------------
	// ----------------
public:
	// Destructor
    /*! \brief Destructeur
     *
     * Le destructeur de la classe Tuile
     */
	~Tuile(){}

	// Constructeurs
    /*!
     * \brief Tuile Constructeur
     * \param z
     * \param x
     * \param y
     * \param parent
     *
     * Constructeur de la classe Tuile
     */
	Tuile( char z, int x, int y, QGraphicsItem *parent = nullptr ) : QGraphicsPixmapItem( parent )
	{
		setPixmap( getTuile( z, x, y ) );
    }
	// ----------------
	// ----------------
	// Méthodes Publiques

    /*!
     * \brief getTuile
     * \param z
     * \param x
     * \param y
     * \return
     *
     * Accesseur de la classe tuile
     */
	QPixmap getTuile( char z, int x, int y )
	{
		QPixmap pixmap;
		QString path = "CacheTuiles";
		QDir dir;
		if ( !dir.exists( path ) ) dir.mkpath( path );
		QByteArray data;
		QString filePath = QString::number( z ) + "." + QString::number( x ) + "." + QString::number( y );

		if ( !pixmap.load( path+"/"+filePath+".png" ) )
		{
			QFile file( path+"/"+filePath+".png" );
			file.open( QIODevice::WriteOnly );
			QNetworkRequest mapReq = QNetworkRequest( QUrl( "https://a.tile.openstreetmap.fr/osmfr/"+filePath.replace('.', '/')+".png" ) );
			QNetworkAccessManager mapMan;
			QNetworkReply *mapRep = mapMan.get( mapReq );
			while ( !mapRep->isFinished() )
			{
				qApp->processEvents();
			}
			mapRep->deleteLater();
			data = mapRep->readAll();
			file.write( data );
			file.close();
			pixmap.loadFromData(data);
		}
		return pixmap;
    }

};
// **********************************
// **********************************
#endif // TUILE_H
