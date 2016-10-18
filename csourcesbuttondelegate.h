#ifndef CSOURCESBUTTONDELEGATE_H
#define CSOURCESBUTTONDELEGATE_H


#include <QItemDelegate>


class cSourcesButtonDelegate : public QItemDelegate
{
	Q_OBJECT
public:
	cSourcesButtonDelegate(QObject *parent = 0);
	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);

signals:

public slots:

};

#endif // CSOURCESBUTTONDELEGATE_H
