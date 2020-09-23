#ifndef DRONEDETAILS_H
#define DRONEDETAILS_H

#include <QWidget>
#include "DronesWindow.h"

namespace Ui {
	class DroneDetails;
}

/*!
 * \brief The DroneDetails class
 * La classe DroneDétails est le widget qui permet d'afficher
 * les détails d'un drone.
 */
class DroneDetails : public QWidget
{
	Q_OBJECT

public:
	explicit DroneDetails(QWidget *parent = nullptr);
	~DroneDetails();
	void hideChange();
	void setDrone(Drone);
    Drone getDrone();
    void updateDetails(Drone);

public slots:
	void reset();
	void lock();
	void unlock();
	void majTheme(QString);
	void setDrone(QString);
	QString name();

private slots:
	void on_pb_change_clicked();

private:
	Ui::DroneDetails *ui;
	QString _theme = "black/";
	bool _locked = true;
    Drone droneSelected;
};

#endif // DRONEDETAILS_H
