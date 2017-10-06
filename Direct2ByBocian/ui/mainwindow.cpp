#include <QtWidgets>

#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setCentralWidget(mainLayout.getMainWidget());
    setFixedSize(900, 550);
}

MainWindow::~MainWindow()
{
}
