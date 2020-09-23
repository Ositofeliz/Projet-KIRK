#include "DroneDetails.h"
#include "ui_DroneDetails.h"
#include "DronesWindow.h"
#include "FenetrePrinc.h"
#include "DroneModif.h"
#include "Options.h"

DroneDetails::DroneDetails(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::DroneDetails)
{
    ui->setupUi(this);

    this->droneSelected = ListDrone().liste().front();
    //this->updateDetails(this->droneSelected);

}

DroneDetails::~DroneDetails()
{
	delete ui;
}

void DroneDetails::lock()
{
	_locked = true;
}

void DroneDetails::unlock()
{
	_locked = false;
}

void DroneDetails::majTheme( QString theme )
{
	_theme = theme;
	ui->pb_change->setIcon( QIcon( ICONS::PATH + _theme + ICONS::CHANGE ) );
}

void DroneDetails::hideChange()
{
    ui->pb_change->hide();
}

void DroneDetails::setDrone(Drone d)
{
    this->droneSelected.setId(d.getId());
    this->droneSelected.setVitesseMax(d.getVitesseMax());
    this->droneSelected.setType(d.getType());
    this->droneSelected.setChargeMax(d.getChargeMax());
    this->droneSelected.setAltitudeMax(d.getAltitudeMax());
    this->droneSelected.setAltitudeMin(d.getAltitudeMin());
    this->droneSelected.setNom(d.getNom());
    this->droneSelected.setAutonomie(d.getAutonomie());
    this->droneSelected.setPoids(d.getPoids());
    this->updateDetails(this->droneSelected);

}

void DroneDetails::setDrone( QString name )
{
	ListDrone drones;
	setDrone( drones.droneByName(name) );
}

QString DroneDetails::name()
{
	return droneSelected.getNom();
}

Drone DroneDetails::getDrone()
{
    return this->droneSelected;
}

void DroneDetails::updateDetails(Drone d)
{
    ui->inf_type->setText       (d.getTypeQS());
    ui->inf_nom->setText        (d.getNom());
    ui->inf_poids->setNum       (d.getPoids());
    ui->inf_vitessemax->setNum  (d.getVitesseMax());
    ui->inf_chargemax->setNum   (d.getChargeMax());
    ui->inf_altitudemax->setNum (d.getAltitudeMax());
	ui->inf_autonomie->setNum   (d.getAutonomie());
}

void DroneDetails::reset()
{
	ui->inf_type->setText       ("--");
	ui->inf_nom->setText        ("--");
	ui->inf_poids->setNum       (0);
	ui->inf_vitessemax->setNum  (0);
	ui->inf_chargemax->setNum   (0);
	ui->inf_altitudemax->setNum (0);
	ui->inf_autonomie->setNum   (0);
}

void DroneDetails::on_pb_change_clicked()
{
	DronesWindow *droneMod = new DronesWindow;
	droneMod->setCurrentDrone( ui->inf_nom->text() );
	droneMod->majTheme( _theme );
	droneMod->lock( _locked );
	connect( droneMod, SIGNAL(droneSelection(QString)), this, SLOT(setDrone(QString)) );
	droneMod->exec();
}
