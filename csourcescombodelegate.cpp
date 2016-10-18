#include "csourcescombodelegate.h"

#include <QComboBox>
#include <QWidget>
#include <QModelIndex>
#include <QApplication>
#include <QString>

#include <iostream>


cSourcesComboDelegate::cSourcesComboDelegate(QObject *parent) :
	QItemDelegate(parent)
{
	m_Items.push_back(" ");
	m_Items.push_back("Movie");
	m_Items.push_back("TV Show");
}

QWidget* cSourcesComboDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &/* index */) const
{
	QComboBox* editor = new QComboBox(parent);
	for(unsigned int i = 0; i < m_Items.size(); ++i)
		editor->addItem(m_Items[i].c_str());

	return editor;
}

void cSourcesComboDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
	QComboBox *comboBox = static_cast<QComboBox*>(editor);
	int value = index.model()->data(index, Qt::EditRole).toUInt();
	comboBox->setCurrentIndex(value);
}

void cSourcesComboDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	QComboBox *comboBox = static_cast<QComboBox*>(editor);
	model->setData(index, comboBox->currentIndex(), Qt::EditRole);
}

void cSourcesComboDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
	editor->setGeometry(option.rect);
}

void cSourcesComboDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	if(index.row() > (int)m_Items.size()-1)
		return;

	QStyleOptionViewItemV4 myOption = option;
	QString text = m_Items[index.row()].c_str();

	myOption.text = text;

	QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &myOption, painter);
}
