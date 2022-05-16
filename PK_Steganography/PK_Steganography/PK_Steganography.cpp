#include "PK_Steganography.h"

PK_Steganography::PK_Steganography(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    text = "";
    offset = 0;
    
    ui.image->setBackgroundRole(QPalette::Base);
    ui.image->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    ui.image->setScaledContents(true);

    ui.scrollArea->setBackgroundRole(QPalette::Dark);
    ui.scrollArea->setWidgetResizable(false);
    //ui.scrollArea->setWidget(ui.scrollAreaWidgetContents);
    //ui.scrollArea->setVisible(false);

    ui.runPushButton->setDisabled(true);

    connect(ui.openTextPushButton, &QPushButton::clicked, this, &PK_Steganography::openTextFile);
    connect(ui.openImagePushButton, &QPushButton::clicked, this, &PK_Steganography::openImageFile);
    connect(ui.actionExit, &QAction::triggered, this, &PK_Steganography::actionExit);
    connect(ui.showTextPushButton, &QPushButton::clicked, this, &PK_Steganography::openTextWindow);
    connect(ui.runPushButton, &QPushButton::clicked, this, &PK_Steganography::runButton);
    connect(ui.actionSave_as, &QAction::triggered, this, &PK_Steganography::saveAs);
    connect(ui.discoverRadioButton, &QRadioButton::clicked, this, &PK_Steganography::checkRunConditions);
    connect(ui.hideRadioButton, &QRadioButton::clicked, this, &PK_Steganography::checkRunConditions); // to rewrite
    connect(ui.actionAbout, &QAction::triggered, this, &PK_Steganography::about);
}

bool PK_Steganography::loadFile(const QString& filename)
{
    QImageReader reader(filename);
    reader.setAutoTransform(true);
    QImage newImage = reader.read();
    if (newImage.isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
            tr("Cannot load %1: %2")
            .arg(QDir::toNativeSeparators(filename), reader.errorString()));
        return false;
    }
    newImage = newImage.convertToFormat(QImage::Format_ARGB32);
    setImage(newImage);

    setWindowFilePath(filename);

    const QString description = image.colorSpace().isValid()
        ? image.colorSpace().description() : tr("unknown");
    const QString message = tr("Opened \"%1\", %2x%3, Depth: %4 (%5)")
        .arg(QDir::toNativeSeparators(filename)).arg(image.width()).arg(image.height())
        .arg(image.depth()).arg(description);
    statusBar()->showMessage(message);

    return true;
}

void PK_Steganography::prepareImageForHiding(QImage* image)
{
    for (size_t vert = 0; vert < image->height(); ++vert) {
        for (size_t hor = 0; hor < image->width(); ++hor) {
            unsigned int val = image->pixel(hor, vert) & (UINT_MAX - unsigned int(1) - unsigned int(1 << 8) - unsigned int(1 << 16) - unsigned int(1 << 24));
            image->setPixel(hor, vert, image->pixel(hor, vert) & (UINT_MAX - unsigned int(1) - unsigned int(1 << 8) - unsigned int(1 << 16) - unsigned int(1 << 24)));
        }
    }
}

void PK_Steganography::hide(QImage* image, QString text, const size_t offset, const bool horizontal)
{
    size_t aIndex, aLimit = image->height();
    size_t bIndex, bLimit = image->width();

    
    if (horizontal) {
        aIndex = (2*offset) / image->width();
        bIndex = (2*offset) % image->width();
    }
    else {
        std::swap(aLimit, bLimit);
        aIndex = (2 * offset) % image->width();
        bIndex = (2 * offset) / image->width();
    }

    QString paddedText = markingSequence + text + markingSequence;
    //unsigned char* imageIterator = image.bits() + offset;
    QString::iterator textIterator = paddedText.begin();
    bool first = true;

    //while (textIterator < paddedText.length() + paddedText.begin()) {
    //    for (size_t i = 7; i >= 0; --i) {
    //        *imageIterator = *imageIterator + ((textIterator->toLatin1() >> i) & 1);
    //    }
    //    ++textIterator;
    //}
    for (; aIndex < aLimit; ++aIndex) {
        for (; bIndex < bLimit; ++bIndex) {
            unsigned int pixel;
            if (horizontal) pixel = image->pixel(bIndex, aIndex);
            else pixel = image->pixel(aIndex, bIndex);
            
            QChar qChar = textIterator->toLatin1();
            unsigned char letter = qChar.toLatin1();

            // only 4 bits inside a pixel
            unsigned char firstHalf = letter >> 4;
            unsigned char secondHalf = letter - (firstHalf << 4);
            
            for (int i = 3; i >= 0; --i) {
                unsigned int val = first ? (unsigned char(1) & (firstHalf >> i)) << 8 * i : (unsigned char(1) & (secondHalf >> i)) << 8 * i;
                pixel += first ? (unsigned char(1) & (firstHalf >> i)) << 8*i : (unsigned char(1) & (secondHalf >> i)) << 8 * i; // TODO obsługa vertical i horizontal
            }
            if (horizontal) image->setPixel(bIndex, aIndex, pixel);
            else image->setPixel(aIndex, bIndex, pixel);
            if (!first) ++textIterator;
            if (textIterator - paddedText.begin() > paddedText.length()) return;
            first = !first;
        }
        bIndex = 0;
    }
}

void PK_Steganography::discover(QImage& image, const bool horizontal)
{
    QString window = "";
    QString text = "";
    unsigned char c = 0;
    bool first = true;
    bool foundText = false;

    size_t imageWidth = image.width();
    size_t imageHeight = image.height();
    if (!horizontal) std::swap(imageWidth, imageHeight);

    for (size_t y = 0; y < imageHeight; ++y) {
        for (size_t x = 0; x < imageWidth; ++x) {
            unsigned int pixel;
            if (horizontal) pixel = image.pixel(x, y);
            else pixel = image.pixel(y, x);

            for (int i = 3; i >= 0; --i) {
                c += first ? (unsigned char(1) & (pixel >> 8 * i)) << (i + 4) : (unsigned char(1) & (pixel >> 8 * i)) << i;
            }
            if (!first) {
                window += QChar(c);
                if (window.length() > markingSequence.length()) window = window.right(markingSequence.length()); // TODO obsługa vertical i horizontal
                if (foundText) text += QChar(c);
                if (window == markingSequence) foundText = !foundText;
                c = 0;
            }

            first = !first;
        }
    }
    if (foundText) text = markingSequence;
    text = text.left(text.length() - markingSequence.length());
    this->text = text;
}

void PK_Steganography::setEnabledAllButtons(const bool state)
{
    ui.openImagePushButton->setEnabled(state);
    ui.discoverRadioButton->setEnabled(state);
    ui.hideRadioButton->setEnabled(state);
    ui.horizontalRadioButton->setEnabled(state);
    ui.openTextPushButton->setEnabled(state);
    ui.runPushButton->setEnabled(state);
    ui.showTextPushButton->setEnabled(state);
    ui.verticalRadioButton->setEnabled(state);
}

void PK_Steganography::offsetUpdate()
{
    if (!image.isNull()) {
        std::stringstream ss;
        ss << "Capacity: " << image.height() * image.width() * (image.depth() / 8) - markingSequence.length() * 16 - offset * 4 << "b";
        ui.imageCapacityLabel->setText(QString::fromStdString(ss.str()));
        //if (!text.isEmpty()) ui.runPushButton->setEnabled(true);
        checkRunConditions();
    }
}


void PK_Steganography::initializeImageFileDialog(QFileDialog& dialog, QFileDialog::AcceptMode acceptMode)
{
    static bool firstDialog = true;

    if (firstDialog) {
        firstDialog = false;
        const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
        dialog.setDirectory(picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last());
    }

    QStringList mimeTypeFilters;
    const QByteArrayList supportedMimeTypes = acceptMode == QFileDialog::AcceptOpen
        ? QImageReader::supportedMimeTypes() : QImageWriter::supportedMimeTypes();
    for (const QByteArray& mimeTypeName : supportedMimeTypes)
        mimeTypeFilters.append(mimeTypeName);
    mimeTypeFilters.sort();
    dialog.setMimeTypeFilters(mimeTypeFilters);
    //dialog.selectMimeTypeFilter("image/png");
    dialog.setMimeTypeFilters(QStringList({"image/png", "image/bmp"}));
    dialog.setAcceptMode(acceptMode);
    if (acceptMode == QFileDialog::AcceptSave)
        dialog.setDefaultSuffix("png");
}

void PK_Steganography::setImage(const QImage& newImage)
{
    image = newImage;
    if (image.colorSpace().isValid())
        image.convertToColorSpace(QColorSpace::SRgb);
    ui.image->setPixmap(QPixmap::fromImage(image));

    scaleFactor = 1.0;

    ui.scrollArea->setVisible(true);
    ui.actionFit_to_window->setEnabled(true);

    ui.scrollAreaWidgetContents->resize(image.size());
    //updateActions();

    if (!ui.actionFit_to_window->isChecked()) {
        ui.scrollArea->setWidgetResizable(false);
        ui.image->adjustSize();
    }
    else {
        ui.scrollArea->setWidgetResizable(true);
        ui.image->adjustSize();
    }
}

void PK_Steganography::openTextFile()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open text file"), "", tr("Text files (*.txt)"));

    QFile inputFile(filename);
    inputFile.open(QIODevice::ReadOnly);

    QTextStream in(&inputFile);
    text = in.readAll();
    inputFile.close();

    if (text.size()) {
        ui.plaintextPathLineEdit->setText(filename);
        std::stringstream ss;
        ss << "Size: " << 8 * text.length() << "b";
        ui.textSizeLabel->setText(QString::fromStdString(ss.str()));
        //if (!image.isNull()) ui.runPushButton->setEnabled(true);
        checkRunConditions();
    }
}

void PK_Steganography::openImageFile()
{
    QFileDialog dialog(this, tr("Open image file"));

    initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);

    while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().constFirst())) {}

    if (dialog.selectedFiles().size()) {
        ui.imagePathLineEdit->setText(dialog.selectedFiles().constFirst());
        std::stringstream ss;
        ss << "Capacity: " << image.height() * image.width() * (image.depth() / 8) - markingSequence.length() * 16 << "b";
        ui.imageCapacityLabel->setText(QString::fromStdString(ss.str()));
        //if (!text.isEmpty()) ui.runPushButton->setEnabled(true);
        checkRunConditions();
    }
}

void PK_Steganography::openTextWindow()
{
    TextWindow* textWindow = new TextWindow(this);
    textWindow->setText(text);
    textWindow->setTextSizeLabelPointer(ui.textSizeLabel);
    textWindow->setModal(true); // modal
    textWindow->setAttribute(Qt::WA_DeleteOnClose);
    textWindow->show();
    checkRunConditions();
}

void PK_Steganography::actionExit()
{
    this->close();
}

void PK_Steganography::runButton()
{
    if (text.length() + (2 * markingSequence.length()) > (image.height() * image.width()) << 2) {
        QMessageBox msgBox;
        msgBox.critical(this, "Message too long", "Provided message is too long for this image or the image is too small to fit provided message.");
        return;
    }

    if (ui.hideRadioButton->isChecked()) {
        setEnabledAllButtons(false);
        prepareImageForHiding(&image);
        hide(&image, text, offset, ui.horizontalRadioButton->isChecked());
        setImage(image);
        setEnabledAllButtons(true);
    }
    else {
        setEnabledAllButtons(false);
        discover(image, ui.horizontalRadioButton->isChecked());
        ui.plaintextPathLineEdit->setText("");
        if (text.size()) {
            std::stringstream ss;
            ss << "Size: " << 8 * text.length() << "b";
            ui.textSizeLabel->setText(QString::fromStdString(ss.str()));
        }
        setEnabledAllButtons(true);
    }
}

void PK_Steganography::saveAs()
{
    QFileDialog dialog(this, tr("Save Image As"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptSave);

    while (dialog.exec() == QDialog::Accepted && !saveFile(dialog.selectedFiles().constFirst())) {}
}

void PK_Steganography::about()
{
    QMessageBox msgBox;
    msgBox.about(this, "About", "Steganography\nAuthor: Paweł Perechuda; 145199\n"
        "Program sets all last bits of every color and alpha channel of a pixel to 0, in preparation to hide a message.\n"
        "This program accepts only bmp and png formats. Only ASCII characters can be hidden inside an image.");
}

void PK_Steganography::checkRunConditions()
{
    if (text.length() && !image.isNull()) ui.runPushButton->setEnabled(true);
    else if (!image.isNull() && ui.discoverRadioButton->isChecked()) ui.runPushButton->setEnabled(true);
    else ui.runPushButton->setEnabled(false);
}

bool PK_Steganography::saveFile(const QString& filename)
{
    QImageWriter writer(filename);

    if (!writer.write(image)) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
            tr("Cannot write %1: %2")
            .arg(QDir::toNativeSeparators(filename), writer.errorString()));
        return false;
    }
    const QString message = tr("Wrote \"%1\"").arg(QDir::toNativeSeparators(filename));
    statusBar()->showMessage(message);
    return true;
}
