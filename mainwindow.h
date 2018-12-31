#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <map>
#include <QSettings>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include "eventnames.h"
#include <QXmlStreamReader>



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
    QFileInfo m_LastFileInfo;

    void SetFile(QSettings &a_Settings, QString a_FileKey, QLineEdit* a_Control);
    void ProcessFile(int a_Day, QString a_File);
    void ProcessAutodownloadFile(int a_Day, QTextStream& in);
    void ProcessOeFile(int a_Day, QTextStream& in);
    void ProcessOeScoreFile(int a_Day, QTextStream& in);
    void ProcessOrScoreFile(int a_Day, QTextStream &a_Instream);
    void ProcessOe2013File(int a_Day, QTextStream &a_Instream, QStringList a_Fields);
    void ProcessResult(int a_Day, QString a_Line, int a_NameCol, int a_Name2Col, int a_ClubCol, int a_ClassCol, int a_TimePointsCol, int a_PositionCol);
    void ProcessResult(int a_Day, QString a_Line, QStringList a_Fields, QString a_NameCol, QString a_Name2Col, QString a_ClubCol, QString a_ClassCol, QString a_TimePosCol, QString a_PositionCol);
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


    int FindField(const QString &a_Target, const QStringList &a_FieldList) const;

    QString GetProbablePath(QLineEdit *a_Control);
    void ProcessXML3Result(int a_Day, QString a_Class, QXmlStreamReader& a_Xml);
    void ProcessXML3File(int a_Day, QString a_Data);
};

#endif // MAINWINDOW_H
