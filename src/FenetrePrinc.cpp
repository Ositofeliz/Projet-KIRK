#include "FenetrePrinc.h"
#include "ui_FenetrePrinc.h"

#include "GestionUtilisateurs/Authentification.h"
#include "GestionUtilisateurs/Utilisateur.h"
#include <QList>
#include <QStyleFactory>
#include <iostream>

using namespace std;
Drone FenetrePrinc::drone1(TypeDrone::hale,"Choisissez un drone");

FenetrePrinc::FenetrePrinc(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::FenetrePrinc)
{
	installEventFilter( this );
	ui->setupUi(this);


	opts = Options();
	opts.lectureOptions();
	char index = 0;
	if ( opts.theme() == "Sombre" )
	{
		index = 1;
	}
	else if ( opts.theme() == "Clair" )
	{
		index = 2;
	}
	ui->themes->addItems( {"Default","Sombre", "Clair"} );
	ui->themes->setCurrentIndex( index );

	int ind = 0;
	for ( int it = 1; it < QStyleFactory::keys().size(); ++it )
	{
		QString styletmp = QStyleFactory::keys().at(it);
		if ( styletmp.toLower() == opts.style().toLower() )
		{
			ind = it;
		}
	}
	ui->styles->addItems( QStyleFactory::keys() );
	ui->styles->setCurrentIndex(ind);
	qApp->setStyle( opts.style() );
	connect( auth, SIGNAL( autorise(Utilisateur) ), this, SLOT( autorisation(Utilisateur) ) );
}

FenetrePrinc::~FenetrePrinc()
{
	delete ui;
	delete auth;
}

void FenetrePrinc::login()
{
	this->hide();
	auth->show();
}

void FenetrePrinc::autorisation( Utilisateur util )
{
	auth->close();
	init();
	//this->show();
	ui->utilWid->setUtilisateur( util );
	// SI Status est ADMIN
	if ( util.statut() == UtilOpts::ADMIN )
	{
		unlock();
	}
	// SI Status est USER
	else
		lock();
}

void FenetrePrinc::lock()
{
	ui->utilWid->lock();
	ui->carteWid->lock();
	ui->missionWid->lock();
}

void FenetrePrinc::unlock()
{
	ui->utilWid->unlock();
	ui->carteWid->unlock();
	ui->missionWid->unlock();
}

void FenetrePrinc::init()
{
	connexions();
	this->showMaximized();
};

void FenetrePrinc::connexions()
{
	connect( ui->utilWid, SIGNAL( logout() ), this, SLOT( login() ) );
	connect( ui->missionWid, SIGNAL( calculTrajectoire() ), ui->carteWid, SLOT( calculTrajectoire() ) );
	connect( this, SIGNAL( demandeArretCalcul() ), ui->carteWid, SIGNAL( demandeArretCalcul() ) );
	connect( ui->missionWid, &MissionWidget::sauvegardeTrajectoire, ui->carteWid, &CarteWidget::sauvegarderTrajectoire );
	connect( ui->carteWid, &CarteWidget::departChanged, ui->missionWid, &MissionWidget::setDepart );
	connect( ui->carteWid, &CarteWidget::cibleChanged, ui->missionWid, &MissionWidget::setCible );
	connect( ui->carteWid, &CarteWidget::checkpointsChanged, ui->missionWid, &MissionWidget::setCheckpoints );
	connect( ui->missionWid, &MissionWidget::departChanged, ui->carteWid, &CarteWidget::setDepart );
	connect( ui->missionWid, &MissionWidget::cibleChanged, ui->carteWid, &CarteWidget::setCible );
	connect( ui->missionWid, &MissionWidget::checkpointsChanged, ui->carteWid, &CarteWidget::setCheckpoints );
	connect( ui->carteWid->carteItem(), &CarteItem::afficherFaisabilite, ui->missionWid, &MissionWidget::setFaisabilite );
}

void FenetrePrinc::disconnexions()
{
	disconnect( ui->utilWid, SIGNAL( logout() ), this, SLOT( login() ) );
	disconnect( ui->missionWid, SIGNAL( calculTrajectoire() ), ui->carteWid, SLOT( calculTrajectoire() ) );
	disconnect( this, SIGNAL( demandeArretCalcul() ), ui->carteWid, SIGNAL( demandeArretCalcul() ) );
	disconnect( ui->missionWid, &MissionWidget::sauvegardeTrajectoire, ui->carteWid, &CarteWidget::sauvegarderTrajectoire );
}

bool FenetrePrinc::eventFilter(QObject *obj, QEvent *event)
{
	Q_UNUSED( obj );
	if ( event->type() == QEvent::ChildAdded )
	{
		QChildEvent *ce = static_cast<QChildEvent*>(event);
		ce->child()->installEventFilter( this );
			return true;
	}
	else if ( event->type() == QEvent::ChildRemoved )
	{
		QChildEvent *ce = static_cast<QChildEvent*>(event);
		ce->child()->removeEventFilter( this );
			return true;
	}
	if ( event->type() == QEvent::KeyPress )
	{
		pressedKeys += ((QKeyEvent*)event)->key();
		if (pressedKeys.contains(Qt::Key_Alt) && pressedKeys.contains(Qt::Key_P))
		{
			emit demandeArretCalcul();
			return true;
		}
	}
	else if ( event->type() == QEvent::KeyRelease )
	{
		pressedKeys -= ((QKeyEvent*)event)->key();
			return true;
	}
	if (event->type() == QEvent::FocusIn)
	{
		if (obj->parent()->parent() == this)
		{
//			ui->droneWid->updateDetails(FenetrePrinc::drone1);
			return true;
		}
	}
	return false;
}

void FenetrePrinc::closeEvent(QCloseEvent *event)
{
	disconnexions();
	emit demandeArretCalcul();
	login();
	event->accept();
}




void FenetrePrinc::on_themes_currentIndexChanged(int index)
{
	QString theme = "black/";
	if ( qApp->palette().color(QPalette::Window).lightness() > 127 ) theme = "white/";
	if ( index == 1 )
	{
		theme = "black/";
	}
	else if ( index == 2 )
	{
		theme = "white/";
	}
	if ( index != 0)
	{
		QFile file(":/icons/"+theme+"style.qss");
		if ( file.open(QFile::ReadOnly) )
		{
			QString style = QLatin1String( file.readAll() );
			style.remove('\n').remove('\t');
			qApp->setStyleSheet( style );
		}
		file.close();
	}
	else
	{
		qApp->setStyleSheet( "" );
	}
	ui->utilWid->majTheme( theme );
	ui->missionWid->majTheme( theme );
	ui->carteWid->majTheme( theme );
	opts.setTheme( ui->themes->currentText() );
	opts.sauvegarder();
}

void FenetrePrinc::on_styles_currentIndexChanged(const QString &arg1)
{
	opts.setStyle( arg1 );
	opts.sauvegarder();
	qApp->setStyle( arg1 );
}
