#ifndef UNREALVERSIONSLOT_H
#define UNREALVERSIONSLOT_H

#include <QWidget>
#include "data.h"

namespace Ui {
class UnrealVersionSlot;
}

class UnrealVersionSlot : public QWidget
{
    Q_OBJECT

public:
    explicit UnrealVersionSlot(QWidget *parent = nullptr);
    ~UnrealVersionSlot();


public:
    QString getLabel_pluginPathText();
    QString getLineEdit_UE_versionNameText();

    void setUnrealVersion(S_UnrealVersion unrealV);

private slots:
    void on_pushButton_find_clicked();

    void on_pushButton_delete_clicked();

private:
    Ui::UnrealVersionSlot *ui;
    S_UnrealVersion unrealVersion;
};

#endif // UNREALVERSIONSLOT_H
