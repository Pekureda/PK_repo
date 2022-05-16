#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QFileDialog>
#include <qstandardpaths.h>
#include <qimagereader.h>
#include <qimagewriter.h>
#include <qmessagebox.h>
#include <qcolorspace>
#include <QtWidgets/qboxlayout.h>
#include "ui_PK_Steganography.h"
#include <sstream>
#include "TextWindow.h"

class PK_Steganography : public QMainWindow
{
    Q_OBJECT

public:
    PK_Steganography(QWidget *parent = Q_NULLPTR);
    bool loadFile(const QString&);
    void checkRunConditions();
    
    //void updateText(const QString& text);

private slots:
    void openTextFile();
    void openImageFile();
    void openTextWindow();
    void actionExit();
    void runButton();
    void saveAs();
    void about();
    
    //void saveAs();
    //void copy();
    //void paste();
    //void about();
    

private:
    static void initializeImageFileDialog(QFileDialog& dialog, QFileDialog::AcceptMode acceptMode);
    //void updateActions();
    void setImage(const QImage& newImage);
    bool saveFile(const QString& filename);
    //void scaleImage(double factor);
    //void adjustScrollBar(QScrollBar* scrollBar, double factor);
    void prepareImageForHiding(QImage* image);
    void hide(QImage* image, QString text, const size_t offset, const bool horizontal);
    void discover(QImage& image, const bool horizontal);
    void setEnabledAllButtons(const bool state);
    void offsetUpdate();


    //TextWindow* textWindow;
    QImage image;
    QString text;
    QScrollArea* scrollArea;
    double scaleFactor = 1;
    QString markingSequence = "marker";
    size_t offset;
    

    Ui::PK_SteganographyClass ui;
};
