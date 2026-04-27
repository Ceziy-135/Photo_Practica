#include "helpform.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QPrinter>
#include <QMessageBox>

HelpForm::HelpForm(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Справка - Фото Мастерская");
    setMinimumSize(700, 500);
    setStyleSheet("background-color: #f0f0f0;");

    QWidget* central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout* mainLayout = new QVBoxLayout(central);

    textEdit = new QTextEdit();
    textEdit->setReadOnly(true);
    textEdit->setHtml(
        "<h1 align='center'>Фото Мастерская - Справка</h1>"
        "<h2>Авторизация</h2>"
        "<p>Логин: admin, пароль: admin (администратор)<br>Логин: user1, пароль: user1 (пользователь)</p>"
        "<h2>Главное меню</h2>"
        "<ul><li><b>Заказчики работ</b> – управление клиентами</li>"
        "<li><b>Исполнители</b> – список фотографов</li>"
        "<li><b>Прайс работ</b> – фиксированные цены 300/100 руб.</li>"
        "<li><b>Журнал заказов</b> – создание и редактирование заказов</li></ul>"
        "<h2>Статусы заказов</h2>"
        "<p><b>Не выполнено</b> – заказ в работе<br><b>Выполнено</b> – заказ готов</p>"
        "<p>© 2026 Беренский Дмитрий</p>"
        );
    mainLayout->addWidget(textEdit);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnSavePdf = new QPushButton("Сохранить справку (PDF)");
    btnSavePdf->setStyleSheet("background-color: #3498db; color: white;");
    btnSavePdf->setFixedWidth(200);
    btnLayout->addWidget(btnSavePdf);
    btnLayout->addStretch();
    mainLayout->addLayout(btnLayout);

    connect(btnSavePdf, &QPushButton::clicked, this, &HelpForm::saveToPdf);
}

HelpForm::~HelpForm() {}

void HelpForm::saveToPdf()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить PDF", "справка.pdf", "*.pdf");
    if (fileName.isEmpty()) return;
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    textEdit->document()->print(&printer);
    QMessageBox::information(this, "Успех", "Справка сохранена в PDF!");
}