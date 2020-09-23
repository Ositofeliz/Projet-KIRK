#ifndef MISSIONWIDGET_H
#define MISSIONWIDGET_H

#include <QWidget>
#include "Mission.h"

namespace Ui {
	class MissionWidget;
}

/*!
 * \brief The MissionWidget class
 * La classe MissionWidget est le widget des missions qui
 * apparaît sur le fenêtre principale.
 */
class MissionWidget : public QWidget
{
	Q_OBJECT

public:
	explicit MissionWidget(QWidget *parent = nullptr);
	~MissionWidget();
	Mission mission() { return _mission; };

public slots:
	void lock();
	void unlock();
	void majTheme(QString);
	void setMission( Mission );
	void setDepart( Coordonnees );
	void setCible( Coordonnees );
	void setCheckpoints( QList<Coordonnees> );
	void setDepartLat( double );
	void setDepartLon( double );
	void setCibleLat( double );
	void setCibleLon( double );
    void setCharge( double );
	void disabled();
	void setFaisabilite(int fais);

protected slots:
	void init();

private slots:
	void on_pb_ouvrir_clicked();
	void on_pb_nouveau_clicked();
	void on_pb_sauvegarder_clicked();
	void on_pb_supprimer_clicked();
	void on_pb_Traj_calculer_clicked();
	void on_pb_Traj_sauvegarder_clicked();

private:
	Ui::MissionWidget *ui;
	QString _theme = "black/";
	Mission _mission;
	QString _currentUser = "";

signals:
	void calculTrajectoire();
	void sauvegardeTrajectoire();
	void listChanged();
	void departChanged( Coordonnees );
	void cibleChanged( Coordonnees );
	void checkpointsChanged( QList<Coordonnees> );
};

#endif // MISSIONWIDGET_H
