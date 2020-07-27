#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString paramFileName = "param.ini";
    m_settings = new QSettings(paramFileName, QSettings::IniFormat, this);
    QFile f(paramFileName);
    if(!f.exists()) {
       setDefaultParam();
    }
    ui->spinBox->setValue(m_settings->value("num-manche").toInt());

    //I found a smarter way to do this
    //QTextBrowser *hideSearchBarWidget = new QTextBrowser(ui->pageWeb);
    //hideSearchBarWidget->setGeometry(0, 65, 1600, 77);

    if(m_settings->value("game-mode").toString() == "playFromUrl") {
        ui->playFromListCheckBox->setCheckState(Qt::CheckState::Unchecked);
        ui->playFromUrlCheckBox->setCheckState(Qt::CheckState::Checked);
    } else {
        ui->playFromListCheckBox->setCheckState(Qt::CheckState::Checked);
        ui->playFromUrlCheckBox->setCheckState(Qt::CheckState::Unchecked);
    }
    if(m_settings->value("enable-back").toBool() == true) {
        ui->returnButton->setEnabled(true);
        ui->enableBackCheckBox->setCheckState(Qt::CheckState::Checked);
    } else {
        ui->returnButton->setEnabled(false);
        ui->enableBackCheckBox->setCheckState(Qt::CheckState::Unchecked);
    }
    m_manager = new QNetworkAccessManager(this);

    ui->pageWeb->load(QUrl(m_settings->value("base-url").toString()));

    QFile file;
    file.setFileName(":/jquery.min.js");
    file.open(QIODevice::ReadOnly);
    jQuery = file.readAll();
    jQuery.append("\nvar qt = { 'jQuery': jQuery.noConflict(true) };");
    file.close();
    connect(ui->pageWeb, &QWebEngineView::loadFinished, this, &MainWindow::finishLoading);

    gameTimer.start();
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pageWeb_urlChanged(const QUrl &url)
{
    if(ingame) {
        historique.append(url.toString());
    }
    if(url == m_settings->value("end-game-url") && ingame) {
        stopGame("Bravo, vous avez gagner", gameTimer.elapsed());
    }
}

void MainWindow::startGame() {
    ui->startGameButton->setEnabled(false);
    ui->stopButton->setEnabled(true);
    hideUiGm(false);
    ui->pageWeb->load(QUrl(m_settings->value("start-game-url").toString()));
    ingame = true;
    gameTimer.restart();
}

void MainWindow::stopGame(QString reason, qint64 elapsedTime) {
    ui->startGameButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
    hideUiGm(true);
    ui->pageWeb->load(QUrl(m_settings->value("base-url").toString()));
    ingame = false;
    std::stringstream ss;
    ss << elapsedTime;
    std::string str = ss.str();
    QString str2 = QString::fromStdString(str);
    QString str3 = "";
    for(int i = 0; i < str2.size()-9; i++) {
        str3+=str2[i];
    }
    str3 += ",";
    for(int i = str3.size()-1; i < str2.size(); i++) {
        str3 += str2[i];
    }
    new endGameDialog(reason + " temps de la partie: " + str3  + " secondes", historique, this);
    QStringList empty;
    historique = empty;
}

void MainWindow::finishLoading(bool) {
    ui->pageWeb->page()->runJavaScript(jQuery);
    removeFrom();
}

void MainWindow::removeFrom() {
    QString code = QStringLiteral("qt.jQuery('form').remove()");
    ui->pageWeb->page()->runJavaScript(code);
}

void MainWindow::setDefaultParam() {
     m_settings->setValue("base-url", tr("https://fr.wikipedia.org/wiki/Wikip%C3%A9dia:Accueil_principal", "url du site wikipedia de la langue"));
     m_settings->setValue("end-game-url", tr("https://fr.wikipedia.org/wiki/Programmation_informatique", "url de la page wikipedia de la programmation informatique"));
     m_settings->setValue("start-game-url", tr("https://fr.wikipedia.org/wiki/Bretagne", "url de la page wikipedia de la bretagne"));
     m_settings->setValue("enable-back", true);
     m_settings->setValue("game-mode", "playFromUrl");
     m_settings->setValue("num-manche", 1);
}

void MainWindow::hideUiGm(bool state) {
    ui->spinBox->setEnabled(state);
    ui->loadListButton->setEnabled(state);
    ui->enableBackCheckBox->setEnabled(state);
    ui->specifierUrlButton->setEnabled(state);
    ui->playFromUrlCheckBox->setEnabled(state);
    ui->playFromListCheckBox->setEnabled(state);
}

void MainWindow::on_playFromUrlCheckBox_stateChanged(int arg1)
{
    if(arg1) {
        ui->playFromListCheckBox->setCheckState(Qt::CheckState::Unchecked);
        m_settings->setValue("game-mode", "playFromUrl");
    } else {
        ui->playFromListCheckBox->setCheckState(Qt::CheckState::Checked);
    }
}

void MainWindow::on_playFromListCheckBox_stateChanged(int arg1)
{
    if(arg1) {
        ui->playFromUrlCheckBox->setCheckState(Qt::CheckState::Unchecked);
        m_settings->setValue("game-mode", "playFromList");
    } else {
        ui->playFromUrlCheckBox->setCheckState(Qt::CheckState::Checked);
    }
}

void MainWindow::on_startGameButton_pressed()
{
    startGame();
}

void MainWindow::on_enableBackCheckBox_stateChanged(int arg1)
{
    if(arg1) {
        m_settings->setValue("enable-back", true);
        ui->returnButton->setEnabled(true);
    } else {
        m_settings->setValue("enable-back", false);
        ui->returnButton->setEnabled(false);
    }
}

void MainWindow::on_returnButton_pressed()
{
    ui->pageWeb->back();
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    m_settings->setValue("num-manche", arg1);
}

void MainWindow::on_stopButton_pressed()
{
    stopGame("vous avez abandonné", gameTimer.nsecsElapsed());
}
