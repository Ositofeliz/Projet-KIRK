#ifndef DRONESWINDOW_H
#define DRONESWINDOW_H

#include <QDialog>
#include "Drone.h"
#include "FenetrePrinc.h"
#include "DroneModif.h"

namespace Ui {
	class DronesWindow;
}

/*!
 * \brief The DronesWindow class
 * La classe DronesWindow est la fenêtre qui permet d'afficher
 * les drones contenus dans le fichier DRONES.txt et d'accéder
 * aux fonctionnalités de gestion des drones.
 */
class DronesWindow : public QDialog
{
	Q_OBJECT

public:
	explicit DronesWindow(QWidget *parent = nullptr);
    ~DronesWindow();

public slots:
	void lock( bool locked );
	void lock();
	void unlock();
	void setCurrentDrone(QString nom);
	void majTheme(QString theme);
    void updateW(int);

private slots:
	void on_pbSelectioner_clicked();
	void on_pbAnnuler_clicked();
	void on_pbModifier_clicked();
    void on_list_drones_currentRowChanged(int currentRow);
    void on_pbSupprimer_clicked();
    void on_pbNouveau_clicked();

private:
    Ui::DronesWindow *ui;
    ListDrone drones;
    void majListe();

signals:
	void droneSelection(QString);
};

#endif // DRONESWINDOW_H
