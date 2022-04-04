/********************************************************************************
** Form generated from reading UI file 'QtBlockCipher.ui'
**
** Created by: Qt User Interface Compiler version 6.2.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTBLOCKCIPHER_H
#define UI_QTBLOCKCIPHER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtBlockCipherClass
{
public:
    QAction *actionExit;
    QAction *actionOpen;
    QAction *actionSaveAs;
    QAction *actionAbout;
    QAction *actionSave_key_as;
    QAction *actionOpen_key;
    QAction *actionOpen_iv;
    QAction *actionSave_iv_as;
    QWidget *centralwidget;
    QTextEdit *InputTextField;
    QTextEdit *KeyTextField;
    QPushButton *RunButton;
    QRadioButton *EncryptRadio;
    QRadioButton *DecryptRadio;
    QTextEdit *OutputTextField;
    QLabel *OutputLabel;
    QLabel *KeyLabel;
    QLabel *InputLabel;
    QPlainTextEdit *InitializationVectorTextField;
    QLabel *InitializationVectorLabel;
    QComboBox *AESModeComboBox;
    QLabel *ModeLabel;
    QCheckBox *DivideIntoBlocksCheckBox;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *QtBlockCipherClass)
    {
        if (QtBlockCipherClass->objectName().isEmpty())
            QtBlockCipherClass->setObjectName(QString::fromUtf8("QtBlockCipherClass"));
        QtBlockCipherClass->resize(541, 498);
        QtBlockCipherClass->setMaximumSize(QSize(541, 498));
        actionExit = new QAction(QtBlockCipherClass);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionOpen = new QAction(QtBlockCipherClass);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        actionSaveAs = new QAction(QtBlockCipherClass);
        actionSaveAs->setObjectName(QString::fromUtf8("actionSaveAs"));
        actionAbout = new QAction(QtBlockCipherClass);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionSave_key_as = new QAction(QtBlockCipherClass);
        actionSave_key_as->setObjectName(QString::fromUtf8("actionSave_key_as"));
        actionOpen_key = new QAction(QtBlockCipherClass);
        actionOpen_key->setObjectName(QString::fromUtf8("actionOpen_key"));
        actionOpen_iv = new QAction(QtBlockCipherClass);
        actionOpen_iv->setObjectName(QString::fromUtf8("actionOpen_iv"));
        actionSave_iv_as = new QAction(QtBlockCipherClass);
        actionSave_iv_as->setObjectName(QString::fromUtf8("actionSave_iv_as"));
        centralwidget = new QWidget(QtBlockCipherClass);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        InputTextField = new QTextEdit(centralwidget);
        InputTextField->setObjectName(QString::fromUtf8("InputTextField"));
        InputTextField->setGeometry(QRect(20, 30, 491, 141));
        InputTextField->setReadOnly(false);
        KeyTextField = new QTextEdit(centralwidget);
        KeyTextField->setObjectName(QString::fromUtf8("KeyTextField"));
        KeyTextField->setGeometry(QRect(20, 200, 231, 31));
        RunButton = new QPushButton(centralwidget);
        RunButton->setObjectName(QString::fromUtf8("RunButton"));
        RunButton->setGeometry(QRect(20, 430, 75, 24));
        EncryptRadio = new QRadioButton(centralwidget);
        EncryptRadio->setObjectName(QString::fromUtf8("EncryptRadio"));
        EncryptRadio->setGeometry(QRect(120, 430, 61, 20));
        EncryptRadio->setChecked(true);
        DecryptRadio = new QRadioButton(centralwidget);
        DecryptRadio->setObjectName(QString::fromUtf8("DecryptRadio"));
        DecryptRadio->setGeometry(QRect(190, 430, 61, 20));
        OutputTextField = new QTextEdit(centralwidget);
        OutputTextField->setObjectName(QString::fromUtf8("OutputTextField"));
        OutputTextField->setGeometry(QRect(20, 270, 491, 151));
        OutputTextField->setReadOnly(true);
        OutputLabel = new QLabel(centralwidget);
        OutputLabel->setObjectName(QString::fromUtf8("OutputLabel"));
        OutputLabel->setGeometry(QRect(240, 250, 38, 16));
        KeyLabel = new QLabel(centralwidget);
        KeyLabel->setObjectName(QString::fromUtf8("KeyLabel"));
        KeyLabel->setGeometry(QRect(130, 180, 21, 16));
        InputLabel = new QLabel(centralwidget);
        InputLabel->setObjectName(QString::fromUtf8("InputLabel"));
        InputLabel->setGeometry(QRect(250, 10, 31, 16));
        InitializationVectorTextField = new QPlainTextEdit(centralwidget);
        InitializationVectorTextField->setObjectName(QString::fromUtf8("InitializationVectorTextField"));
        InitializationVectorTextField->setGeometry(QRect(280, 200, 231, 31));
        InitializationVectorLabel = new QLabel(centralwidget);
        InitializationVectorLabel->setObjectName(QString::fromUtf8("InitializationVectorLabel"));
        InitializationVectorLabel->setGeometry(QRect(340, 180, 101, 16));
        AESModeComboBox = new QComboBox(centralwidget);
        AESModeComboBox->addItem(QString());
        AESModeComboBox->addItem(QString());
        AESModeComboBox->addItem(QString());
        AESModeComboBox->setObjectName(QString::fromUtf8("AESModeComboBox"));
        AESModeComboBox->setGeometry(QRect(420, 430, 91, 24));
        ModeLabel = new QLabel(centralwidget);
        ModeLabel->setObjectName(QString::fromUtf8("ModeLabel"));
        ModeLabel->setGeometry(QRect(380, 430, 31, 16));
        DivideIntoBlocksCheckBox = new QCheckBox(centralwidget);
        DivideIntoBlocksCheckBox->setObjectName(QString::fromUtf8("DivideIntoBlocksCheckBox"));
        DivideIntoBlocksCheckBox->setGeometry(QRect(390, 250, 121, 20));
        QtBlockCipherClass->setCentralWidget(centralwidget);
        menubar = new QMenuBar(QtBlockCipherClass);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 541, 22));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuHelp = new QMenu(menubar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        QtBlockCipherClass->setMenuBar(menubar);
        statusbar = new QStatusBar(QtBlockCipherClass);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        QtBlockCipherClass->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuHelp->menuAction());
        menuFile->addSeparator();
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionOpen_key);
        menuFile->addAction(actionOpen_iv);
        menuFile->addAction(actionSaveAs);
        menuFile->addAction(actionSave_key_as);
        menuFile->addAction(actionSave_iv_as);
        menuFile->addAction(actionExit);
        menuHelp->addAction(actionAbout);

        retranslateUi(QtBlockCipherClass);

        QMetaObject::connectSlotsByName(QtBlockCipherClass);
    } // setupUi

    void retranslateUi(QMainWindow *QtBlockCipherClass)
    {
        QtBlockCipherClass->setWindowTitle(QCoreApplication::translate("QtBlockCipherClass", "MainWindow", nullptr));
        actionExit->setText(QCoreApplication::translate("QtBlockCipherClass", "Exit", nullptr));
        actionOpen->setText(QCoreApplication::translate("QtBlockCipherClass", "Open", nullptr));
        actionSaveAs->setText(QCoreApplication::translate("QtBlockCipherClass", "Save as", nullptr));
        actionAbout->setText(QCoreApplication::translate("QtBlockCipherClass", "About", nullptr));
        actionSave_key_as->setText(QCoreApplication::translate("QtBlockCipherClass", "Save key as", nullptr));
        actionOpen_key->setText(QCoreApplication::translate("QtBlockCipherClass", "Open key", nullptr));
        actionOpen_iv->setText(QCoreApplication::translate("QtBlockCipherClass", "Open iv", nullptr));
        actionSave_iv_as->setText(QCoreApplication::translate("QtBlockCipherClass", "Save iv as", nullptr));
        RunButton->setText(QCoreApplication::translate("QtBlockCipherClass", "Run", nullptr));
        EncryptRadio->setText(QCoreApplication::translate("QtBlockCipherClass", "Encrypt", nullptr));
        DecryptRadio->setText(QCoreApplication::translate("QtBlockCipherClass", "Decrypt", nullptr));
        OutputLabel->setText(QCoreApplication::translate("QtBlockCipherClass", "Output", nullptr));
        KeyLabel->setText(QCoreApplication::translate("QtBlockCipherClass", "Key", nullptr));
        InputLabel->setText(QCoreApplication::translate("QtBlockCipherClass", "Input", nullptr));
        InitializationVectorLabel->setText(QCoreApplication::translate("QtBlockCipherClass", "Initialization vector", nullptr));
        AESModeComboBox->setItemText(0, QCoreApplication::translate("QtBlockCipherClass", "ECB", nullptr));
        AESModeComboBox->setItemText(1, QCoreApplication::translate("QtBlockCipherClass", "CBC", nullptr));
        AESModeComboBox->setItemText(2, QCoreApplication::translate("QtBlockCipherClass", "CFB", nullptr));

        ModeLabel->setText(QCoreApplication::translate("QtBlockCipherClass", "Mode", nullptr));
        DivideIntoBlocksCheckBox->setText(QCoreApplication::translate("QtBlockCipherClass", "Divide into blocks", nullptr));
        menuFile->setTitle(QCoreApplication::translate("QtBlockCipherClass", "File", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("QtBlockCipherClass", "Help", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QtBlockCipherClass: public Ui_QtBlockCipherClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTBLOCKCIPHER_H
