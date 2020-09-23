#include "UtilissWindow.h"
#include "ui_UtilissWindow.h"
#include "UtilisModif.h"
#include "Options.h"

#include <iostream>
using namespace std;

UtilissWindow::UtilissWindow(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::UtilissWindow)
{
	ui->setupUi(this);
	ui->coStatus->addItems( {"Utilisateur", "Administrateur"} );
	// Construit la liste des utilisateur
	utilisateurs = ListeUtilisateur();
	majListe();
}

UtilissWindow::~UtilissWindow()
{
	delete ui;
}
void UtilissWindow::update( int index)
{
	cout << "id : " << index << endl;
}

void UtilissWindow::majTheme( QString theme )
{
	ui->pbAjouter->setIcon( QIcon( ICONS::PATH + theme + ICONS::ADD ) );
	ui->pbEnregistrer->setIcon( QIcon( ICONS::PATH + theme + ICONS::SAVE ) );
	ui->pbSupprimer->setIcon( QIcon( ICONS::PATH + theme + ICONS::DELETE ) );
}

void UtilissWindow::setStatusCO( char status )
{
	if ( status == UtilOpts::USER )
	{
		ui->coStatus->setCurrentIndex( 0 );
	}
	else if ( status == UtilOpts::ADMIN )
	{
		ui->coStatus->setCurrentIndex( 1 );
	}
}

char UtilissWindow::getStatusCo()
{
	if ( ui->coStatus->currentIndex() == 1 )
	{
		return UtilOpts::ADMIN;
	}
	return UtilOpts::USER;
}

void UtilissWindow::on_listWidget_currentRowChanged( int currentRow )
{
	//cout << "id : " << currentRow << endl;
	if ( currentRow >= 0 )
	{
		Utilisateur local = utilisateurs.liste().at( currentRow );
		ui->le_ID->setText( local.id() );
		ui->le_MDP->setText( local.mdp() );
		setStatusCO( local.statut() );
		ui->lb_infos->setText( "" );
	}
}

void UtilissWindow::majListe()
{
	ui->listWidget->clear();
	for ( auto elm : utilisateurs.liste() )
	{
		ui->listWidget->addItem( elm.id() );
	}
}


void UtilissWindow::on_pbAjouter_clicked()
{
	ui->le_ID->setText( "" );
	ui->le_MDP->setText( "" );
	ui->coStatus->setCurrentIndex( 0 );
}

void UtilissWindow::on_pbEnregistrer_clicked()
{
	QString erreurs = "";
	QString id = ui->le_ID->text().remove( ' ' );
	QString mdp = ui->le_MDP->text().remove( ' ' );
	char status = getStatusCo();
	cout << "Util : " << id.toStdString() << " - " << mdp.toStdString() << endl;
	if ( id == "" )
	{
		erreurs += "Nom d'utilisateur invalide !";
	}
	if ( mdp == "" )
	{
		erreurs += "\nMot de passe invalide !";
	}
	if ( erreurs == "" )
	{
		int index = utilisateurs.existeDeja( id );
		if ( index != -1  )
		{
			utilisateurs.modifier( mdp, status, index );
			ui->lb_infos->setText( "Utilisateur modifié !" );
		}
		else
		{
			utilisateurs.ajouter( Utilisateur( id, mdp, status) );
			ui->lb_infos->setText( "Utilisateur ajouté !" );
		}
		majListe();
	}
	else
	{
		QMessageBox::warning( 0, "Erreur", erreurs );
		return;
	}
}

void UtilissWindow::on_pbSupprimer_clicked()
{
	utilisateurs.supprimer( ui->listWidget->currentRow() );
	majListe();
	ui->le_ID->setText( "" );
	ui->le_MDP->setText( "" );
	ui->coStatus->setCurrentIndex( 0 );
}

void UtilissWindow::on_pb_Valider_clicked()
{
	// Sauvegarde de la liste modifier
	utilisateurs.sauvegarder();
	this->close();
}

void UtilissWindow::on_pbAnnuler_clicked()
{
	this->close();
}
