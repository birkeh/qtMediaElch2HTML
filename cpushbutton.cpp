#include "cpushbutton.h"


cPushButton::cPushButton(QWidget *parent) :
	QPushButton(parent)
{
	connect(this, SIGNAL(clicked()), this, SLOT(triggered()));
}

cPushButton::cPushButton(const QString& text, QWidget* parent) :
	QPushButton(text, parent)
{
	connect(this, SIGNAL(clicked()), this, SLOT(triggered()));
}

cPushButton::cPushButton(const QIcon& icon, const QString& text, QWidget* parent) :
	QPushButton(icon, text, parent)
{
	connect(this, SIGNAL(clicked()), this, SLOT(triggered()));
}

void cPushButton::triggered()
{
	emit(buttonClicked(this));
}
