#include "delegationtread.h"

#include <iostream>
#include <QtWidgets>

DelegationTread::~DelegationTread()
{
    mutex.lock();
    condition.wakeOne();
    mutex.unlock();


    wait();
}

DelegationTread::DelegationTread(double centerX, double centerY, double scaleFactor, QSize* resultSize, QImage* image, int maxIterations, int y0, int y1, bool* restart, bool* abort, uint* colormap, QObject *parent)
    :QThread(parent), centerX(centerX), centerY(centerY), scaleFactor(scaleFactor), resultSize(resultSize), image(image), maxIterations(maxIterations), y0(y0), y1(y1), restart(restart), abort(abort), colormap(colormap)
{
}

/**
 * @brief DelegationTread::run
 * Calcul de madelbrot sur un proportion y de l'image.
 * Pas besoin de faire une copie local des valeurs de calculs car elles ne peuvent être modifiées pendant l'execution.
 *
 */
void DelegationTread::run(){

    int halfWidth = resultSize->width() / 2;
    int halfHeight = resultSize->height() / 2;

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
