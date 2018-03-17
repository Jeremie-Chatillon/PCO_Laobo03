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
    abort = true;
    condition.wakeOne();
    mutex.unlock();


    wait();
}

DelegationTread::DelegationTread(double centerX, double centerY, double scaleFactor, QSize resultSize, QImage* image, int maxIterations, int y0, int y1, bool* restart, bool* abort, uint* colormap, QObject *parent)
    :QThread(parent), centerX(centerX), centerY(centerY), scaleFactor(scaleFactor), resultSize(resultSize), image(image), maxIterations(maxIterations), y0(y0), y1(y1), restart(restart), abort(abort), colormap(colormap)
{
}

void DelegationTread::run(){

    mutex.lock();
    QSize resultSize = this->resultSize;
    double scaleFactor = this->scaleFactor;
    double centerX = this->centerX;
    double centerY = this->centerY;

    mutex.unlock();

    int halfWidth = resultSize.width() / 2;
    int halfHeight = resultSize.height() / 2;

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
            } while (numIterations < maxIterations);

            if (numIterations < maxIterations) {
                *scanLine++ = colormap[numIterations % ColormapSize];
            } else {
                *scanLine++ = qRgb(0, 0, 0);
            }
        }
    }

}
