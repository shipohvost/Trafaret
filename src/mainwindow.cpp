#include "./ui_mainwindow.h"
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initTables();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initTables() {}
