#ifndef PHONEDIALOG_H
#define PHONEDIALOG_H

#include <QDialog>

namespace Ui
{
class PhoneDialog;
}

class PhoneDialog: public QDialog
{
    Q_OBJECT

public:
    explicit PhoneDialog(QWidget *parent = nullptr);

    ~PhoneDialog();

    QString  name() const;

    QString  phoneNum() const;

    void     edit(const QString &name, const QString &number);

private slots:
    void     on_buttonBox_accepted();

signals:
    void     addingContact(QString &name, QString &number);

private:
    Ui::PhoneDialog *ui;
};

#endif // PHONEDIALOG_H
