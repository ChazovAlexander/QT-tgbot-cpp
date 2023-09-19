#include "mainwindow.h"

#include <QApplication>

#include <tgbot/tgbot.h>

#include <thread>
#include <atomic>
#include <mutex>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

static std::atomic<bool> exitRequested(false);
static std::mutex exitMutex;

void botThread(MainWindow& w) {
    TgBot::Bot bot("Your token here!");

    bot.getEvents().onCommand("start", [&bot, &w](TgBot::Message::Ptr message) {
        w.chatIDsToInsert.push_back(message->chat->id);
        bot.getApi().sendMessage(message->chat->id, "Hi!");
    });

    bot.getEvents().onNonCommandMessage([&bot](TgBot::Message::Ptr message) {
        bot.getApi().sendMessage(message->chat->id, "User wrote: " + message->text);
        printf("User wrote: %s\n", message->text.c_str());
        fflush(stdout);
    });

    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        fflush(stdout);
        TgBot::TgLongPoll longPoll(bot);
        while (true) {
            {
                std::lock_guard<std::mutex> lock(exitMutex);
                if (exitRequested) {
                    break;
                }
            }

            printf("Long poll started\n");
            fflush(stdout);

            longPoll.start();

        }

    } catch (TgBot::TgException& e) {
        printf("error: %s\n", e.what());
        fflush(stdout);
    }
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    std::thread botThreadObj([&w]() { botThread(w); });
    QObject::connect(&w, &MainWindow::aboutToClose, [&botThreadObj]() {
        {
            std::lock_guard<std::mutex> lock(exitMutex);
            exitRequested = true;
        }
        botThreadObj.join();
    });

    w.show();

    return a.exec();
}
