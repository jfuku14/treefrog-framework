#ifndef TWEBAPPLICATION_H
#define TWEBAPPLICATION_H

#ifdef TF_USE_GUI_MODULE
# include <QApplication>
#else
# include <QCoreApplication>
#endif

#include <TGlobal>
#include "qplatformdefs.h"
#include <QVector>
#include <QBasicTimer>
#include <QVariant>

class QTextCodec;
using TSettings = QMap<QString, QVariant>;


class T_CORE_EXPORT TWebApplication
#ifdef TF_USE_GUI_MODULE
    : public QApplication
#else
    : public QCoreApplication
#endif
{
    Q_OBJECT
public:
    enum MultiProcessingModule {
        Invalid = 0,
        Thread,
        Hybrid,
    };

    TWebApplication(int &argc, char **argv);
    ~TWebApplication();

    int exec();
    QString webRootPath() const { return _webRootAbsolutePath; }
    bool webRootExists() const;
    QString publicPath() const;
    QString configPath() const;
    QString libPath() const;
    QString logPath() const;
    QString pluginPath() const;
    QString tmpPath() const;
    QString databaseEnvironment() const { return _dbEnvironment; }
    void setDatabaseEnvironment(const QString &environment);

    bool appSettingsFileExists() const;
    QString appSettingsFilePath() const;
    const TSettings &sqlDatabaseSettings(int databaseId) const;
    int sqlDatabaseSettingsCount() const;
    bool isSqlDatabaseAvailable() const;
    int databaseIdForInternalUse() const;
    const TSettings &mongoDbSettings() const;
    bool isMongoDbAvailable() const;
    const TSettings &redisSettings() const;
    bool isRedisAvailable() const;
    const TSettings &loggerSettings() const { return _loggerSetting; }
    const TSettings &validationSettings() const { return _validationSetting; }
    QString validationErrorMessage(int rule) const;
    QByteArray internetMediaType(const QString &ext, bool appendCharset = false);
    MultiProcessingModule multiProcessingModule() const;
    int maxNumberOfAppServers() const;
    int maxNumberOfThreadsPerAppServer() const;
    QString routesConfigFilePath() const;
    QString systemLogFilePath() const;
    QString accessLogFilePath() const;
    QString sqlQueryLogFilePath() const;
    QTextCodec *codecForInternal() const { return _codecInternal; }
    QTextCodec *codecForHttpOutput() const { return _codecHttp; }
    int applicationServerId() const { return _appServerId; }
    QThread *databaseContextMainThread() const;
    const QVariantMap &getConfig(const QString &configName);
    QVariant getConfigValue(const QString &configName, const QString &key, const QVariant &defaultValue = QVariant());
    QString cacheBackend() const;

#if defined(Q_OS_UNIX)
    void watchUnixSignal(int sig, bool watch = true);
    void ignoreUnixSignal(int sig, bool ignore = true);
#endif

#if defined(Q_OS_WIN)
    void watchConsoleSignal();
    void ignoreConsoleSignal();
    void watchLocalSocket();
    static bool sendLocalCtrlMessage(const QByteArray &msg,  int targetProcess);

private slots:
    void recvLocalSocket();
#endif // Q_OS_WIN

protected:
    void timerEvent(QTimerEvent *event);
    static int signalNumber();

private:
    QString _webRootAbsolutePath;
    QString _dbEnvironment;
    QVector<TSettings> _sqlSettings;
    TSettings _mongoSetting;
    TSettings _redisSetting;
    TSettings _loggerSetting;
    TSettings _validationSetting;
    TSettings _mediaTypes;
    QTextCodec *_codecInternal  {nullptr};
    QTextCodec *_codecHttp  {nullptr};
    int _appServerId  {-1};
    QBasicTimer _timer;
    mutable MultiProcessingModule _mpm  {Invalid};
    QMap<QString, QVariantMap> _configMap;

    static void resetSignalNumber();

    T_DISABLE_COPY(TWebApplication)
    T_DISABLE_MOVE(TWebApplication)
};


/*!
  Sets the database environment to \a environment.
  \sa databaseEnvironment()
*/
inline void TWebApplication::setDatabaseEnvironment(const QString &environment)
{
    _dbEnvironment = environment;
}


#if defined(Q_OS_WIN)
#include <QAbstractNativeEventFilter>

class T_CORE_EXPORT TNativeEventFilter : public QAbstractNativeEventFilter
{
public:
    virtual bool nativeEventFilter(const QByteArray &eventType, void *message, long *);
};

#endif // Q_OS_WIN

#endif // TWEBAPPLICATION_H
