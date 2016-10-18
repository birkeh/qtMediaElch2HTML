#ifndef CGENRECOMBINE_H
#define CGENRECOMBINE_H


#include <QString>
#include <QStringList>
#include <QMetaType>


class cGenreCombine
{
public:
	cGenreCombine(const QString& szGenreTo);
	void		addFrom(const QString& szGenreFrom);
	void		addFrom(const QStringList& szGenreFrom);
	void		removeFrom(const QString& szGenreFrom);
	QString		genreTo();
	QStringList	genreFrom();
	void		clearFrom();
private:
	QString     m_szGenreTo;
	QStringList m_szGenreFrom;
};

Q_DECLARE_METATYPE(cGenreCombine*)

class cGenreCombineList : public QList<cGenreCombine *>
{
public:
	cGenreCombineList();
	cGenreCombine*	add(const QString& szGenreTo);

	bool			existTo(const QString& szGenreTo, Qt::CaseSensitivity iSensitive);
	bool			existFrom(const QString& szGenreFrom, Qt::CaseSensitivity iSensitive);
	cGenreCombine*	findTo(const QString& szGenreTo, Qt::CaseSensitivity iSensitive);
	cGenreCombine*	findFrom(const QString& szGenreFrom, Qt::CaseSensitivity iSensitive);
	void			addFrom(const QString& szGenreTo, const QString& szGenreFrom);
	void			addFrom(const QString& szGenreTo, const QStringList& szGenreFrom);
	QString			to(const QString& szFrom);
};

#endif // CGENRECOMBINE_H
