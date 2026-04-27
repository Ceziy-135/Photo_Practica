#include <QApplication>
#include <QMessageBox>
#include "databasehelper.h"
#include "loginform.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    if (!DatabaseHelper::connect())
    {
        QMessageBox::critical(nullptr, "Ошибка",
                              "Не удалось подключиться к базе данных!\n"
                              "Проверьте настройки в databasehelper.cpp");
        return -1;
    }

    // Глобальный стиль: чёрный текст на всех виджетах и фоновое изображение
    app.setStyleSheet(
        // Цвет текста для всех основных элементов
        "QWidget, QPushButton, QLabel, QTableWidget, QLineEdit, QTextEdit, QCheckBox, QHeaderView::section { color: black; }"

        // Фоновое изображение для всех окон (форм)
        "QMainWindow, QDialog, QWidget { background-image: url(/home/ceziy/Photo_Master/background.jpg); background-repeat: no-repeat; background-position: center; background-attachment: fixed; background-size: cover; }"

        // Дополнительно: белый фон для таблиц и полей ввода, чтобы текст читался
        "QTableWidget { background-color: white; alternate-background-color: #f2f2f2; }"
        "QLineEdit, QTextEdit, QComboBox { background-color: white; }"
        );

    LoginForm loginForm;
    loginForm.show();

    return app.exec();
}