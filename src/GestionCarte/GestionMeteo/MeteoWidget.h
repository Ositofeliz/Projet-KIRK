#ifndef METEOWIDGET_H
#define METEOWIDGET_H

#include <QWidget>
#include <QTimer>

namespace Ui {
	class MeteoWidget;
}

/*!
 * \brief The MeteoWidget class
 *
 * La classe MeteoWidget permet d'afficher les informations météorologiques récupérées
 * via l'API OpenWeatherMap au point de départ sélectionné.
 */
class MeteoWidget : public QWidget
{
	Q_OBJECT

public:
	explicit MeteoWidget(QWidget *parent = nullptr);
	~MeteoWidget();

    void activer()
	{
		timer->start();
    }

	void desactiver()
	{
		timer->stop();
    }

	void setCondIcon( QString id );

	void setCoordonnees(double lat, double lon);

public slots:
	void majMeteo();
	void reset();
	void majTheme(QString);

private:
	Ui::MeteoWidget *ui;
	QString _lat = "0";
	QString _lon = "0";
	QTimer *timer = nullptr;
	int intervalleMAJ = 1; // en minutes
	void setVentDirection(double angle);
	QString _theme = "black";
};

#endif // METEOWIDGET_H
