#ifndef OPTIONS_H
#define OPTIONS_H

#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <iostream>
using namespace std;

namespace ICONS
{
	static const QString PATH = ":/icons/";
	static const QString ABORT = "abort.svg";
	static const QString ADD = "add.svg";
	static const QString CHANGE = "change.svg";
	static const QString CHECKPOINT = "checkpoint.svg";
	static const QString CIBLE = "cible.svg";
	static const QString CLOUDWINDY = "cloud-windy.svg";
	static const QString CLOUDS = "cloudy.svg";
	static const QString DELETE = "delete.svg";
	static const QString DEPART = "depart.svg";
	static const QString DRIZZLE = "drizzle.svg";
	static const QString EARTHQUAKE = "earthquake.svg";
	static const QString EDIT = "edit.svg";
	static const QString FLASHLIGHT = "flashlight.svg";
	static const QString FLOOD = "flood.svg";
	static const QString FOGGY = "foggy.svg";
	static const QString HAZE = "haze.svg";
	static const QString HEAVYSHOWERS = "heavy-showers.svg";
	static const QString LOADING = "loading.gif";
	static const QString LOGOUT = "logout.svg";
	static const QString METEOR = "meteor.svg";
	static const QString MIST = "mist.svg";
	static const QString MOONCLEAR = "moon-clear.svg";
	static const QString MOONCLOUDY = "moon-cloudy.svg";
	static const QString MOON = "moon.svg";
	static const QString OPEN = "open.svg";
	static const QString SAVE = "save.svg";
	static const QString SHOWERS = "showers.svg";
	static const QString SNOWY = "snowy.svg";
	static const QString SUNCLOUDY = "sun-cloudy.svg";
	static const QString SUNFOGGY = "sun-foggy.svg";
	static const QString SUN = "sun.svg";
	static const QString THUNDERSTORMS = "thunderstorms.svg";
	static const QString TORNADO = "tornado.svg";
	static const QString TYPHOON = "typhoon.svg";
	static const QString USERS = "users.svg";
	static const QString USERSETS = "userSets.svg";
	static const QString WINDDIREC = "wind-direc.svg";
	static const QString WINDY = "windy.svg";
};

class Options
{
private:
	QString optsPath = "CONFIG.txt";
	QString _theme = "default";
	QString _style = "default";
public:
    /*!
     * \brief Destructeur
     * Destructeur de la classe Option.
     */
    ~Options(){}

    /*!
     * \brief Options
     * Constructeur par défaut de la classe Option.
     */
    Options(){}

    /*!
     * \brief setStyle
     * \param style
     * Mutateur permettant de choisir le style de l'interface.
     */
	void setStyle( QString style )
	{
		_style = style;
    }

    /*!
     * \brief style
     * \return Retourne le style.
     */
	QString style()
	{
		return _style;
    }

    /*!
     * \brief setTheme
     * \param theme
     * Mutateur qui permet de choisir le thème de l'interface.
     */
	void setTheme( QString theme )
	{
		_theme = theme;
    }

    /*!
     * \brief theme
     * \return Retourne le thème de l'interface.
     */
	QString theme()
	{
		return _theme;
    }

    /*!
     * \brief lectureOptions
     * Méthode qui définit les options de lecture.
     */
	void lectureOptions()
	{
		QFile file( optsPath);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			QMessageBox::warning(0, "Error", file.errorString());
			return;
		}
		QTextStream stream( &file );
		while ( !stream.atEnd() )
		{
			QString line = stream.readLine().remove(" ");
			if ( line != "" && !line.contains("//") )
			{
				QStringList sets = line.split('=');
				if ( sets.size() > 1 )
				{
					if ( sets.at(0) == "theme" )
					{
						_theme = sets.at(1);
					}
					else if ( sets.at(0) == "style" )
					{
						_style = sets.at(1);
					}
				}
			}
		}
		file.close();
    }

    /*!
     * \brief sauvegarder
     * Méthode qui permet de sauvegarder les options.
     */
	void sauvegarder()
	{
		QFile file( optsPath );
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			QMessageBox::warning(0, "Error", file.errorString());
			return;
		}
		QTextStream out( &file );
		out << "style=" << _style << "\n";;
		out << "theme=" << _theme << "\n";;
		file.close();
    }
};

//QString theme = THEME::ICONSPATH + "/black/";

#endif // OPTIONS_H
