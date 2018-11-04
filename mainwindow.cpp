#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include "QStandardPaths"
#include "cclass.h"
#include <QSettings>
#include <QMessageBox>
#include <QStringList>
#include "crunner.h"
#include "cresult.h"
#include <QDebug>
#include <QLineEdit>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    statusBar()->showMessage(tr("Waiting to process"));

    QSettings settings(QSettings::IniFormat,  QSettings::UserScope, "undy", "Xmas5DaysResults");
    settings.beginGroup("Configuration");
    SetFile(settings, "Day1", ui->day1ResultFile);
    SetFile(settings, "Day2", ui->day2ResultFile);
    SetFile(settings, "Day3", ui->day3ResultFile);
    SetFile(settings, "Day4", ui->day4ResultFile);
    SetFile(settings, "Day5", ui->day5ResultFile);
    SetFile(settings, "Output", ui->outputFile);

    settings.endGroup();
}

void MainWindow::ClearData()
{
    for (auto i = m_Classes.begin(); i != m_Classes.end(); i++)
        delete i->second;
    m_Classes.clear();
}

MainWindow::~MainWindow()
{
    QSettings settings(QSettings::IniFormat,  QSettings::UserScope, "undy", "Xmas5DaysResults");
    settings.beginGroup("Configuration");
    settings.setValue("Day1", ui->day1ResultFile->text());
    settings.setValue("Day2", ui->day2ResultFile->text());
    settings.setValue("Day3", ui->day3ResultFile->text());
    settings.setValue("Day4", ui->day4ResultFile->text());
    settings.setValue("Day5", ui->day5ResultFile->text());
    settings.setValue("Output", ui->outputFile->text());
    settings.endGroup();

    delete ui;
    ClearData();
}

void MainWindow::on_browseForDay1File_clicked()
{
    QString file = QFileDialog::getOpenFileName(this,tr("Select Day 1 data file"), QStandardPaths::writableLocation(QStandardPaths::DesktopLocation), "Result files(*.csv);;All files (* *.*)");

    if (!file.isEmpty())
        ui->day1ResultFile->setText(file);
}



void MainWindow::on_processButton_clicked()
{
    if (ui->outputFile->text().isEmpty())
    {
        QMessageBox::information(0, "Doh !" , "You must specify an output file");
        ui->outputFile->setFocus();
        return;
    }
    ui->processButton->blockSignals(true);
    setCursor(Qt::WaitCursor);
    ClearData();
    Process();
    setCursor(Qt::ArrowCursor);
    ui->processButton->blockSignals(false);
}

void MainWindow::Process()
{

    statusBar()->showMessage(tr("Reading files"));
    ProcessFile(1, ui->day1ResultFile->text());
    ProcessFile(2, ui->day2ResultFile->text());
    ProcessFile(3, ui->day3ResultFile->text());
    ProcessFile(4, ui->day4ResultFile->text());
    ProcessFile(5, ui->day5ResultFile->text());
    AddMissingResults();
    statusBar()->showMessage(tr("Creating output"));
    WriteResults();
    statusBar()->showMessage(tr("Finished !"));
}

void MainWindow::on_browseForDay2File_clicked()
{
    QString file = QFileDialog::getOpenFileName(this,tr("Select Day 2 data file"), QStandardPaths::writableLocation(QStandardPaths::DesktopLocation), "Result files(*.csv);;All files (* *.*)");

    if (!file.isEmpty())
        ui->day2ResultFile->setText(file);
}

void MainWindow::on_browseForDay3File_clicked()
{
    QString file = QFileDialog::getOpenFileName(this,tr("Select Day 3 data file"), QStandardPaths::writableLocation(QStandardPaths::DesktopLocation), "Result files(*.csv);;All files (* *.*)");

    if (!file.isEmpty())
        ui->day3ResultFile->setText(file);
}

void MainWindow::on_browseForDay4File_clicked()
{
    QString file = QFileDialog::getOpenFileName(this,tr("Select Day 4 data file"), QStandardPaths::writableLocation(QStandardPaths::DesktopLocation), "Result files(*.csv);;All files (* *.*)");

    if (!file.isEmpty())
        ui->day4ResultFile->setText(file);
}

void MainWindow::on_browseForDay5File_clicked()
{
    QString file = QFileDialog::getOpenFileName(this,tr("Select Day 5 data file"), QStandardPaths::writableLocation(QStandardPaths::DesktopLocation), "Result files(*.csv);;All files (* *.*)");

    if (!file.isEmpty())
        ui->day5ResultFile->setText(file);
}

void MainWindow::on_browseForOutputFile_clicked()
{
    QString file = QFileDialog::getSaveFileName(this,tr("Select output file"), QStandardPaths::writableLocation(QStandardPaths::DesktopLocation), "HTML files(*.htm),Also HTML Files(*.html);;All files (* *.*)");

    if (!file.isEmpty())
        ui->outputFile->setText(file);
}

void MainWindow::SetFile(QSettings& a_Settings, QString a_FileKey, QLineEdit* a_Control)
{
    QString file = a_Settings.value(a_FileKey).toString();
    if (!file.isEmpty() && QFile::exists(file))
        a_Control->setText(file);
}

void MainWindow::ProcessFile(int a_Day, QString a_File)
{
    if (a_File.isEmpty())
        return;

    qDebug() << "Processing " << a_File;
    // Read first line of file
    QFile file(a_File);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::information(nullptr, "Can't read file" , "Can't read " + a_File + ":" + file.errorString());
        return;
    }

    if(file.atEnd())
    {
        QMessageBox::information(nullptr, "Empty file" , a_File + " is empty");
        file.close();
        return;
    }

    QTextStream in(&file);
    QString line = in.readLine();
    auto commaCount = line.count(",");
    auto semiCount = line.count(";");
    m_Separator = commaCount > semiCount ? QChar(',') : QChar(';');
    QStringList fields = line.split(m_Separator);

    if (fields[0] == "RaceNumber")
        ProcessAutodownloadFile(a_Day, in);
    else if (fields[1] == "Stno")
        ProcessOeFile(a_Day, in);
    else if (fields[0] == "Stno")
        ProcessOeScoreFile(a_Day, in);
    else
        QMessageBox::information(nullptr, "Unrecognised file" , "Can't identify results format of " + a_File);

    file.close();
}

void MainWindow::ProcessAutodownloadFile(int a_Day, QTextStream &a_InStream)
{
    while (!a_InStream.atEnd())
    {
        QString line = a_InStream.readLine();
        ProcessResult(a_Day, line,3,-1,5,7,10,12);
    }
}

void MainWindow::ProcessOeFile(int a_Day, QTextStream &a_InStream)
{
    while (!a_InStream.atEnd())
    {
        QString line = a_InStream.readLine();
        ProcessResult(a_Day, line,6,5,20,29,13,57);
    }
}

void MainWindow::ProcessOeScoreFile(int a_Day, QTextStream &a_Instream)
{
    while (!a_Instream.atEnd())
    {
        QString line = a_Instream.readLine();
        ProcessResult(a_Day, line,4,3,14,19,46,45);
    }
}


QStringList MainWindow::SplitQuoted(QString a_Line)
{
    QStringList csvlist;
    bool inside(false);
    QString temp;
    for (int i = 0; i < a_Line.size(); i++)
    {
        if (a_Line.at(i) == "\"")
        {
            inside = !inside;
            continue;
        }
        if (!inside && (a_Line.at(i) == m_Separator || i == a_Line.size() -1))
        {
            csvlist.append(temp);
            temp.clear();
        }
        else
            temp += a_Line.at(i);
    }

    return csvlist;
}


void MainWindow::ProcessResult(int a_Day, QString a_Line, int a_NameCol, int a_Name2Col, int a_ClubCol, int a_ClassCol, int a_TimePointsCol, int a_PositionCol)
{
    QStringList fields = SplitQuoted(a_Line);

    QString name = fields[a_NameCol];
    if (a_Name2Col > 0)
        name += " " + fields[a_Name2Col];

    CClass* oClass = nullptr;
    QString className(fields[a_ClassCol]);
//    qDebug() << "Processing class " << className << "size" << className.size();
    if (className.size() == 2 && className.at(0) == 'M')
        className = "Men " + className.mid(1,1);
    if (className.size() == 2 && className.at(0) == 'W')
        className = "Women " + className.mid(1,1);
//qDebug() << "Class now called " << className;
    for (auto i = m_Classes.begin(); i != m_Classes.end(); i++)
        if (NormaliseName(i->second->Name()) == NormaliseName(className))
            oClass = i->second;

    if (!oClass)
        m_Classes[className] = oClass = new CClass(className);
    if (a_Day > 1)
    {
        if (!oClass->GotName(name))
        {
        //    qDebug() << "No name for " << name;
            QString realName(name);
            if (oClass->FindNearName(realName) > 90)
            {
                qDebug() << name << "changed to" << realName;
                name = realName;
            }
        }
    }
    CRunner* runner = oClass->AddRunner(name,fields[a_ClubCol]);
    runner->AddResult(a_Day, fields[a_PositionCol], fields[a_TimePointsCol]);
}

void MainWindow::AddMissingResults()
{

    // May have no results for a specific class for a day (voided course)

    // Also set the maximum name length whilst parsing everyone
    m_MaxNameLen = 0;
    m_MaxClubLen = 0;

    for (auto i = m_Classes.begin(); i != m_Classes.end(); i++)
    {
        CClass* cls = i->second;
        for (int day = 1; day <=5; day++)
        {
            bool dayHasResults(false);
            int results(0);
            for (auto j = cls->m_Runners.begin(); j != cls->m_Runners.end(); j++)
            {
                if (j->second->Name().size() > m_MaxNameLen)
                    m_MaxNameLen = j->second->Name().size();
                if (j->second->Club().size() > m_MaxClubLen)
                    m_MaxClubLen = j->second->Club().size();
                if (j->second->ResultForDay(day))
                {
                    dayHasResults = true;
                    results++;
                }
            }
            if (dayHasResults)
            {
                for (auto j = cls->m_Runners.begin(); j != cls->m_Runners.end(); j++)
                    if (!j->second->ResultForDay(day))
                    {
                        j->second->AddResult(day, results+1); // Give him n+1th place
                    }
            }
        }
    }

}

void MainWindow::WriteHeaders(QTextStream& a_Stream)
{
    a_Stream << "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">" << endl;
    a_Stream << "<html xmlns=\"http://www.w3.org/1999/xhtml\" lang=\"en\" xml:lang=\"en\">" << endl;
    a_Stream << "<head>" << endl;
    a_Stream << "<meta http-equiv=\"Content-Type\" content=\"text/html;charset=UTF-8\" />" << endl;
    a_Stream << "<title>Christmas 5 Days 2014</title>" << endl;

    a_Stream << "<style>" << endl;
    a_Stream << "body {" << endl;
    a_Stream << "color: #000000;" << endl;
    a_Stream << "font-family: Arial,Helvetica,sans-serif;" << endl;
    a_Stream << "font-size: 0.83em;" << endl;
    a_Stream << "}" << endl;

    a_Stream << ".course-name {" << endl;
    a_Stream << "background: linear-gradient(#0c0, #C00);" << endl;
    a_Stream << "margin-bottom: 5px;" << endl;
    a_Stream << "padding: 9px 11px 8px;" << endl;
    a_Stream << "}" << endl;

    a_Stream << ".course-name h2 {" << endl;
    a_Stream << "color: #ffffff;" << endl;
    a_Stream << "font-size: 1.23em;" << endl;
    a_Stream << "font-weight: bold;" << endl;
    a_Stream << "margin: 0;" << endl;
    a_Stream << "padding: 0;" << endl;
    a_Stream << "}" << endl;

    a_Stream << "table th {" << endl;
    a_Stream << "background: linear-gradient(Gainsboro, Gray);" << endl;
    a_Stream << "border-bottom: 1px solid #d9d9d9;" << endl;
    a_Stream << "font-weight: bold;" << endl;
    a_Stream << "margin: 0;" << endl;
    a_Stream << "padding: 2px 5px;" << endl;
    a_Stream << "text-align: left;" << endl;
    a_Stream << "vertical-align: top;" << endl;
    a_Stream << "}" << endl;

    a_Stream << "td {" << endl;
    a_Stream << "margin: 0;" << endl;
    a_Stream << "padding: 2px 5px;" << endl;
    a_Stream << "}" << endl;

    a_Stream << "td.right {" << endl;
    a_Stream << "text-align: right;" << endl;
    a_Stream << "}" << endl;

    a_Stream << "tr:nth-child(odd) {" << endl;
    a_Stream << "background-color: lightyellow;" << endl;
    a_Stream << "}" << endl;

    a_Stream << "tr:hover {" << endl;
    a_Stream << "background-color: yellow;" << endl;
    a_Stream << "}" << endl;

    a_Stream << "</style>" << endl;
    a_Stream << "</head>" << endl;
    a_Stream << "<body>" << endl;
    a_Stream << "<H1>Christmas 5 Days 2014 - Who's been good ?</h1>";
}

QString MainWindow::ClassLabelName(QString a_Name)
{
    return a_Name.replace(" ","_");
}

QString MainWindow::Menu()
{
    QString retVal;

    for (auto k = m_Classes.begin(); k != m_Classes.end(); k++)
    {
        if (k->second->Name().left(3) == "EOD")
            continue;

        retVal += "<a href=""#" + ClassLabelName(k->second->Name()) + ">" + k->second->Name() + "</a> | ";
    }

    if (retVal.size() > 0)
        return retVal.left(retVal.size() - 2);
    else
        return retVal;
}

void MainWindow::WriteClass(QTextStream& a_Stream, QString a_Class)
{
    a_Stream << "<a name=" << ClassLabelName(a_Class) << "></a>";
    a_Stream << Menu() << "<br/><br/>";
    a_Stream << "<div class=\"course-name\">" << endl;
    a_Stream << "<h2>" << a_Class << "</h2>" << endl;
    a_Stream << "</div>" << endl;
}

void MainWindow::WriteTableHeaders(QTextStream& a_Stream)
{
    a_Stream << "<table class=\"data\" cellspacing=\"0\" cellpadding=\"0\">"<< endl;
    a_Stream << "<tr>"<< endl;
    a_Stream << "<th>Pos</th>"<< endl;
    a_Stream << "<th>Name</th>"<< endl;
    a_Stream << "<th>Club</th>"<< endl;
  //  a_Stream << "<th>Class</th>"<< endl;
    a_Stream << "<th class=\"centre stage\">Georges Heights</th>"<< endl;
    a_Stream << "<th class=\"centre stage\">Cockatoo Island</th>"<< endl;
    a_Stream << "<th class=\"centre stage\">Pyrmont</th>"<< endl;
    a_Stream << "<th class=\"centre stage\">Darks Common</th>"<< endl;
    a_Stream << "<th class=\"centre stage\">Centennial Park</th>"<< endl;
    a_Stream << "<th>Total</th>"<< endl;
    a_Stream << "</tr>"<< endl;
}

QString toHtml(QString s)
{
    QString t;
    for (int i = 0; i < s.size(); i++)
    {
        if (s.at(i) == "Á") t += "&Aacute;";
        else if (s.at(i) == "á") t += "&aacute;";
        else if (s.at(i) == "À") t += "&Agrave;";
        else if (s.at(i) == "à") t += "&agrave;";
        else if (s.at(i) == "Â") t += "&Acirc;";
        else if (s.at(i) == "â") t += "&acirc;";
        else if (s.at(i) == "Ä") t += "&Auml;";
        else if (s.at(i) == "ä") t += "&auml;";
        else if (s.at(i) == "Ã") t += "&Atilde;";
        else if (s.at(i) == "ã") t += "&atilde;";
        else if (s.at(i) == "Å") t += "&Aring;";
        else if (s.at(i) == "å") t += "&aring;";
        else if (s.at(i) == "Æ") t += "&AElig;";
        else if (s.at(i) == "æ") t += "&aelig;";
        else if (s.at(i) == "Ç") t += "&Ccedil;";
        else if (s.at(i) == "ç") t += "&ccedil;";
        else if (s.at(i) == "Ð") t += "&ETH;";
        else if (s.at(i) == "ð") t += "&eth;";
        else if (s.at(i) == "É") t += "&Eacute;";
        else if (s.at(i) == "é") t += "&eacute;";
        else if (s.at(i) == "È") t += "&Egrave;";
        else if (s.at(i) == "è") t += "&egrave;";
        else if (s.at(i) == "Ê") t += "&Ecirc;";
        else if (s.at(i) == "ê") t += "&ecirc;";
        else if (s.at(i) == "Ë") t += "&Euml;";
        else if (s.at(i) == "ë") t += "&euml;";
        else if (s.at(i) == "Í") t += "&Iacute;";
        else if (s.at(i) == "í") t += "&iacute;";
        else if (s.at(i) == "Ì") t += "&Igrave;";
        else if (s.at(i) == "ì") t += "&igrave;";
        else if (s.at(i) == "Î") t += "&Icirc;";
        else if (s.at(i) == "î") t += "&icirc;";
        else if (s.at(i) == "Ï") t += "&Iuml;";
        else if (s.at(i) == "ï") t += "&iuml;";
        else if (s.at(i) == "Ñ") t += "&Ntilde;";
        else if (s.at(i) == "ñ") t += "&ntilde;";
        else if (s.at(i) == "Ó") t += "&Oacute;";
        else if (s.at(i) == "ó") t += "&oacute;";
        else if (s.at(i) == "Ò") t += "&Ograve;";
        else if (s.at(i) == "ò") t += "&ograve;";
        else if (s.at(i) == "Ô") t += "&Ocirc;";
        else if (s.at(i) == "ô") t += "&ocirc;";
        else if (s.at(i) == "Ö") t += "&Ouml;";
        else if (s.at(i) == "ö") t += "&ouml;";
        else if (s.at(i) == "Õ") t += "&Otilde;";
        else if (s.at(i) == "õ") t += "&otilde;";
        else if (s.at(i) == "Ø") t += "&Oslash;";
        else if (s.at(i) == "ø") t += "&oslash;";
        else if (s.at(i) == "Œ") t += "&OElig;";
        else if (s.at(i) == "œ") t += "&oelig;";
        else if (s.at(i) == "ß") t += "&szlig;";
        else if (s.at(i) == "Þ") t += "&THORN;";
        else if (s.at(i) == "þ") t += "&thorn;";
        else if (s.at(i) == "Ú") t += "&Uacute;";
        else if (s.at(i) == "ú") t += "&uacute;";
        else if (s.at(i) == "Ù") t += "&Ugrave;";
        else if (s.at(i) == "ù") t += "&ugrave;";
        else if (s.at(i) == "Û") t += "&Ucirc;";
        else if (s.at(i) == "û") t += "&ucirc;";
        else if (s.at(i) == "Ü") t += "&Uuml;";
        else if (s.at(i) == "ü") t += "&uuml;";
        else if (s.at(i) == "Ý") t += "&Yacute;";
        else if (s.at(i) == "ý") t += "&yacute;";
        else if (s.at(i) == "Ÿ") t += "&Yuml;";
        else if (s.at(i) == "ÿ") t += "&yuml;";
        else t+= s.at(i);
    }

    return t;
}

void MainWindow::WriteRunner(QTextStream& a_Stream, int a_Pos, CRunner* a_Runner)
{
    a_Stream << "<tr>"<< endl;
    a_Stream << "<td class=\"right\">" << a_Pos << "</td>" << endl;
    a_Stream << "<td>" << toHtml(a_Runner->Name()) << "</td>" << endl;
    a_Stream << "<td>" << toHtml(a_Runner->Club()) << "</td>"<< endl;
  //  a_Stream << "<td>Class</td>"<< endl;
    for (auto i = 1; i <= 5; i++)
        if (a_Runner->ResultForDay(i))
            a_Stream << "<td class=\"right\">" << a_Runner->ResultForDay(i)->PositionForWeb() << "</td>"<< endl;
        else
            a_Stream << "<td>" << "" << "</td>"<< endl;

    a_Stream << "<td class=\"right\">" << a_Runner->Score() << "</td>"<< endl;
    a_Stream << "</tr>"<< endl;
}

void MainWindow::WriteResults()
{
    QFile output(ui->outputFile->text());
    output.open(QIODevice::Text | QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream stream(&output);
    WriteHeaders(stream);

    for (auto k = m_Classes.begin(); k != m_Classes.end(); k++)
    {
        if (k->second->Name().left(3) == "EOD")
            continue;
        std::vector<CRunner*>* runners = k->second->RunnersByScore();
        WriteClass(stream, k->second->Name());
        WriteTableHeaders(stream);
        int lastScore(0);
        int j(0);
        for (unsigned int i = 0; i < runners->size(); i++)
        {
            if (runners->at(i)->Score() > lastScore)
            {
                lastScore = runners->at(i)->Score();
                j = static_cast<int>(i)+1;
            }
            WriteRunner(stream, j, runners->at(i));
        }
        stream << "</table><br/><br/>" << endl;
        delete runners;
    }
    output.close();
    stream << "</body>" << endl;
    stream << "</html>" << endl;
}

QString MainWindow::NormaliseName(QString a_Name)
{
    a_Name = a_Name.toLower();
    a_Name.remove(' ');
    return a_Name;
}

