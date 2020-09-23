#ifndef DRONEMODIF_H
#define DRONEMODIF_H

#include <QDialog>
#include "Drone.h"

namespace Ui {
	class DroneModif;
}

/*!
 * \brief The DroneModif class
 * La classe DroneModif est le widget qui permet de
 * modifier un drone à partir de l'interface.
 */
class DroneModif : public QDialog
{
	Q_OBJECT

public:
	explicit DroneModif(Drone drone=Drone(), QWidget *parent = nullptr);
    ~DroneModif();

public slots:
    void closeEvent(QCloseEvent *e);

signals:
    void closed(int=0);

private slots:
    void on_pbAnnuler_clicked();
    void on_pbValider_clicked();

private:
	Ui::DroneModif *ui;
    ListDrone listDrone;
    Drone droneAmodif;
};

#endif // DRONEMODIF_H
