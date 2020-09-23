#include "ObstacleModif.h"
#include "ui_ObstacleModif.h"
#include "Obstacles.h"

ObstacleModif::ObstacleModif(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ObstacleModif)
{
	ui->setupUi(this);
	ui->co_Type->addItems( {"Rectangle", "Ellipse"} );
}

ObstacleModif::~ObstacleModif()
{
	delete ui;
}

void ObstacleModif::setTheme( QString )
{

}

void ObstacleModif::setCentPos( Coordonnees co )
{
	ui->dsp_LatCent->setValue( co.lat() );
	ui->dsp_LonCent->setValue( co.lon() );
}

void ObstacleModif::on_pb_Valider_clicked()
{
	if ( !Obstacles::dejaDansFichier( ui->le_Nom->text() ) && ui->le_Nom->text().size() > 0 && ui->dsp_TailLat->value() != 0 && ui->dsp_TailLon->value() != 0 )
	{
		QString type = "rect";
		if ( ui->co_Type->currentText().contains("Ellipse") )
		{
			type = "ell";
		}
		Obstacles::ajouterDansFichier(
					Obstacle(
						ui->le_Nom->text()+";"+
						type+";"+
						QString::number(ui->dsp_LatCent->value())+";"+
						QString::number(ui->dsp_LonCent->value())+";"+
						QString::number(ui->dsp_TailLat->value())+";"+
						QString::number(ui->dsp_TailLon->value())
						));
		emit ajoutObstacle();
	}
	this->close();
}

void ObstacleModif::on_pb_Annuler_clicked()
{
	this->close();
}
