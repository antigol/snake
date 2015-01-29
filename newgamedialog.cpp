#include "newgamedialog.h"
#include <QSettings>

NewGameDialog::NewGameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewGameDialog)
{
    ui->setupUi(this);

    QSettings set;
    ui->widthSpinBox->setValue(set.value("width", ui->widthSpinBox->value()).toInt());
    ui->heigthSpinBox->setValue(set.value("height", ui->heigthSpinBox->value()).toInt());
    ui->speedSpinBox->setValue(set.value("speed", ui->speedSpinBox->value()).toInt());

    connect(this, SIGNAL(accepted()), this, SLOT(saveSettings()));
}

NewGameDialog::~NewGameDialog()
{
    delete ui;
}

void NewGameDialog::saveSettings()
{
    QSettings set;
    set.setValue("width", ui->widthSpinBox->value());
    set.setValue("height", ui->heigthSpinBox->value());
    set.setValue("speed", ui->speedSpinBox->value());
}
