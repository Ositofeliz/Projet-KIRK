#include "Drone.h"

TypeDrone Drone::getType() const
{
	return type;
}

void Drone::setType(const TypeDrone &value)
{
	type = value;
}

double Drone::getVitesseMax() const
{
	return vitesseMax;
}

void Drone::setVitesseMax(double value)
{
	vitesseMax = value;
}

double Drone::getChargeMax() const
{
	return chargeMax;
}

void Drone::setChargeMax(double value)
{
	chargeMax = value;
}

double Drone::getPoids() const
{
	return poids;
}

void Drone::setPoids(double value)
{
	poids = value;
}

double Drone::getAutonomie() const
{
	return autonomie;
}

void Drone::setAutonomie(double value)
{
	autonomie = value;
}

int Drone::getAltitudeMax() const
{
	return altitudeMax;
}

void Drone::setAltitudeMax(int value)
{
	altitudeMax = value;
}

int Drone::getAltitudeMin() const
{
	return altitudeMin;
}

void Drone::setAltitudeMin(int value)
{
	altitudeMin = value;
}

QString Drone::getNom() const
{
    return nom;
}

void Drone::setNom(const QString &value)
{
    nom = value;
}

int Drone::getId()
{
	return id;
}

void Drone::setId(int value)
{
	id = value;
}

Drone::Drone()
{
    setId(21);
    setType(TypeDrone::hale);
    setNom("Drone");
    setVitesseMax(0);
    setChargeMax(0);
    setPoids(0);
    setAutonomie(0);
    setAltitudeMax(0);
    setAltitudeMin(0);
}

Drone::Drone(int id)
{
    setId(id);
    setType(TypeDrone::hale);
    setNom("Drone"+QString::number(id));
    setVitesseMax(0);
    setChargeMax(0);
    setPoids(0);
    setAutonomie(0);
    setAltitudeMax(0);
    setAltitudeMin(0);
}

Drone::Drone(TypeDrone t, QString d, int aMax, int vMax)
{
	setType(t);
    setNom(d);
	setAltitudeMax(aMax);
    setVitesseMax(vMax);
}

Drone::Drone(int id, TypeDrone t, QString d, double vM, double cM, double p, double a, int aMax, int aMin)
{
	setId(id);
	setType(t);
    setNom(d);
	setVitesseMax(vM);
	setChargeMax(cM);
	setPoids(p);
	setAutonomie(a);
	setAltitudeMax(aMax);
	setAltitudeMin(aMin);
}

QString Drone::getTypeQS()
{
	switch(this->getType()){
	case TypeDrone::male :
		return "male";
	case TypeDrone::hale:
		return "hale";
	}
}
void Drone::setTypeQS(QString t)
{
    try {

        if(t.toLower() =="male"){
            this->setType(TypeDrone::male);
        }
        else if(t.toLower() =="hale"){
            this->setType(TypeDrone::hale);
        }else{
            throw "invalid type";
        }
    } catch (QString e) {
       qDebug() << "Error : "<< e;
    }
}
