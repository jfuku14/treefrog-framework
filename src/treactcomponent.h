#ifndef TREACTCOMPONENT_H
#define TREACTCOMPONENT_H

#include <QString>
#include <QDateTime>
#include <TGlobal>

class TJSContext;
class QJSValue;
class TJSLoader;

class T_CORE_EXPORT TReactComponent
{
public:
    TReactComponent(const QString &moduleName, const QStringList &searchPaths);
    virtual ~TReactComponent() { }

    void import(const QString &moduleName);
    void import(const QString &defaultMember, const QString &moduleName);
    QString renderToString(const QString &component);
    QDateTime loadedDateTime() const { return loadedTime; }

private:
    TJSLoader *jsLoader;
    QDateTime loadedTime;

    Q_DISABLE_COPY(TReactComponent)
};

#endif // TREACTCOMPONENT_H
