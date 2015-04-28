#include "mainwindow.h"
#include "ui_mainwindow.h"

QFont newFont("Sans Serif", 16, QFont::Bold, false);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::change_Text(const QString &link)
{
    ui->label->setFont(newFont);
    ui->label->setText(link);
}
