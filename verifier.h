#ifndef VERIFIER_H
#define VERIFIER_H

#include <QString>

class Verifier
{
protected:
    Verifier(){}
public:
    virtual ~Verifier(){}
    virtual bool verify(const QString &value) const = 0;
};

#endif // VERIFIER_H
