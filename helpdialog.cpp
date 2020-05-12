#include "helpdialog.h"
#include "ui_helpdialog.h"
#include "mainwindow.h"

HelpDialog::HelpDialog(QWidget *parent, std::string helpText) :
    QDialog(parent),
    ui(new Ui::HelpDialog)
{
    ui->setupUi(this);
    ui->textEdit->setReadOnly(true);
    ui->textEdit->setText(helpText.c_str());
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setFixedSize(size());
    setWindowTitle("Help");
}

HelpDialog::~HelpDialog()
{
    delete ui;
}

void HelpDialog::on_okButton_clicked()
{
    this->close();
}
