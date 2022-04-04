#include "QtBlockCipher.h"
#include "AES_crypto.h"

QtBlockCipher::QtBlockCipher(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    this->setWindowTitle("Block ciphers; Author: Paweł Perechuda; 145199");
    ui.InitializationVectorTextField->setDisabled(true); // Starting at ECB
}

void QtBlockCipher::on_RunButton_clicked()
{
    QString temp;
    QMessageBox msgBox;
    if (ui.KeyTextField->toPlainText().length() != BLOCK_SIZE) {
        msgBox.setText("Key length is not of block size (16).\nCurrent key length: " + QString::number(ui.KeyTextField->toPlainText().length()));
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return;
    }
    if (ui.InitializationVectorTextField->toPlainText().length() != BLOCK_SIZE && ui.AESModeComboBox->currentIndex() != 0) {
        msgBox.setText("Initialization vector length is not of block size (16).\nCurrent IV length: " + QString::number(ui.InitializationVectorTextField->toPlainText().length()));
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return;
    }
    if (ui.DecryptRadio->isChecked() && ui.InputTextField->toPlainText().contains(QRegularExpression("[^0-9a-fA-F ]"))) {
        msgBox.setText("Input constains illegal characters for decryption.");
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return;
    }
    if (ui.DecryptRadio->isChecked()) {
        temp = ui.InputTextField->toPlainText().split(QRegularExpression("\\s+")).join("");
    }
    else {
        temp = ui.InputTextField->toPlainText();
    }

    switch (ui.AESModeComboBox->currentIndex()) {
    case 0: // ECB
        if (ui.EncryptRadio->isChecked())
            ui.OutputTextField->setPlainText(QString::fromStdString(
                ustrtohex(
                    AES_ECB_encrypt(
                        strtoustr(temp.toStdString()),
                        strtoustr(ui.KeyTextField->toPlainText().toStdString())
                    )
                ))
            );
        else
            ui.OutputTextField->setPlainText(QString::fromStdString(
                ustrtostr(
                    AES_ECB_decrypt(
                        strtoustr(hextostr(temp.toStdString())),
                        strtoustr(ui.KeyTextField->toPlainText().toStdString())
                    )
                ))
            );
        break;
    case 1: // CBC
        if (ui.EncryptRadio->isChecked())
            ui.OutputTextField->setPlainText(QString::fromStdString(
                ustrtohex(
                    AES_CBC_encrypt(
                        strtoustr(temp.toStdString()),
                        strtoustr(ui.KeyTextField->toPlainText().toStdString()),
                        strtoustr(ui.InitializationVectorTextField->toPlainText().toStdString())
                    )
                ))
            );
        else
            ui.OutputTextField->setPlainText(QString::fromStdString(
                ustrtostr(
                    AES_CBC_decrypt(
                        strtoustr(hextostr(temp.toStdString())),
                        strtoustr(ui.KeyTextField->toPlainText().toStdString()),
                        strtoustr(ui.InitializationVectorTextField->toPlainText().toStdString())
                    )
                ))
            );
        break;
    case 2: // CFB
        if (ui.EncryptRadio->isChecked())
            ui.OutputTextField->setPlainText(QString::fromStdString(
                ustrtohex(
                    AES_CFB_encrypt(
                        strtoustr(temp.toStdString()),
                        strtoustr(ui.KeyTextField->toPlainText().toStdString()),
                        strtoustr(ui.InitializationVectorTextField->toPlainText().toStdString())
                    )
                ))
            );
        else
            ui.OutputTextField->setPlainText(QString::fromStdString(
                ustrtostr(
                    AES_CFB_decrypt(
                        strtoustr(hextostr(temp.toStdString())),
                        strtoustr(ui.KeyTextField->toPlainText().toStdString()),
                        strtoustr(ui.InitializationVectorTextField->toPlainText().toStdString())
                    )
                ))
            );
        break;
    }


    //ui.OutputTextField->setPlainText(ui.AESModeComboBox->currentText());
}


void QtBlockCipher::on_actionExit_triggered()
{
    this->close();
}

void QtBlockCipher::on_actionOpen_triggered()
{
    // get absolute path, as qstring, to one text file
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open text file"), "", tr("Text files (*.txt)"));

    QFile inputFile(fileName);
    inputFile.open(QIODevice::ReadOnly);

    QTextStream in(&inputFile);
    QString text = in.readAll();
    inputFile.close();

    ui.InputTextField->setPlainText(text);
    QTextCursor cursor = ui.InputTextField->textCursor();
    cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor, 1);
    ui.InputTextField->setTextCursor(cursor);
}


void QtBlockCipher::on_KeyTextField_textChanged()
{
    QString keyText = ui.KeyTextField->toPlainText();

    ssize_t keyTextBeforeLength = keyText.length();

    auto lmbd = [](QChar c) {
        QString s = c;
        int pos = 0;
        return QRegularExpressionValidator(QRegularExpression("[[:ascii:]]")).validate(s, pos) == QRegularExpressionValidator::Invalid;
    };

    // Handle only text from keyboard
    QTextCursor cursor = ui.KeyTextField->textCursor();

    keyText.removeIf(lmbd);
    keyText.truncate(BLOCK_SIZE);

    if (keyText.length() != keyTextBeforeLength) {
        cursor.movePosition(QTextCursor::End); // just move to the end
        ui.KeyTextField->setPlainText(keyText);
        ui.KeyTextField->setTextCursor(cursor);
    }
}


void QtBlockCipher::on_actionSaveAs_triggered()
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
        out << ui.OutputTextField->toPlainText();
    }
}


void QtBlockCipher::on_actionSave_key_as_triggered()
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
        out << ui.KeyTextField->toPlainText();
    }
}


void QtBlockCipher::on_actionOpen_key_triggered()
{
    // get absolute path, as qstring, to one text file
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open key file"), "", tr("Text files (*.txt)"));

    QFile inputFile(fileName);
    inputFile.open(QIODevice::ReadOnly);

    QTextStream in(&inputFile);
    QString text = in.readAll();
    inputFile.close();

    ui.KeyTextField->setPlainText(text);
    QTextCursor cursor = ui.KeyTextField->textCursor();
    cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor, 1);
    ui.KeyTextField->setTextCursor(cursor);
}


void QtBlockCipher::on_actionAbout_triggered()
{
    QMessageBox msgBox;
    msgBox.about(this, "About", "Block ciphers: AES128\nAuthor: Paweł Perechuda; 145199\n"
        "Modes implemented:\n"
        "- CBC\n"
        "- CFB\n"
        "- ECB");
}


void QtBlockCipher::on_AESModeComboBox_currentIndexChanged(int index)
{
    switch (index) {
    case 0: // ECB
        //ui.InitializationVectorTextField->setReadOnly(true);
        ui.InitializationVectorTextField->setDisabled(true);
        break;
    default:
        //ui.InitializationVectorTextField->setReadOnly(false);
        ui.InitializationVectorTextField->setDisabled(false);
    }
}

void QtBlockCipher::on_DivideIntoBlocksCheckBox_toggled(bool checked)
{
    if (ui.EncryptRadio->isChecked()) { // should check if it was previously decrypted or encrypted but meh
        QString temp;
        if (checked) {
            //QTextStream ss;
            //QString temp = ui.OutputTextField->toPlainText();
            //for (QString::iterator it = temp.begin(); it != temp.end(); it += 4) {
            //    //if ((it - temp.begin()) % 4 == 3)
            //    //    ss << " ";
            //    //else
            //    //    ss << *it;
            //    ss << temp.right(it - temp.begin())
            //}
            //ui.OutputTextField->setPlainText(ss.readAll());
            temp = ui.OutputTextField->toPlainText().replace(QRegularExpression("(.{8})"), "\\1 ");
            temp.chop(1);
            ui.OutputTextField->setPlainText(temp);
        }
        else {
            temp = ui.OutputTextField->toPlainText().split(QRegularExpression("\\s+")).join("");
            ui.OutputTextField->setPlainText(temp);
        }
    }
}

void QtBlockCipher::on_InitializationVectorTextField_textChanged()
{
    QString keyText = ui.InitializationVectorTextField->toPlainText();

    ssize_t keyTextBeforeLength = keyText.length();

    auto lmbd = [](QChar c) {
        QString s = c;
        int pos = 0;
        return QRegularExpressionValidator(QRegularExpression("[[:ascii:]]")).validate(s, pos) == QRegularExpressionValidator::Invalid;
    };

    // Handle only text from keyboard
    QTextCursor cursor = ui.InitializationVectorTextField->textCursor();

    keyText.removeIf(lmbd);
    keyText.truncate(BLOCK_SIZE);

    if (keyText.length() != keyTextBeforeLength) {
        cursor.movePosition(QTextCursor::End); // just move to the end
        ui.InitializationVectorTextField->setPlainText(keyText);
        ui.InitializationVectorTextField->setTextCursor(cursor);
    }
}
