#ifndef FENETREPRINC_H
#define FENETREPRINC_H

#include <QWidget>
#include <QKeyEvent>
#include "Options.h"
#include "GestionUtilisateurs/Authentification.h"
#include "GestionDrones/Drone.h"


namespace Ui {
	class FenetrePrinc;
}

class FenetrePrinc : public QWidget
{
	Q_OBJECT

public:
	explicit FenetrePrinc(QWidget *parent = nullptr);
	~FenetrePrinc();

	static Drone drone1;
public slots:
	void login();
	void autorisation(Utilisateur util);
	void lock();
	void unlock();
	bool eventFilter( QObject *obj, QEvent *event ) override;
	void closeEvent(QCloseEvent *event) override;
	void connexions();
	void disconnexions();

private:
	Ui::FenetrePrinc *ui;
	QPalette _palette;
	Authentification *auth = new Authentification;
	Options opts;
	QSet<int> pressedKeys;
	void init();

signals:
	   void demandeArretCalcul();
private slots:
	void on_themes_currentIndexChanged(int index);
	void on_styles_currentIndexChanged(const QString &arg1);
};

#endif // FENETREPRINC_H
