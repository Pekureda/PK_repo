#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtBlockCipher.h"

#include <QFileDialog>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionValidator>
#include <QRegularExpressionMatchIterator>
#include <QMessageBox>
#include <qtextstream.h>

#include <initializer_list>

typedef long long ssize_t;

class QtBlockCipher : public QMainWindow
{
    Q_OBJECT

public:
    QtBlockCipher(QWidget *parent = Q_NULLPTR);

private slots:
    void on_RunButton_clicked();

    void on_actionExit_triggered();

    void on_actionOpen_triggered();

    void on_KeyTextField_textChanged();

    void on_actionSaveAs_triggered();

    void on_actionSave_key_as_triggered();

    void on_actionOpen_key_triggered();

    void on_actionAbout_triggered();

    void on_AESModeComboBox_currentIndexChanged(int index);

    void on_DivideIntoBlocksCheckBox_toggled(bool checked);

    void on_InitializationVectorTextField_textChanged();

private:
    Ui::QtBlockCipherClass ui;
};
