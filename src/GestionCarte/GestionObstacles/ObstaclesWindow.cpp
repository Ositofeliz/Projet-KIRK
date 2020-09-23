#include "ObstaclesWindow.h"
#include "ui_ObstaclesWindow.h"
#include "Options.h"

ObstaclesWindow::ObstaclesWindow(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ObstaclesWindow)
{
	ui->setupUi(this);
	ui->coType->addItems( { "Rectangle", "Ellipse"} );
	obsts = Obstacles();
	majListWidget();
}

ObstaclesWindow::~ObstaclesWindow()
{
	delete ui;
}

void ObstaclesWindow::majListWidget()
{
	ui->listWidget->clear();
	for ( int it = 0; it < obsts.liste().size(); it++ )
		ui->listWidget->addItem( obsts.liste().at(it).ID() );
}

void ObstaclesWindow::setTheme( QString theme )
{
	ui->pbNew->setIcon( QIcon( ICONS::PATH + theme + ICONS::ADD ) );
	ui->pbSave->setIcon( QIcon( ICONS::PATH + theme + ICONS::SAVE ) );
	ui->pbDelete->setIcon( QIcon( ICONS::PATH + theme + ICONS::DELETE ) );
}

void ObstaclesWindow::on_listWidget_currentRowChanged(int currentRow)
{
	ui->lbInfos->setText( "" );
	if ( currentRow >= 0 && currentRow < obsts.liste().size() )
	{
		Obstacle obst = obsts.liste().at(currentRow);
		ui->leID->setText( ui->listWidget->currentItem()->text() );
		ui->coType->setCurrentIndex(0);
		if ( obst.type() == OBST::ELLIPSE )
		{
			ui->coType->setCurrentIndex(1);
		}
		ui->dspLatCent->setValue( obst.centLat() );
		ui->dspLonCent->setValue( obst.centLon() );
		ui->dspLatTail->setValue( obst.latTail() );
		ui->dspLonTail->setValue( obst.lonTail() );
	}
}

void ObstaclesWindow::on_pbValider_clicked()
{
	obsts.sauvegarder();
	emit obstaclesChange();
	this->close();
}

void ObstaclesWindow::on_pbAnnuler_clicked()
{
	this->close();
}

void ObstaclesWindow::on_pbDelete_clicked()
{
	if ( obsts.supprimer( ui->listWidget->currentRow() ) )
	{
		ui->lbInfos->setText( "Obstacle supprimer !" );
		majListWidget();
	}
}

void ObstaclesWindow::on_pbNew_clicked()
{
	ui->leID->setText( "Nouveau" );
	ui->coType->setCurrentIndex(0);
	ui->dspLatCent->setValue( 0 );
	ui->dspLonCent->setValue( 0 );
	ui->dspLatTail->setValue( 0 );
	ui->dspLonTail->setValue( 0 );
	ui->lbInfos->setText( "" );
	//obsts.supprimer( ui->listWidget->currentRow() );
}

void ObstaclesWindow::on_pbSave_clicked()
{
	Obstacle neoObst = Obstacle();
	neoObst.setID( ui->leID->text().remove(" ") );
	neoObst.setType( ui->coType->currentIndex() == 0 ? OBST::RECTANGLE : OBST::ELLIPSE );
	neoObst.setCentLat( ui->dspLatCent->value() );
	neoObst.setCentLon( ui->dspLonCent->value() );
	neoObst.setLatTail( ui->dspLatTail->value() );
	neoObst.setLonTail( ui->dspLonTail->value() );
	if ( !obsts.existeDeja( neoObst) )
	{
		obsts.ajouter( neoObst );
		ui->lbInfos->setText( "Obstacle ajouté !" );
		majListWidget();
	}
	else
	{
		if ( obsts.modifier( neoObst, ui->listWidget->currentRow() ) )
		{
			ui->lbInfos->setText( "Obstacle sauvegardé !" );
		}
		else
		{
			ui->lbInfos->setText( "Obstacle non sauvegardé !\nL'identifiant existe déjà !" );
		}
	}
	obsts.sauvegarder();
}
