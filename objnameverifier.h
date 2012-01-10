#ifndef OBJNAMEVERIFIER_H
#define OBJNAMEVERIFIER_H

#include "verifier.h"
#include <QMap>

class QGraphicsItem;

class ObjNameVerifier : public Verifier
{
public:
    ObjNameVerifier(const QMap<QString, QGraphicsItem*> *const _mapObjects);
    ~ObjNameVerifier();
    virtual bool verify(const QString &value) const;

private:
    //This class don't delete the object.
    const QMap<QString, QGraphicsItem*> *const _mapObjects;
};

#endif // OBJNAMEVERIFIER_H
