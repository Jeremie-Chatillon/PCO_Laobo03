#ifndef DELEGATIONTREAD_H
#define DELEGATIONTREAD_H

#include <QMutex>
#include <QSize>
#include <QThread>
#include <QWaitCondition>
#include <QList>

QT_BEGIN_NAMESPACE
class QImage;
QT_END_NAMESPACE

//! [0]
class DelegationTread: public Qthread {
    Q_OBJECT
public:
    //delegationTread(QObject *parent, bool* abort, bool restart, uint** colorMap, QMutex* mutex);
    ~DelegationTread();
    DelegationTread(QObject* parent = 0);
    //static void initialize(unsigned int nbThreads);

    void render(double centerX, double centerY, double scaleFactor, QSize resultSize);

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

    QWaitCondition condition;
    double centerX;
    double centerY;
    double scaleFactor;
    QSize resultSize;
    bool restart;
    bool abort;
    size_t id;

    size_t* nbTreads;

    enum { ColormapSize = 512 };
    uint colormap[ColormapSize];

};

#endif // DELEGATIONTREAD_H
