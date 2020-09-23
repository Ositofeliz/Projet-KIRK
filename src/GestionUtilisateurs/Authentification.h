#ifndef AUTHENTIFICATION_H
#define AUTHENTIFICATION_H

#include <QWidget>
#include <QKeyEvent>
#include <QLineEdit>
#include <QPushButton>
#include "Utilisateur.h"

namespace Ui {
	class Authentification;
}
/*!
 * \brief The Authentification class
 * La classe Authentification est le widget qui s'affiche
 * pour l'authentification des utilisateurs au démarrage
 * de l'application.
 */
class Authentification : public QWidget
{
	Q_OBJECT

public:
	explicit Authentification( QWidget *parent = nullptr );
	~Authentification();
	void setID( QString );

private:
	Ui::Authentification *ui;
	bool authentifier = false;

private slots:
	void on_pb_login_clicked();
	void on_pbQuitter_clicked();
	bool eventFilter( QObject *obj, QEvent *event );

signals:
	void autorise( Utilisateur util );
	//void arretApp();
};

#endif // AUTHENTIFICATION_H
