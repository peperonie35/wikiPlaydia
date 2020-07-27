#include "endgamedialog.h"
#include "ui_endgamedialog.h"

endGameDialog::endGameDialog(QString title, QStringList history, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::endGameDialog)
{
    ui->setupUi(this);
    setWindowTitle(title);
    historyModel = new QStringListModel;
    historyModel->setStringList(history);
    ui->historyList->setModel(historyModel);
    show();
}

endGameDialog::~endGameDialog()
{
    delete ui;
}
