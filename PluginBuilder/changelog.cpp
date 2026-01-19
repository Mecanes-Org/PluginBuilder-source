#include "changelog.h"
#include "ui_changelog.h"

#include <QPropertyAnimation>

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

// void ChangeLog::on_pushButton_clicked()
// {
//     QWidget* widget_log = ui->widget_log;
//     int height = widget_log->height();

//     int targetHeight;
//     if(height <= 60) {
//         targetHeight = height * 3;
//     } else {
//         targetHeight = height / 3;
//     }

//     // Animation du widget qui se redimensionne
//     QPropertyAnimation *animationSize = new QPropertyAnimation(widget_log, "geometry");
//     animationSize->setDuration(300);
//     animationSize->setStartValue(widget_log->geometry());

//     QRect endGeometry = widget_log->geometry();
//     endGeometry.setHeight(targetHeight);
//     animationSize->setEndValue(endGeometry);
//     animationSize->setEasingCurve(QEasingCurve::InOutQuad);

//     animationSize->start(QAbstractAnimation::DeleteWhenStopped);
// }
