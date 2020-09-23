#ifndef MISSIONSWINDOW_H
#define MISSIONSWINDOW_H

#include <QDialog>
#include <QListWidget>
#include "Mission.h"

namespace Ui {
	class MissionsWindow;
}

/*!
 * \brief The MissionsWindow class
 * La classe MissionWindow est la fenêtre de sélection des missions.
 */
class MissionsWindow : public QDialog
{
	Q_OBJECT

public:
	explicit MissionsWindow(QWidget *parent = nullptr);
	~MissionsWindow();

public slots:
	void majTheme(QString);
	void majList();

private slots:
	void on_pb_selectionner_clicked();
	void on_pb_Annuler_clicked();
	void on_listWidget_currentRowChanged(int currentRow);

private:
	Ui::MissionsWindow *ui;
	ListeMissions _missions;

signals:
	void selectedMission( Mission );
};

#endif // MISSIONSWINDOW_H
