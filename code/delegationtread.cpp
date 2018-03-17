#include "delegationtread.h"

//delegationTread::delegationTread(QObject *parent, bool* abort, bool* restart, size** colorMap, Mutex* mutex, size_t* nbThreads, size_t id)
//    : QThread(parent), abort(abort), restart(restart), colorMap(colorMap), mutex(mutex), nbTreads(nbThreads), id (id)
//{

//}

#include <iostream>
#include <QtWidgets>

DelegationTread::~DelegationTread()
{
    mutex.lock();
    //abort = true;
    condition.wakeOne();
    mutex.unlock();

    wait();
}

DelegationTread::DelegationTread(QObject *parent)
    :QThread(parent)
{

    for (int i = 0; i < ColormapSize; ++i)
        colormap[i] = rgbFromWaveLength(380.0 + (i * 400.0 / ColormapSize));
    //restart = false;
    //abort = false;
}

void DelegationTread::render(double centerX, double centerY, double scaleFactor,
                          QSize resultSize, QImage* image, int pass, int y0, int y1, bool* restart, bool* abort)
{


    this->restart = restart;
    this->abort = abort;
    this->centerX = centerX;
    this->centerY = centerY;
    this->scaleFactor = scaleFactor;
    this->resultSize = resultSize;

    this->y0 = y0;
    this->y1 = y1;

    if (!isRunning()) {
        start(LowPriority);
    } else {
        //restart = true;
        condition.wakeOne();
    }

    this->image = image;
    this->pass = pass;
}

void DelegationTread::run(){

    //mutex.lock();
    // Stoppper tout les threads
    QSize resultSize = this->resultSize;
    double scaleFactor = this->scaleFactor;
    double centerX = this->centerX;
    double centerY = this->centerY;
    //mutex.unlock();

    int halfWidth = resultSize.width() / 2;
    int halfHeight = resultSize.height() / 2;






    const int MaxIterations = (1 << (2 * pass + 6)) + 32;

    const int Limit = 4;

    for (int y = y0; y < y1; ++y) {
        if (*restart)
            break;
        if (*abort)
            return;

        QRgb *scanLine =
                reinterpret_cast<QRgb *>(image->scanLine(y + halfHeight));
        double ay = centerY + (y * scaleFactor);

        for (int x = -halfWidth; x < halfWidth; ++x) {
            double ax = centerX + (x * scaleFactor);
            double a1 = ax;
            double b1 = ay;
            int numIterations = 0;

            do {
                ++numIterations;
                double a2 = (a1 * a1) - (b1 * b1) + ax;
                double b2 = (2 * a1 * b1) + ay;
                if ((a2 * a2) + (b2 * b2) > Limit)
                    break;

                ++numIterations;
                a1 = (a2 * a2) - (b2 * b2) + ax;
                b1 = (2 * a2 * b2) + ay;
                if ((a1 * a1) + (b1 * b1) > Limit)
                    break;
            } while (numIterations < MaxIterations);

            if (numIterations < MaxIterations) {
                *scanLine++ = colormap[numIterations % ColormapSize];
            } else {
                *scanLine++ = qRgb(0, 0, 0);
            }
        }
    }

}


uint DelegationTread::rgbFromWaveLength(double wave)
{
    double r = 0.0;
    double g = 0.0;
    double b = 0.0;

    if (wave >= 380.0 && wave <= 440.0) {
        r = -1.0 * (wave - 440.0) / (440.0 - 380.0);
        b = 1.0;
    } else if (wave >= 440.0 && wave <= 490.0) {
        g = (wave - 440.0) / (490.0 - 440.0);
        b = 1.0;
    } else if (wave >= 490.0 && wave <= 510.0) {
        g = 1.0;
        b = -1.0 * (wave - 510.0) / (510.0 - 490.0);
    } else if (wave >= 510.0 && wave <= 580.0) {
        r = (wave - 510.0) / (580.0 - 510.0);
        g = 1.0;
    } else if (wave >= 580.0 && wave <= 645.0) {
        r = 1.0;
        g = -1.0 * (wave - 645.0) / (645.0 - 580.0);
    } else if (wave >= 645.0 && wave <= 780.0) {
        r = 1.0;
    }

    double s = 1.0;
    if (wave > 700.0)
        s = 0.3 + 0.7 * (780.0 - wave) / (780.0 - 700.0);
    else if (wave <  420.0)
        s = 0.3 + 0.7 * (wave - 380.0) / (420.0 - 380.0);

    r = pow(r * s, 0.8);
    g = pow(g * s, 0.8);
    b = pow(b * s, 0.8);
    return qRgb(int(r * 255), int(g * 255), int(b * 255));
}
