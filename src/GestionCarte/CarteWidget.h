#ifndef CARTEWIDGET_H
#define CARTEWIDGET_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QScrollBar>
#include <QContextMenuEvent>
#include <QMenu>
#include "CarteItem.h"
#include "GestionMeteo/MeteoWidget.h"
#include "GestionObstacles/ObstaclesWindow.h"
#include "GestionObstacles/ObstacleModif.h"
#include "ProgressWidget.h"

#include <iostream>
using namespace std;

/*!
 * \brief The CarteWidget class
 *
 * \ingroup GestionCarte
 *
 * CarteWidget est le widget qui accueille la Carte
 * Hérite de QGraphicsView
 */
class CarteWidget : public QGraphicsView
{
	Q_OBJECT

private:
	QString _theme = "black/";
	// _map est l'Item qui affiche la carte
	CarteItem *_carteItem = nullptr;
	ProgressWidget *_progressWidget = nullptr;
	// meteoWid est le Widget affiche les contions météo
	MeteoWidget *_meteoWid = nullptr;
	// Déclarations des différentes Actions  et du menu !
    QAction *acDepart = new QAction("Placer Départ", this);
	QAction *acCheck = new QAction("Placer Checkpoint", this);
	QAction *acCible = new QAction("Placer Cible", this);
	QAction *acDeleteCheck = new QAction("Supprimer Checkpoints", this);
	QAction *acAjoutObstacle = new QAction("Ajouter obstacle", this);
	QAction *acGestionObstacle = new QAction("Gestion des obstacles", this);
	QMenu *menu = nullptr;
	//
	QPointF _deltaPos = { 0, 0 };
	QPoint _oldPos = { 0, 0 };
	QPointF currentPos = { -1, -1 };
	bool rightButtonPressed = false;
	double _scale = 1;

public:
	// Destructeur
    /*! \brief Destructeur
     *
     * Destructeur de la classe CarteWidget
      */
	~CarteWidget()
	{
		delete _carteItem;
		delete _meteoWid;
		delete _progressWidget;
		delete acDepart;
		delete acCheck;
		delete acCible;
		delete acDeleteCheck;
		delete acAjoutObstacle;
		delete acGestionObstacle;
		delete menu;
    }

	// Constructeur
    /*!
     * \brief CarteWidget
     * \param parent
     *
     * Constructeur de la classe CarteWidget
     */
	CarteWidget( QWidget *parent = nullptr ) : QGraphicsView( parent )
	{
		setAccessibleName("Carte_wid");
		// Set render
		setRenderHints( QPainter::Antialiasing | QPainter::SmoothPixmapTransform );
		setTransformationAnchor(QGraphicsView::AnchorViewCenter);
		// Set Scene -> Essentiel pour ajouter des Items
		setScene( new QGraphicsScene() );
		//
		// Initialisation du menu contextuel
		menu = new QMenu(this);
		menu->addAction( acDepart );
		menu->addAction( acCheck );
		menu->addAction( acCible );
		menu->addSeparator();
		menu->addAction( acDeleteCheck );
		// Connexion entre actions et les slots à effectuer
		connect( acDepart, &QAction::triggered, this, &CarteWidget::envoiDepart );
		connect( acCheck, &QAction::triggered, this, &CarteWidget::envoiCheck );
		connect( acCible, &QAction::triggered, this, &CarteWidget::envoiCible );
		connect( acDeleteCheck, &QAction::triggered, this, &CarteWidget::envoiSupprimerChecks );
		connect( acAjoutObstacle, &QAction::triggered, this, &CarteWidget::ajouterObstacle );
		connect( acGestionObstacle, &QAction::triggered, this, &CarteWidget::gestionObstacles );
		//
		// Initialisation de MapItem
		_carteItem = new CarteItem;
		connect( this, &CarteWidget::demandeArretCalcul, _carteItem, &CarteItem::demandeArretCalcul );
		connect( this, &CarteWidget::arretApp, _carteItem, &CarteItem::demandeArretCalcul );
		scene()->addItem( _carteItem ); // Ajout de MapItem à la scène
		// Ajout séparateur au menu
		menu->addSeparator();
		//
		// Initialisation du Widget METEO
		_meteoWid = new MeteoWidget( this );
		//
		// Initialisation du Widget de Progression
		_progressWidget = new ProgressWidget( this );
		connect( _progressWidget, &ProgressWidget::demandeArretCalcul, _carteItem, &CarteItem::demandeArretCalcul );
		//
    }

	// Getters & Setters
    /*!
     * \brief carteItem Retourne l'objet CarteItem
     * \return
     */
    CarteItem *carteItem() { return _carteItem; }

private:

    /*!
     * \brief wheelEvent Zoom sur la carte
     * \param event
     *
     * Permet de zoomer ou dézoomer en appuyant sur CTRL et la molette de la souris.
     */
	void wheelEvent(QWheelEvent* event) override
    {
		if ( event->modifiers() == Qt::ControlModifier
			 && event->angleDelta().x() == 0)
		{
			double angle = event->angleDelta().y();
			if ( angle > 0 )
			{
				_carteItem->zoomIn();
			}
			else if ( angle < 0 )
			{
				_carteItem->zoomOut();
			}
			event->accept();
		}
		else if ( event->modifiers() == Qt::ShiftModifier )
		{
			double dx = event->angleDelta().x();
			double dy = event->angleDelta().y();
			_carteItem->move( dx, dy );
			event->accept();
		}
		event->ignore();
    }

    /*!
     * \brief mousePressEvent
     * \param event
     *
     * Permet de bouger l'image
     */
	void mousePressEvent(QMouseEvent *event) override
    {
		if ( event->modifiers() == Qt::ControlModifier
			 && event->button() == Qt::LeftButton)
		{
			rightButtonPressed = true;
			_deltaPos.setX( event->x() );
			_deltaPos.setY( event->y() );
			setCursor(Qt::ClosedHandCursor);
			event->accept();
			return;
		}
		event->ignore();
    }

    /*!
     * \brief mouseReleaseEvent
     * \param event
     *
     * Désactive la possiblité de bouger l'image.
     */
	void mouseReleaseEvent(QMouseEvent *event) override
	{
		// Désactive la possibilité de bouger l'image
		if (event->button() == Qt::LeftButton)
		{
			rightButtonPressed = false;
			setCursor(Qt::ArrowCursor);
			event->accept();
			return;
		}
		event->ignore();
    }


    /*!
     * \brief mouseDoubleClickEvent
     * \param event
     *
     * Permet de détecter le double clic
     */
	void mouseDoubleClickEvent(QMouseEvent *event) override
	{
		if ( event->button() == Qt::LeftButton )
		{
			QGraphicsScene *scen = this->scene();
			scen->setSceneRect( 0, 0, scen->sceneRect().width(), scen->sceneRect().height() );
			this->setScene( scen );
		}
    }

    /*!
     * \brief mouseMoveEvent
     * \param event
     * Ctrl + Clic-gauche permet de bouger dans l'image.
     */
	void mouseMoveEvent(QMouseEvent *event) override
	{
		// Si on appuie sur Ctrl et sur le button gauche de la souris,
		// on peut bouger dans l'image !
		if (rightButtonPressed)
		{
			QPoint delta = _oldPos - event->globalPos();
			if ( delta.x() < 0 ) delta.setX( -10 );
			else if ( delta.x() > 0 ) delta.setX( 10 );
			else  delta.setX( 0 );
			if ( delta.y() < 0 ) delta.setY( -10 );
			else if ( delta.y() > 0 ) delta.setY( 10 );
			else  delta.setY( 0 );
			_oldPos = event->globalPos();
			_carteItem->move( delta.x(), delta.y() );
			event->accept();
			return;
		}
		event->ignore();
    }

    /*!
     * \brief contextMenuEvent
     * \param event
     *
     * Le clic droit de la souris permet d'activer le menu contextuel
     * si le curseur est dans la carte.
     */
	void contextMenuEvent(QContextMenuEvent *event) override
    {
        currentPos = mapToScene( event->pos() );
			menu->exec(event->globalPos());
    }


	void resizeEvent(QResizeEvent *event) override
	{
		Q_UNUSED( event );
		QGraphicsScene *scen = this->scene();
		scen->setSceneRect( 0, 0, this->width(), this->height() );
		this->setScene( scen );
		_carteItem->majTailleVue( this->size() );
		_progressWidget->setParentSize( event->size() );
    }


	void keyPressEvent(QKeyEvent *event) override
	{
		// Active la possibilité de bouger l'image
		if ( event->key() == Qt::Key_Up )
		{
			_carteItem->move( 0, -60 );
			event->accept();
			return;
		}
		else if ( event->key() == Qt::Key_Down )
		{
			_carteItem->move( 0, 60 );
			event->accept();
			return;
		}
		else if ( event->key() == Qt::Key_Right )
		{
			_carteItem->move( 60, 0 );
			event->accept();
			return;
		}
		else if ( event->key() == Qt::Key_Left )
		{
			_carteItem->move( -60, 0 );
			event->accept();
			return;
		}
		event->ignore();
    }



public slots:
    /*!
     * \brief majTheme Mise-à-jour du thème
     * \param theme
     *
     * Méthode qui permet de mettre à jour le thème sélectionné.
     */
	void majTheme( QString theme )
	{
		_theme = theme;
		_carteItem->majTheme( _theme );
		_meteoWid->majTheme( _theme );
		_progressWidget->majTheme( _theme );
    }

    /*!
     * \brief envoiDepart
     *
     * Méthode qui envoie le départ à CarteItem _map.
     */
	void envoiDepart()
	{
		_carteItem->setDepart( currentPos );
		Coordonnees dep = _carteItem->getDepartCoord();
		emit departChanged( dep );
		_meteoWid->setCoordonnees(  dep.lat(), dep.lon() );
		_meteoWid->activer();
		_meteoWid->majMeteo();
    }

    /*!
     * \brief envoiCheck
     *
     * Méthode qui envoie le checkpoint à CarteItem _map.
     */
	void envoiCheck()
	{
		_carteItem->addCheckpoint( currentPos );
		emit checkpointsChanged( _carteItem->getCheckpointsCoord() );
    }

    /*!
     * \brief envoiCible
     *
     * Méthode qui envoie la cible à CarteItem _map.
     */
	void envoiCible()
	{
		_carteItem->setCible( currentPos );
		emit cibleChanged( _carteItem->getCibleCoord() );
    }

    /*!
     * \brief envoiSupprimerChecks
     *
     * Envoie la suppression des checkpoints.
     */
	void envoiSupprimerChecks()
	{
		_carteItem->clearCheckpoints();
		emit checkpointsChanged( _carteItem->getCheckpointsCoord() );
    }

    /*!
     * \brief calculTrajectoire
     *
     * Méthode qui envoie la demande de calcul de la trajectoire.
     */
	void calculTrajectoire()
	{
		_progressWidget->show();
		_progressWidget->start();
		connect( _carteItem, &CarteItem::calculTermine, _progressWidget, &ProgressWidget::stop );
		connect( _carteItem, &CarteItem::demandeArretCalcul, _progressWidget, &ProgressWidget::arret );
		connect( _carteItem, &CarteItem::arretPremature, _progressWidget, &ProgressWidget::close );
		connect( this, &CarteWidget::arretApp, _progressWidget, &ProgressWidget::close );
		_carteItem->calculTrajectoire();
    }

    /*!
     * \brief sauvegarderTrajectoire
     *
     * Méthode qui permet de sauvegarder la trajectoire.
     */
	void sauvegarderTrajectoire()
	{
		_carteItem->sauvegarderTrajectoire();
    }

    /*!
     * \brief ajouterObstacle
     *
     * Méthode qui permet d'ajouter un obstacle sur la carte.
     */
	void ajouterObstacle()
	{
		ObstacleModif *obstModif = new ObstacleModif;
		obstModif->setCentPos( _carteItem->versCoordReel( currentPos ) );
		connect( obstModif, SIGNAL(ajoutObstacle()), _carteItem, SLOT(majObstacles()) );
		obstModif->exec();
    }
	// Ouvre la fenêtre de gestion des obstacles
    /*!
     * \brief gestionObstacles
     *
     * Méthode qui permet d'ouvrir la fenêtre de gestion des obstacles.
     */
	void gestionObstacles()
	{
		ObstaclesWindow *obstWind = new ObstaclesWindow;
		obstWind->setTheme( _theme );
		connect( obstWind, SIGNAL(obstaclesChange()), _carteItem, SLOT(majObstacles()) );
		obstWind->exec();
    }

	void lock()
	{
		menu->removeAction( acAjoutObstacle );
		menu->removeAction( acGestionObstacle );
    }

	void unlock()
	{
		menu->addAction( acAjoutObstacle );
		menu->addAction( acGestionObstacle );
    }

    /*!
     * \brief setDepart
     * \param dep
     *
     * Méthode qui permet de définir les coordonées du point de départ.
     */
	void setDepart( Coordonnees dep )
	{
		_carteItem->setDepart( dep );
    }

    /*!
     * \brief setCible
     * \param cib
     *
     * Méthode qui permet de définir les coordonnées de la cible.
     */
	void setCible( Coordonnees cib )
	{
		_carteItem->setCible( cib );
    }

    /*!
     * \brief setCheckpoints
     * \param checks
     *
     * Méthode qui permet de définir les coordonnées d'un checkpoint.
     */
	void setCheckpoints( QList<Coordonnees> checks )
	{
		_carteItem->setCheckpoints( checks );
    }


signals:
    /*!
     * \fn demandeArretCalcul()
     *
     * Ce signal est émis lorsque le bouton "Arrêt calcul" est cliqué.
     */
	void demandeArretCalcul();

    /*!
     * \fn arretApp()
     *
     * Ce signal est émis lorsque ...
     */
	void arretApp();

    /*!
     * \fn departChanged()
     *
     * Ce signal est émis lorsque le départ est modifié.
     */
	void departChanged( Coordonnees );

    /*!
     * \fn cibleChanged()
     *
     * Ce signal est émis lorsque la cible est modifiée.
     */
	void cibleChanged( Coordonnees );

    /*!
     * \fn checkpointsChanged()
     *
     * Ce signal est émis lorsque le ou les checkpoints sont modifiés.
     */
	void checkpointsChanged( QList<Coordonnees> );
};


#endif // CARTEWIDGET_H
