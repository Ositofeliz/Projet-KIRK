#ifndef OBSTACLESWINDOW_H
#define OBSTACLESWINDOW_H

#include <QDialog>
#include "Obstacles.h"

namespace Ui {
	class ObstaclesWindow;
}

/*!
 * \brief The ObstaclesWindow class
 * La classe ObstacleWindow est la fenêtre qui permet d'afficher les obstacles enregistrés dans le fichier OBSTACLES.txt.
 */
class ObstaclesWindow : public QDialog
{
	Q_OBJECT

public:
	explicit ObstaclesWindow(QWidget *parent = nullptr);
	~ObstaclesWindow();
	void majListWidget();

public slots:
	void setTheme(QString);

private slots:
	void on_listWidget_currentRowChanged(int currentRow);
	void on_pbValider_clicked();
	void on_pbAnnuler_clicked();
	void on_pbDelete_clicked();
	void on_pbNew_clicked();
	void on_pbSave_clicked();

private:
	Ui::ObstaclesWindow *ui;
	Obstacles obsts;

signals:
	void obstaclesChange();
};

#endif // OBSTACLESWINDOW_H
