#ifndef SETTINGS_H
#define SETTINGS_H

#include "data.h"

#include <QDialog>

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


private slots:
    void on_pushButton_find_plugin_dist_clicked();

    void on_buttonBox_accepted();

private:
    Ui::Settings *ui;

    QString defaultDir;

    S_GeneralSettings GeneralSettings;

    Data data;
};

#endif // SETTINGS_H
