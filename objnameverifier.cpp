#include "objnameverifier.h"
#include <QGraphicsItem>

ObjNameVerifier::ObjNameVerifier(const QMap<QString, QGraphicsItem*> *const mapObjects):
        _mapObjects(mapObjects)
{
}

ObjNameVerifier::~ObjNameVerifier()
{
}

bool ObjNameVerifier::verify(const QString &value) const
{
    if(_mapObjects->contains(value))
        return false;
    else
        return true;
}
