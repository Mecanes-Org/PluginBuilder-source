#include "unrealversionslot.h"
#include "ui_unrealversionslot.h"

#include <QFileDialog>
#include <QMessageBox>

UnrealVersionSlot::UnrealVersionSlot(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UnrealVersionSlot)
{
    ui->setupUi(this);

    // EXEMPLE :  "C:\Program Files\Epic Games\UE_5.4\Engine\Build\BatchFiles"
    defaultDir = "C:\\Program Files\\Epic Games";
}

UnrealVersionSlot::~UnrealVersionSlot()
{
    delete ui;
}

QString UnrealVersionSlot::getLabel_pluginPathText()
{
    return ui->label_pluginPath->text();
}

QString UnrealVersionSlot::getLineEdit_UE_versionNameText()
{
    return ui->lineEdit_UE_versionName->text();
}

void UnrealVersionSlot::setUnrealVersion(S_UnrealVersion unrealV)
{
    unrealVersion = unrealV;

    ui->lineEdit_UE_versionName->setText(unrealVersion.name);
    ui->label_pluginPath->setText(unrealVersion.path);
}

void UnrealVersionSlot::on_pushButton_find_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Unreal ( Ex : UE_5.7)"), defaultDir, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if( !dir.isEmpty() ){
        ui->label_pluginPath->setText(dir);
    }else{
        QMessageBox::warning(this, tr("Error"), tr("Incorrect file"));
    }

}


void UnrealVersionSlot::on_pushButton_delete_clicked()
{
    if (auto lay = parentWidget() ? parentWidget()->layout() : nullptr) {
        lay->removeWidget(this);   // le sortir du layout
    }

    this->deleteLater();           // le détruire proprement
}

