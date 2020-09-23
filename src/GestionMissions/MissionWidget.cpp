#include "MissionWidget.h"
#include "ui_MissionWidget.h"
#include "MissionsWindow.h"
#include "Options.h"

MissionWidget::MissionWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::MissionWidget)
{
	ui->setupUi(this);
	ui->lbFaisTitle->hide();
	ui->lbFaisInfos->hide();
	connect( ui->dsp_dep_lat, SIGNAL( valueChanged(double) ), this, SLOT( setDepartLat(double) ) );
	connect( ui->dsp_dep_lon, SIGNAL( valueChanged(double) ), this, SLOT( setDepartLon(double) ) );
	connect( ui->dsp_cib_lat, SIGNAL( valueChanged(double) ), this, SLOT( setCibleLat(double) ) );
    connect( ui->dsp_cib_lon, SIGNAL( valueChanged(double) ), this, SLOT( setCibleLon(double) ) );
    connect( ui->dsp_charge, SIGNAL( valueChanged(double) ), this, SLOT( setCharge(double) ) );//Changer l'origin
}

MissionWidget::~MissionWidget()
{
	delete ui;
}

void MissionWidget::disabled()
{
	ui->pb_nouveau->hide();
	ui->pb_ouvrir->hide();
	ui->pb_sauvegarder->hide();
	ui->pb_supprimer->show();
	ui->lb_Traj->hide();
	ui->pb_Traj_calculer->hide();
	ui->pb_Traj_sauvegarder->hide();
	ui->lbFaisTitle->hide();
	ui->lbFaisInfos->hide();
	ui->le_nom->setEnabled( false );
	ui->tim_Date->setEnabled( false );
	ui->dsp_dep_lat->setEnabled( false );
	ui->dsp_dep_lon->setEnabled( false );
	ui->dsp_cib_lat->setEnabled( false );
	ui->dsp_cib_lon->setEnabled( false );
	ui->dsp_charge->setEnabled( false );
	ui->droneWid->hideChange();
}

void MissionWidget::lock()
{
	ui->droneWid->lock();
	ui->lbFaisTitle->hide();
	ui->lbFaisInfos->hide();
}

void MissionWidget::unlock()
{
	ui->droneWid->unlock();
}

void MissionWidget::init()
{
	ui->le_nom->setText( "--");
	ui->dsp_dep_lat->setValue( 0 );
	ui->dsp_dep_lon->setValue( 0 );
	ui->dsp_cib_lat->setValue( 0 );
	ui->dsp_cib_lon->setValue( 0 );
	ui->dsp_charge->setValue( 0 );
	ui->droneWid->reset();
}

void MissionWidget::majTheme( QString theme )
{
	_theme = theme;
	ui->pb_nouveau->setIcon( QIcon( ICONS::PATH + _theme + ICONS::ADD ) );
	ui->pb_ouvrir->setIcon( QIcon( ICONS::PATH + _theme + ICONS::OPEN ) );
	ui->pb_sauvegarder->setIcon( QIcon( ICONS::PATH + _theme + ICONS::SAVE ) );
	ui->pb_supprimer->setIcon( QIcon( ICONS::PATH + _theme + ICONS::DELETE ) );
	ui->pb_Traj_sauvegarder->setIcon( QIcon( ICONS::PATH + _theme + ICONS::SAVE ) );
	ui->droneWid->majTheme( theme );
}

void MissionWidget::setMission( Mission mis )
{
	_mission = mis;
	ui->le_nom->setText( mis.id() );
	ui->tim_Date->setTime( mis.time() );
	ui->tim_Date->setDate( mis.date() );
	ui->dsp_dep_lat->setValue( mis.depart().lat() );
	ui->dsp_dep_lon->setValue( mis.depart().lon() );
	ui->dsp_cib_lat->setValue( mis.cible().lat() );
	ui->dsp_cib_lon->setValue( mis.cible().lon() );
	ui->dsp_charge->setValue( mis.charge() );
	ui->droneWid->setDrone( mis.drone() );
	emit departChanged( _mission.depart() );
	emit cibleChanged( _mission.cible() );
	emit checkpointsChanged( _mission.checkpoints() );
}

void MissionWidget::setDepart( Coordonnees co )
{
	_mission.setDepart( co );
	ui->dsp_dep_lat->setValue( co.lat() );
	ui->dsp_dep_lon->setValue( co.lon() );
}

void MissionWidget::setCible( Coordonnees co )
{
	_mission.setCible( co );
	ui->dsp_cib_lat->setValue( co.lat() );
	ui->dsp_cib_lon->setValue( co.lon() );
}

void MissionWidget::setCheckpoints( QList<Coordonnees> list )
{
	_mission.setCheckpoints( list );
}

void MissionWidget::setDepartLat( double lat )
{
	Coordonnees dep = _mission.depart();
	dep.setLat( lat );
	_mission.setDepart( dep );
	emit departChanged( _mission.depart() );
}

void MissionWidget::setDepartLon( double lon )
{
	Coordonnees dep = _mission.depart();
	dep.setLon( lon );
	_mission.setDepart( dep );
	emit departChanged( _mission.depart() );
}

void MissionWidget::setCibleLat( double lat )
{
	Coordonnees cib = _mission.cible();
	cib.setLat( lat );
	_mission.setCible( cib );
	emit cibleChanged( _mission.cible() );
}

void MissionWidget::setCibleLon( double lon )
{
	Coordonnees cib = _mission.cible();
	cib.setLon( lon );
	_mission.setCible( cib );
	emit cibleChanged( _mission.cible() );
}
void MissionWidget::setCharge( double c )
{
    //double charge = _mission.charge();
    _mission.setCharge( c );
    //emit cibleChanged( _mission.cible() );
}

void MissionWidget::on_pb_ouvrir_clicked()
{
	MissionsWindow *FenetreMissions = new MissionsWindow;
	FenetreMissions->majTheme( _theme );
	connect( FenetreMissions, &MissionsWindow::selectedMission, this, &MissionWidget::setMission );
	FenetreMissions->exec();
}

void MissionWidget::on_pb_nouveau_clicked()
{
	// Creation d'une nouvelle mission
	init();
	ui->le_nom->setText( "Nvlle Mission");
	ui->tim_Date->setTime( QTime::currentTime() );
	ui->tim_Date->setDate( QDate::currentDate() );
}

void MissionWidget::on_pb_sauvegarder_clicked()
{
	// Sauvegarde d'une mission
	QString nom = ui->le_nom->text().remove(';');
	if ( nom.size() > 0 )
	{
		_mission.setID( nom );
		_mission.setTime( ui->tim_Date->time() );
		_mission.setDate( ui->tim_Date->date() );
		_mission.setDepart( Coordonnees( ui->dsp_dep_lat->value(), ui->dsp_dep_lon->value() ) );
		_mission.setCible( Coordonnees( ui->dsp_cib_lat->value(), ui->dsp_cib_lon->value() ) );
		_mission.setCharge( ui->dsp_charge->value() );
		_mission.setDrone( ui->droneWid->name() );
		_mission.setUtilisateur( _currentUser );
		ListeMissions missions;
		missions.sauvegarder( _mission );
		emit listChanged();
	}
}

void MissionWidget::on_pb_supprimer_clicked()
{
	// Suppression d'une mission
	ListeMissions missions;
	missions.supprimer( ui->le_nom->text() );
	init();
	emit listChanged();
}

void MissionWidget::on_pb_Traj_calculer_clicked()
{
	// Calcul de la trajectoire
	emit calculTrajectoire();
}

void MissionWidget::on_pb_Traj_sauvegarder_clicked()
{
	// Sauvegarde de la trajectoire
	emit sauvegardeTrajectoire();
}

void MissionWidget::setFaisabilite( int fais )
{
	if ( fais >= 0 )
	{
        qDebug() << "mission charge: "<<_mission.charge();
        qDebug() << "Drone charge: "<<ui->droneWid->getDrone().getChargeMax();
		//QIcon(QMessageBox::Information)
		QString distance = "Distance entre le point de depart\net la cible : " + QString::number( fais ) + " km\n";
		if ( ui->droneWid->getDrone().getAutonomie() > 2*fais )
		{
			ui->lbFaisInfos->setText(distance+"Mission réalisable !\nAller-retour réalisable !");
            ui->lbFaisInfos->setStyleSheet("color:green;");
		}
		else if ( ui->droneWid->getDrone().getAutonomie() > fais )
		{
			ui->lbFaisInfos->setText(distance+"Mission réalisable !\nAller uniquement réalisable !");
            ui->lbFaisInfos->setStyleSheet("color:orange;");
		}
		else
		{
			ui->lbFaisInfos->setText(distance+"Mission non réalisable !");
            ui->lbFaisInfos->setStyleSheet("color:red;");
		}
        if(_mission.charge()> ui->droneWid->getDrone().getChargeMax()){
            ui->lbFaisInfos->setText(ui->lbFaisInfos->text()+" Charge trop importante");
            ui->lbFaisInfos->setStyleSheet("color:red;");
        }
		ui->droneWid->getDrone();

		ui->lbFaisTitle->show();
		ui->lbFaisInfos->show();
	}
	else
	{
		ui->lbFaisTitle->hide();
		ui->lbFaisInfos->hide();
	}

}
