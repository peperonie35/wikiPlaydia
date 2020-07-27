#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "endgamedialog.h"

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QSettings>
#include <QFile>
#include <QNetworkRequest>

#include <sstream>
#include <string>
#include <iostream>

#include <QtWebEngineWidgets/QtWebEngineWidgets>
#include <QWebEngineView>
#include <QDebug>
#include <QClipboard>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void setDefaultParam();
    void startGame();
    void stopGame(QString reason, double time);
    void hideUiGm(bool state);

    ~MainWindow();

private slots:
    void on_pageWeb_urlChanged(const QUrl &);

    void on_playFromUrlCheckBox_stateChanged(int arg1);

    void on_playFromListCheckBox_stateChanged(int arg1);

    void on_startGameButton_pressed();

    void on_enableBackCheckBox_stateChanged(int arg1);

    void on_returnButton_pressed();

    void on_spinBox_valueChanged(int arg1);

    void on_stopButton_pressed();

    void on_specifierUrlButton_pressed();

    void on_copyUrlButton_pressed();

public slots:
    void finishLoading(bool);
    void removeFrom();
private:
    QStringList historique;
    bool ingame = false;
    QString jQuery;
    QNetworkAccessManager *m_manager;
    QSettings *m_settings;
    QElapsedTimer gameTimer;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
