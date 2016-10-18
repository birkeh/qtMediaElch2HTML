#ifndef CPUSHBUTTON_H
#define CPUSHBUTTON_H


#include <QPushButton>


class cPushButton : public QPushButton
{
	Q_OBJECT
public:
	cPushButton(QWidget *parent = 0);
	cPushButton(const QString& text, QWidget* parent = 0);
	cPushButton(const QIcon& icon, const QString& text, QWidget* parent = 0);

signals:
	void	buttonClicked(cPushButton* lpButton);

public slots:
   void		triggered();
};

#endif // CPUSHBUTTON_H
