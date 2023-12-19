
#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QJsonObject>
#include <QPointer>

class QString;
class QLabel;
class QJsonObject;
class QTableView;
class QLineEdit;
class QPushButton;
class LevelModel;
class ReportParser;
class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QLabel* projectTxtPtr_ {};
    QLabel* allTxtPtr_ {};
    QLabel* level1TxtPtr_ {};
    QLabel* level2TxtPtr_ {};
    QLabel* level3TxtPtr_ {};

    QTableView* level1ViewPtr_ {};
    QTableView* level2ViewPtr_ {};
    QTableView* level3ViewPtr_ {};

    LevelModel* level1ModelPtr_ {};
    LevelModel* level2ModelPtr_ {};
    LevelModel* level3ModelPtr_ {};

    QTableView* tableViewPtr_ {};
    QLineEdit* filenameLineEditPtr_ {};
    QPushButton* openButtonPtr_ {};
    QPushButton* saveButtonPtr_ {};
    QPointer<ReportParser> parserPtr_ {};
    QJsonObject warningsResultObject_ {};
private slots:
    void openSlot();
    void saveSlot();
    void parseFinishedSlot(bool success, const QJsonObject& warningsResultObject, const QString& lastError);
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow()=default;
signals:

};

#endif // MAINWINDOW_H
