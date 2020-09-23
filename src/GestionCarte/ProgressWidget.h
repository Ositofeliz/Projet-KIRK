#ifndef PROGRESSWIDGET_H
#define PROGRESSWIDGET_H

#include <QWidget>
#include <QTimer>

namespace Ui {
	class ProgressWidget;
}

/*!
 * \brief The ProgressWidget class
 *
 * La classe ProgressWidget définit le widget affichant le calcul de la trajectoire.
 */
class ProgressWidget : public QWidget
{
	Q_OBJECT

public:
	explicit ProgressWidget(QWidget *parent = nullptr);
	~ProgressWidget();

public slots:
	void maj(int centreX, int centreY);
	void stop();
	void start();
	void majDistances( double dep, double cib );
	void arret();
	void resetDistances();
	void majTheme(QString);
	void setParentSize(QSize taille);

protected slots:

private slots:
	void on_arret_clicked();

private:
	Ui::ProgressWidget *ui;
	QString _theme = "black/";
	QTimer *timer = new QTimer(this);

signals:
	void demandeArretCalcul();
};

#endif // PROGRESSWIDGET_H
