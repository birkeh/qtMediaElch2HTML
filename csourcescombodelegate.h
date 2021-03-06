#ifndef CSOURCESCOMBODELEGATE_H
#define CSOURCESCOMBODELEGATE_H


#include <string>
#include <vector>

#include <QItemDelegate>

class QModelIndex;
class QWidget;
class QVariant;


class cSourcesComboDelegate : public QItemDelegate
{
Q_OBJECT
public:
	cSourcesComboDelegate(QObject *parent = 0);

	QWidget*	createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	void		setEditorData(QWidget *editor, const QModelIndex &index) const;
	void		setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
	void		updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	void		paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
	std::vector<std::string> m_Items;

};

#endif // CSOURCESCOMBODELEGATE_H
