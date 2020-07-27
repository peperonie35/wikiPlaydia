#ifndef ENDGAMEDIALOG_H
#define ENDGAMEDIALOG_H

#include <QDialog>
#include <QStringListModel>

namespace Ui {
class endGameDialog;
}

class endGameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit endGameDialog(QString title, QStringList history, QWidget *parent = nullptr);
    ~endGameDialog();

private:
    QStringListModel *historyModel;
    Ui::endGameDialog *ui;
};

#endif // ENDGAMEDIALOG_H
