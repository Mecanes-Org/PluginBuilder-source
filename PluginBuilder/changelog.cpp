#include "changelog.h"
#include "ui_changelog.h"

ChangeLog::ChangeLog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChangeLog)
{
    ui->setupUi(this);
}

ChangeLog::~ChangeLog()
{
    delete ui;
}
