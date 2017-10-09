#pragma once

#include <QMainWindow>

#include "MainWindowApp.h"

/*
 * MainWindow Q_OBJECT class declaration. Contains MainWindowApp widget.
 */

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    static const int MAIN_WINDOW_WIDTH = 1500;
    static const int MAIN_WINDOW_HEIGHT = 950;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    MainWindowApp mainApp;
};
