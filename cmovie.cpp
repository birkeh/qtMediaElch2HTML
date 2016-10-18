#include "cmovie.h"
#include "common.h"

#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QDir>
#ifdef WITHDB
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDriver>
#endif
#include <QDebug>


cMovie::cMovie(const QString& szPath, const QString& szNFO) :
	m_szPath(""),
	m_szNFO(""),
	m_szBanner(""),
	m_szFanart(""),
	m_szLandscape(""),
	m_szPoster(""),
	m_szClearart(""),
	m_szDisc(""),
	m_szLogo(""),
	m_szTitle(""),
	m_szOriginalTitle(""),
	m_iYear(-1),
	m_dRating(-1),
	m_iVotes(-1),
	m_iTop250(-1),
	m_szPlot(""),
	m_szOutline(""),
	m_szTagline(""),
	m_iRuntime(-1),
	m_szMPAA(""),
	m_iPlaycount(-1),
	m_szSet(""),
	m_szSortTitle(""),
	m_szTrailer(""),
	m_bWatched(false)
{
	load(szPath, szNFO);
}

bool cMovie::isValid()
{
	return(!m_szPath.isEmpty() && !m_szNFO.isEmpty());
}

void cMovie::load(const QString& szPath, const QString& szNFO)
{
	QDir		dir(szPath);

	if(!dir.exists())
		return;

	QStringList	nfo	= dir.entryList(QStringList() << "*.nfo", QDir::Files);
	if(!nfo.count())
		return;

	if(!loadNFO(szPath, szNFO))
		return;

	if(m_szNFO.isEmpty())
		return;

	QString		szPrefix	= szNFO.left(szNFO.lastIndexOf(".nfo"));

	QStringList	banner		= dir.entryList(QStringList() << QString("%1-banner.*").arg(szPrefix),		QDir::Files);
	QStringList	fanart		= dir.entryList(QStringList() << QString("%1-fanart.*").arg(szPrefix),		QDir::Files);
	QStringList	landscape	= dir.entryList(QStringList() << QString("%1-landscape.*").arg(szPrefix),	QDir::Files);
	QStringList	poster		= dir.entryList(QStringList() << QString("%1-poster.*").arg(szPrefix),		QDir::Files);
	QStringList	clearart	= dir.entryList(QStringList() << "clearart.*",								QDir::Files);
	QStringList	disc		= dir.entryList(QStringList() << "disc.*",									QDir::Files);
	QStringList	logo		= dir.entryList(QStringList() << "logo.*",									QDir::Files);

	if(banner.count())
		m_szBanner		= banner.at(0);
	if(fanart.count())
		m_szFanart		= fanart.at(0);
	if(landscape.count())
		m_szLandscape	= landscape.at(0);
	if(poster.count())
		m_szPoster		= poster.at(0);
	if(clearart.count())
		m_szClearart	= clearart.at(0);
	if(disc.count())
		m_szDisc		= disc.at(0);
	if(logo.count())
		m_szLogo		= logo.at(0);

	m_szPath	= szPath;
}

bool cMovie::loadNFO(const QString& szPath, const QString& szNFO)
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
	if(root.tagName().compare("movie", Qt::CaseInsensitive))
		return(false);

	QDomNode			child		= root.firstChild();
	QString				szTmp;
	QString				szGenre;

	while(!child.isNull())
	{
		if(!child.toElement().tagName().compare("id", Qt::CaseInsensitive))
		{
			m_szTMDBID	= getAttributeStr(child.toElement(), "TMDB");
			m_szIMDBID	= child.toElement().text();
		}
		else CHECKELEMENTS(child,	"title",			m_szTitle);
		else CHECKELEMENTS(child,	"originaltitle",	m_szOriginalTitle);
		else CHECKELEMENTI(child,	"year",				m_iYear);
		else CHECKELEMENTD(child,	"rating",			m_dRating);
		else CHECKELEMENTI(child,	"votes",			m_iVotes);
		else CHECKELEMENTI(child,	"top250",			m_iTop250);
		else CHECKELEMENTS(child,	"plot",				m_szPlot);
		else CHECKELEMENTS(child,	"outline",			m_szOutline);
		else CHECKELEMENTS(child,	"tagline",			m_szTagline);
		else CHECKELEMENTI(child,	"runtime",			m_iRuntime);
		else CHECKELEMENTS(child,	"mpaa",				m_szMPAA);
		else CHECKELEMENTL(child,	"credits",			m_szCreditsList);
		else CHECKELEMENTL(child,	"director",			m_szDirectorList);
		else CHECKELEMENTI(child,	"playcount",		m_iPlaycount);
		else CHECKELEMENTS(child,	"tmdbid",			m_szTMDBID);
		else CHECKELEMENTS(child,	"set",				m_szSet);
		else CHECKELEMENTS(child,	"sorttitle",		m_szSortTitle);
		else CHECKELEMENTS(child,	"trailer",			m_szTrailer);
		else CHECKELEMENTB(child,	"watched",			m_bWatched);
		else CHECKELEMENTL(child,	"studio",			m_szStudioList);
		else CHECKELEMENTS(child,	"genre",			szGenre);
		else CHECKELEMENTL(child,	"country",			m_szCountryList);
		else CHECKELEMENTF(child,	"actor",			parseActor);
		else if(!child.toElement().tagName().compare("thumb", Qt::CaseInsensitive))
			parseThumb(child.toElement(), m_ThumbList);
		else CHECKELEMENTF(child,	"fanart",			parseFanart);
		else CHECKELEMENTF(child,	"fileinfo",			parseFileInfo);

		child	= child.nextSibling();
	}

	if(szGenre.length())
		m_szGenreList.append(szGenre.split(" / "));

	m_szNFO	= szNFO;
	return(true);
}

void cMovie::parseActor(const QDomElement& element)
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

void cMovie::parseThumb(const QDomElement& element, QList<THUMB>& thumbList)
{
	THUMB	thumb;
	thumb.szPreview	= getAttributeStr(element, "preview");
	thumb.szThumb	= element.text();
	thumbList.append(thumb);
}

void cMovie::parseFanart(const QDomElement& element)
{
	QDomNode	child		= element.firstChild();

	while(!child.isNull())
	{
		if(!child.toElement().tagName().compare("thumb", Qt::CaseInsensitive))
			parseThumb(child.toElement(), m_FanartList);

		child	= child.nextSibling();
	}
}

void cMovie::parseFileInfo(const QDomElement& element)
{
	QDomNode	child		= element.firstChild();

	while(!child.isNull())
	{
		CHECKELEMENTF(child,	"streamdetails",	parseStreamDetails);

		child	= child.nextSibling();
	}
}

void cMovie::parseStreamDetails(const QDomElement& element)
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

void cMovie::parseStreamDetailsVideo(const QDomElement& element)
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

void cMovie::parseStreamDetailsAudio(const QDomElement& element)
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

void cMovie::parseStreamDetailsSubtitle(const QDomElement& element)
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

QString cMovie::path()
{
	return(m_szPath);
}

QString cMovie::nfo()
{
	return(QString("%1/%2").arg(m_szPath).arg(m_szNFO));
}

QString cMovie::banner()
{
	return(QString("%1/%2").arg(m_szPath).arg(m_szBanner));
}

QString cMovie::fanart()
{
	return(QString("%1/%2").arg(m_szPath).arg(m_szFanart));
}

QString cMovie::poster()
{
	return(QString("%1/%2").arg(m_szPath).arg(m_szPoster));
}

QString cMovie::clearart()
{
	return(QString("%1/%2").arg(m_szPath).arg(m_szClearart));
}

QString cMovie::disc()
{
	return(QString("%1/%2").arg(m_szPath).arg(m_szClearart));
}

QString cMovie::logo()
{
	return(QString("%1/%2").arg(m_szPath).arg(m_szLogo));
}

QString cMovie::tmdbID()
{
	return(m_szTMDBID);
}

QString cMovie::imdbID()
{
	return(m_szIMDBID);
}

QString cMovie::title()
{
	return(m_szTitle);
}

QString cMovie::originalTitle()
{
	return(m_szOriginalTitle);
}

qint16 cMovie::year()
{
	return(m_iYear);
}

double cMovie::rating()
{
	return(m_dRating);
}
qint16 cMovie::votes()
{
	return(m_iVotes);
}

qint16 cMovie::top250()
{
	return(m_iTop250);
}

QString cMovie::plot()
{
	return(m_szPlot);
}

QString cMovie::outline()
{
	return(m_szOutline);
}

QString cMovie::tagline()
{
	return(m_szTagline);
}

qint16 cMovie::runtime()
{
	return(m_iRuntime);
}

QString cMovie::mpaa()
{
	return(m_szMPAA);
}

QStringList cMovie::creditsList()
{
	return(m_szCreditsList);
}

QStringList cMovie::directorList()
{
	return(m_szDirectorList);
}

qint16 cMovie::playcount()
{
	return(m_iPlaycount);
}

QString cMovie::set()
{
	return(m_szSet);
}

QString cMovie::sortTitle()
{
	return(m_szSortTitle);
}

QString cMovie::trailer()
{
	return(m_szTrailer);
}

bool cMovie::watched()
{
	return(m_bWatched);
}

QStringList cMovie::studioList()
{
	return(m_szStudioList);
}

QStringList cMovie::genreList()
{
	return(m_szGenreList);
}

QStringList cMovie::countryList()
{
	return(m_szCountryList);
}

QList<ACTOR> cMovie::actorList()
{
	return(m_ActorList);
}

QList<THUMB> cMovie::thumbList()
{
	return(m_ThumbList);
}

QList<THUMB> cMovie::fanartList()
{
	return(m_FanartList);
}

QList<VIDEO> cMovie::videoList()
{
	return(m_VideoList);
}

QList<AUDIO> cMovie::audioList()
{
	return(m_AudioList);
}

QList<SUBTITLE> cMovie::subtitleList()
{
	return(m_SubtitleList);
}

QDateTime cMovie::added()
{
	return(m_Added);
}

void cMovie::setBanner(const QString& szBanner)
{
	m_szBanner	= szBanner;
}

void cMovie::setFanart(const QString& szFanart)
{
	m_szFanart	= szFanart;
}

void cMovie::setPoster(const QString& szPoster)
{
	m_szPoster	= szPoster;
}

void cMovie::setClearart(const QString& szClearart)
{
	m_szClearart	= szClearart;
}

void cMovie::setDisc(const QString& szDisc)
{
	m_szDisc	= szDisc;
}

void cMovie::setLogo(const QString& szLogo)
{
	m_szLogo	= szLogo;
}

void cMovie::settmdbID(const QString& szTMDBID)
{
	m_szTMDBID	= szTMDBID;
}

void cMovie::setimdbID(const QString& szIMDBID)
{
	m_szIMDBID	= szIMDBID;
}

void cMovie::setTitle(const QString& szTitle)
{
	m_szTitle	= szTitle;
}

void cMovie::setOriginalTitle(const QString& szOriginalTitle)
{
	m_szOriginalTitle	= szOriginalTitle;
}

void cMovie::setYear(const qint16& iYear)
{
	m_iYear		= iYear;
}

void cMovie::setRating(const double& dRating)
{
	m_dRating	= dRating;
}

void cMovie::setVotes(const qint16& iVotes)
{
	m_iVotes	= iVotes;
}

void cMovie::setTop250(const qint16& iTop250)
{
	m_iTop250	= iTop250;
}

void cMovie::setPlot(const QString& szPlot)
{
	m_szPlot	= szPlot;
}

void cMovie::setOutline(const QString& szOutline)
{
	m_szOutline	= szOutline;
}

void cMovie::setTagline(const QString& szTagline)
{
	m_szTagline	= szTagline;
}

void cMovie::setRuntime(const qint16& iRuntime)
{
	m_iRuntime	= iRuntime;
}

void cMovie::setMPAA(const QString& szMPAA)
{
	m_szMPAA	= szMPAA;
}

void cMovie::addCreditsList(const QString& szCredit)
{
	m_szCreditsList.append(szCredit);
}

void cMovie::addDirectorList(const QString& szDirector)
{
	m_szDirectorList.append(szDirector);
}

void cMovie::setPlaycount(const qint16& iPlaycount)
{
	m_iPlaycount	= iPlaycount;
}

void cMovie::setSet(const QString& szSet)
{
	m_szSet		= szSet;
}

void cMovie::setSortTitle(const QString& szSortTitle)
{
	m_szSortTitle	= szSortTitle;
}

void cMovie::setTrailer(const QString& szTrailer)
{
	m_szTrailer		= szTrailer;
}

void cMovie::setWatched(const bool& bWatched)
{
	m_bWatched		= bWatched;
}

void cMovie::addStudioList(const QString& szStudio)
{
	m_szStudioList.append(szStudio);
}

void cMovie::addGenreList(const QString& szGenre)
{
	m_szGenreList.append(szGenre);
}

void cMovie::addCountryList(const QString& szCountry)
{
	m_szCountryList.append(szCountry);
}

void cMovie::addActorList(const QString& szActor, const QString& szRole, const QString& szThumb)
{
	ACTOR	a;
	a.szName	= szActor;
	a.szRole	= szRole;
	a.szThumb	= szThumb;
	m_ActorList.append(a);
}

void cMovie::addThumbList(const QString& szPreview, const QString& szThumb)
{
	THUMB	t;
	t.szPreview	= szPreview;
	t.szThumb	= szThumb;
	m_ThumbList.append(t);
}

void cMovie::addFanartList(const QString& szPreview, const QString& szThumb)
{
	THUMB	t;
	t.szPreview	= szPreview;
	t.szThumb	= szThumb;
	m_FanartList.append(t);
}

void cMovie::addVideoList(const double& dAspect, const QString& szCodec, const qint16& iDuration, const qint16& iHeight, const QString& szScantype, const qint16& iWidth)
{
	VIDEO	v;
	v.dAspect		= dAspect;
	v.szCodec		= szCodec;
	v.iDuration		= iDuration;
	v.iHeight		= iHeight;
	v.szScantype	= szScantype;
	v.iWidth		= iWidth;
	m_VideoList.append(v);
}

void cMovie::addAudioList(const qint16& iChannels, const QString& szCodec, const QString& szLanguage)
{
	AUDIO	a;
	a.iChannels		= iChannels;
	a.szCodec		= szCodec;
	a.szLanguage	= szLanguage;
	m_AudioList.append(a);
}

void cMovie::addSubtitleList(const QString& szLanguge)
{
	SUBTITLE	s;
	s.szLanguage	= szLanguge;
	m_SubtitleList.append(s);
}

void cMovie::setAdded(const QDateTime &added)
{
	m_Added	= added;
}

cMovieList::cMovieList()
{
}

#ifdef WITHDB
void cMovieList::open()
{
	QSqlDatabase	database	= QSqlDatabase::addDatabase("QSQLITE");
	QString			szDatabase	= QString("%1/.qtMediaElch2HTML").arg(QDir::homePath());
	szDatabase.append("/qtMediaElch2HTML.sqlite");
	database.setDatabaseName(szDatabase);
	if(!database.open())
		return;

	QSqlQuery		qMovie		= database.exec("SELECT path, nfo, banner, fanart, poster, clearart, disc, logo, tmdbid, imdbid, title, originaltitle, year, rating, votes, top250, plot, outline, tagline, runtime, mpaa, playcount, movieset, sorttitle, trailer, watched, sort FROM movie ORDER BY sort;");

	while(qMovie.next())
	{
		cMovie*	lpMovie	= new cMovie(qMovie.value(0).toString(), qMovie.value(1).toString());

		lpMovie->setBanner(qMovie.value(2).toString());
		lpMovie->setFanart(qMovie.value(3).toString());
		lpMovie->setPoster(qMovie.value(4).toString());
		lpMovie->setClearart(qMovie.value(5).toString());
		lpMovie->setDisc(qMovie.value(6).toString());
		lpMovie->setLogo(qMovie.value(7).toString());
		lpMovie->settmdbID(qMovie.value(8).toString());
		lpMovie->setimdbID(qMovie.value(9).toString());
		lpMovie->setTitle(qMovie.value(10).toString());
		lpMovie->setOriginalTitle(qMovie.value(11).toString());
		lpMovie->setYear(qMovie.value(12).toInt());
		lpMovie->setRating(qMovie.value(13).toDouble());
		lpMovie->setVotes(qMovie.value(14).toInt());
		lpMovie->setTop250(qMovie.value(15).toInt());
		lpMovie->setPlot(qMovie.value(16).toString());
		lpMovie->setOutline(qMovie.value(17).toString());
		lpMovie->setTagline(qMovie.value(18).toString());
		lpMovie->setRuntime(qMovie.value(19).toInt());
		lpMovie->setMPAA(qMovie.value(20).toString());
		lpMovie->setPlaycount(qMovie.value(21).toInt());
		lpMovie->setSet(qMovie.value(22).toString());
		lpMovie->setSortTitle(qMovie.value(23).toString());
		lpMovie->setTrailer(qMovie.value(24).toString());
		lpMovie->setWatched(qMovie.value(25).toInt());

		QSqlQuery	q;

		q	= database.exec(QString("SELECT credit FROM moviecredit WHERE movieid=%1 ORDER BY sort;").arg(qMovie.value(26).toInt()));
		while(q.next())
			lpMovie->addCreditsList(q.value(0).toString());

		q	= database.exec(QString("SELECT director FROM moviedirector WHERE movieid=%1 ORDER BY sort;").arg(qMovie.value(26).toInt()));
		while(q.next())
			lpMovie->addDirectorList(q.value(0).toString());

		q	= database.exec(QString("SELECT studio FROM moviestudio WHERE movieid=%1 ORDER BY sort;").arg(qMovie.value(26).toInt()));
		while(q.next())
			lpMovie->addStudioList(q.value(0).toString());

		q	= database.exec(QString("SELECT genre FROM moviegenre WHERE movieid=%1 ORDER BY sort;").arg(qMovie.value(26).toInt()));
		while(q.next())
			lpMovie->addGenreList(q.value(0).toString());

		q	= database.exec(QString("SELECT country FROM moviecountry WHERE movieid=%1 ORDER BY sort;").arg(qMovie.value(26).toInt()));
		while(q.next())
			lpMovie->addCountryList(q.value(0).toString());

		q	= database.exec(QString("SELECT name, role, thumb FROM movieactor WHERE movieid=%1 ORDER BY sort;").arg(qMovie.value(26).toInt()));
		while(q.next())
			lpMovie->addActorList(q.value(0).toString(), q.value(1).toString(), q.value(2).toString());

		q	= database.exec(QString("SELECT preview, thumb FROM moviethumb WHERE movieid=%1 ORDER BY sort;").arg(qMovie.value(26).toInt()));
		while(q.next())
			lpMovie->addThumbList(q.value(0).toString(), q.value(1).toString());

		q	= database.exec(QString("SELECT preview, thumb FROM moviefanart WHERE movieid=%1 ORDER BY sort;").arg(qMovie.value(26).toInt()));
		while(q.next())
			lpMovie->addFanartList(q.value(0).toString(), q.value(1).toString());

		q	= database.exec(QString("SELECT aspect, codec, duration, height, scantype, width FROM movievideo WHERE movieid=%1 ORDER BY sort;").arg(qMovie.value(26).toInt()));
		while(q.next())
			lpMovie->addVideoList(q.value(0).toDouble(), q.value(1).toString(), q.value(2).toInt(), q.value(3).toInt(), q.value(4).toString(), q.value(5).toInt());

		q	= database.exec(QString("SELECT channels, codec, language FROM movieaudio WHERE movieid=%1 ORDER BY sort;").arg(qMovie.value(26).toInt()));
		while(q.next())
			lpMovie->addAudioList(q.value(0).toInt(), q.value(1).toString(), q.value(2).toString());

		q	= database.exec(QString("SELECT language FROM moviesubtitle WHERE movieid=%1 ORDER BY sort;").arg(qMovie.value(26).toInt()));
		while(q.next())
			lpMovie->addSubtitleList(q.value(0).toString());

		append(lpMovie);
	}

	database.close();
}
#endif
void cMovieList::parse(const QString& szPath, cXBMC* lpXBMC, QStatusBar* lpStatusBar)
{
	parsePath(szPath, lpXBMC, lpStatusBar);
	m_szGenreList.removeDuplicates();

#ifdef WITHDB
	QSqlDatabase	database	= QSqlDatabase::addDatabase("QSQLITE");
	QString			szDatabase	= QString("%1/.qtMediaElch2HTML").arg(QDir::homePath());
	QDir			dir;
	dir.mkpath(szDatabase);

	szDatabase.append("/qtMediaElch2HTML.sqlite");
	database.setDatabaseName(szDatabase);
	if(!database.open())
		return;

	QStringList		tables		= database.tables();

	if(!tables.contains("movie"))
		database.exec("CREATE TABLE movie ( \"path\" TEXT, \"nfo\" TEXT, \"banner\" TEXT, \"fanart\" TEXT, \"poster\" TEXT, \"clearart\" TEXT, \"disc\" TEXT, \"logo\" TEXT, \"tmdbid\" TEXT, \"imdbid\" TEXT, \"title\" TEXT, \"originaltitle\" TEXT, \"year\" INTEGER, \"rating\" REAL, \"votes\" INTEGER, \"top250\" INTEGER, \"plot\" TEXT, \"outline\" TEXT, \"tagline\" TEXT, \"runtime\" INTEGER, \"mpaa\" TEXT, \"playcount\" INTEGER, \"movieset\" TEXT, \"sorttitle\" TEXT, \"trailer\" TEXT, \"watched\" INTEGER, \"sort\" INTEGER);");
	if(!tables.contains("movieactor"))
		database.exec("CREATE TABLE movieactor ( \"movieid\" INTEGER, \"name\" TEXT, \"role\" TEXT, \"thumb\" TEXT, \"sort\" INTEGER);");
	if(!tables.contains("moviecountry"))
		database.exec("CREATE TABLE moviecountry ( \"movieid\" INTEGER, \"country\" TEXT, \"sort\" INTEGER);");
	if(!tables.contains("moviecredits"))
		database.exec("CREATE TABLE moviecredits ( \"movieid\" INTEGER, \"credit\" TEXT, \"sort\" INTEGER);");
	if(!tables.contains("moviedirector"))
		database.exec("CREATE TABLE moviedirector ( \"movieid\" INTEGER, \"director\" TEXT, \"sort\" INTEGER);");
	if(!tables.contains("moviegenre"))
		database.exec("CREATE TABLE moviegenre ( \"movieid\" INTEGER, \"genre\" TEXT, \"sort\" INTEGER);");
	if(!tables.contains("moviestudio"))
		database.exec("CREATE TABLE moviestudio ( \"movieid\" INTEGER, \"studio\" TEXT, \"sort\" INTEGER);");
	if(!tables.contains("moviethumb"))
		database.exec("CREATE TABLE \"moviethumb\" ( \"movieid\" INTEGER, \"preview\" TEXT, \"thumb\" TEXT, \"sort\" INTEGER);");
	if(!tables.contains("moviefanart"))
		database.exec("CREATE TABLE \"moviefanart\" ( \"movieid\" INTEGER, \"preview\" TEXT, \"thumb\" TEXT, \"sort\" INTEGER);");
	if(!tables.contains("movievideo"))
		database.exec("CREATE TABLE \"movievideo\" ( \"movieid\" INTEGER, \"aspect\" REAL, \"codec\" TEXT, \"duration\" INTEGER, \"height\" INTEGER, \"scantype\" TEXT, \"width\" INTEGER, \"sort\" INTEGER);");
	if(!tables.contains("movieaudio"))
		database.exec("CREATE TABLE \"movieaudio\" ( \"movieid\" INTEGER, \"channels\" INTEGER, \"codec\" TEXT, \"language\" TEXT, \"sort\" INTEGER);");
	if(!tables.contains("moviesubtitle"))
		database.exec("CREATE TABLE \"moviesubtitle\" ( \"movieid\" INTEGER, \"language\" TEXT, \"sort\" INTEGER);");

	database.exec("DELETE FROM movie;");
	database.exec("DELETE FROM movieactor;");
	database.exec("DELETE FROM moviecountry;");
	database.exec("DELETE FROM moviecredits;");
	database.exec("DELETE FROM moviedirector;");
	database.exec("DELETE FROM moviegenre;");
	database.exec("DELETE FROM moviestudio;");
	database.exec("DELETE FROM moviethumb;");
	database.exec("DELETE FROM moviefanart;");
	database.exec("DELETE FROM movievideo;");
	database.exec("DELETE FROM movieaudio;");
	database.exec("DELETE FROM moviesubtitle;");

	database.driver()->beginTransaction();

	for(int z = 0;z < count();z++)
	{
		cMovie*	lpMovie	= at(z);

		database.exec(QString("INSERT INTO movie ( \"path\", \"nfo\", \"banner\", \"fanart\", \"poster\", \"clearart\", \"disc\", \"logo\", \"tmdbid\", \"imdbid\", \"title\", \"originaltitle\", \"year\", \"rating\", \"votes\", \"top250\", \"plot\", \"outline\", \"tagline\", \"runtime\", \"mpaa\", \"playcount\", \"movieset\", \"sorttitle\", \"trailer\", \"watched\", \"sort\") VALUES (\"%1\", \"%2\", \"%3\", \"%4\", \"%5\", \"%6\", \"%7\", \"%8\", \"%9\", \"%10\", \"%11\", \"%12\", %13, %14, %15, %16, \"%17\", \"%18\", \"%19\", %20, \"%21\", %22, \"%23\", \"%24\", \"%25\", %26, %27);")
					  .arg(lpMovie->path()).arg(lpMovie->nfo()).arg(lpMovie->banner()).arg(lpMovie->fanart()).arg(lpMovie->poster()).arg(lpMovie->clearart()).arg(lpMovie->disc()).arg(lpMovie->logo()).arg(lpMovie->tmdbID()).arg(lpMovie->imdbID()).arg(lpMovie->title()).arg(lpMovie->originalTitle()).arg(lpMovie->year()).arg(lpMovie->rating()).arg(lpMovie->votes()).arg(lpMovie->top250())
					  .arg(lpMovie->plot()).arg(lpMovie->outline()).arg(lpMovie->tagline()).arg(lpMovie->runtime()).arg(lpMovie->mpaa()).arg(lpMovie->playcount()).arg(lpMovie->set()).arg(lpMovie->sortTitle()).arg(lpMovie->trailer()).arg(lpMovie->watched()).arg(z));

		for(int y = 0;y < lpMovie->creditsList().count();y++)
			database.exec(QString("INSERT INTO moviecredits (\"movieid\", \"credit\", \"sort\") values (%1, \"%2\", %3);")
						  .arg(z).arg(lpMovie->creditsList().at(y)).arg(y));

		for(int y = 0;y < lpMovie->directorList().count();y++)
			database.exec(QString("INSERT INTO moviedirector (\"movieid\", \"director\", \"sort\") values (%1, \"%2\", %3);")
						  .arg(z).arg(lpMovie->directorList().at(y)).arg(y));

		for(int y = 0;y < lpMovie->studioList().count();y++)
			database.exec(QString("INSERT INTO moviestudio (\"movieid\", \"studio\", \"sort\") values (%1, \"%2\", %3);")
						  .arg(z).arg(lpMovie->studioList().at(y)).arg(y));

		for(int y = 0;y < lpMovie->genreList().count();y++)
			database.exec(QString("INSERT INTO moviegenre (\"movieid\", \"genre\", \"sort\") values (%1, \"%2\", %3);")
						  .arg(z).arg(lpMovie->genreList().at(y)).arg(y));

		for(int y = 0;y < lpMovie->countryList().count();y++)
			database.exec(QString("INSERT INTO moviecountry (\"movieid\", \"country\", \"sort\") values (%1, \"%2\", %3);")
						  .arg(z).arg(lpMovie->countryList().at(y)).arg(y));

		for(int y = 0;y < lpMovie->actorList().count();y++)
			database.exec(QString("INSERT INTO movieactor (\"movieid\", \"name\", \"role\", \"thumb\", \"sort\") values (%1, \"%2\", \"%3\", \"%4\", \"%5\");")
						  .arg(z).arg(lpMovie->actorList().at(y).szName).arg(lpMovie->actorList().at(y).szRole).arg(lpMovie->actorList().at(y).szThumb).arg(y));

		for(int y = 0;y < lpMovie->thumbList().count();y++)
			database.exec(QString("INSERT INTO moviethumb (\"movieid\", \"preview\", \"thumb\", \"sort\") values (%1, \"%2\", \"%3\", %4);")
						  .arg(z).arg(lpMovie->thumbList().at(y).szPreview).arg(lpMovie->thumbList().at(y).szThumb).arg(y));

		for(int y = 0;y < lpMovie->fanartList().count();y++)
			database.exec(QString("INSERT INTO moviefanart (\"movieid\", \"preview\", \"thumb\", \"sort\") values (%1, \"%2\", \"%3\", %4);")
						  .arg(z).arg(lpMovie->fanartList().at(y).szPreview).arg(lpMovie->fanartList().at(y).szThumb).arg(y));

		for(int y = 0;y < lpMovie->videoList().count();y++)
			database.exec(QString("INSERT INTO movievideo (\"movieid\", \"aspect\", \"codec\", \"duration\", \"height\", \"scantype\", \"width\", \"sort\") values (%1, %2, \"%3\", %4, %5, \"%6\", %7, %8);")
						  .arg(z).arg(lpMovie->videoList().at(y).dAspect).arg(lpMovie->videoList().at(y).szCodec).arg(lpMovie->videoList().at(y).iDuration).arg(lpMovie->videoList().at(y).iHeight).arg(lpMovie->videoList().at(y).szScantype).arg(lpMovie->videoList().at(y).iWidth).arg(y));

		for(int y = 0;y < lpMovie->audioList().count();y++)
			database.exec(QString("INSERT INTO movieaudio (\"movieid\", \"channels\", \"codec\", \"language\", \"sort\") values (%1, %2, \"%3\", \"%4\", %5);")
						  .arg(z).arg(lpMovie->audioList().at(y).iChannels).arg(lpMovie->audioList().at(y).szCodec).arg(lpMovie->audioList().at(y).szLanguage).arg(y));

		for(int y = 0;y < lpMovie->subtitleList().count();y++)
			database.exec(QString("INSERT INTO moviesubtitle (\"movieid\", \"language\", \"sort\") values (%1, \"%2\", %3);")
						  .arg(z).arg(lpMovie->subtitleList().at(y).szLanguage).arg(y));
	}

	database.driver()->commitTransaction();

	database.close();
#endif
}

void cMovieList::parsePath(const QString& szPath, cXBMC* lpXBMC, QStatusBar* lpStatusBar)
{
	QDir			dir(szPath);
	if(!dir.exists())
		return;

	if(lpStatusBar)
		lpStatusBar->showMessage(QString("%1 %2 ...").arg("scanning").arg(szPath));

	QStringList	dirList	= dir.entryList(QDir::Dirs);
	dirList.removeAll(".");
	dirList.removeAll("..");

	for(int z = 0;z < dirList.count();z++)
		parsePath(QString("%1/%2").arg(szPath).arg(dirList.at(z)), lpXBMC, lpStatusBar);

	QStringList	nfoList	= dir.entryList(QStringList() << "*.nfo", QDir::Files);
	for(int z = 0;z < nfoList.count();z++)
	{
		cMovie*	lpMovie	= new cMovie(szPath, nfoList.at(z));
		if(lpMovie->isValid())
		{
			if(lpXBMC)
			{
				lpMovie->setWatched(lpXBMC->movieWatched(QString("%1/%2").arg(szPath).arg(nfoList.at(z))));
				lpMovie->setAdded(lpXBMC->movieAdded(QString("%1/%2").arg(szPath).arg(nfoList.at(z))));
			}
			append(lpMovie);
			m_szGenreList.append(lpMovie->genreList());
		}
		else
			delete(lpMovie);
	}
}

QStringList cMovieList::genreList()
{
	return(m_szGenreList);
}
