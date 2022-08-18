#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPoint>

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class TableModel;


class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

    void  addEntry(const QString &name, const QString &phoneNumber);

    void  editEntry();

    void  removeEntry();

    void  openDialog();

protected:
    void  mousePressEvent(QMouseEvent *event);

    void  mouseMoveEvent(QMouseEvent *event);

private slots:
    void  on_tb_close_clicked();

    void  on_tb_minimize_clicked();

    void  on_pb_add_clicked();

    void  on_pb_save_clicked();

    void  on_pb_load_clicked();

    void  on_pb_exp_csv_clicked();

private:
    Ui::MainWindow *ui;
    QPoint          dragPosition;
    TableModel     *table;
    QString         currentFile;
    QString         currentCSVFile;
};

#endif // MAINWINDOW_H
