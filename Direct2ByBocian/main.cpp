#include "ui/MainWindow.h"
#include <QApplication>
#include <QLabel>
#include <QPicture>
#include <QPainter>

#include "simulation/SimulationWaypointGenerator.h"
#include "simulation/RouteSimulation.h"

int main(int argc, char *argv[])
{
    RouteSimulation rt = RouteSimulation();

    rt.generateFlightPlan();
    rt.start();

    Sleep(15000);
    rt.pause();
    Sleep(5000);
    rt.unpause();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
