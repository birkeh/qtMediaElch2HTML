#ifndef CTVSHOWEPISODE_H
#define CTVSHOWEPISODE_H


#include "common.h"

#include <QString>
#include <QList>
#include <QStringList>
#include <QMetaType>
#include <QDomElement>
#include <QDate>
#include <QStatusBar>
#include <QByteArray>


class cTVShowEpisode
{
public:
	cTVShowEpisode(const QString& szPath, const QString& szFile, const QByteArray& szNFO);

	bool			isValid();

	QString			showTitle();
	QString			title();
	qint16			season();
	qint16			episode();

	QString			plot();

	QList<VIDEO>	videoList();
	QList<AUDIO>	audioList();

	void			setWatched(bool bWatched);
	void			setAdded(const QDateTime& added);
	bool			watched();
	QDateTime		added();

private:
	QString			m_szPath;

	QByteArray		m_szNFO;
	QString			m_szThumb;

	QString			m_szTitle;
	QString			m_szShowTitle;
	double			m_dRating;
	qint16			m_iSeason;
	qint16			m_iEpisode;
	QString			m_szPlot;
	QString			m_szOutline;
	QString			m_szMPAA;
	qint16			m_iPlaycount;
	QDate			m_Aired;
	QStringList		m_szStudioList;
	QStringList		m_szCreditsList;
	QStringList		m_szDirectorList;
	QStringList		m_szThumbList;
	QList<VIDEO>	m_VideoList;
	QList<AUDIO>	m_AudioList;
	QList<SUBTITLE>	m_SubtitleList;
	bool			m_bWatched;
	QDateTime		m_Added;

protected:
	void			load(const QString& szPath, const QString& szFile, const QByteArray& szNFO);
	bool			loadNFO(const QByteArray& szNFO);

	void			parseFileInfo(const QDomElement& element);
	void			parseStreamDetails(const QDomElement& element);
	void			parseStreamDetailsVideo(const QDomElement& element);
	void			parseStreamDetailsAudio(const QDomElement& element);
	void			parseStreamDetailsSubtitle(const QDomElement& element);
};

Q_DECLARE_METATYPE(cTVShowEpisode*)

class cTVShowEpisodeList : public QList<cTVShowEpisode *>
{
public:
	cTVShowEpisodeList();

protected:
};


#endif // CTVSHOWEPISODE_H
