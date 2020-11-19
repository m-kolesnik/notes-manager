#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "database.h"
#include "addnote.h"
#include "editnote.h"
#include "priority.h"
#include <fstream>
#include <iostream>
#include <QMainWindow>
#include <QMessageBox>
#include <QListView>
#include <QSystemTrayIcon>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void updateListView();
    void filterByDate();
    void filterByPriority();

private slots:
    void on_pushButton_clicked();
    void on_listView_pressed(const QModelIndex &index);
    void on_pushButtonRemoveNote_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButtonEditNote_clicked();
    void on_comboBoxCategory_currentIndexChanged(const QString &arg1);

    void on_pushButtonCompleteNote_clicked();

    void on_pushButton_4_clicked();

private:
    Note currentNote;
    Ui::MainWindow *ui;
    QSystemTrayIcon *icon;
    Database *database;
};
#endif // MAINWINDOW_H
