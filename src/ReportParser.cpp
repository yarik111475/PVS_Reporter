
#include "ReportParser.h"
#include <QMap>
#include <QString>
#include <QVector>
#include <QStringList>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <map>
#include <algorithm>

ReportParser::ReportParser(QObject *parent)
    : QObject{parent}
{
}

void ReportParser::parseDocument(const QJsonDocument &reportDocument)
{
    QJsonObject warningsResultObject {};
    const QString tag {"warnings"};
    const QJsonObject reportObject {reportDocument.object()};
    if(reportObject.contains(tag) && reportObject.value(tag).isArray()){
        const QJsonArray warningsArray =reportObject.value(tag).toArray();
        QVector<QJsonObject> warningsObjects {};
        std::for_each(warningsArray.begin(),warningsArray.end(),[&](const QJsonValue& jsonValue){
            const QJsonObject jsonObject {jsonValue.toObject()};
            warningsObjects.push_back(jsonObject);
        });
        const auto allWarningsCount {std::distance(warningsObjects.begin(),warningsObjects.end())};
        warningsResultObject.insert("allWarningsCount",allWarningsCount);

        const QJsonArray projectsArray=warningsObjects.first().value("projects").toArray();
        const QString projectName {projectsArray.first().toString()};
        warningsResultObject.insert("projectName",projectName);

        {//level-1 partitioning
            const int level {1};
            const QString tag {"level"};
            auto begin {warningsObjects.begin()};
            auto end {std::partition(warningsObjects.begin(),warningsObjects.end(),[&](const QJsonValue& jsonValue){
                const QJsonObject jsonObject {jsonValue.toObject()};
                return jsonObject.value(tag).toInt()==level;
            })};
            const auto level1WarningsCount {std::distance(begin,end)};
            warningsResultObject.insert("level1WarningsCount",level1WarningsCount);

            QMap<QString,int> filesMap {};
            while(begin!=end){
                const QJsonObject jsonObject {*begin};
                const int level {jsonObject.value(tag).toInt()};
                const QJsonArray positionObjects=jsonObject.value("positions").toArray();
                const QJsonObject positionObject {positionObjects.first().toObject()};

                const QString filePath {positionObject.value("file").toString()};
                const QString fileName {filePath.split('/').last()};
                if(fileName.startsWith("moc_")){
                    ++begin;
                    continue;
                }

                ++filesMap[fileName];
                ++begin;
            }

            QJsonArray levelWariningsArray {};
            auto mapBegin {filesMap.begin()};
            while(mapBegin!=filesMap.end()){
                const QJsonObject levelWarningsObject{
                    {"fileName",mapBegin.key()},
                    {"warningsCount",mapBegin.value()}
                };
                levelWariningsArray.push_back(levelWarningsObject);
                ++mapBegin;
            }
            warningsResultObject.insert("level1WariningsArray",levelWariningsArray);
        }
        {//level-2 partitioning
            const int level {2};
            const QString tag {"level"};
            auto begin {warningsObjects.begin()};
            auto end {std::partition(warningsObjects.begin(),warningsObjects.end(),[&](const QJsonValue& jsonValue){
                const QJsonObject jsonObject {jsonValue.toObject()};
                return jsonObject.value(tag).toInt()==level;
            })};
            const auto level2WarningsCount {std::distance(begin,end)};
            warningsResultObject.insert("level2WarningsCount",level2WarningsCount);

            QMap<QString,int> filesMap {};
            while(begin!=end){
                const QJsonObject jsonObject {*begin};
                const int level {jsonObject.value(tag).toInt()};
                const QJsonArray positionObjects=jsonObject.value("positions").toArray();
                const QJsonObject positionObject {positionObjects.first().toObject()};

                const QString filePath {positionObject.value("file").toString()};
                const QString fileName {filePath.split('/').last()};
                if(fileName.startsWith("moc_")){
                    ++begin;
                    continue;
                }

                ++filesMap[fileName];
                ++begin;
            }

            QJsonArray levelWariningsArray {};
            auto mapBegin {filesMap.begin()};
            while(mapBegin!=filesMap.end()){
                const QJsonObject levelWarningsObject{
                    {"fileName",mapBegin.key()},
                    {"warningsCount",mapBegin.value()}
                };
                levelWariningsArray.push_back(levelWarningsObject);
                ++mapBegin;
            }
            warningsResultObject.insert("level2WariningsArray",levelWariningsArray);
        }
        {//level-3 partitioning
            const int level {3};
            const QString tag {"level"};
            auto begin {warningsObjects.begin()};
            auto end {std::partition(warningsObjects.begin(),warningsObjects.end(),[&](const QJsonValue& jsonValue){
                const QJsonObject jsonObject {jsonValue.toObject()};
                return jsonObject.value(tag).toInt()==level;
            })};
            const auto level3WarningsCount {std::distance(begin,end)};
            warningsResultObject.insert("level3WarningsCount",level3WarningsCount);

            QMap<QString,int> filesMap {};
            while(begin!=end){
                const QJsonObject jsonObject {*begin};
                const int level {jsonObject.value(tag).toInt()};
                const QJsonArray positionObjects=jsonObject.value("positions").toArray();
                const QJsonObject positionObject {positionObjects.first().toObject()};

                const QString filePath {positionObject.value("file").toString()};
                const QString fileName {filePath.split('/').last()};
                if(fileName.startsWith("moc_")){
                    ++begin;
                    continue;
                }

                ++filesMap[fileName];
                ++begin;
            }

            QJsonArray levelWariningsArray {};
            auto mapBegin {filesMap.begin()};
            while(mapBegin!=filesMap.end()){
                const QJsonObject levelWarningsObject{
                    {"fileName",mapBegin.key()},
                    {"warningsCount",mapBegin.value()}
                };
                levelWariningsArray.push_back(levelWarningsObject);
                ++mapBegin;
            }
            warningsResultObject.insert("level3WariningsArray",levelWariningsArray);
        }
        emit parseFinishedSignal(true,warningsResultObject,QString{});
        return;
    }
    emit parseFinishedSignal(false,warningsResultObject,"Parse report file fail!");
}

