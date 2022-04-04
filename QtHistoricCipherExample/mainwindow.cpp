#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Permutation cipher; Author: Paweł Perechuda; 145199");
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString historicCipherStep(QString text, QVector<size_t> key) {
    QString result = "";

    for (QVector<size_t>::Iterator it = key.begin(); it != key.end(); ++it) {
        result += text[*it - 1];
    }

    return result;
}

QString historicCipher(QString input, QVector<size_t> key, const bool decrypt) {
    QString result = "";
    input += QString(input.length() % key.size() ? key.size() - input.length() % key.size() : 0, ';');

    if (decrypt) {
        QVector<size_t> newKey(key.size());
        for (ssize_t i = 0; i < key.size(); ++i) {
            newKey[i] = key.indexOf(i+1)+1;
        }
        key = newKey;
    }

    for (ssize_t i = 0; i < input.length() / key.size(); ++i) {
        result += historicCipherStep(input.mid(i * key.size(), key.size()), key);
    }

    return result;
}


void handleHistoricCipher(QTextEdit* Qinput, QTextEdit* Qoutput, QTextEdit* Qkey, const bool decrypt) {
    QMessageBox msgBox;
    QString key = Qkey->toPlainText();
    QString result;

    QRegularExpression reNumbers("\\d+(?=\";., /\\\")*");
    //QRegularExpressionMatch keyMatch = reNumbers.match(key);
    QRegularExpressionMatchIterator it = reNumbers.globalMatch(key);

    QVector<size_t> keyVec;
    long long maxElem = -1;
    while(it.hasNext()) {
        long long elem = it.next().captured(0).toLongLong();
        maxElem = qMax(maxElem, elem);
        if (elem <= 0) {
            msgBox.setText("A key element \"" + QString::number(elem) + "\" is less than 1.");
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();
            return;
        }
        if (std::find(keyVec.begin(), keyVec.end(), elem) != keyVec.end()) {
            msgBox.setText("A key element \"" + QString::number(elem) + "\" is a duplicate.");
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();
            return;
        }
        keyVec.push_back(elem);
    }
    if (maxElem > keyVec.size()) {
        msgBox.setText("A key element \"" + QString::number(maxElem) + "\" is larger than key length.\nKey length is " + QString::number(keyVec.size()) + ".");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return;
    }
    else if (!keyVec.size()) {
        Qoutput->setPlainText(Qinput->toPlainText());
        return;
    }

    QString input = Qinput->toPlainText();

    result = historicCipher(input, keyVec, decrypt);
    Qoutput->setPlainText(result);
}



void MainWindow::on_RunButton_clicked()
{
    handleHistoricCipher(ui->InputTextField, ui->OutputTextField, ui->KeyTextField, ui->DecryptRadio->isChecked());
}


void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_actionOpen_triggered()
{
    // get absolute path, as qstring, to one text file
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open text file"), "", tr("Text files (*.txt)"));

    QFile inputFile(fileName);
    inputFile.open(QIODevice::ReadOnly);

    QTextStream in(&inputFile);
    QString text = in.readAll();
    inputFile.close();

    ui->InputTextField->setPlainText(text);
    QTextCursor cursor = ui->InputTextField->textCursor();
    cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor, 1);
    ui->InputTextField->setTextCursor(cursor);
}


void MainWindow::on_KeyTextField_textChanged()
{
    QString keyText = ui->KeyTextField->toPlainText();

    ssize_t keyTextBeforeLength = keyText.length();

    auto lmbd =  [](QChar c) {
        QString s = c;
        int pos = 0;
        return QRegularExpressionValidator(QRegularExpression("[[:digit:] ,/;]")).validate(s, pos) == QRegularExpressionValidator::Invalid;
    };

    // Handle only text from keyboard
    QTextCursor cursor = ui->KeyTextField->textCursor();

    keyText.removeIf(lmbd);

   if (keyText.length() != keyTextBeforeLength) {
       cursor.movePosition(QTextCursor::End); // just move to the end
       ui->KeyTextField->setPlainText(keyText);
       ui->KeyTextField->setTextCursor(cursor);
   }
}


void MainWindow::on_actionSaveAs_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save encrypted text file"), "", tr("Text files (*.txt)"));

    if (fileName.isEmpty()) {
        return;
    }
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
            file.errorString());
            return;
        }
        QTextStream out(&file);
        out << ui->OutputTextField->toPlainText();
    }
}


void MainWindow::on_actionSave_key_as_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save key file"), "", tr("Text files (*.txt)"));

    if (fileName.isEmpty()) {
        return;
    }
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
            file.errorString());
            return;
        }
        QTextStream out(&file);
        out << ui->KeyTextField->toPlainText();
    }
}


void MainWindow::on_actionOpen_key_triggered()
{
    // get absolute path, as qstring, to one text file
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open key file"), "", tr("Text files (*.txt)"));

    QFile inputFile(fileName);
    inputFile.open(QIODevice::ReadOnly);

    QTextStream in(&inputFile);
    QString text = in.readAll();
    inputFile.close();

    ui->KeyTextField->setPlainText(text);
    QTextCursor cursor = ui->KeyTextField->textCursor();
    cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor, 1);
    ui->KeyTextField->setTextCursor(cursor);
}


void MainWindow::on_actionAbout_triggered()
{
    QMessageBox msgBox;
    msgBox.about(this, "About", "Historic cipher implemenation: permutation cipher\nAuthor: Paweł Perechuda; 145199\n"
                                "Cipher explanation:"
                                "Text is split into text_length / key_length substrings of length key_length (last substring is filled with ';' to be of required length)\n"
                                "Next for each substring encryption is performed:\n"
                                "n-th character in a resulting output is the i-th character of the provided substring\n"
                                "where i is the value of a key element and n is the index of the i value in the key\n"
                                "For decryption the key is modified so the i and n values are swapped, then output substring is generated in the same way.");
}

