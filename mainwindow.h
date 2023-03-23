#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QtAlgorithms>
#include <QVector>
#include <QDebug>
#include <string>
#include <functional>
#include <QPainter>
#include <QStaticText>
#include <QGraphicsView>
#include "applogic.h"
#include "Plot.h"

#define NO_DATA 00
#define S_Y 350
#define S_X 50
#define LEN_X 1000
#define LEN_Y 300
#define MARGIN 10
#define PIX_MAP_W 400
#define PIX_MAP_H 1100


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
    void on_OpenButton_clicked();
    void on_CalculateButton_clicked();
    void on_LoadButton_clicked();
private:
    Ui::MainWindow *ui;
    void showData(FuncReturningValue*);
    void fillTable(QStringList, size_t, int);
    char*** getDataFromTable();
    bool isLoadBtnCkiced;
    QString curRegion;
    void drawAxis(FuncReturningValue*, QPainter*);
    void drawPoints(FuncReturningValue*, QPainter*);
    void clean(FuncReturningValue*);
};
#endif // MAINWINDOW_H
