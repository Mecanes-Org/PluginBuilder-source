#ifndef CHANGELOG_H
#define CHANGELOG_H

#include <QDialog>

namespace Ui {
class ChangeLog;
}

class ChangeLog : public QDialog
{
    Q_OBJECT

public:
    explicit ChangeLog(QWidget *parent = nullptr);
    ~ChangeLog();

private slots:

private:
    Ui::ChangeLog *ui;
};

#endif // CHANGELOG_H
