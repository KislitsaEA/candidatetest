#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <filemanip.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_csvOpen_clicked();

    void on_jsonOpen_clicked();

    void on_exportButton_clicked();

private:
    jsonProcessing* myJSON = new jsonProcessing;
    csvProcessing* myCSV = new csvProcessing;
    getXML* myXML = new getXML;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
