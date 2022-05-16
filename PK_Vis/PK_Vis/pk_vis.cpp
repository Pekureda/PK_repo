#include "pk_vis.h"

PK_Vis::PK_Vis(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    connect(ui.openImage1PushButton, &QPushButton::clicked, this, &PK_Vis::openImageFile);
    connect(ui.openImage2PushButton, &QPushButton::clicked, this, &PK_Vis::openImageFile);
    connect(ui.openSourceImagePushButton, &QPushButton::clicked, this, &PK_Vis::openImageFile);
    connect(ui.splitImageCheckBox, &QCheckBox::stateChanged, this, &PK_Vis::checkBoxClicked);
    connect(ui.actionSave_image_1_as, &QAction::triggered, this, &PK_Vis::saveAs);
    connect(ui.actionSave_image_2_as, &QAction::triggered, this, &PK_Vis::saveAs);
    connect(ui.actionSave_combined_image_as, &QAction::triggered, this, &PK_Vis::saveAs);
    connect(ui.actionReduce_noise, &QAction::triggered, this, &PK_Vis::noiseReductionClicked);
    connect(ui.actionAbout, &QAction::triggered, this, &PK_Vis::about);
    connect(ui.actionExit, &QAction::triggered, this, &PK_Vis::exit);
}

bool PK_Vis::loadFile(const QString& filename, QImage& image, QScrollArea& scrollArea, QWidget& scrollAreaWidgetContent, QLabel& uiImage)
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
    newImage = newImage.convertToFormat(QImage::Format_Mono);
    setImage(newImage, image, scrollArea, scrollAreaWidgetContent, uiImage);

    setWindowFilePath(filename);

    //const QString description = image.colorSpace().isValid()
    //    ? image.colorSpace().description() : tr("unknown");
    //const QString message = tr("Opened \"%1\", %2x%3, Depth: %4 (%5)")
    //    .arg(QDir::toNativeSeparators(filename)).arg(image.width()).arg(image.height())
    //    .arg(image.depth()).arg(description);
    //statusBar()->showMessage(message);

    return true;
}

void PK_Vis::checkBoxClicked()
{
    bool state;
    if (ui.splitImageCheckBox->isChecked()) {
        state = false;
        toClear = false;
    }
    else {
        state = true;
        toClear = true;
    }

    ui.openImage1PushButton->setEnabled(state);
    ui.openImage2PushButton->setEnabled(state);
    ui.image1PathLineEdit->setEnabled(state);
    ui.image2PathLineEdit->setEnabled(state);

    ui.sourceImageLineEdit->setEnabled(!state);
    ui.openSourceImagePushButton->setEnabled(!state);
}

void PK_Vis::saveAs()
{
    QAction* actionSender = qobject_cast<QAction*>(sender());
    QString actionText = actionSender->text();
    QImage* imageToSave;

    if (actionText == "Save image 1 as") imageToSave = &image1;
    else if (actionText == "Save image 2 as") imageToSave = &image2;
    else if (actionText == "Save combined image as") imageToSave = &combinedImage;

    QFileDialog dialog(this, tr("Save Image As"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptSave);

    while (dialog.exec() == QDialog::Accepted && !saveFile(dialog.selectedFiles().constFirst(), *imageToSave)) {}
}

void PK_Vis::noiseReductionClicked()
{
    int width = image1.isNull() ? image2.width() : image1.width();
    int height = image1.isNull() ? image2.height() : image1.height();
    QImage newImage = QImage(width / 2, height, QImage::Format::Format_Mono);

    if (image1.isNull() || image2.isNull() || combinedImage.isNull()) {
        QMessageBox msg;
        msg.critical(this, "Images missing", "Please open both image parts in order to reduce noise.");
        return;
    }

    for (int hor = 0; hor < width; hor += 2) {
        for (int vert = 0; vert < height; ++vert) {
            bool color = image1.pixelColor(hor, vert).black() == image2.pixelColor(hor, vert).black() && image1.pixelColor(hor + 1, vert).black() == image2.pixelColor(hor + 1, vert).black();
            newImage.setPixel(hor >> 1, vert, color);
        }
    }

    setImage(newImage, combinedImage, *ui.combinedScrollArea, *ui.combinedScrollAreaWidgetContents, *ui.combinedImage);
}

void PK_Vis::about()
{
    QMessageBox msgBox;
    msgBox.about(this, "About", "Visual cryptography\nAuthor: Paweł Perechuda; 145199\n"
        "Program accepts images in jpeg, png and bmp formats. Images are automatically converted to black and white, each pixel coded with 1 bit. Noise reduction functionality recovers imported parts into black and white original image.");
}

void PK_Vis::exit()
{
    QCoreApplication::quit();
}

void PK_Vis::initializeImageFileDialog(QFileDialog& dialog, QFileDialog::AcceptMode acceptMode)
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
    dialog.setMimeTypeFilters(QStringList({ "image/png", "image/bmp", "image/jpeg" }));
    dialog.setAcceptMode(acceptMode);
    if (acceptMode == QFileDialog::AcceptSave)
        dialog.setDefaultSuffix("png");
}

void PK_Vis::setImage(const QImage& newImage, QImage& image, QScrollArea& scrollArea, QWidget& scrollAreaWidgetContent, QLabel& uiImage)
{
    image = newImage;
    //if (image.colorSpace().isValid())
    //    image.convertToColorSpace(QColorSpace::SRgb);
    uiImage.setPixmap(QPixmap::fromImage(image));

    scrollArea.setVisible(true);
    
    scrollAreaWidgetContent.resize(image.size());
    
    scrollArea.setWidgetResizable(false);
    uiImage.adjustSize();
}

void PK_Vis::splitImage(const QImage& image)
{
    std::random_device rd;
    std::mt19937 generator(rd());
    QImage part1, part2;
    part1 = QImage(image.width() << 1, image.height(), QImage::Format::Format_Mono);
    part2 = QImage(image.width() << 1, image.height(), QImage::Format::Format_Mono);

    for (int hor = 0; hor < 2 * image.width(); hor += 2) {
        for (int vert = 0; vert < image.height(); ++vert) {
            bool color = generator() & 1;
            part1.setPixel(hor, vert, color);
            part1.setPixel(hor + 1, vert, !color);

            if (image.pixelColor(hor / 2, vert).black()) {
                part2.setPixel(hor, vert, !color);
                part2.setPixel(hor + 1, vert, color);
            }
            else {
                part2.setPixel(hor, vert, color);
                part2.setPixel(hor + 1, vert, !color);
            }
        }
    }

    setImage(part1, image1, *ui.scrollArea1, *ui.scrollAreaWidgetContents1, *ui.image1);
    setImage(part2, image2, *ui.scrollArea2, *ui.scrollAreaWidgetContents2, *ui.image2);

    /*this->image1 = part1;
    this->image2 = part2;

    ui.image1->setPixmap(QPixmap::fromImage(part1));
    ui.image2->setPixmap(QPixmap::fromImage(part2));*/
}

bool PK_Vis::saveFile(const QString& filename, const QImage& image)
{
    QImageWriter writer(filename);

    if (!writer.write(image)) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
            tr("Cannot write %1: %2")
            .arg(QDir::toNativeSeparators(filename), writer.errorString()));
        return false;
    }
    return true;
}

void PK_Vis::combineImages()
{
    int width = image1.isNull() ? image2.width() : image1.width();
    int height = image1.isNull() ? image2.height() : image1.height();
    QImage newImage = QImage(width, height, QImage::Format::Format_Mono);

    if (image1.isNull()) {
        setImage(image2, combinedImage, *ui.combinedScrollArea, *ui.combinedScrollAreaWidgetContents, *ui.combinedImage);
        return;
    }
    else if (image2.isNull()) {
        setImage(image1, combinedImage, *ui.combinedScrollArea, *ui.combinedScrollAreaWidgetContents, *ui.combinedImage);
        return;
    }

    if (image1.width() != image2.width() || image1.height() != image2.height()) {
        QMessageBox mbox;
        mbox.critical(this, "Cannot combine images", "Images cannot be combined, because their dimensions are not the same.");
        return;
    }

    for (int hor = 0; hor < width; ++hor) {
        for (int vert = 0; vert < height; ++vert) {
            newImage.setPixel(hor, vert, image1.pixelColor(hor, vert).black() || image2.pixelColor(hor, vert).black());
        }
    }

    setImage(newImage, combinedImage, *ui.combinedScrollArea, *ui.combinedScrollAreaWidgetContents, *ui.combinedImage);
}

void PK_Vis::openImageFile()
{
    QFileDialog dialog(this, tr("Open image file"));

    initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);

    QScrollArea* scrollArea;
    QWidget* scrollAreaWidgetContent;
    QLabel* uiImage;
    QImage* image;

    QPushButton* buttonSender = qobject_cast<QPushButton*>(sender());
    QString buttonText = buttonSender->text();

    if (buttonText == "Open image 1") {
        scrollArea = ui.scrollArea1;
        scrollAreaWidgetContent = ui.scrollAreaWidgetContents1;
        uiImage = ui.image1;
        image = &image1;
    }
    else if (buttonText == "Open image 2") {
        scrollArea = ui.scrollArea2;
        scrollAreaWidgetContent = ui.scrollAreaWidgetContents2;
        uiImage = ui.image2;
        image = &image2;
    }
    else if (buttonText == "Open source") {
        scrollArea = ui.combinedScrollArea;
        scrollAreaWidgetContent = ui.combinedScrollAreaWidgetContents;
        uiImage = ui.combinedImage;
        image = &combinedImage;
    }

    if (toClear) {
        ui.image1->clear();
        ui.image2->clear();
        ui.combinedImage->clear();

        image1 = QImage();
        image2 = QImage();
        combinedImage = QImage();

        toClear = false;
    }

    while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().constFirst(), *image, *scrollArea, *scrollAreaWidgetContent, *uiImage)) {}

    if (dialog.selectedFiles().size()) {
        if (buttonText == "Open image 1") {
            ui.image1PathLineEdit->setText(dialog.selectedFiles().constFirst());
            combineImages();
        }
        else if (buttonText == "Open image 2") {
            ui.image2PathLineEdit->setText(dialog.selectedFiles().constFirst());
            combineImages();
        }
        else if (buttonText == "Open source") {
            ui.sourceImageLineEdit->setText(dialog.selectedFiles().constFirst());
            splitImage(combinedImage);
        }
    }
}