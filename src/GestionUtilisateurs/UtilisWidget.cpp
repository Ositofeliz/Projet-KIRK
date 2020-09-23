#include "UtilisWidget.h"
#include "ui_UtilisWidget.h"
#include "UtilissWindow.h"
#include "UtilisModif.h"
#include "Authentification.h"
#include <QWidget>
#include "Options.h"

UtilisWidget::UtilisWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::UtilisWidget)
{
	ui->setupUi(this);
}

UtilisWidget::~UtilisWidget()
{
	delete ui;
}

void UtilisWidget::setUtilisateur( Utilisateur util )
{
	_util = util;
	ui->leNom->setText( _util.id() );
}

void UtilisWidget::lock()
{
	ui->pb_gestion->hide();
}

void UtilisWidget::unlock()
{
	ui->pb_gestion->show();
}

void UtilisWidget::majTheme( QString theme )
{
	_theme = theme;
	ui->pb_modif->setIcon( QIcon( ICONS::PATH + theme + ICONS::USERSETS ) );
	ui->pb_gestion->setIcon( QIcon( ICONS::PATH + theme + ICONS::USERS ) );
	ui->pb_logout->setIcon( QIcon( ICONS::PATH + theme + ICONS::LOGOUT ) );
}

void UtilisWidget::on_pb_modif_clicked()
{
	UtilisModif *modfiUtilis = new UtilisModif( _util );
	connect( modfiUtilis, SIGNAL(envoiUtilisateur( Utilisateur )), this, SLOT(setUtilisateur( Utilisateur) ));
	modfiUtilis->exec();
}

void UtilisWidget::on_pb_gestion_clicked()
{
	UtilissWindow *gestionUtilis = new UtilissWindow;
	gestionUtilis->majTheme( _theme );
	gestionUtilis->exec();
}

void UtilisWidget::on_pb_logout_clicked()
{
	emit logout();
}
