/**
  * PCO_Labo03
  *
  * Classe permettant la délégation de calculs de la courbe de mandelbrot
  *
  * Autors : Châtillon, Smith
  */


#include "delegationthread.h"

#include <iostream>
#include <QtWidgets>


DelegationThread::DelegationThread(double centerX, double centerY,
                    double scaleFactor, QSize* resultSize, QImage* image,
                    int pass, int y0, int y1, bool* restart,
                    bool* abort, uint* colormap, QObject *parent)
    :QThread(parent), centerX(centerX), centerY(centerY),
        scaleFactor(scaleFactor), resultSize(resultSize), image(image),
        pass(pass), y0(y0), y1(y1), restart(restart),
        abort(abort), colormap(colormap)
{
}


void DelegationThread::run(){

    int halfWidth = resultSize->width() / 2;
    int halfHeight = resultSize->height() / 2;

    const int Limit = 4;

    // Seul changement fonctionnel dans la méthode. Ne parcout plus toutes la
    //  hauteur de l'image mais seulement une proportion de y0 à y1
    for (int y = y0; y < y1; ++y) {
        // Conditions pour quitter la méthode prématurément
        if (*restart)
            return;
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
            } while (numIterations < pass);

            if (numIterations < pass) {
                *scanLine++ = colormap[numIterations % ColormapSize];
            } else {
                *scanLine++ = qRgb(0, 0, 0);
            }
        }
    }

}
