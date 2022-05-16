#pragma once

#include <QMainWindow>
#include <QDialog>
#include <ui_textwindow.h>
#include "PK_Steganography.h"

class TextWindow : public QDialog
{
	Q_OBJECT

public:
	TextWindow(QWidget *parent);
	~TextWindow();

	void setText(QString& text);
	void setTextSizeLabelPointer(QLabel* ptr);

private slots:
	void updateText();

private:
	QString* text;
	QWidget* parent;
	QLabel* mainWindowLabel;
	Ui::Dialog ui;
};
