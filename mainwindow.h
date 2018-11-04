#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <map>
#include <QSettings>
#include <QFile>
#include <QTextStream>
#include "eventnames.h"



namespace Ui {
class MainWindow;
}


class CClass;
class CRunner;
class QLineEdit;
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
    void ClearData();

private slots:
    void on_browseForDay1File_clicked();
    void on_processButton_clicked();
    void on_browseForDay2File_clicked();
    void on_browseForDay3File_clicked();
    void on_browseForDay4File_clicked();
    void on_browseForDay5File_clicked();
    void on_browseForOutputFile_clicked();

    void on_pushButtonSettings_clicked();

private:
    Ui::MainWindow *ui;
    void Process();
    std::map <QString, CClass*>  m_Classes;
    QChar m_Separator;
    EventNames* m_EventNames;

    void SetFile(QSettings &a_Settings, QString a_FileKey, QLineEdit* a_Control);
    void ProcessFile(int a_Day, QString a_File);
    void ProcessAutodownloadFile(int a_Day, QTextStream& in);
    void ProcessOeFile(int a_Day, QTextStream& in);
    void ProcessOeScoreFile(int a_Day, QTextStream& in);
    void ProcessResult(int a_Day, QString a_Line, int a_NameCol, int a_Name2Col, int a_ClubCol, int a_ClassCol, int a_TimePointsCol, int a_PositionCol);
    void AddMissingResults();
    void WriteResults();
    void WriteHeaders(QTextStream &a_Stream);
    void WriteClass(QTextStream &a_Stream, QString a_Class);
    void WriteTableHeaders(QTextStream &a_Stream);
    void WriteRunner(QTextStream &a_Stream, int a_Pos, CRunner *a_Runner);
    QString NormaliseName(QString a_Name);
    QStringList SplitQuoted(QString);
    QString ClassLabelName(QString a_Name);
    QString Menu();
    int m_MaxNameLen;
    int m_MaxClubLen;

};

#endif // MAINWINDOW_H
