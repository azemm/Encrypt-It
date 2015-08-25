#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtWidgets>
#include <QtCore>
#include <QtDebug>
#include "zipencrypt.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void updateLabelChooseFile(QLabel *lineEdit,const QString &path);
    void updateLabelChooseDirectory(QLabel *lineEdit,const QString &path);


private slots:
    void on_enc_choose_clicked();
    void on_dec_choose_clicked();

    void on_enc_btn_clicked();

    void on_dec_btn_clicked();

private:
    Ui::MainWindow *ui;
    QString encPath;
    QString encPassword;
    QString decPath;
    QString decPassword;

    ZipEncrypt zipEncrypt;
};

#endif // MAINWINDOW_H
