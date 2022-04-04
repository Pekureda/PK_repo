#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionValidator>
#include <QRegularExpressionMatchIterator>
#include <QMessageBox>
#include <type_traits>

typedef long long ssize_t;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_RunButton_clicked();

    void on_actionExit_triggered();

    void on_actionOpen_triggered();

    void on_KeyTextField_textChanged();

    void on_actionSaveAs_triggered();

    void on_actionSave_key_as_triggered();

    void on_actionOpen_key_triggered();

    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
