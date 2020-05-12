#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <QDialog>
#include <string>

namespace Ui {
class HelpDialog;
}

class HelpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HelpDialog(QWidget *parent = nullptr, std::string helpText = "");
    ~HelpDialog();

private slots:
    void on_okButton_clicked();

private:
    Ui::HelpDialog *ui;
};

#endif // HELPDIALOG_H
