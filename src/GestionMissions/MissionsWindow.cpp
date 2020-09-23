#include "MissionsWindow.h"
#include "ui_MissionsWindow.h"
#include <QListWidget>

MissionsWindow::MissionsWindow(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::MissionsWindow)
{
	ui->setupUi(this);
	majList();
	ui->missionWid->disabled();
	ui->missionWid->lock();
	connect( ui->missionWid, &MissionWidget::listChanged, this, &MissionsWindow::majList );
}

MissionsWindow::~MissionsWindow()
{
	delete ui;
}

void MissionsWindow::majList()
{
	_missions.importer();
	ui->listWidget->clear();
	for ( auto elm : _missions.liste() )
	{
		ui->listWidget->addItem( elm.id() );
	}
}

void MissionsWindow::majTheme( QString theme )
{
	ui->missionWid->majTheme( theme );
}

void MissionsWindow::on_pb_selectionner_clicked()
{
	// Envoi la mission sélectionner
	emit selectedMission( _missions.liste().at( ui->listWidget->currentRow() ) );
	this->close();
}

void MissionsWindow::on_pb_Annuler_clicked()
{
	this->close();
}

void MissionsWindow::on_listWidget_currentRowChanged(int currentRow)
{
	if ( currentRow >= 0 )
	{
		Mission local = _missions.liste().at( currentRow );
		ui->missionWid->setMission( local );
//		ui->le_MDP->setText( local.mdp() );
//		setStatusCO( local.statut() );
//		ui->lb_infos->setText( "" );
	}
}
