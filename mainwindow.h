#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_SliderApproximation_sliderMoved(int position);

    void on_isPolyFill_stateChanged(int arg1);

    void on_SliderLight_sliderMoved(int position);

    void on_Apply_clicked();

    void on_isBorderLines_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
