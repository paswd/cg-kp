#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QVector3D>
#include <QVector>

class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = 0);
    void setScalePercent(int percent);
    void setHeightPercent(int percent);
    void setDiameterPercent(int percent);
    void setApproximationPercent(int percent);
    void setLightPercent(int percent);
    void setPolyFillState(bool status);
    void animationRealize(void);
    void animationClear(void);
    void ApplyChanges(QVector3D *p);
    void SetBorderLinesShowing(bool val);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    void mousePressEvent(QMouseEvent* pe);
    void mouseMoveEvent(QMouseEvent* pe);
    void mouseReleaseEvent(QMouseEvent* pe);
    void wheelEvent(QWheelEvent *);


private:
    //qreal currentTimeValue;
    int xRotation, yRotation, zRotation;
    bool polyFillStatus;
    QPoint mousePos;
    qreal scale;
    qreal currentScale;
    int Approximation;
    //qreal functionLen;
    //qreal heightParam;
    qreal lightIntensity;
    bool borderLinesShowing;

    QVector <QVector3D> Points;
    qreal FuncX(qreal a, qreal b);
    qreal FuncY(qreal a, qreal b);
    qreal FuncZ(qreal a, qreal b);
    //QVector3D getVector(qreal pos_in, qreal angle);
    //QVector3D getNormalZero(qreal pos_in);

    void drawAxis();
    void drawParaboloide();
    void drawSurface();
};

#endif // GLWIDGET_H
