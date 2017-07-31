#ifndef CTVSHOW_H
#define CTVSHOW_H


#include "common.h"
#include "ctvshowepisode.h"
#include "cxbmc.h"

#include <QString>
#include <QList>
#include <QStringList>
#include <QMetaType>
#include <QDomElement>
#include <QDate>
#include <QStatusBar>


class cTVShow
{
public:
	cTVShow(const QString& szPath, cXBMC* lpXBMC);

	bool					isValid();

	QString					path();

	QString					nfo();
	QList<SEASONPIC>		seasonPicList();
	QString					banner();
	QString					character();
	QString					clearart();
	QString					fanart();
	QString					landscape();
	QString					logo();
	QString					poster();

	QString					title();
	double					rating();
	QString					plot();
	QString					outline();
	QString					mpaa();
	QDate					premiereDate();
	QStringList				studioList();
	QString					tvdbID();
	qint16					runtime();
	QStringList				genreList();
	QList<ACTOR>			actorList();

	qint16					year();

	QList<cTVShowEpisode*>	episodeList();
private:
	QString					m_szPath;

	QString					m_szNFO;
	QList<SEASONPIC>		m_SeasonPicList;
	QString					m_szBanner;
	QString					m_szCharacter;
	QString					m_szClearart;
	QString					m_szFanart;
	QString					m_szLandscape;
	QString					m_szLogo;
	QString					m_szPoster;

	QString					m_szTitle;
	double					m_dRating;
	QString					m_szPlot;
	QString					m_szOutline;
	QString					m_szMPAA;
	QDate					m_PremiereDate;
	QStringList				m_szStudioList;
	QString					m_szTVDBID;
	QString					m_szID;
	qint16					m_iRuntime;
	QStringList				m_szGenreList;
	QList<ACTOR>			m_ActorList;
	bool					m_bWatched;

	QList<cTVShowEpisode*>	m_TVShowEpisodeList;
protected:
	void					load(const QString& szPath, cXBMC* lpXBMC);
	bool					loadNFO(const QString& szPath, const QString& szNFO);

	void					parseActor(const QDomElement& element);

	void					searchSeasons(const QString& szPath, cXBMC* lpXBMC);
};

Q_DECLARE_METATYPE(cTVShow*)

class cTVShowList : public QList<cTVShow *>
{
public:
	cTVShowList();

	//cMovie*	add(const QString& szFileName, QObject *parent = 0);
	void		parse(const QString& szPath, const QStringList& exceptions, cXBMC* lpXBMC = 0, QStatusBar* lpStatusBar = 0);
	QStringList	genreList();

private:
	QStringList	m_szGenreList;
protected:
};

#endif // CTVSHOW_H
