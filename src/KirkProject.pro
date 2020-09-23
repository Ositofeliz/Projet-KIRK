QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    FenetrePrinc.cpp \
    GestionCarte/GestionMeteo/MeteoWidget.cpp \
    GestionCarte/GestionObstacles/ObstacleModif.cpp \
    GestionCarte/GestionObstacles/ObstaclesWindow.cpp \
    GestionCarte/ProgressWidget.cpp \
    GestionDrones/Drone.cpp \
    GestionDrones/DroneDetails.cpp \
    GestionDrones/DroneModif.cpp \
    GestionDrones/DronesWindow.cpp \
    GestionMissions/MissionWidget.cpp \
    GestionMissions/MissionsWindow.cpp \
    GestionUtilisateurs/Authentification.cpp \
    GestionUtilisateurs/UtilisModif.cpp \
    GestionUtilisateurs/UtilisWidget.cpp \
    GestionUtilisateurs/UtilissWindow.cpp \
    main.cpp

HEADERS += \
    FenetrePrinc.h \
    GestionCarte/CarteItem.h \
    GestionCarte/CarteOpts.h \
    GestionCarte/CarteWidget.h \
    GestionCarte/GestionMeteo/MeteoWidget.h \
    GestionCarte/GestionObstacles/ObstacleItem.h \
    GestionCarte/GestionObstacles/ObstacleModif.h \
    GestionCarte/GestionObstacles/Obstacles.h \
    GestionCarte/GestionObstacles/ObstaclesWindow.h \
    GestionCarte/ProgressWidget.h \
    GestionCarte/Tuile.h \
    GestionDrones/Drone.h \
    GestionDrones/DroneDetails.h \
    GestionDrones/DroneModif.h \
    GestionDrones/DronesWindow.h \
    GestionMissions/GestionTrajectoires/AlgoAStar.h \
    GestionMissions/Mission.h \
    GestionMissions/MissionWidget.h \
    GestionMissions/MissionsWindow.h \
    GestionUtilisateurs/Authentification.h \
    GestionUtilisateurs/UtilisModif.h \
    GestionUtilisateurs/UtilisWidget.h \
    GestionUtilisateurs/Utilisateur.h \
    GestionUtilisateurs/UtilissWindow.h \
    Options.h

FORMS += \
    FenetrePrinc.ui \
    GestionCarte/GestionMeteo/MeteoWidget.ui \
    GestionCarte/GestionObstacles/ObstacleModif.ui \
    GestionCarte/GestionObstacles/ObstaclesWindow.ui \
    GestionCarte/ProgressWidget.ui \
    GestionDrones/DroneDetails.ui \
    GestionDrones/DroneModif.ui \
    GestionDrones/DronesWindow.ui \
    GestionMissions/MissionWidget.ui \
    GestionMissions/MissionsWindow.ui \
    GestionUtilisateurs/Authentification.ui \
    GestionUtilisateurs/UtilisModif.ui \
    GestionUtilisateurs/UtilisWidget.ui \
    GestionUtilisateurs/UtilissWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
	res.qrc
