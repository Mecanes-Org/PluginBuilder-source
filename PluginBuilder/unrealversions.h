#ifndef UNREALVERSIONS_H
#define UNREALVERSIONS_H

#include <QDialog>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>

#include "mainwindow.h"

namespace Ui {
class UnrealVersions;
}

class UnrealVersions : public QDialog
{
    Q_OBJECT

public:
    explicit UnrealVersions(QWidget *parent = nullptr);
    ~UnrealVersions();


private slots:
    void on_pushButton_addUE_Version_clicked();

    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::UnrealVersions *ui;
    MainWindow *mainWin;
};

#endif // UNREALVERSIONS_H
