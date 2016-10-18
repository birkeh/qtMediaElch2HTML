#include "cgenrecombine.h"


cGenreCombine::cGenreCombine(const QString& szGenreTo)
{
	m_szGenreTo		= szGenreTo;
}

void cGenreCombine::addFrom(const QString& szGenreFrom)
{
	if(!m_szGenreFrom.contains(szGenreFrom, Qt::CaseInsensitive))
		m_szGenreFrom.append(szGenreFrom);
}

void cGenreCombine::addFrom(const QStringList& szGenreFrom)
{
	m_szGenreFrom.append(szGenreFrom);
	m_szGenreFrom.removeDuplicates();
}

void cGenreCombine::removeFrom(const QString& szGenreFrom)
{
	m_szGenreFrom.removeOne(szGenreFrom);
}

void cGenreCombine::clearFrom()
{
	m_szGenreFrom.clear();
}

QString cGenreCombine::genreTo()
{
	return(m_szGenreTo);
}

QStringList cGenreCombine::genreFrom()
{
	return(m_szGenreFrom);
}

cGenreCombineList::cGenreCombineList()
{
}

cGenreCombine* cGenreCombineList::add(const QString& szGenreTo)
{
	for(int z = 0;z < count();z++)
		if(!at(z)->genreTo().compare(szGenreTo, Qt::CaseInsensitive))
			return(0);

	cGenreCombine*	lpNew	= new cGenreCombine(szGenreTo);
	append(lpNew);
	return(lpNew);
}

bool cGenreCombineList::existTo(const QString& szGenreTo, Qt::CaseSensitivity iSensitive)
{
	for(int z = 0;z < count();z++)
	{
		if(!at(z)->genreTo().compare(szGenreTo, iSensitive))
			return(true);
	}
	return(false);
}

bool cGenreCombineList::existFrom(const QString& szGenreFrom, Qt::CaseSensitivity iSensitive)
{
	for(int z = 0;z < count();z++)
	{
		if(at(z)->genreFrom().contains(szGenreFrom, iSensitive))
			return(true);
	}
	return(false);
}

cGenreCombine* cGenreCombineList::findTo(const QString& szGenreTo, Qt::CaseSensitivity iSensitive)
{
	for(int z = 0;z < count();z++)
	{
		if(!at(z)->genreTo().compare(szGenreTo, iSensitive))
			return(at(z));
	}
	return(0);
}

cGenreCombine* cGenreCombineList::findFrom(const QString& szGenreFrom, Qt::CaseSensitivity iSensitive)
{
	for(int z = 0;z < count();z++)
	{
		if(at(z)->genreFrom().contains(szGenreFrom, iSensitive))
			return(at(z));
	}
	return(0);
}

void cGenreCombineList::addFrom(const QString& szGenreTo, const QString& szGenreFrom)
{
	cGenreCombine*	lpCombine	= findTo(szGenreTo, Qt::CaseInsensitive);
	if(lpCombine)
		lpCombine->addFrom(szGenreFrom);
}

void cGenreCombineList::addFrom(const QString& szGenreTo, const QStringList& szGenreFrom)
{
	cGenreCombine*	lpCombine	= findTo(szGenreTo, Qt::CaseInsensitive);
	if(lpCombine)
		lpCombine->addFrom(szGenreFrom);
}

QString cGenreCombineList::to(const QString& szFrom)
{
	cGenreCombine*	lpItem	= findFrom(szFrom, Qt::CaseInsensitive);
	if(!lpItem)
		return(szFrom);
	return(lpItem->genreTo());
}
