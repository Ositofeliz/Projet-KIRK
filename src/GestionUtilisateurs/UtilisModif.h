#ifndef UTILISMODIF_H
#define UTILISMODIF_H

#include <QDialog>
#include "Utilisateur.h"

namespace Ui {
	class UtilisModif;
}

/*!
 * \brief The UtilisModif class
 * La classe UtilisModif est la fenêtre qui permet d'accéder aux fonctions
 * de modifications d'un utilisateur.
 */
class UtilisModif : public QDialog
{
	Q_OBJECT

public:
	explicit UtilisModif(QWidget *parent = nullptr);
	UtilisModif(QString type, QWidget *parent = nullptr);
	UtilisModif(Utilisateur util, QWidget *parent = nullptr);
	~UtilisModif();

private slots:
	void on_pb_Valider_clicked();
	void on_pb_Annuler_clicked();

private:
	Ui::UtilisModif *ui;
	QString _type = "modif";

signals:
	void envoiUtilisateur( Utilisateur );
};

#endif // UTILISMODIF_H
