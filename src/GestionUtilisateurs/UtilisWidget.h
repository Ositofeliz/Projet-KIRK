#ifndef UTILISWIDGET_H
#define UTILISWIDGET_H

#include <QWidget>
#include "Utilisateur.h"

namespace Ui {
	class UtilisWidget;
}

/*!
 * \brief The UtilisWidget class
 * La classe UtilisWidget est la classe qui permet d'afficher les informations
 * de l'utilisateur dans la fenêtre principale de l'application.
 */
class UtilisWidget : public QWidget
{
	Q_OBJECT

public:
	explicit UtilisWidget(QWidget *parent = nullptr);
	~UtilisWidget();

public slots:
	void setUtilisateur(Utilisateur util);
	void lock();
	void unlock();
	void majTheme(QString);

private slots:
	void on_pb_modif_clicked();
	void on_pb_gestion_clicked();
	void on_pb_logout_clicked();

private:
	Ui::UtilisWidget *ui;
	Utilisateur _util = {"", ""};
	QString _theme = "black/";

signals:
	void logout();
};

#endif // UTILISWIDGET_H
