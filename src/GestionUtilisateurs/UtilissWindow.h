#ifndef UTILISSWINDOW_H
#define UTILISSWINDOW_H

#include <QDialog>
#include "Utilisateur.h"

namespace Ui {
	class UtilissWindow;
}

/*!
 * \brief The UtilissWindow class
 * La classe UtilissWindow est la classe qui permet d'afficher la fenêtre
 * des utilisateurs.
 */
class UtilissWindow : public QDialog
{
	Q_OBJECT

public:
	explicit UtilissWindow(QWidget *parent = nullptr);
	~UtilissWindow();

public slots:
	void update(int index);
	void majTheme(QString);

private slots:
	void on_pb_Valider_clicked();
	void on_pbAnnuler_clicked();
	void on_listWidget_currentRowChanged(int currentRow);
	void on_pbAjouter_clicked();
	void on_pbEnregistrer_clicked();
	void on_pbSupprimer_clicked();

private:
	Ui::UtilissWindow *ui;
	ListeUtilisateur utilisateurs;
	void setStatusCO(char status);
	char getStatusCo();
	void majListe();
};

#endif // UTILISSWINDOW_H
