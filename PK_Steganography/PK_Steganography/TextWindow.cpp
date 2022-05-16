#include "TextWindow.h"

TextWindow::TextWindow(QWidget *parent)
	: QDialog(parent), parent(parent)
{
	ui.setupUi(this);

	mainWindowLabel = nullptr;

	connect(ui.textEdit, &QTextEdit::textChanged, this, &TextWindow::updateText);
}

void TextWindow::updateText() {
	*text = ui.textEdit->toPlainText();
	if (mainWindowLabel != nullptr) {
		std::stringstream ss;
		ss << "Size: " << 8 * text->length() << "b";
		mainWindowLabel->setText(QString::fromStdString(ss.str()));
	}
}

TextWindow::~TextWindow()
{
	// delete this window in parent
}

void TextWindow::setText(QString& text)
{
	this->text = &text;
	ui.textEdit->setText(*this->text);
}

void TextWindow::setTextSizeLabelPointer(QLabel* ptr)
{
	mainWindowLabel = ptr;
}
