#ifndef NEWGAMEDIALOG_H
#define NEWGAMEDIALOG_H

#include <QDialog>
#include "ui_newgamedialog.h"

namespace Ui {
class NewGameDialog;
}

class NewGameDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit NewGameDialog(QWidget *parent = 0);
    ~NewGameDialog();
    
    Ui::NewGameDialog *ui;

private slots:
    void saveSettings();
};

#endif // NEWGAMEDIALOG_H
