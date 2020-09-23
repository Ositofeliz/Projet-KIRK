#include "UtilisModif.h"
#include "ui_UtilisModif.h"
#include "Utilisateur.h"

UtilisModif::UtilisModif(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::UtilisModif)
{
	ui->setupUi(this);
	ui->lbStatus->hide();
	ui->coStatus->hide();
}

UtilisModif::UtilisModif(QString type, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::UtilisModif)
{
	ui->setupUi(this);
	_type = type;
	ui->lbStatus->hide();
	ui->coStatus->hide();
	if ( _type == "ajout" )
	{
		setWindowTitle("Kirk Project - Ajout d'un Utilisateur");
		ui->label->setText( "Ajout d'un Utilisateur");
		ui->pb_Valider->setText( "Valider ajout" );
		ui->lbStatus->show();
		ui->coStatus->show();
		ui->coStatus->addItems( {"Utilisateur", "Administrateur"} );
		ui->coStatus->setCurrentIndex(0);
	}
}

UtilisModif::UtilisModif(Utilisateur util, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::UtilisModif)
{
	ui->setupUi(this);
	ui->lbStatus->hide();
	ui->coStatus->hide();
	ui->le_ID->setText( util.id() );
	ui->le_ID->setReadOnly( true );
	ui->le_MDP->setText( util.mdp() );
}

UtilisModif::~UtilisModif()
{
	delete ui;
}

void UtilisModif::on_pb_Valider_clicked()
{
	// Envoi les nouvelles infos de l'utilisateur
	// Enregistre ces mêmes infos
	if  ( _type == "ajout" )
	{
		char statusLoc = UtilOpts::USER;
		if ( ui->coStatus->currentText() == "Administrateur") statusLoc = UtilOpts::ADMIN;
		Utilisateur local = Utilisateur(
								ui->le_ID->text().remove( ' '), ui->le_MDP->text().remove( ' '), statusLoc );
	}
	else if  ( _type == "modif" )
	{
		ListeUtilisateur _users = ListeUtilisateur();
		_users.modifier( ui->le_ID->text(), ui->le_MDP->text().remove(' ') );
		_users.sauvegarder();
		emit envoiUtilisateur( Utilisateur(ui->le_ID->text(), ui->le_MDP->text().remove(' '), _users.autorise( ui->le_ID->text(),  ui->le_MDP->text().remove(' '))) );
	}
	this->close();
}

void UtilisModif::on_pb_Annuler_clicked()
{
	this->close();
}
