#include "ctvshowepisode.h"

#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QDir>


cTVShowEpisode::cTVShowEpisode(const QString& szPath, const QString& szFile, const QByteArray& szNFO)
{
	m_bWatched	= false;
	load(szPath, szFile, szNFO);
}

bool cTVShowEpisode::isValid()
{
	return(!m_szPath.isEmpty() && !m_szNFO.isEmpty());
}

void cTVShowEpisode::load(const QString& szPath, const QString& szFile, const QByteArray& szNFO)
{
	QDir		dir(szPath);

	if(!dir.exists())
		return;

	if(!loadNFO(szNFO))
		return;
	if(m_szNFO.isEmpty())
		return;

	QString		szPrefix	= szFile.left(szFile.lastIndexOf(".nfo"));
	QStringList	thumb		= dir.entryList(QStringList() << QString("%1-thumb.*").arg(szPrefix), QDir::Files);
	if(thumb.count())
		m_szThumb	= thumb.at(0);

	m_szPath	= szPath;
}

bool cTVShowEpisode::loadNFO(const QByteArray& szNFO)
{
	QDomDocument		doc;
	QString				errorStr;
	int					errorLine;
	int					errorColumn;

	if(!doc.setContent(szNFO, false, &errorStr, &errorLine, &errorColumn))
		return(false);

	QDomElement			root	= doc.documentElement();
	if(root.tagName().compare("episodedetails", Qt::CaseInsensitive))
		return(false);

	QDomNode			child		= root.firstChild();
	QString				szTmp;

	while(!child.isNull())
	{
			 CHECKELEMENTS(child,	"title",			m_szTitle);
		else CHECKELEMENTS(child,	"showtitle",		m_szShowTitle);
		else CHECKELEMENTD(child,	"rating",			m_dRating);
		else CHECKELEMENTI(child,	"season",			m_iSeason);
		else CHECKELEMENTI(child,	"episode",			m_iEpisode);
		else CHECKELEMENTS(child,	"plot",				m_szPlot);
		else CHECKELEMENTS(child,	"outline",			m_szOutline);
		else CHECKELEMENTS(child,	"mpaa",				m_szMPAA);
		else CHECKELEMENTI(child,	"playcount",		m_iPlaycount);
		else CHECKELEMENTP(child,	"aired",			m_Aired);
		else CHECKELEMENTL(child,	"studio",			m_szStudioList);
		else CHECKELEMENTL(child,	"credits",			m_szCreditsList);
		else CHECKELEMENTL(child,	"director",			m_szDirectorList);
		else CHECKELEMENTL(child,	"thumb",			m_szThumbList);
		else CHECKELEMENTF(child,	"fileinfo",			parseFileInfo);

		child	= child.nextSibling();
	}

	m_szNFO	= szNFO;
	return(true);
}

void cTVShowEpisode::parseFileInfo(const QDomElement& element)
{
	QDomNode	child		= element.firstChild();

	while(!child.isNull())
	{
		CHECKELEMENTF(child,	"streamdetails",	parseStreamDetails);

		child	= child.nextSibling();
	}
}

void cTVShowEpisode::parseStreamDetails(const QDomElement& element)
{
	QDomNode	child		= element.firstChild();

	while(!child.isNull())
	{
			 CHECKELEMENTF(child,	"video",	parseStreamDetailsVideo);
		else CHECKELEMENTF(child,	"audio",	parseStreamDetailsAudio);
		else CHECKELEMENTF(child,	"subtitle",	parseStreamDetailsSubtitle);

		child	= child.nextSibling();
	}
}

void cTVShowEpisode::parseStreamDetailsVideo(const QDomElement& element)
{
	QDomNode	child		= element.firstChild();
	VIDEO		video;

	while(!child.isNull())
	{
			 CHECKELEMENTD(child,	"aspect",				video.dAspect);
		else CHECKELEMENTS(child,	"codec",				video.szCodec);
		else CHECKELEMENTI(child,	"durationinseconds",	video.iDuration);
		else CHECKELEMENTI(child,	"height",				video.iHeight);
		else CHECKELEMENTS(child,	"scantype",				video.szScantype);
		else CHECKELEMENTI(child,	"width",				video.iWidth);

		child	= child.nextSibling();
	}
	m_VideoList.append(video);
}

void cTVShowEpisode::parseStreamDetailsAudio(const QDomElement& element)
{
	QDomNode	child		= element.firstChild();
	AUDIO		audio;

	while(!child.isNull())
	{
			 CHECKELEMENTI(child,	"channels",				audio.iChannels);
		else CHECKELEMENTS(child,	"codec",				audio.szCodec);
		else CHECKELEMENTS(child,	"language",				audio.szLanguage);

		child	= child.nextSibling();
	}
	m_AudioList.append(audio);
}

void cTVShowEpisode::parseStreamDetailsSubtitle(const QDomElement& element)
{
	QDomNode	child		= element.firstChild();
	SUBTITLE	subtitle;

	while(!child.isNull())
	{
		CHECKELEMENTS(child,	"language",	subtitle.szLanguage);

		child	= child.nextSibling();
	}
	m_SubtitleList.append(subtitle);
}

QString cTVShowEpisode::showTitle()
{
	return(m_szShowTitle);
}

QString cTVShowEpisode::title()
{
	return(m_szTitle);
}

qint16 cTVShowEpisode::season()
{
	return(m_iSeason);
}

qint16 cTVShowEpisode::episode()
{
	return(m_iEpisode);
}

QString cTVShowEpisode::plot()
{
	return(m_szPlot);
}

QList<VIDEO> cTVShowEpisode::videoList()
{
	return(m_VideoList);
}

QList<AUDIO> cTVShowEpisode::audioList()
{
	return(m_AudioList);
}

void cTVShowEpisode::setWatched(bool bWatched)
{
	m_bWatched	= bWatched;
}

void cTVShowEpisode::setAdded(const QDateTime &added)
{
	m_Added	= added;
}

bool cTVShowEpisode::watched()
{
	return(m_bWatched);
}

QDateTime cTVShowEpisode::added()
{
	return(m_Added);
}
