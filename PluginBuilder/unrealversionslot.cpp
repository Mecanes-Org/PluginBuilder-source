#include "unrealversionslot.h"
#include "ui_unrealversionslot.h"

#include <QFileDialog>
#include <QMessageBox>

UnrealVersionSlot::UnrealVersionSlot(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UnrealVersionSlot)
{
    ui->setupUi(this);
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
    QString file_name = QFileDialog::getOpenFileName(this, tr("Search your plugin"), "");
    QFile file(file_name);

    if(file_name.isEmpty()){
        QMessageBox::warning(this, tr("Error"), tr("Incorrect file"));
        return;
    }

    ui->label_pluginPath->setText(file_name);
}


void UnrealVersionSlot::on_pushButton_delete_clicked()
{
    if (auto lay = parentWidget() ? parentWidget()->layout() : nullptr) {
        lay->removeWidget(this);   // le sortir du layout
    }

    this->deleteLater();           // le détruire proprement
}

