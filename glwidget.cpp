#include "glwidget.h"
#include <QtCore/qmath.h>
#include <QVector3D>
#include <QDebug>
#include <qmath.h>
#include <QMouseEvent>

const double LARGE_SIDE = 1.6;
const qreal GL_PI = 3.1415926;

const qreal SCALE_MIN = .2;
const qreal SCALE_MAX = 3.;
const qreal HEIGHT_MIN = .5;
const qreal HEIGHT_MAX = 20.;
const qreal DIAMETER_MIN = 1;
const qreal DIAMETER_MAX = 5;
const int APPROXIMATION_MIN = 4.;
const int APPROXIMATION_MAX = 60.;
const qreal LIGHT_MIN = .5;
const qreal LIGHT_MAX = 5.;

const qreal STRETCH = 1.;

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    resize(300,300);
    xRotation = 0;
    yRotation = 0;
    zRotation = 0;
    scale = 2;
    Approximation = 5;
    borderLinesShowing = false;
}

void GLWidget::initializeGL() {
    qglClearColor(Qt::white);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_FLAT);
    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

    glEnable (GL_BLEND);
    //glEnable(GL_DIFFUSE);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glEnable(GL_NORMALIZE);
}

void GLWidget::paintGL() {
    //drawPiramide();
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);

    float ambience[4] = {0.3f, 0.3f, 0.3f, 1.0};
    glLightfv( GL_LIGHT0, GL_AMBIENT,  ambience);
    GLfloat light0_diffuse[] = {lightIntensity, lightIntensity, lightIntensity};
    GLfloat light0_direction[] = {lightIntensity, lightIntensity, 0.};

    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_direction);

    drawSurface();
}

void GLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, h, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
}

void GLWidget::mousePressEvent(QMouseEvent* pe)
{
    mousePos = pe->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent* pe)
{
    xRotation += (pe->y() - mousePos.y()) * 1.1 * GL_PI / 180;
    zRotation += (pe->x() - mousePos.x()) * 1.1 * GL_PI / 180;

    updateGL();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *pe)
{
}

void GLWidget::setApproximationPercent(int percent) {
    qreal diff = APPROXIMATION_MAX - APPROXIMATION_MIN;
    qreal one_perc = diff / 100;
    Approximation = APPROXIMATION_MIN + one_perc * percent;

    updateGL();
}

void GLWidget::setLightPercent(int percent) {
    qreal diff = LIGHT_MAX - LIGHT_MIN;
    qreal one_perc = diff / 100;
    lightIntensity = LIGHT_MIN + one_perc * percent;

    updateGL();
}

void GLWidget::setPolyFillState(bool status) {
    polyFillStatus = status;

    updateGL();
}

void GLWidget::drawAxis()
{
    glLineWidth(3.0f);

    glColor4f(1.00f, 0.00f, 0.00f, 1.0f);
    glBegin(GL_LINES);
        glVertex3f( 1.0f,  0.0f,  0.0f);
        glVertex3f(-1.0f,  0.0f,  0.0f);
    glEnd();

    QColor halfGreen(0, 128, 0, 255);
    qglColor(halfGreen);
    glBegin(GL_LINES);
        glVertex3f( 0.0f,  1.0f,  0.0f);
        glVertex3f( 0.0f, -1.0f,  0.0f);

        glColor4f(0.00f, 0.00f, 1.00f, 1.0f);
        glVertex3f( 0.0f,  0.0f,  1.0f);
        glVertex3f( 0.0f,  0.0f, -1.0f);
    glEnd();
}

void GLWidget::wheelEvent(QWheelEvent* pe)
{
    if ((pe->delta()) > 0) {
        scale *= 1.1;
    } else if ((pe->delta())<0) {
        scale /= 1.1;
    }
    updateGL();
}

qreal GLWidget::FuncX(qreal a, qreal b) {
    if (this->Points.size() < 4) {
        return 0.;
    }

    qreal p0 = this->Points[0].x();
    qreal p1 = this->Points[1].x();
    qreal p2 = this->Points[2].x();
    qreal p3 = this->Points[3].x();

    return (p0*(1.-a)*(1.-b) + p1*(1.-a)*b + p2*a*(1.-b) + p3*a*b) * STRETCH;
}

qreal GLWidget::FuncY(qreal a, qreal b) {
    if (this->Points.size() < 4) {
        return 0.;
    }

    qreal p0 = this->Points[0].y();
    qreal p1 = this->Points[1].y();
    qreal p2 = this->Points[2].y();
    qreal p3 = this->Points[3].y();

    return (p0*(1.-a)*(1.-b) + p1*(1.-a)*b + p2*a*(1.-b) + p3*a*b) * STRETCH;
}

qreal GLWidget::FuncZ(qreal a, qreal b) {
    if (this->Points.size() < 4) {
        return 0.;
    }

    qreal p0 = this->Points[0].z();
    qreal p1 = this->Points[1].z();
    qreal p2 = this->Points[2].z();
    qreal p3 = this->Points[3].z();

    return (p0*(1.-a)*(1.-b) + p1*(1.-a)*b + p2*a*(1.-b) + p3*a*b) * STRETCH;
}

void GLWidget::drawSurface() {
    if (this->Points.size() < 4) {
        return;
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glScalef(scale, scale, scale);
    glRotatef(xRotation, 1.0f, 0.0f, 0.0f);
    glRotatef(yRotation, 0.0f, 1.0f, 0.0f);
    glRotatef(zRotation, 0.0f, 0.0f, 1.0f);

    QColor clr(0, 64, 128, 255);
    qglColor(clr);

    qreal step = 1. / Approximation;
    if (borderLinesShowing) {
        glBegin(GL_LINE_LOOP);
        glVertex3f(Points[0].x(), Points[0].y(), Points[0].z());
        glVertex3f(Points[1].x(), Points[1].y(), Points[1].z());
        glVertex3f(Points[2].x(), Points[2].y(), Points[2].z());
        glVertex3f(Points[3].x(), Points[3].y(), Points[3].z());
        glEnd();
    }

    for (qreal a = step; a <= 1.; a += step) {
        for (qreal b = step; b <= 1.; b += step) {
            QVector3D current_poly[4];
            current_poly[0] = QVector3D(FuncX(a, b - step), FuncY(a, b - step), FuncZ(a, b - step));
            current_poly[1] = QVector3D(FuncX(a - step, b - step), FuncY(a - step, b - step), FuncZ(a - step, b - step));
            current_poly[2] = QVector3D(FuncX(a - step, b), FuncY(a - step, b), FuncZ(a - step, b));
            current_poly[3] = QVector3D(FuncX(a, b), FuncY(a, b), FuncZ(a, b));

            if (!polyFillStatus) {
                glBegin(GL_LINE_LOOP);
            } else {
                glBegin(GL_POLYGON);
            }
            glVertex3f(current_poly[0].x(), current_poly[0].y(), current_poly[0].z());
            glVertex3f(current_poly[1].x(), current_poly[1].y(), current_poly[1].z());
            glVertex3f(current_poly[2].x(), current_poly[2].y(), current_poly[2].z());
            glVertex3f(current_poly[3].x(), current_poly[3].y(), current_poly[3].z());
            glEnd();

            if (!polyFillStatus) {
                glBegin(GL_LINE_LOOP);
            } else {
                glBegin(GL_POLYGON);
            }
            glVertex3f(current_poly[3].x(), current_poly[3].y(), current_poly[3].z());
            glVertex3f(current_poly[2].x(), current_poly[2].y(), current_poly[2].z());
            glVertex3f(current_poly[1].x(), current_poly[1].y(), current_poly[1].z());
            glVertex3f(current_poly[0].x(), current_poly[0].y(), current_poly[0].z());
            glEnd();
        }
    }
}

void GLWidget::ApplyChanges(QVector3D *p) {
    bool is_same = false;
    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 4; j++) {
            if (p[i] == p[j]) {
                is_same = true;
                break;
            }
        }
    }

    Points.clear();

    if (!is_same) {
        Points.push_back(p[0]);
        Points.push_back(p[1]);
        Points.push_back(p[2]);
        Points.push_back(p[3]);
    }

    updateGL();
}

void GLWidget::SetBorderLinesShowing(bool val) {
    borderLinesShowing = val;
    updateGL();
}
