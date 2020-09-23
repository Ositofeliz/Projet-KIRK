#ifndef OBSTACLEMODIF_H
#define OBSTACLEMODIF_H

#include <QDialog>
#include "GestionCarte/CarteOpts.h"

namespace Ui {
	class ObstacleModif;
}

/*!
 * \brief The ObstacleModif class
 *
 * Widget de modification des obstacles affichés sur la carte.
 */
class ObstacleModif : public QDialog
{
	Q_OBJECT

public:
	explicit ObstacleModif(QWidget *parent = nullptr);
	~ObstacleModif();

public slots:
	void setTheme( QString );
	void setCentPos( Coordonnees );

private slots:
	void on_pb_Valider_clicked();
	void on_pb_Annuler_clicked();

private:
	Ui::ObstacleModif *ui;

signals:
	void ajoutObstacle();
};

#endif // OBSTACLEMODIF_H
