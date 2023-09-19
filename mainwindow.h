#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QCoreApplication>
#include <tgbot/tgbot.h>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setExitRequested(bool value);
    QString gettext();
    const char* cleartext();
    void setupDatabase();
    void insertChatID(int64_t chatID);
    std::vector<int64_t> chatIDsToInsert;
protected:
    void closeEvent(QCloseEvent *event) override;

signals:
    void aboutToClose();

private slots:
    void on_pushButton_clicked();

private:

    QString copiedText;
    std::atomic<bool> exitRequested;

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
