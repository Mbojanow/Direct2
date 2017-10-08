#pragma once

#include <QMainWindow>
#include "MainWindowApp.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    MainWindowApp mainLayout;
};
