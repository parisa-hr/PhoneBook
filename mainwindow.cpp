#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMouseEvent>
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <QFile>
#include <QDataStream>
#include <QString>
#include <QList>
#include <QFileDialog>
#include "tablemodel.h"

#include "phonedialog.h"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    table(new TableModel(this))
{
    ui->setupUi(this);
    setWindowFlag(Qt::FramelessWindowHint, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_AlwaysStackOnTop, true);
    setAutoFillBackground(true);


    auto  proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(table);
    proxyModel->setFilterKeyColumn(0);

    ui->tableView->setModel(proxyModel);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);

    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSortingEnabled(true);


    connect(ui->pb_edit, &QPushButton::clicked, this, &MainWindow::editEntry);
    connect(ui->pb_remove, &QPushButton::clicked, this, &MainWindow::removeEntry);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void  MainWindow::addEntry(const QString &name, const QString &phoneNumber)
{
    if (!table->getContacts().contains({ name, phoneNumber }))
    {
        table->insertRows(0, 1, QModelIndex());

        QModelIndex  index = table->index(0, 0, QModelIndex());
        table->setData(index, name, Qt::EditRole);
        index = table->index(0, 1, QModelIndex());
        table->setData(index, phoneNumber, Qt::EditRole);
    }
    else
    {
        QMessageBox::information(this, tr("Duplicate Name"),
                                 tr("The name \"%1\" already exists.").arg(name));
    }
}

void  MainWindow::editEntry()
{
    QTableView            *temp           = static_cast<QTableView *>(ui->tableView);
    QSortFilterProxyModel *proxy          = static_cast<QSortFilterProxyModel *>(temp->model());
    QItemSelectionModel   *selectionModel = temp->selectionModel();
    const QModelIndexList  indexes        = selectionModel->selectedRows();
    QString                name;
    QString                phoneNumber;
    int                    row = -1;

    for (const QModelIndex &index : indexes)
    {
        row = proxy->mapToSource(index).row();
        QModelIndex  nameIndex = table->index(row, 0, QModelIndex());
        QVariant     varName   = table->data(nameIndex, Qt::DisplayRole);
        name = varName.toString();

        QModelIndex  phoneNumIndex = table->index(row, 1, QModelIndex());
        QVariant     varAddr       = table->data(phoneNumIndex, Qt::DisplayRole);
        phoneNumber = varAddr.toString();
    }

    PhoneDialog  aDialog(this);
    aDialog.setWindowTitle(tr("Edit a Contact"));
    aDialog.edit(name, phoneNumber);

    if (aDialog.exec())
    {
        const QString  newNumber = aDialog.phoneNum();
        const QString  newName   = aDialog.name();

        if (newNumber != phoneNumber)
        {
            const QModelIndex  index = table->index(row, 1, QModelIndex());
            table->setData(index, newNumber, Qt::EditRole);
        }

        if (newName != name)
        {
            const QModelIndex  index = table->index(row, 0, QModelIndex());
            table->setData(index, newName, Qt::EditRole);
        }
    }
}

void  MainWindow::removeEntry()
{
    QTableView            *temp           = static_cast<QTableView *>(ui->tableView);
    QSortFilterProxyModel *proxy          = static_cast<QSortFilterProxyModel *>(temp->model());
    QItemSelectionModel   *selectionModel = temp->selectionModel();
    const QModelIndexList  indexes        = selectionModel->selectedRows();

    for (QModelIndex index : indexes)
    {
        int  row = proxy->mapToSource(index).row();
        table->removeRows(row, 1, QModelIndex());
    }

    if (table->rowCount(QModelIndex()) == 0)
    {
        QMessageBox::information(this, tr("Error"),
                                 tr("There is not any Contact :) "));
    }
}

void  MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void  MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        move(event->globalPos() - dragPosition);
        event->accept();
    }
}

void  MainWindow::on_tb_close_clicked()
{
    close();
}

void  MainWindow::on_tb_minimize_clicked()
{
    showMinimized();
}

void  MainWindow::on_pb_add_clicked()
{
    PhoneDialog  diag(this);

    connect(&diag, &PhoneDialog::addingContact, this, &MainWindow::addEntry);
    diag.exec();
}

void  MainWindow::on_pb_save_clicked()
{
    QString  fileName;

    // If we don't have a filename from before, get one.
    if (currentFile.isEmpty())
    {
        fileName    = QFileDialog::getSaveFileName(this, "Save", "/home", tr("PhoneBook (*.dat)"));
        currentFile = fileName;
    }
    else
    {
        fileName = currentFile;
    }

    QFile  file(fileName);

    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::information(this, tr("Unable to open file"), file.errorString());

        return;
    }

    QDataStream  out(&file);
    out << table->getContacts();

    file.flush();
    file.close();
}

void  MainWindow::on_pb_load_clicked()
{
    QString  fileName = QFileDialog::getOpenFileName(this);

    if (!fileName.isEmpty())
    {
        QFile  file(fileName);

        if (!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::information(this, tr("Unable to open file"),
                                     file.errorString());

            return;
        }

        QList<Contact>  contacts;
        QDataStream     in(&file);
        in >> contacts;

        if (contacts.isEmpty())
        {
            QMessageBox::information(this, tr("No contacts in file"),
                                     tr("The file you are attempting to open contains no contacts."));
        }
        else
        {
            for (const auto &contact : qAsConst(contacts))
            {
                addEntry(contact.name, contact.phoneNumber);
            }
        }
    }
}

void  MainWindow::on_pb_exp_csv_clicked()
{
    QString  fileName;

    // If we don't have a filename from before, get one.
    if (currentCSVFile.isEmpty())
    {
        fileName       = QFileDialog::getSaveFileName(this, "Save", "/home", tr("csv (*.csv)"));
        currentCSVFile = fileName;
    }
    else
    {
        fileName = currentCSVFile;
    }

    QFile  file(fileName);

    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::information(this, tr("Unable to open file"), file.errorString());

        return;
    }

    QTextStream  out(&file);

    for (auto i : table->getContacts())
    {
        out << i.name << ";" << i.phoneNumber << "\n";
    }

    file.flush();
    file.close();
}
