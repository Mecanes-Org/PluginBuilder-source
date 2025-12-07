#ifndef ABOUTUS_H
#define ABOUTUS_H

#include <QDialog>

#include "data.h"

namespace Ui {
class AboutUs;
}

class AboutUs : public QDialog
{
    Q_OBJECT

public:
    explicit AboutUs(QWidget *parent = nullptr);
    ~AboutUs();

private slots:
    void on_pushButton_unrealEngine_clicked();

    void on_pushButton_discord_clicked();

private:
    Ui::AboutUs *ui;
    Data data;
};

#endif // ABOUTUS_H
