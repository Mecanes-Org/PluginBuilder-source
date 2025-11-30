#include "unrealversions.h"
#include "ui_unrealversions.h"

#include <QDebug>

#include "unrealversionslot.h"
#include "data.h"



UnrealVersions::UnrealVersions(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UnrealVersions)
{
    ui->setupUi(this);

    // Récupérer le MainWindow passé comme parent
    mainWin = qobject_cast<MainWindow*>(parent);


    // for (int i = 0; i < 3; ++i) {
    //     UnrealVersionSlot *slot = new UnrealVersionSlot(ui->scrollAreaWidgetContents_UE);
    //     ui->scrollAreaWidgetContents_UE->layout()->addWidget(slot);
    // }

    // INIT VARIABLES
    if( mainWin ){

        for (const S_UnrealVersion &val : mainWin->getUnrealVersions() ) {

            UnrealVersionSlot *slot = new UnrealVersionSlot(ui->scrollAreaWidgetContents_UE);
            slot->setUnrealVersion(val);
            ui->scrollAreaWidgetContents_UE->layout()->addWidget(slot);

        }
    }

}

UnrealVersions::~UnrealVersions()
{
    delete ui;
}

void UnrealVersions::on_pushButton_addUE_Version_clicked()
{
    ui->pushButton_addUE_Version->setDisabled(true);

    UnrealVersionSlot *slot = new UnrealVersionSlot(ui->scrollAreaWidgetContents_UE);
    ui->scrollAreaWidgetContents_UE->layout()->addWidget(slot);

    ui->pushButton_addUE_Version->setDisabled(false);
}


void UnrealVersions::on_buttonBox_accepted()
{
    qDebug() << "Save All";



    QList<S_UnrealVersion> unrealVersions;

    QList<UnrealVersionSlot*> unrealVSlot = this->findChildren<UnrealVersionSlot*>();

    // qDebug() << unrealVSlot;

    if (!unrealVSlot.isEmpty()) {

        qDebug() << "Unreal : " << unrealVSlot.length();

        for (UnrealVersionSlot* slot : std::as_const(unrealVSlot)) {
            if (!slot)
                continue;


            if( ( slot->getLabel_pluginPathText().length() > 5 && slot->getLabel_pluginPathText() != "Plugin Link" ) &&  slot->getLineEdit_UE_versionNameText().length() > 1 ){

                S_UnrealVersion sUV;
                sUV.path = slot->getLabel_pluginPathText();
                sUV.name = slot->getLineEdit_UE_versionNameText();

                unrealVersions.append(sUV); // N’oublie pas d’ajouter à ta liste
            }
        }

        if (mainWin) {
            // Tu peux maintenant appeler des méthodes / accéder à des membres publics
            mainWin->setUnrealVersions(unrealVersions);
            mainWin->saveUnrealVersions();
        }
    }else{
        if (mainWin) {
            // Tu peux maintenant appeler des méthodes / accéder à des membres publics
            mainWin->setUnrealVersions(unrealVersions);
            mainWin->saveUnrealVersions();
        }
    }



}


void UnrealVersions::on_buttonBox_rejected()
{
    qDebug() << "Cancel";
}

