#include "phonedialog.h"
#include "ui_phonedialog.h"

#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QValidator>
#include <QMessageBox>

PhoneDialog::PhoneDialog(QWidget *parent):
    QDialog(parent),
    ui(new Ui::PhoneDialog)
{
    ui->setupUi(this);
    QRegularExpression  re("^[0-9]{11}$");
    QValidator         *validator = new QRegularExpressionValidator(re, this);
    ui->lineEdit_number->setValidator(validator);
}

PhoneDialog::~PhoneDialog()
{
    delete ui;
}

QString  PhoneDialog::name() const
{
    return ui->lineEdit_name->text();
}

QString  PhoneDialog::phoneNum() const
{
    return ui->lineEdit_number->text();
}

void  PhoneDialog::edit(const QString &name, const QString &number)
{
    ui->lineEdit_name->setText(name);
    ui->lineEdit_number->setText(number);
}

void  PhoneDialog::on_buttonBox_accepted()
{
    if (!(ui->lineEdit_name->text().isEmpty()) && !(ui->lineEdit_number->text().isEmpty()))
    {
        auto  name = ui->lineEdit_name->text();
        auto  num  = ui->lineEdit_number->text();
        emit  addingContact(name, num);
    }
    else
    {
        QMessageBox::information(this, tr("Error"),
                                 tr("You Should Fill Both Name and Phone number"));
    }
}
