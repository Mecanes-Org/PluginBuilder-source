#ifndef SETTINGS_H
#define SETTINGS_H

#include "data.h"
#include "mainwindow.h"

#include <QDialog>
#include <QFontComboBox>

#include <QApplication>


namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();

    QString getSettingsFilePath();
    bool notificationNameIsChecked(QString &notificationName) const;

private slots:
    void on_pushButton_find_plugin_dist_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::Settings *ui;

    QString defaultDir;

    S_GeneralSettings generalSettings;
    Data data;
    MainWindow *mainWindow = nullptr;
};

#endif // SETTINGS_H
