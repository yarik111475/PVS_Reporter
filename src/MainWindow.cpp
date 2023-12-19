
#include "MainWindow.h"
#include "ReportParser.h"
#include "LevelModel.h"
#include "qtcsv/writer.h"
#include "qtcsv/stringdata.h"

#include <QFile>
#include <QLabel>
#include <QTableView>
#include <QHeaderView>
#include <QLineEdit>
#include <QGroupBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include <QTextStream>
#include <tuple>

struct Errors
{
    int High;
    int Medium;
    int Low;
};

void MainWindow::openSlot()
{
    const QString filenameFilter {"PVS report files (*.json)"};
    const QString reportFilename {QFileDialog::getOpenFileName(this,QObject::tr("Open file"),"",filenameFilter)};
    if(!reportFilename.isEmpty()){
        QFile file {reportFilename};
        if(file.open(QIODevice::ReadOnly)){
            const QByteArray bArray {file.readAll()};
            file.close();
            const QJsonDocument reportDocument {QJsonDocument::fromJson(bArray)};
            if(!reportDocument.isEmpty()){
                parserPtr_->parseDocument(reportDocument);
                filenameLineEditPtr_->setText(reportFilename);
            }
        }
    }
}

void MainWindow::saveSlot()
{ 
    if(!warningsResultObject_.isEmpty()){
        const QString filter {"CSV files (*.csv)"};
        const QString filename {QFileDialog::getSaveFileName(this,QObject::tr("Save file"),"",filter)};
        if(!filename.isEmpty()){
            const QString projectName {warningsResultObject_.value("projectName").toString()};

            const int level1Warnings {warningsResultObject_.value("level1WarningsCount").toInt()};
            const int level2Warnings {warningsResultObject_.value("level2WarningsCount").toInt()};
            const int level3Warnings {warningsResultObject_.value("level3WarningsCount").toInt()};

            QJsonArray level1ModelData=warningsResultObject_.value("level1WariningsArray").toArray();
            QJsonArray level2ModelData=warningsResultObject_.value("level2WariningsArray").toArray();
            QJsonArray level3ModelData=warningsResultObject_.value("level3WariningsArray").toArray();

            QMap<QString,Errors> errorsMap {};
            for(const auto& jsonValue: level1ModelData){
                const QString fileName {jsonValue.toObject().value("fileName").toString()};
                const int warnings {jsonValue.toObject().value("warningsCount").toInt()};
                errorsMap[fileName].High+=warnings;
            }
            for(const auto& jsonValue: level2ModelData){
                const QString fileName {jsonValue.toObject().value("fileName").toString()};
                const int warnings {jsonValue.toObject().value("warningsCount").toInt()};
                errorsMap[fileName].Medium+=warnings;
            }
            for(const auto& jsonValue: level3ModelData){
                const QString fileName {jsonValue.toObject().value("fileName").toString()};
                const int warnings {jsonValue.toObject().value("warningsCount").toInt()};
                errorsMap[fileName].Low+=warnings;
            }

            QtCSV::StringData stringData {};
            const QStringList titles {"Проект","Компонент","Уровень ошибок","","","Количество файлов"};
            stringData.addRow(titles);
            stringData.addRow({"Операционная система: MOS",projectName,"High","Medium","Low",QString::number(errorsMap.size())});
            stringData.addRow({"","",QString::number(level1Warnings),QString::number(level2Warnings),QString::number(level3Warnings)});
            stringData.addEmptyRow();
            stringData.addRow({"Файл","Уровень ошибок (шт.)","",""});
            stringData.addRow({"","High","Medium","Low"});

            auto begin {errorsMap.begin()};
            while(begin!=errorsMap.end()){
                stringData.addRow({begin.key(),QString::number(begin.value().High),
                                   QString::number(begin.value().Medium),
                                   QString::number(begin.value().Low)});
                ++begin;
            }
            QtCSV::Writer::write(filename,stringData);
            QMessageBox::information(this,QObject::tr("Info"),QObject::tr("csv file created success"));
        }
    }
}

void MainWindow::parseFinishedSlot(bool success, const QJsonObject &warningsResultObject, const QString &lastError)
{
    if(success){
        warningsResultObject_=warningsResultObject;

        QJsonArray level1ModelData=warningsResultObject_.value("level1WariningsArray").toArray();
        level1ModelPtr_->setModelData(level1ModelData);

        QJsonArray level2ModelData=warningsResultObject_.value("level2WariningsArray").toArray();
        level2ModelPtr_->setModelData(level2ModelData);

        QJsonArray level3ModelData=warningsResultObject_.value("level3WariningsArray").toArray();
        level3ModelPtr_->setModelData(level3ModelData);

        const QString projectName {warningsResultObject_.value("projectName").toString()};
        projectTxtPtr_->setText(projectName);

        const int allWarnings {warningsResultObject_.value("allWarningsCount").toInt()};
        allTxtPtr_->setText(QString::number(allWarnings));

        const int level1Warnings {warningsResultObject_.value("level1WarningsCount").toInt()};
        level1TxtPtr_->setText(QString::number(level1Warnings));

        const int level2Warnings {warningsResultObject_.value("level2WarningsCount").toInt()};
        level2TxtPtr_->setText(QString::number(level2Warnings));

        const int level3Warnings {warningsResultObject_.value("level3WarningsCount").toInt()};
        level3TxtPtr_->setText(QString::number(level3Warnings));
    }
    else{
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow{parent},
    projectTxtPtr_{new QLabel},
    allTxtPtr_{new QLabel},
    level1TxtPtr_{new QLabel},
    level2TxtPtr_{new QLabel},
    level3TxtPtr_{new QLabel},
    level1ViewPtr_{new QTableView},
    level2ViewPtr_{new QTableView},
    level3ViewPtr_{new QTableView},
    level1ModelPtr_{new LevelModel},
    level2ModelPtr_{new LevelModel},
    level3ModelPtr_{new LevelModel},
    parserPtr_{new ReportParser},
    filenameLineEditPtr_{new QLineEdit},
    openButtonPtr_{new QPushButton{QObject::tr("Open")}},
    saveButtonPtr_{new QPushButton{QObject::tr("Save")}}
{
    filenameLineEditPtr_->setReadOnly(true);
    QObject::connect(openButtonPtr_,&QPushButton::clicked,
                     this,&MainWindow::openSlot);
    QObject::connect(saveButtonPtr_,&QPushButton::clicked,
                     this,&MainWindow::saveSlot);

    QHBoxLayout* topHBOxLayoutPtr {new QHBoxLayout};
    topHBOxLayoutPtr->addWidget(new QLabel{QObject::tr("Report file path:")});
    topHBOxLayoutPtr->addWidget(filenameLineEditPtr_);
    topHBOxLayoutPtr->addWidget(openButtonPtr_);
    topHBOxLayoutPtr->addWidget(saveButtonPtr_);

    QGroupBox* allGroupBoxPtr {new QGroupBox{QObject::tr("Base info")}};
    QFormLayout* allFormLayoutPtr {new QFormLayout};
    allFormLayoutPtr->addRow(new QLabel{QObject::tr("Project:")},projectTxtPtr_);
    allFormLayoutPtr->addRow(new QLabel(QObject::tr("All warnings:")),allTxtPtr_);
    allFormLayoutPtr->addRow(new QLabel(QObject::tr("Level-1 warnings:")),level1TxtPtr_);
    allFormLayoutPtr->addRow(new QLabel(QObject::tr("Level-2 warnings:")),level2TxtPtr_);
    allFormLayoutPtr->addRow(new QLabel(QObject::tr("Level-3 warnings:")),level3TxtPtr_);
    allGroupBoxPtr->setLayout(allFormLayoutPtr);

    QGroupBox* level1GroupBoxPtr {new QGroupBox{QObject::tr("Level-1")}};
    QVBoxLayout* level1VBoxLayoutPtr {new QVBoxLayout};
    level1VBoxLayoutPtr->addWidget(level1ViewPtr_);
    level1GroupBoxPtr->setLayout(level1VBoxLayoutPtr);

    QGroupBox* level2GroupBoxPtr {new QGroupBox{QObject::tr("Level-2")}};
    QVBoxLayout* level2VBoxLayoutPtr {new QVBoxLayout};
    level2VBoxLayoutPtr->addWidget(level2ViewPtr_);
    level2GroupBoxPtr->setLayout(level2VBoxLayoutPtr);

    QGroupBox* level3GroupBoxPtr {new QGroupBox{QObject::tr("Level-3")}};
    QVBoxLayout* level3VBoxLayoutPtr {new QVBoxLayout};
    level3VBoxLayoutPtr->addWidget(level3ViewPtr_);
    level3GroupBoxPtr->setLayout(level3VBoxLayoutPtr);

    QVBoxLayout* levelsVBoxLayoutPtr {new QVBoxLayout};
    levelsVBoxLayoutPtr->addWidget(allGroupBoxPtr);
    levelsVBoxLayoutPtr->addWidget(level1GroupBoxPtr);
    levelsVBoxLayoutPtr->addWidget(level2GroupBoxPtr);
    levelsVBoxLayoutPtr->addWidget(level3GroupBoxPtr);

    QVBoxLayout* mainVBoxLayoutPtr {new QVBoxLayout};
    mainVBoxLayoutPtr->addLayout(topHBOxLayoutPtr,0);
    mainVBoxLayoutPtr->addLayout(levelsVBoxLayoutPtr,5);

    QWidget* centralWidgetPtr {new QWidget};
    centralWidgetPtr->setLayout(mainVBoxLayoutPtr);
    setCentralWidget(centralWidgetPtr);

    QObject::connect(parserPtr_,&ReportParser::parseFinishedSignal,
                     this,&MainWindow::parseFinishedSlot);

    level1ViewPtr_->setModel(level1ModelPtr_);
    level2ViewPtr_->setModel(level2ModelPtr_);
    level3ViewPtr_->setModel(level3ModelPtr_);

    level1ViewPtr_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    level2ViewPtr_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    level3ViewPtr_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    resize(800,600);
}

