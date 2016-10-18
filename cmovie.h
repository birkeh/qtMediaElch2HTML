#ifndef CMOVIE_H
#define CMOVIE_H


#include "common.h"
#include "cxbmc.h"

#include <QString>
#include <QList>
#include <QStringList>
#include <QMetaType>
#include <QDomElement>
#include <QStatusBar>


class cMovie
{
public:
	cMovie(const QString& szPath, const QString& szNFO);

	bool			isValid();

	QString			path();
	QString			nfo();

	QString			banner();
	QString			fanart();
	QString			poster();
	QString			clearart();
	QString			disc();
	QString			logo();

	QString			tmdbID();
	QString			imdbID();
	QString			title();
	QString			originalTitle();
	qint16			year();
	double			rating();
	qint16			votes();
	qint16			top250();
	QString			plot();
	QString			outline();
	QString			tagline();
	qint16			runtime();
	QString			mpaa();
	QStringList		creditsList();
	QStringList		directorList();
	qint16			playcount();
	QString			set();
	QString			sortTitle();
	QString			trailer();
	bool			watched();
	QStringList		studioList();
	QStringList		genreList();
	QStringList		countryList();
	QList<ACTOR>	actorList();
	QList<THUMB>	thumbList();
	QList<THUMB>	fanartList();
	QList<VIDEO>	videoList();
	QList<AUDIO>	audioList();
	QList<SUBTITLE>	subtitleList();
	QDateTime		added();

	void			setBanner(const QString& szBanner);
	void			setFanart(const QString& szFanart);
	void			setPoster(const QString& szPoster);
	void			setClearart(const QString& szClearart);
	void			setDisc(const QString& szDisc);
	void			setLogo(const QString& szLogo);

	void			settmdbID(const QString& szTMDBID);
	void			setimdbID(const QString& szIMDBID);
	void			setTitle(const QString& szTitle);
	void			setOriginalTitle(const QString& szOriginalTitle);
	void			setYear(const qint16& iYear);
	void			setRating(const double& dRating);
	void			setVotes(const qint16& iVotes);
	void			setTop250(const qint16& iTop250);
	void			setPlot(const QString& szPlot);
	void			setOutline(const QString& szOutline);
	void			setTagline(const QString& szTagline);
	void			setRuntime(const qint16& iRuntime);
	void			setMPAA(const QString& szMPAA);
	void			addCreditsList(const QString& stCredit);
	void			addDirectorList(const QString& szDirector);
	void			setPlaycount(const qint16& iPlaycount);
	void			setSet(const QString& szSet);
	void			setSortTitle(const QString& szSortTitle);
	void			setTrailer(const QString& szTrailer);
	void			setWatched(const bool& bWatched);
	void			addStudioList(const QString& szStudio);
	void			addGenreList(const QString& szGenre);
	void			addCountryList(const QString& szCountry);
	void			addActorList(const QString& szActor, const QString& szRole, const QString& szThumb);
	void			addThumbList(const QString& szPreview, const QString& szThumb);
	void			addFanartList(const QString& szPreview, const QString& szThumb);
	void			addVideoList(const double& dAspect, const QString& szCodec, const qint16& iDuration, const qint16& iiHeight, const QString& szScantype, const qint16& iWidth);
	void			addAudioList(const qint16& iChannels, const QString& szCodec, const QString& szLanguage);
	void			addSubtitleList(const QString& szLanguge);
	void			setAdded(const QDateTime& added);
private:
	QString			m_szPath;

	QString			m_szNFO;
	QString			m_szBanner;
	QString			m_szFanart;
	QString			m_szLandscape;
	QString			m_szPoster;
	QString			m_szClearart;
	QString			m_szDisc;
	QString			m_szLogo;

	QString			m_szTMDBID;
	QString			m_szIMDBID;
	QString			m_szTitle;
	QString			m_szOriginalTitle;
	qint16			m_iYear;
	double			m_dRating;
	qint16			m_iVotes;
	qint16			m_iTop250;
	QString			m_szPlot;
	QString			m_szOutline;
	QString			m_szTagline;
	qint16			m_iRuntime;
	QString			m_szMPAA;
	QStringList		m_szCreditsList;
	QStringList		m_szDirectorList;
	qint16			m_iPlaycount;
	QString			m_szSet;
	QString			m_szSortTitle;
	QString			m_szTrailer;
	bool			m_bWatched;
	QStringList		m_szStudioList;
	QStringList		m_szGenreList;
	QStringList		m_szCountryList;
	QList<ACTOR>	m_ActorList;
	QList<THUMB>	m_ThumbList;
	QList<THUMB>	m_FanartList;
	QList<VIDEO>	m_VideoList;
	QList<AUDIO>	m_AudioList;
	QList<SUBTITLE>	m_SubtitleList;
	QDateTime		m_Added;

protected:
	void			load(const QString& szPath, const QString& szNFO);
	bool			loadNFO(const QString& szPath, const QString& szNFO);

	void			parseActor(const QDomElement& element);
	void			parseThumb(const QDomElement& element, QList<THUMB>& thumbList);
	void			parseFanart(const QDomElement& element);
	void			parseFileInfo(const QDomElement& element);
	void			parseStreamDetails(const QDomElement& element);
	void			parseStreamDetailsVideo(const QDomElement& element);
	void			parseStreamDetailsAudio(const QDomElement& element);
	void			parseStreamDetailsSubtitle(const QDomElement& element);
};

Q_DECLARE_METATYPE(cMovie*)

class cMovieList : public QList<cMovie *>
{
public:
	cMovieList();

#ifdef WITHDB
	void		open();
#endif
	void		parse(const QString& szPath, cXBMC* lpXBMC = 0, QStatusBar* lpStatusBar = 0);

	QStringList	genreList();
private:
	QStringList	m_szGenreList;
protected:
	void		parsePath(const QString& szPath, cXBMC* lpXBMC, QStatusBar* lpStatusBar);
};

#endif // CMOVIE_H
