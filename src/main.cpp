/*!
 * \file main.c
 * \brief Programme de gestion de trajectoire
 * \authors Nicolas Charon, Johann Colas, Rémy Costa, Karim Houanoh
 * \version 1.0
 * \date 16/09/2019
 */

#include <QApplication>

#include "FenetrePrinc.h"


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	FenetrePrinc w;
	w.login();

	return a.exec();
}
