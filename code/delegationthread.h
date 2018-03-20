#ifndef DELEGATIONTREAD_H
#define DELEGATIONTREAD_H

#include <QSize>
#include <QThread>



QT_BEGIN_NAMESPACE
class QImage;
QT_END_NAMESPACE

/**
 * @brief The DelegationThread class
 * Classe permettant les les calculs de la courbe de mandelbrot sur une certaine
 *  proportion y d'une Qimage
 */
class DelegationThread: public QThread{
    Q_OBJECT
public:
    DelegationThread(double centerX, double centerY, double scaleFactor,
                     QSize* resultSize, QImage* image, int pass,
                     int y0, int y1, bool* restart, bool* abort,uint* colormap,
                     QObject* parent = 0);

protected:

    /**
     * \brief Méthode lancée par le thread pendant son execution.
     *  effectue les calculs de la courbe de mandelbrot sur une certaine
     *  proportion y d'une Qimage
     */
    void run() Q_DECL_OVERRIDE;

private:

    double centerX;         // Centre X de l'image à traiter
    double centerY;         // Centre Y de l'image à traiter
    double scaleFactor;     // Facteur de zoom sur l'image à traiter
    QSize* resultSize;      // Dimentions de l'image à traiter
    QImage* image;          // Image à traitrer
    int pass;               // Nombre d'itéation de calculs à effectuer
    int y0;                 // Début Y de la zone de calculs sur l'image
    int y1;                 // Fin  Y de la zone de calculs sur l'image

    bool* restart;          // Variable sonde qui sert à finir la fonction run()
    bool* abort;            // Variable sonde qui sert à finir la fonction run()

    enum { ColormapSize = 512 }; // Nombre de couleurs
    uint* colormap;         // Panneau de couleurs

};

#endif // DELEGATIONTREAD_H
