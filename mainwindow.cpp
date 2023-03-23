#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    on_LoadButton_clicked();
    on_OpenButton_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
}

char* QstringToCharArray(QString qstr)
{
    char *str = (char*)malloc(sizeof(char)*(qstr.size() + 1));
    if(str == NULL){
        free(str);
    }else{
        long long i;
        for (i = 0; i < qstr.size(); i++){
            str[i] = qstr.at(i).unicode();
        }
        str[i] = 0;
    }
    return str;
}

QStringList ConvertRowToQTFormat(char **row, size_t size)
{
    QStringList qsl = {};
    for(size_t i = 0; i < size; i++){
        qsl.append(QString::fromUtf8(row[i]));
    }
    return qsl;
}

void MainWindow::showData(FuncReturningValue* frv)
{
    ui->tableWidget->setColumnCount(frv->fields_num);
    QStringList QColumns = ConvertRowToQTFormat(frv->headers, frv->fields_num);
    QString region = ui->lineEditRegion->text();
    QString year = ui->lineEditLineYearT->text();
    ui->tableWidget->setHorizontalHeaderLabels(QColumns);
    size_t m = 0;
    if (frv->data != NULL){
        ui->tableWidget->setRowCount(0);
        for (size_t i = 0; i < frv->len; i++){
            QStringList currentRow = ConvertRowToQTFormat(frv->data[i], frv->fields_num);
            if(region.isEmpty()){
                ui->tableWidget->setRowCount(i + 1);
                for (int j = 0; j < currentRow.count(); j++){
                   fillTable(currentRow, i, j);
                }
            }else{
                for (int k = 0; k < currentRow.count(); k++){
                    if((strcmp(frv->data[i][k], frv->region) == 0)){
                        ui->tableWidget->setRowCount(m + 1);
                        for (int j = 0; j < currentRow.count(); j++){
                           fillTable(currentRow, m, j);
                                    }
                             m++;
                        }
                    }
                 }
        }
    }else
        free(frv->data);
}

void MainWindow::fillTable(QStringList currentRow, size_t i, int j)
{
    QTableWidgetItem *item = new QTableWidgetItem();
    item->setData(Qt::EditRole, currentRow.at(j).toDouble());
    item->setText(currentRow.at(j));
    ui->tableWidget->setItem(i, j, item);
}

char*** MainWindow::getDataFromTable()
{
    char*** data;
    if(ui->tableWidget->rowCount() == 0)
        data = NULL;
    else
        data = (char ***)malloc(sizeof(char**) * ui->tableWidget->rowCount());
    if(data != NULL){
        for (size_t i = 0; i < (size_t)ui->tableWidget->rowCount(); i++){
            data[i] = (char **)malloc(sizeof(char*) * ui->tableWidget->columnCount());
            if(data[i] != NULL){
                for (size_t j = 0; j < (size_t)ui->tableWidget->columnCount(); j++){
                    QTableWidgetItem *item = ui->tableWidget->item(i,j);
                    char* str = QstringToCharArray(item->text());
                    data[i][j] = str;
                }
            }else{
                for(size_t k = 0; k < i; k++)
                    free(data[k]);
                free(data);
                data = NULL;
            }
        }
    }
    return data;
}

void MainWindow::on_OpenButton_clicked()
{
    QString pathFileName = ui->FilePathLabel->text();
    QString region = ui->lineEditRegion->text();
    if(region.toDouble())
        QMessageBox::critical(this, "warning", "wrong region");
    if (pathFileName.isEmpty())
        QMessageBox::critical(this, "warning", "file not opened");
    else{
        FuncArgument fa = {
            .filename = QstringToCharArray(pathFileName),
            .region = QstringToCharArray(region),
        };
        FuncReturningValue* frv = entryPoint(getData, &fa);
        showData(frv);
        free(frv);
        entryPoint(cleanData, &fa);
        curRegion = region;
    }
}

void MainWindow::on_LoadButton_clicked()
{
    QString checkPathFileName = ui->FilePathLabel->text();
    QString pathFileName = QFileDialog::getOpenFileName(this, tr("Open File"), "C://", "CSV File (*.csv)");
    ui->FilePathLabel->setText(pathFileName);
    if(!checkPathFileName.isEmpty()){
        ui->tableWidget->setRowCount(0);
     }
}

void MainWindow::on_CalculateButton_clicked()
{
    size_t field = ui->lineEditLine->text().toDouble();
    QString region = ui->lineEditRegion->text();
    if((!field) || (field > (size_t)ui->tableWidget->columnCount() || field < 1 ) or region.isEmpty())
        QMessageBox::critical(this, "warning", "data can't be calculated");
    else if(region != curRegion)
        QMessageBox::critical(this, "warning", "data can't be calculated, press load button");
    else{
        FuncArgument fa = {
            .data = getDataFromTable(),
            .field = field-1,
            .len =  (size_t)ui->tableWidget->rowCount(),
            .fields_num = (size_t)ui->tableWidget->columnCount(),
        };
        if(fa.data == NULL)
            QMessageBox::critical(this, "warning", "data can't be calculated, no data in table");
        else{
            FuncReturningValue* frv = entryPoint(calculateData, &fa);
                if(frv->valuesX != NULL && frv->valuesY != NULL && frv->plotValues != NULL){
                    ui->ResultLabelMax->setText(QString::number(frv->solution[MAX_VALUE_INDEX]));
                    ui->ResultLabelMid->setText(QString::number(frv->solution[MID_VALUE_INDEX]));
                    ui->ResultLabelMin->setText(QString::number(frv->solution[MIN_VALUE_INDEX]));
                    QPixmap* pix;
                    pix = createPlot(frv);
                    ui->label->setPixmap(*pix);
            }
            clean(frv);
        }
        entryPoint(cleanData, &fa);
    }
}

void MainWindow::clean(FuncReturningValue* frv)
{
        free(frv->plotValues);
        free(frv->valuesX);
        free(frv->valuesY);
}
