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
    rt.init();

    for (int i = 0; i <= 12; i++)
    {
        //rt.executeStep();
    }

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
