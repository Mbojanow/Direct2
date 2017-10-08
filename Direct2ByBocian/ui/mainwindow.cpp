#include <QtWidgets>

#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setCentralWidget(mainApp.getMainWidget());
    setFixedSize(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT);
}

MainWindow::~MainWindow()
{
}
