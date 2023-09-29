
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QCloseEvent>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    exitRequested = false;

    connect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit, [&]() {
        setExitRequested(true);
    });
}

MainWindow::~MainWindow()
{
    disconnect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit, nullptr, nullptr);
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    emit aboutToClose();
    event->accept();
}
void MainWindow::setExitRequested(bool value) {
    exitRequested = value;
}
void MainWindow::setupDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("chat_ids.db");

    if (!db.open()) {
        qDebug() << "Error opening database:" << db.lastError().text();
    } else {
        QSqlQuery query;
        if (query.exec("CREATE TABLE IF NOT EXISTS chat_ids (chat_id INTEGER PRIMARY KEY)")) {
            qDebug() << "Database and table created or already exist";
        } else {
            qDebug() << "Error creating table:" << query.lastError().text();
        }
    }
    QSqlQuery query;
    for (int64_t chatID : chatIDsToInsert) {
        query.prepare("INSERT OR IGNORE INTO chat_ids (chat_id) VALUES (:chatID)");
        query.bindValue(":chatID", chatID);

        if (query.exec()) {
            qDebug() << "Chat ID" << chatID << "inserted or ignored successfully";
        } else {
            qDebug() << "Error inserting chat ID" << chatID << ":" << query.lastError().text();
        }
    }
    chatIDsToInsert.clear();
}
void MainWindow::on_pushButton_clicked()
{
    TgBot::Bot bot("Your Token Here");
    setupDatabase();

    copiedText = ui->textEdit->toPlainText();

    if (!copiedText.isEmpty()) {
        printf("Admin wrote %s \n", copiedText.toStdString().c_str());
        fflush(stdout);

        ui->textEdit->clear();

    }
    QSqlQuery query;
    QString userText = gettext();
    int count = 0;
    if (!userText.isEmpty()) {
        query.exec("SELECT chat_id FROM chat_ids");
        while (query.next()) {
            int64_t userID = query.value(0).toLongLong();
            try {
                std::string userTextStdString = userText.toStdString();
                bot.getApi().sendMessage(userID, userTextStdString);
                count++;
            } catch (TgBot::TgException& e) {
                printf("Error sending message to user %lld: %s\n", userID, e.what());
                fflush(stdout);

                std::string errorMessage = e.what();
                if (bot.getApi().blockedByUser(userID)) {
                    QSqlQuery deleteQuery;
                    deleteQuery.prepare("DELETE FROM chat_ids WHERE chat_id = ?");
                    deleteQuery.addBindValue(QVariant(userID));
                    if (!deleteQuery.exec()) {
                        printf("Error deleting user from the database: %s\n", deleteQuery.lastError().text().toStdString().c_str());
                        fflush(stdout);
                    } else {
                        printf("User %lld deleted from the database.\n", userID);
                        fflush(stdout);
                    }
                }
            }
        }

        printf("Message send to %i user(s)\n", count);
        fflush(stdout);

        userText = cleartext();
    }
}
QString MainWindow::gettext(){
    return copiedText;
}
const char* MainWindow::cleartext() {
    copiedText.clear();
    return "";
}
