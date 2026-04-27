#ifndef HELPFORM_H
#define HELPFORM_H

#include <QMainWindow>
#include <QTextEdit>
#include <QPushButton>

class HelpForm : public QMainWindow
{
    Q_OBJECT

public:
    HelpForm(QWidget *parent = nullptr);
    ~HelpForm();

private slots:
    void saveToPdf();

private:
    QTextEdit* textEdit;
    QPushButton* btnSavePdf;
};

#endif // HELPFORM_H