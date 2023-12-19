
#ifndef REPORTPARSER_H
#define REPORTPARSER_H


#include <QObject>

class QString;
class QJsonArray;
class QJsonObject;
class QJsonDocument;
class ReportParser : public QObject
{
    Q_OBJECT
public:
    explicit ReportParser(QObject *parent = nullptr);
    ~ReportParser()=default;
    void parseDocument(const QJsonDocument& reportDocument);

signals:
    void parseFinishedSignal(bool success,const QJsonObject& warningsResultObject,const QString& lastError);
};

#endif // REPORTPARSER_H
