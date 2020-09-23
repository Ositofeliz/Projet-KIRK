#include "Authentification.h"
#include "ui_Authentification.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QCloseEvent>
#include <QAction>
#include "Utilisateur.h"

Authentification::Authentification( QWidget *parent ) :
	QWidget(parent),
	ui(new Ui::Authentification)
{
	ui->setupUi(this);
	//this->setWindowFlag( Qt::FramelessWindowHint );

	QRect desktopRect = QApplication::desktop()->availableGeometry(this);
	QPoint center = desktopRect.center();
	move(center.x() - width() * 0.5, center.y() - height() * 0.5);
	qApp->installEventFilter(this);
	//qApp->installEventFilter( ui->le_ID );
}

Authentification::~Authentification()
{
	delete ui;
}

void Authentification::setID( QString id )
{
	ui->le_ID->setText( id );
}

void Authentification::on_pb_login_clicked()
{
	// Checking si le couple ID/MDP est valide
	ListeUtilisateur _users = ListeUtilisateur();
	int index = _users.verification( ui->le_ID->text().remove(' ') );
	if ( index >= 0 )
	{
		char autorisation = _users.autorisation( index, ui->le_MDP->text().remove(' ') );
		if ( autorisation == UtilOpts::UNAUTHORIZED )
		{
			QMessageBox::warning(0, "Erreur", "Mot de passe invalide !!" );
		}
		else if ( autorisation == UtilOpts::USER || autorisation == UtilOpts::ADMIN )
		{
			emit autorise( _users.liste().at( index ) );
		}
	}
	else
	{
		QMessageBox::warning(0, "Erreur", "Utilisateur introuvable !!" );
	}
	ui->le_MDP->setText("");
}

void Authentification::on_pbQuitter_clicked()
{
	//emit arretApp();
	qApp->exit();
}

bool Authentification::eventFilter(QObject *obj, QEvent *event)
{
	if ( event->type() == QEvent::ChildAdded )
	{
		QChildEvent *ce = static_cast<QChildEvent*>(event);
		ce->child()->installEventFilter( this );
	}
	else if ( event->type() == QEvent::ChildRemoved )
	{
		QChildEvent *ce = static_cast<QChildEvent*>(event);
		ce->child()->removeEventFilter( this );
	}
	if ( event->type() == QEvent::KeyPress )
	{
		QKeyEvent *keyEvent = dynamic_cast<QKeyEvent *>(event);
		//cout << "key " << keyEvent->key() << "from" << obj << endl;
		if ( keyEvent->key() == 16777220 )
		{
			if ( obj == ui->le_ID || obj == ui->le_MDP || obj == ui->pb_login )
			{
				on_pb_login_clicked();
			}
			else if ( obj == ui->pbQuitter )
			{
				on_pbQuitter_clicked();
			}
		}
	}
	return QObject::eventFilter(obj, event);
}


