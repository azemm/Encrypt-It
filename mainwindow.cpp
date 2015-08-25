#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->tabWidget);
    ui->enc_pw->setEchoMode(QLineEdit::Password);
    ui->enc_confirm_pw->setEchoMode(QLineEdit::Password);

    ui->dec_password->setEchoMode(QLineEdit::Password);

    ui->tabWidget->setStyleSheet("QTabWidget::tab-bar {alignment: center;}");
    ui->tabWidget->setTabPosition(QTabWidget::North);
    ui->tabWidget->setTabShape(QTabWidget::Rounded);

    updateLabelChooseDirectory(ui->enc_label, encPath);
    updateLabelChooseFile(ui->dec_label, decPath);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateLabelChooseFile(QLabel *lineEdit, const  QString &path){
    if(!path.isEmpty()){
        lineEdit->setText(path);
    } else {
        lineEdit->setText("Choose file to decrypt");
    }
}

void MainWindow::updateLabelChooseDirectory(QLabel *lineEdit, const  QString &path){
    if(!path.isEmpty()){
        lineEdit->setText(path);
    } else {
        lineEdit->setText("Choose directory to encrypt");
    }
}

void MainWindow::on_enc_choose_clicked()
{
    QString file = QFileDialog::getExistingDirectory(this, "Choose directory");
    if(!file.isEmpty()){
        encPath = file;
        updateLabelChooseDirectory(ui->enc_label, encPath);
        qDebug() << "Folder to encrypt : " << encPath;
    }
}

void MainWindow::on_dec_choose_clicked()
{
    QString file = QFileDialog::getOpenFileName(this, "Choose file");
    if(!file.isEmpty()){
        decPath = file;
        updateLabelChooseFile(ui->enc_label, encPath);
        qDebug() << "Folder to decrypt : " << encPath;
    }
}

void MainWindow::on_enc_btn_clicked()
{
    if(ui->enc_pw->text() == ui->enc_confirm_pw->text()){
        encPassword = ui->enc_pw->text();
    }

    if(encPath.isEmpty()){
        QMessageBox::warning(this, "Directory not choosen", "You must a choose a directory to encrypt");
    }
    else if(encPassword.isEmpty()){
        QMessageBox::warning(this, "Password incorrect", "Passwords not equals or not set");
    } else {
        bool result = zipEncrypt.zipEncrypt(encPath.toStdString() ,encPassword.toStdString(), ui->enc_delFiles->isChecked(), ui->enc_delOrigin->isChecked());
        if(result){
            QString title = "Successful Encryption";
            QString message =  "The directory " + encPath + " has been successfully encrypted";
            QMessageBox::information(this, title, message);
        } else {
            QString title = "Encryption failed";
            QString message =  "An error occured";
            QMessageBox::warning(this, title, message);
        }

        encPath = "";
        encPassword = "";
        ui->enc_label->setText("");
        ui->enc_pw->setText("");
        ui->enc_confirm_pw->setText("");
        ui->enc_delFiles->setChecked(false);
        ui->enc_delOrigin->setChecked(false);
        updateLabelChooseDirectory(ui->enc_label, encPath);
    }
}

void MainWindow::on_dec_btn_clicked()
{
    decPassword = ui->dec_password->text();

    if(decPath.isEmpty()){
        QMessageBox::warning(this, "Directory not choosen", "You must a choose a directory to encrypt");
    }
    else if(decPassword.isEmpty()){
        QMessageBox::warning(this, "Password incorrect", "Please enter a password in order to decryt the file");
    } else {
        bool result = zipEncrypt.zipDecrypt(decPath.toStdString() ,decPassword.toStdString(), ui->dec_delFiles->isChecked(), ui->dec_delOrigin->isChecked());
        if(result){
            QString title = "Successful Decryption";
            QString message =  "The directory " + encPath + " has been successfully decrypted";
            QMessageBox::information(this, title, message);

            decPath = "";
            decPassword = "";
            ui->dec_label->setText("");
            ui->dec_password->setText("");
            ui->dec_delFiles->setChecked(false);
            ui->dec_delOrigin->setChecked(false);
            updateLabelChooseFile(ui->dec_label, decPath);
        } else {
            QString title = "Decryption failed";
            QString message =  "Please verify you entered the corred password";
            QMessageBox::warning(this, title, message);

            ui->dec_password->setText("");
        }
    }
}
