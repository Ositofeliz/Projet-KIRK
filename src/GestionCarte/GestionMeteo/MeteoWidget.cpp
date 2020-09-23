#include "MeteoWidget.h"
#include "ui_MeteoWidget.h"
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPixmap>
#include <QIcon>
#include "Options.h"

#include <iostream>
using namespace std;

MeteoWidget::MeteoWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::MeteoWidget)
{
	ui->setupUi(this);

	setAutoFillBackground( true );

	timer = new QTimer( this );
	timer->setInterval( 1000*60*intervalleMAJ );
	connect( timer, &QTimer::timeout, this, &MeteoWidget::majMeteo );
}

MeteoWidget::~MeteoWidget()
{
	delete ui;
	delete timer;
}

void MeteoWidget::majMeteo()
{
	if ( _lat != "" && _lon != "" )
	{
		QString url = "http://api.openweathermap.org/data/2.5/weather?lat="
					  + _lat + "&lon=" + _lon + "&appid=a1f16d241646bcda618e94e6971ae459&lang=fr" ;

		QNetworkRequest request = QNetworkRequest( QUrl( url ) );
		request.setHeader( QNetworkRequest::ContentTypeHeader, "application/json" );

		QNetworkAccessManager nam;
		QNetworkReply *reply = nam.get( request );
		while ( !reply->isFinished() )
		{
			qApp->processEvents();
		}
		reply->deleteLater();
		QByteArray rep_data = reply->readAll();

		QJsonDocument jsonDoc = QJsonDocument::fromJson( rep_data );
		QJsonObject jsonObject = jsonDoc.object();

		QVariantMap jsonMap = jsonObject.toVariantMap();

		for (auto elm : jsonMap.keys() )
		{
			if ( elm == "main" )
			{
				QJsonObject coordObj = jsonMap[elm].toJsonObject();
				QVariantMap coordMap = coordObj.toVariantMap();
				for (auto elm : coordMap.keys() )
				{
					if ( elm == "temp" )
					{
						double temp = coordMap[elm].toDouble() - 273.15;
						ui->lbTemp->setText( QString::number(temp) );
						//cout << coordMap[elm].toString().toStdString() << endl;
					}
					else if ( elm == "pressure" )
					{
						ui->lbPress->setText( coordMap[elm].toString() );
					}
					else if ( elm == "humidity" )
					{
						ui->lbHum->setText( coordMap[elm].toString() );
					}
				}
			}
			else if ( elm == "weather" )
			{
				QVariantList weatMap = jsonMap[elm].toList();
				QVariantMap map = weatMap.first().toMap();
				for (auto elm : map.keys() )
				{
					if ( elm == "description" )
					{
						ui->lbCond->setText( map[elm].toString() );
					}
					else if ( elm == "id" )
					{
						setCondIcon( map[elm].toString() );
					}
				}
			}
			else if ( elm == "wind" )
			{
				QJsonObject coordObj = jsonMap[elm].toJsonObject();
				QVariantMap coordMap = coordObj.toVariantMap();
				for (auto elm : coordMap.keys() )
				{
					if ( elm == "deg" )
					{
						setVentDirection( coordMap[elm].toDouble() );
					}
					else if ( elm == "speed" )
					{
						ui->lbVent->setText( coordMap[elm].toString() );
					}
				}
			}
		}
		ui->lbCond->adjustSize();
		adjustSize();
	}
}

void MeteoWidget::setCondIcon( QString id )
{
	QString path = ICONS::PATH + _theme + ICONS::SUN;
	if ( id.size() > 0 )
	{
		if ( id.at(0) == '8' && id != "800" )
		{
			path = ICONS::PATH + _theme + ICONS::CLOUDS;
		}
		if ( id.at(0) == '7' )
		{
			if ( id.at(1) == '0' || id.at(1) == '1'  || id.at(1) == '3' || id.at(1) == '5' || id.at(1) == '6' || id.at(1) == '7' )
			{
				path = ICONS::PATH + _theme +  ICONS::MIST;
			}
			else if ( id.at(1) == '2' )
			{
				path = ICONS::PATH + _theme + ICONS::HAZE;
			}
			else if ( id.at(1) == '4' )
			{
				path = ICONS::PATH + _theme + ICONS::FOGGY;
			}
			else if ( id.at(1) == '8' )
			{
				path = ICONS::PATH + _theme + ICONS::TORNADO;
			}
		}
	}
	if ( id.at(0) == '6' )
	{
		path = ICONS::PATH + _theme + ICONS::SNOWY;
	}
	if ( id.at(0) == '5' )
	{
		path = ICONS::PATH + _theme + ICONS::SHOWERS;
	}
	if ( id.at(0) == '3' )
	{
		path = ICONS::PATH + _theme +  ICONS::DRIZZLE;
	}
	if ( id.at(0) == '2' )
	{
		path = ICONS::PATH + _theme + ICONS::THUNDERSTORMS;
	}
	QPixmap pix = QIcon(path).pixmap(QSize(25,25));
	ui->iconCond->setPixmap( pix );
}

void MeteoWidget::setVentDirection( double angle )
{
	QPixmap orig = QIcon( ICONS::PATH + _theme + ICONS::WINDDIREC ).pixmap(QSize(200, 200));
	QMatrix rm;
	rm.rotate( angle + 90 );
	orig = orig.transformed(rm);
	orig = orig.copy((orig.width() - 200)/2, (orig.height() - 200)/2, 200, 200);
	orig = orig.scaledToWidth(15);
	ui->lbVentDirec->setPixmap( orig );
}

void MeteoWidget::setCoordonnees( double lat, double lon )
{
	_lat = QString::number( lat );
	_lon = QString::number( lon );
	timer->start();
	majMeteo();
}

void MeteoWidget::reset()
{
	ui->lbTemp->setText( "--" );
	ui->lbPress->setText( "--" );
	ui->lbHum->setText( "--" );
	ui->lbCond->setText( "--" );
	ui->iconCond->setPixmap( QPixmap() );
	ui->lbVent->setText( "--" );
	ui->lbVentDirec->setPixmap( QPixmap() );
}

void MeteoWidget::majTheme( QString theme )
{
	_theme = theme;
	majMeteo();
}
