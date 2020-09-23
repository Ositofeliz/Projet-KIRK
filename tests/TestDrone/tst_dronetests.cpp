#include <QtTest>
#include <QtTest/QtTest>
#include <QCoreApplication>
#include "../../src/GestionDrones/Drone.h"

// add necessary includes here

class DroneTests : public QObject
{
    Q_OBJECT

public:
    DroneTests();
    ~DroneTests();
    Drone drone;


private slots:
    void types(); //test les setter/getter relatifs au type
    void name();
    void fromList();
};
DroneTests::~DroneTests(){}

DroneTests::DroneTests()
{
    //Initialisation des données
    drone = Drone(0,TypeDrone::hale,"Drone de test",0,0,0,0,0,0);

}

void DroneTests::types()
{
	drone.setTypeQS("male");
    QCOMPARE(drone.getTypeQS(),"male");
    drone.setType(TypeDrone::hale);
    QCOMPARE(drone.getType(), TypeDrone::hale);
}

void DroneTests::name()
{
    drone.getNom();
    QCOMPARE(drone.getNom(),"Drone de test");
    drone.setNom("Drone modifie");
    QCOMPARE(drone.getNom(), "Drone modifie");
}
void DroneTests::fromList()
{
    ListDrone list("a");
    Drone drone2(1,TypeDrone::hale,"Drone 2",0,0,0,0,0,0);
    Drone drone3(2,TypeDrone::hale,"Drone 3",0,0,0,0,0,0);
    list.ajouter(drone);
    list.ajouter(drone2);
    list.ajouter(drone3);

    QCOMPARE(drone.getId(),list.droneByName("Drone modifie").getId());

    list.supprimer(2);
    QCOMPARE(list.liste().back().getNom(),drone2.getNom());

    list.ajouter(drone3);
    list.supprimer(drone3);
    QCOMPARE(list.liste().back().getNom(),"Drone 2");

    list.modifier(drone3,"Drone modifie");
    QCOMPARE(list.liste().first().getNom(),"Drone 3");

    list.modifier(drone,0);
    QCOMPARE(list.liste().first().getNom(),"Drone modifie");

}


QTEST_MAIN(DroneTests)

#include "tst_dronetests.moc"
