#include "ProgressWidget.h"
#include "ui_ProgressWidget.h"
#include <QMovie>
#include <QMessageBox>
#include "Options.h"

#include <iostream>
using namespace std;

ProgressWidget::ProgressWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ProgressWidget)
{
	ui->setupUi(this);
	setAutoFillBackground( true );
	connect(timer, &QTimer::timeout, this, &ProgressWidget::hide );
	connect(timer, &QTimer::timeout, timer, &QTimer::stop );
	ui->depuisDepart->hide();
	ui->jusqueCible->hide();
	ui->lbDepart->hide();
	ui->lbcible->hide();
	ui->unit->hide();
	this->hide();
}

ProgressWidget::~ProgressWidget()
{
	delete ui;
	delete timer;
}

void ProgressWidget::start()
{
	ui->titre->setText( "Calcul en cours !");
	QMovie *movie = new QMovie( ICONS::PATH + _theme + ICONS::LOADING );
	movie->setScaledSize( {40, 40} );
	ui->icon->setFixedSize( 40, 40 );
	ui->icon->setMovie( movie );
	ui->icon->show();
	movie->start();
}

void ProgressWidget::stop()
{
	ui->titre->setText( "Calcul Terminé !");
	resetDistances();
	delete ui->icon->movie();
	ui->icon->setMovie( new QMovie() );
	timer->start( 1000 ); // 1000ms - Temps jusqu'à disparition
}

void ProgressWidget::arret()
{
		stop();
		ui->titre->setText( "Calcul Arrêté !");
}

void ProgressWidget::maj( int centreX, int centreY )
{
	if ( this->isHidden() )
		this->setGeometry( centreX - 0.5 * this->rect().width(),
						   centreY - 0.5 * this->rect().height(),
						   this->rect().width(), this->rect().height() );
}

void ProgressWidget::majDistances( double dep, double cib )
{
	ui->depuisDepart->setText( QString::number( dep ) );
	ui->jusqueCible->setText( QString::number( cib ) );
}

void ProgressWidget::resetDistances()
{
	ui->depuisDepart->setText( "--" );
	ui->jusqueCible->setText( "--" );
}

void ProgressWidget::majTheme( QString theme )
{
	_theme = theme;
	ui->arret->setIcon( QIcon(  ICONS::PATH + _theme + ICONS::ABORT ) );
}

void ProgressWidget::setParentSize( QSize taille )
{
	int x = 0.5*( taille.width() - this->width() );
	int y = taille.height() - this->height();
//	int w = this->width();
//	int h = this->height();

	this->move( x, y );
}

void ProgressWidget::on_arret_clicked()
{
	QMessageBox msgBox;
	msgBox.setWindowTitle( "Demande d'arrêt du calcul" );
	msgBox.setText("Voulez-vous vraiment arrêter le calcul en cours ?");
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	msgBox.setDefaultButton(QMessageBox::No);
	int reponse = msgBox.exec();
	if ( reponse == QMessageBox::Yes)
	{
		arret();
		emit demandeArretCalcul();
	}
}
