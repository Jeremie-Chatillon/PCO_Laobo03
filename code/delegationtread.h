#ifndef DELEGATIONTREAD_H
#define DELEGATIONTREAD_H

#include <QMutex>
#include <QSize>
#include <QThread>
#include <QWaitCondition>


QT_BEGIN_NAMESPACE
class QImage;
QT_END_NAMESPACE

//! [0]
class DelegationTread: public QThread{
    Q_OBJECT
public:
    //delegationTread(QObject *parent, bool* abort, bool restart, uint** colorMap, QMutex* mutex);
    ~DelegationTread();
    DelegationTread(QObject* parent = 0);
    //static void initialize(unsigned int nbThreads);

    void render(double centerX, double centerY, double scaleFactor, QSize resultSize, QImage* image, int pass, int y0, int y1, bool* restart, bool* abort);

signals:
    void renderedImage(const QImage &image, double scaleFactor);
public slots:

protected:

    /**
     * \brief run Méthode lancée par le thread
     */
    void run() Q_DECL_OVERRIDE;

private:
    uint rgbFromWaveLength(double wave);


    QMutex mutex;
    QWaitCondition condition;
    double centerX;
    double centerY;
    double scaleFactor;
    QSize resultSize;
    bool* restart;
    bool* abort;
    size_t id;
    int y0;
    int y1;


    QImage* image;

    int pass;

    size_t* nbTreads;

    enum { ColormapSize = 512 };
    uint colormap[ColormapSize];

};

#endif // DELEGATIONTREAD_H
