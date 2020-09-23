// **********************************
// **********************************
// ObstacleItem permet d'afficher
// les obstacles sur la map
// **********************************
// **********************************
#ifndef OBSTACLEITEMS_H
#define OBSTACLEITEMS_H

#include <QGraphicsPathItem>
#include <QPen>
/*!
 *\namespace OBST
 * Espace de nommage des options des obstacles
 */
namespace OBST
{
	static const char RECTANGLE = 0;
	static const char ELLIPSE = 1;
	static const char INCONNU = -1;
}

/*!
 * \brief The ObstacleItem class
 * \ingroup GestionCarte
 * \ingroup GestionObstacle
 *
 * La classe ObstacleItem permet d'afficher les obstacles sur la carte.
 */
class ObstacleItem : public QGraphicsPathItem
{
public:
    /*!
      \brief Destructeur

      Destructeur de la classe ObstacleItem
     */
	~ObstacleItem(){}

	// Constructeurs
    /*!
     * \brief ObstacleItem Constructeur
     * \param type Forme de l'obstacle
     * \param x Position x
     * \param y Position y
     * \param w Largeur
     * \param h Hauteur
     * \param parent
     *
     * Constructeur de la classe ObstacleItem qui permet de définir et visualiser un obstacle sur la carte interactive.
     */
	ObstacleItem( char type, double x, double y, double w, double h, QGraphicsItem *parent = nullptr )
		: QGraphicsPathItem( parent )
	{
		setPos( x, y );
		QPainterPath path;
		if ( type == OBST::RECTANGLE )
			path.addRect( -0.5*w, -0.5*h, w, h );
		else if ( type == OBST::ELLIPSE )
			path.addEllipse( -0.5*w, -0.5*h, w, h );
		path.closeSubpath();
		setPath( path );
    }
    /*!
     * \brief ObstacleItem
     * \param type Forme de l'obstacle
     * \param x Position x
     * \param y Position y
     * \param w Largeur
     * \param h Hauteur
     * \param meth
     * \param parent
     *
     * Constructeur de la classe ObstacleItem qui permet de définir et visualiser un obstacle sur la carte interactive.
     */
	ObstacleItem( char type, double x, double y, double w, double h, char meth, QGraphicsItem *parent = nullptr )
		: QGraphicsPathItem( parent )
	{
		Q_UNUSED( meth );
		setPos( x, y );
		QPainterPath path;
		if ( type == OBST::RECTANGLE )
			path.addRect( 0, 0, w, h );
		else if ( type == OBST::ELLIPSE )
			path.addEllipse( 0, 0, w, h );
		path.closeSubpath();
		setPath( path );
	};
};

#endif // OBSTACLEITEMS_H
