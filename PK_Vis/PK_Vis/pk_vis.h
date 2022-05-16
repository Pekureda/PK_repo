#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QFileDialog>
#include <qstandardpaths.h>
#include <qimagereader.h>
#include <qimagewriter.h>
#include <qmessagebox.h>
#include <qcolorspace>
#include <random>
#include "ui_pk_vis.h"

class PK_Vis : public QMainWindow
{
    Q_OBJECT

public:
    PK_Vis(QWidget *parent = Q_NULLPTR);
    bool loadFile(const QString&, QImage&, QScrollArea&, QWidget&, QLabel&);

private slots:
    void openImageFile();
    void checkBoxClicked();
    void saveAs();
    void noiseReductionClicked();
    void about();
    void exit();

private:
    QImage image1;
    QImage image2;
    QImage combinedImage;
    bool toClear = false;

    static void initializeImageFileDialog(QFileDialog& dialog, QFileDialog::AcceptMode acceptMode);
    void setImage(const QImage& newImage, QImage& image, QScrollArea& scrollArea, QWidget& scrollAreaWidgetContent, QLabel& uiImage);
    void splitImage(const QImage& image);
    bool saveFile(const QString& filename, const QImage& image);
    void combineImages();

    Ui::PK_VisClass ui;
};
