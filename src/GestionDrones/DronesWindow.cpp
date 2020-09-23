#include "DronesWindow.h"
#include "ui_DronesWindow.h"
#include "DroneModif.h"
#include "DroneDetails.h"
#include "Options.h"

DronesWindow::DronesWindow(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DronesWindow)
{
	ui->setupUi(this);
    ui->detDrone->hideChange();
    drones = ListDrone();
    majListe();
}

void DronesWindow::majListe()
{
    ui->list_drones->clear();
    for ( auto elm : drones.liste() )
    {
        ui->list_drones->addItem( elm.getNom() );
    }
}

DronesWindow::~DronesWindow()
{
	delete ui;
}

void DronesWindow::lock(bool locked)
{
	if ( locked == true )
	{
		ui->pbModifier->hide();
	}
	else
	{
		ui->pbModifier->show();
	}
}

void DronesWindow::lock()
{
}

void DronesWindow::unlock()
{
}

void DronesWindow::setCurrentDrone( QString nom )
{
	for ( int it = 0; it < ui->list_drones->count(); ++it )
	{
		if ( ui->list_drones->item(it)->text() == nom )
			ui->list_drones->setCurrentRow(it);
	}
}

void DronesWindow::majTheme( QString theme )
{
	ui->pbModifier->setIcon( QIcon( ICONS::PATH + theme + ICONS::EDIT ) );
    ui->pbNouveau->setIcon(QIcon(ICONS::PATH+theme+ICONS::ADD));
    ui->pbSupprimer->setIcon(QIcon(ICONS::PATH+theme+ICONS::DELETE));
}

void DronesWindow::updateW(int i)
{
    drones.importer();
    majListe();
    ui->list_drones->setCurrentRow(i);
}

void DronesWindow::on_pbSelectioner_clicked()
{
    // Envoie le drone sélectionné
    //FIXME Lorsqu'aucun drone n'est sélectionné, l'application plante et se ferme
    try {

            int v= this->ui->list_drones->currentIndex().row(); //donne l'id
//            qDebug() <<"current row"<< v;
//            qDebug() << drones.liste().at(v).getNom();
//            qDebug() << "-----------------------";
            FenetrePrinc::drone1=drones.liste().at(v);
            this->close();

    } catch (exception e) {
        qDebug() << e.what();
    }
	//cout << ui->list_drones->currentItem()->text().toStdString() << endl;
	emit droneSelection( ui->list_drones->currentItem()->text() );
}

void DronesWindow::on_pbAnnuler_clicked()
{
	this->close();
}

void DronesWindow::on_pbModifier_clicked()
{
	DroneModif *droneMod = new DroneModif(ui->detDrone->getDrone(),this);
	droneMod->exec();
}


void DronesWindow::on_list_drones_currentRowChanged(int currentRow)
{
    if(currentRow==-1){
        currentRow=0;
    }
    //qDebug() << drones.liste().at(currentRow).getNom();
    ui->detDrone->setDrone(drones.liste().at(currentRow));
}

void DronesWindow::on_pbSupprimer_clicked()
{
    int v= this->ui->list_drones->currentIndex().row(); //donne l'id
    //ui->detDrone->updateDetails(this->ui->list_drones->currentItem()->;
//    qDebug() <<"current row"<< v;
//    qDebug() << drones.liste().at(v).getNom();
    drones.supprimer(v);
    //NOTE Faire une fonction qui permet de faire toutes ces actions (jusqu'au prochain note)
    drones.sauvegarder();
    drones.importer();
    majListe();
    int i = drones.liste().length()-1;
    ui->list_drones->setCurrentRow(i);
    ui->detDrone->updateDetails(drones.liste().at(i));
    //NOTE Demander confirmation à l'utilisateur?


}

void DronesWindow::on_pbNouveau_clicked()
{
    //TODO ouvrir une dialog pour écrire directement les caractéristiques
	//qDebug() << drones.liste().length();

	drones.ajouter(Drone(drones.liste().length()+1));

    drones.sauvegarder();
    drones.importer();
    majListe();
    int i = drones.liste().length()-1;
    ui->list_drones->setCurrentRow(i);
    ui->detDrone->updateDetails(drones.liste().at(i));
}
