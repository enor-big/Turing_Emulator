#include "mainwindow.h"

#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Эмулятор машины тьюринга");
    resize(1000, 700);

    QLabel *label= new QLabel("Здесь будет эмулятор машины тьюринга");
    label ->setAlignment(Qt::AlignCenter);
    setCentralWidget(label);
}

MainWindow::~MainWindow()
{
}
