#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->widget->setApproximationPercent(ui->SliderApproximation->value());
    ui->widget->setLightPercent(ui->SliderLight->value());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_SliderApproximation_sliderMoved(int position)
{
    ui->widget->setApproximationPercent(position);
}

void MainWindow::on_SliderLight_sliderMoved(int position)
{
    ui->widget->setLightPercent(position);
}

void MainWindow::on_isPolyFill_stateChanged(int arg1)
{
     ui->widget->setPolyFillState(arg1 != 0);
}

void MainWindow::on_Apply_clicked()
{
    qreal x1 = ui->p1x->text().toDouble();
    qreal y1 = ui->p1y->text().toDouble();
    qreal z1 = ui->p1z->text().toDouble();

    qreal x2 = ui->p2x->text().toDouble();
    qreal y2 = ui->p2y->text().toDouble();
    qreal z2 = ui->p2z->text().toDouble();

    qreal x3 = ui->p3x->text().toDouble();
    qreal y3 = ui->p3y->text().toDouble();
    qreal z3 = ui->p3z->text().toDouble();

    qreal x4 = ui->p4x->text().toDouble();
    qreal y4 = ui->p4y->text().toDouble();
    qreal z4 = ui->p4z->text().toDouble();

    QVector3D p[4];

    p[0] = QVector3D(x1, y1, z1);
    p[1] = QVector3D(x2, y2, z2);
    p[2] = QVector3D(x3, y3, z3);
    p[3] = QVector3D(x4, y4, z4);

    ui->widget->ApplyChanges(p);
}


void MainWindow::on_isBorderLines_stateChanged(int arg1)
{
    ui->widget->SetBorderLinesShowing(arg1 != 0);
}
