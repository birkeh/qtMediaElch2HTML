#include "ctvshow.h"
#include "common.h"

#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QDir>


cTVShow::cTVShow(const QString& szPath, cXBMC* lpXBMC)
{
	load(szPath, lpXBMC);
}

bool cTVShow::isValid()
{
	return(!m_szPath.isEmpty() && !m_szNFO.isEmpty());
}

bool tvshowEpisodeSort(cTVShowEpisode* left, cTVShowEpisode *right)
{
	if(left->season() < right->season())
		return(true);
	else if(left->season() > right->season())
		return(false);
	if(left->episode() < right->episode())
		return(true);
	return(false);
}

void cTVShow::load(const QString& szPath, cXBMC* lpXBMC)
{
	QDir		dir(szPath);

	if(!dir.exists())
		return;

	QStringList	nfo	= dir.entryList(QStringList() << "tvshow.nfo", QDir::Files);
	if(!nfo.count())
		return;

	if(!loadNFO(szPath, nfo.at(0)))
		return;

	if(m_szNFO.isEmpty())
		return;

	QStringList	banner		= dir.entryList(QStringList() << "banner.*",	QDir::Files);
	QStringList	character	= dir.entryList(QStringList() << "character.*",	QDir::Files);
	QStringList	clearart	= dir.entryList(QStringList() << "clearart.*",	QDir::Files);
	QStringList	fanart		= dir.entryList(QStringList() << "fanart.*",	QDir::Files);
	QStringList	landscape	= dir.entryList(QStringList() << "landscape.*",	QDir::Files);
	QStringList	logo		= dir.entryList(QStringList() << "logo.*",		QDir::Files);
	QStringList	poster		= dir.entryList(QStringList() << "poster.*",	QDir::Files);

	if(banner.count())
		m_szBanner		= banner.at(0);
	if(character.count())
		m_szCharacter	= character.at(0);
	if(clearart.count())
		m_szClearart	= clearart.at(0);
	if(fanart.count())
		m_szFanart		= fanart.at(0);
	if(landscape.count())
		m_szLandscape	= landscape.at(0);
	if(logo.count())
		m_szLogo		= logo.at(0);
	if(poster.count())
		m_szPoster		= poster.at(0);

	banner		= dir.entryList(QStringList() << "season*banner.*", QDir::Files);
	landscape	= dir.entryList(QStringList() << "season*landscape.*", QDir::Files);
	poster		= dir.entryList(QStringList() << "season*poster.*", QDir::Files);

	for(int z = 0;z < banner.count();z++)
	{
		QString		s	= banner.at(z).mid(6, 2);
		SEASONPIC	p;

		p.iSeason		= banner.at(z).mid(6, 2).toInt();
		p.szBanner		= banner.at(z);
		if(landscape.filter(QString("season%1").arg(s)).count())
			p.szLandscape	= landscape.filter(QString("season%1").arg(s)).at(0);
		if(poster.filter(QString("season%1").arg(s)).count())
			p.szPoster		= poster.filter(QString("season%1").arg(s)).at(0);

		m_SeasonPicList.append(p);
	}

	m_szPath	= szPath;

	searchSeasons(szPath, lpXBMC);
	qSort(m_TVShowEpisodeList.begin(), m_TVShowEpisodeList.end(), tvshowEpisodeSort);
}

bool cTVShow::loadNFO(const QString& szPath, const QString& szNFO)
{
	QFile				file(QString("%1/%2").arg(szPath).arg(szNFO));
	if(!file.open(QFile::ReadOnly | QFile::Text))
		return(false);

	QDomDocument		doc;
	QString				errorStr;
	int					errorLine;
	int					errorColumn;

	if(!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn))
		return(false);
	file.close();

	QDomElement			root	= doc.documentElement();
	if(root.tagName().compare("tvshow", Qt::CaseInsensitive))
		return(false);

	QDomNode			child		= root.firstChild();
	QString				szTmp;
	QString				szGenre;

	while(!child.isNull())
	{
			 CHECKELEMENTS(child,	"title",			m_szTitle);
		else CHECKELEMENTD(child,	"rating",			m_dRating);
		else CHECKELEMENTS(child,	"plot",				m_szPlot);
		else CHECKELEMENTS(child,	"outline",			m_szOutline);
		else CHECKELEMENTS(child,	"mpaa",				m_szMPAA);
		else CHECKELEMENTP(child,	"premiered",		m_PremiereDate);
		else CHECKELEMENTL(child,	"studio",			m_szStudioList);
		else CHECKELEMENTS(child,	"tvdbid",			m_szTVDBID);
		else CHECKELEMENTS(child,	"id",				m_szID);
		else CHECKELEMENTI(child,	"runtime",			m_iRuntime);
		else CHECKELEMENTS(child,	"genre",			szGenre);
		else CHECKELEMENTF(child,	"actor",			parseActor);

		child	= child.nextSibling();
	}

	if(szGenre.length())
		m_szGenreList.append(szGenre.split(" / "));

	m_szNFO	= szNFO;
	return(true);
}

void cTVShow::parseActor(const QDomElement& element)
{
	QDomNode	child		= element.firstChild();
	ACTOR		actor;

	while(!child.isNull())
	{
			 CHECKELEMENTS(child,	"name",		actor.szName);
		else CHECKELEMENTS(child,	"role",		actor.szRole);
		else CHECKELEMENTS(child,	"thumb",	actor.szThumb);

		child	= child.nextSibling();
	}
	if(!actor.szName.isEmpty())
		m_ActorList.append(actor);
}

void cTVShow::searchSeasons(const QString& szPath, cXBMC* lpXBMC)
{
	QDir		dir(szPath);
	QStringList	nfo		= dir.entryList(QStringList() << "*.nfo", QDir::Files);
	QStringList	dirs	= dir.entryList(QDir::Dirs);

	dirs.removeAll(".");
	dirs.removeAll("..");

	for(int z = 0;z < dirs.count();z++)
		searchSeasons(QString("%1/%2").arg(szPath).arg(dirs.at(z)), lpXBMC);

	nfo.removeAll("tvshow.nfo");

	for(int z = 0;z < nfo.count();z++)
	{
		QFile	file(QString("%1/%2").arg(szPath).arg(nfo.at(z)));
		if(file.open(QIODevice::ReadOnly))
		{
			bool		bEpisode	= false;
			QByteArray	nfoData;
			QString		szInput;

			do
			{
				szInput	= file.readLine();
				if(szInput.contains("<episodedetails>", Qt::CaseInsensitive))
				{
					nfoData.append(szInput);
					bEpisode	= true;
				}
				else if(szInput.contains("</episodedetails>", Qt::CaseInsensitive))
				{
					nfoData.append(szInput);
					bEpisode	= false;

					cTVShowEpisode*	lpEpisode	= new cTVShowEpisode(szPath, nfo.at(z), nfoData);
					if(lpEpisode->isValid() && lpEpisode->showTitle() == m_szTitle)
					{
						if(lpXBMC)
							lpEpisode->setWatched(lpXBMC->episodeWatched(QString("%1/%2").arg(szPath).arg(nfo.at(z))));
						m_TVShowEpisodeList.append(lpEpisode);
					}
					else
						delete lpEpisode;
					nfoData.clear();
				}
				else if(bEpisode)
					nfoData.append(szInput);
			} while(!file.atEnd());

			file.close();

		}
	}
}

QString cTVShow::path()
{
	return(m_szPath);
}

QString cTVShow::nfo()
{
	return(m_szNFO);
}

QList<SEASONPIC> cTVShow::seasonPicList()
{
	return(m_SeasonPicList);
}

QString cTVShow::banner()
{
	return(QString("%1/%2").arg(m_szPath).arg(m_szFanart));
	return(m_szBanner);
}

QString cTVShow::character()
{
	return(QString("%1/%2").arg(m_szPath).arg(m_szCharacter));
}

QString cTVShow::clearart()
{
	return(QString("%1/%2").arg(m_szPath).arg(m_szClearart));
}

QString cTVShow::fanart()
{
	return(QString("%1/%2").arg(m_szPath).arg(m_szFanart));
}

QString cTVShow::landscape()
{
	return(QString("%1/%2").arg(m_szPath).arg(m_szLandscape));
}

QString cTVShow::logo()
{
	return(QString("%1/%2").arg(m_szPath).arg(m_szLogo));
}

QString cTVShow::poster()
{
	return(QString("%1/%2").arg(m_szPath).arg(m_szPoster));
}

QString cTVShow::title()
{
	return(m_szTitle);
}

double cTVShow::rating()
{
	return(m_dRating);
}

QString cTVShow::plot()
{
	return(m_szPlot);
}

QString cTVShow::outline()
{
	return(m_szOutline);
}

QString cTVShow::mpaa()
{
	return(m_szMPAA);
}

QDate cTVShow::premiereDate()
{
	return(m_PremiereDate);
}

QStringList cTVShow::studioList()
{
	return(m_szStudioList);
}

QString cTVShow::tvdbID()
{
	return(m_szTVDBID);
}

qint16 cTVShow::runtime()
{
	return(m_iRuntime);
}

QStringList cTVShow::genreList()
{
	return(m_szGenreList);
}

QList<ACTOR> cTVShow::actorList()
{
	return(m_ActorList);
}

qint16 cTVShow::year()
{
	if(m_PremiereDate.isValid())
		return(m_PremiereDate.year());
	else
		return(0);
}

QList<cTVShowEpisode*> cTVShow::episodeList()
{
	return(m_TVShowEpisodeList);
}

cTVShowList::cTVShowList()
{
}

void cTVShowList::parse(const QString& szPath, const QStringList& exceptions, cXBMC* lpXBMC, QStatusBar* lpStatusBar)
{
	QDir		dir(szPath);

	if(!dir.exists())
		return;

	foreach(const QString& p, exceptions)
	{
		QString a = szPath;
		QString	b = p;
		if(a.replace("\\", "/").startsWith(b.replace("\\", "/")))
			return;
	}

	if(lpStatusBar)
		lpStatusBar->showMessage(QString("%1 %2 ...").arg("scanning").arg(szPath));

	QStringList	dirList	= dir.entryList(QDir::Dirs);
	dirList.removeAll(".");
	dirList.removeAll("..");

	for(int z = 0;z < dirList.count();z++)
		parse(QString("%1/%2").arg(szPath).arg(dirList.at(z)), exceptions, lpXBMC, lpStatusBar);

	QStringList	nfoList	= dir.entryList(QStringList() << "tvshow.nfo", QDir::Files);
	for(int z = 0;z < nfoList.count();z++)
	{
		cTVShow*	lpTVShow	= new cTVShow(szPath, lpXBMC);
		if(lpTVShow->isValid())
		{
			append(lpTVShow);
			m_szGenreList.append(lpTVShow->genreList());
			m_szGenreList.removeDuplicates();
		}
		else
			delete(lpTVShow);
	}
}

QStringList cTVShowList::genreList()
{
	return(m_szGenreList);
}
