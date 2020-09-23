#include "DroneModif.h"
#include "ui_DroneModif.h"
#include "DroneDetails.h"
#include "DronesWindow.h"

DroneModif::DroneModif(Drone drone, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DroneModif)
{
	ui->setupUi(this);
    ui->coType->addItems( {"HALE", "MALE"} );
    int index = (drone.getTypeQS()=="hale")? 0 : 1;
    ui->coType->setCurrentText(ui->coType->itemText(index));
    ui->leNom->setText(drone.getNom());
    ui->dspPoids->setValue(drone.getPoids());
    ui->dspAutonomie->setValue(drone.getAutonomie());
    ui->dspVitesseMax->setValue(drone.getVitesseMax());
    ui->dspAltitudeMax->setValue(drone.getAltitudeMax());
	ui->dspChargeMax->setValue(drone.getChargeMax());
    droneAmodif = drone;
    connect(this,SIGNAL(closed(int)),this->parent(),SLOT(updateW(int)));
}

DroneModif::~DroneModif()
{
	delete ui;
}

void DroneModif::closeEvent(QCloseEvent * e)
{

	//qDebug() << "Parent de DroneModif: "<< this->parent();

    emit closed(droneAmodif.getId());

    e->accept();
}

void DroneModif::on_pbValider_clicked()
{
    // Envoi les nouvelles infos du drone
        // Enregistre ces mêmes infos
		//TODO ajouter les paramètres manquant à modifier
		cout << "0-";
		QString oldName = droneAmodif.getNom();
        droneAmodif.setNom(ui->leNom->text());
        droneAmodif.setTypeQS(ui->coType->currentText());
        droneAmodif.setPoids(ui->dspPoids->value());
        droneAmodif.setVitesseMax(ui->dspVitesseMax->value());
        droneAmodif.setAutonomie(ui->dspAutonomie->value());
        droneAmodif.setAltitudeMax(ui->dspAltitudeMax->value());
        droneAmodif.setChargeMax(ui->dspChargeMax->value());
		//droneAmodif.setDescription(ui->leNom->text());
		ListDrone listDrones = ListDrone();
		listDrones.modifier(droneAmodif, oldName);
		listDrones.sauvegarder();
//        qDebug() << "Validation modification";
//        qDebug() << droneAmodif.getId()<<" "<< droneAmodif.getNom();
//        qDebug() << "-----------------------";
        this->close();
}

void DroneModif::on_pbAnnuler_clicked()
{
    this->close();
}
