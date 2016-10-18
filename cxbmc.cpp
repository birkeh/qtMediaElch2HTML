#include "cxbmc.h"

#include <QJsonObject>
#include <QJsonArray>

#include <QNetworkAccessManager>
#include <QNetworkProxyFactory>
#include <QNetworkReply>
#include <QEventLoop>


cXBMC::cXBMC()
{
}

QJsonDocument cXBMC::getJson(QJsonObject& request, const QString& szServer, const QString& szPort, const QString& szUser, const QString& szPass)
{
	QString	szConnectionString	= QString("http://");

	if(!szUser.isEmpty())
	{
		szConnectionString.append(szUser);
		if(!szPass.isEmpty())
			szConnectionString.append(QString(":%1").arg(szPass));
		szConnectionString.append("@");
	}
	szConnectionString.append(QString("%1:%2").arg(szServer).arg(szPort));

	QJsonDocument	doc(request);
	QString			str		= QString(doc.toJson());

	QNetworkAccessManager	m_NetworkMngr;
	QNetworkReply*			reply		= m_NetworkMngr.get(QNetworkRequest(szConnectionString+QString("/jsonrpc?request=")+str.replace("\n", "")));
	QEventLoop              loop;

	QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
	loop.exec();

	QJsonDocument	d	= QJsonDocument::fromJson(reply->readAll());
	delete reply;
	return(d);
}

void cXBMC::loadMovies(const QString& szServer, const QString& szPort, const QString& szUser, const QString& szPass)
{
	m_MovieList.clear();

	QJsonObject				request;
	QJsonObject				params;
	QJsonArray				properties;

	properties.append(QString("file"));
	properties.append(QString("lastplayed"));
	properties.append(QString("dateadded"));

	params["properties"]	= properties;

	request["jsonrpc"]		= QString("2.0");
	request["method"]		= QString("VideoLibrary.GetMovies");
	request["params"]		= params;
	request["id"]			= QString("libMovies");

	QJsonDocument	d		= getJson(request, szServer, szPort, szUser, szPass);

	if(d.isObject())
	{
		QJsonObject	o;
		o	= d.object();
		if(o["result"].isObject())
		{
			QJsonObject	result	= o["result"].toObject();
			if(result["limits"].isObject())
			{
				QJsonObject	limits	= result["limits"].toObject();
				if(limits["total"].isDouble())
				{
					qint16	total	= (qint16)limits["total"].toInt();
					if(total)
					{
						if(result["movies"].isArray())
						{
							QJsonArray	movies	= result["movies"].toArray();
							for(int z = 0;z < movies.count();z++)
							{
								if(movies[z].isObject())
								{
									QJsonObject	movie	= movies[z].toObject();

									LIST	l;
									l.szMovie	= movie["file"].toString();
									l.bWatched	= false;
									l.date		= QDateTime(QDate(1980, 1, 1), QTime(0, 0, 0));

									if(movie["lastplayed"].toString().length())
										l.bWatched	= true;
									if(movie["dateadded"].toString().length())
										l.date		= QDateTime::fromString(movie["dateadded"].toString(), "yyyy-MM-dd HH:mm:ss");

									m_MovieList.append(l);
								}
							}
						}
					}
				}
			}
		}
	}
}

void cXBMC::loadTVShows(const QString& szServer, const QString& szPort, const QString& szUser, const QString& szPass)
{
	m_EpisodeList.clear();

	QJsonObject				request;
	QJsonObject				params;
	QJsonArray				properties;

	properties.append(QString("file"));
	properties.append(QString("lastplayed"));
	properties.append(QString("dateadded"));

	params["properties"]	= properties;

	request["jsonrpc"]		= QString("2.0");
	request["method"]		= QString("VideoLibrary.GetEpisodes");
	request["params"]		= params;
	request["id"]			= QString("libMovies");

	QJsonDocument	d	= getJson(request, szServer, szPort, szUser, szPass);

	if(d.isObject())
	{
		QJsonObject	o;
		o	= d.object();
		if(o["result"].isObject())
		{
			QJsonObject	result	= o["result"].toObject();
			if(result["episodes"].isArray())
			{
				QJsonArray	episodes	= result["episodes"].toArray();
				for(int z = 0;z < episodes.count();z++)
				{
					if(episodes[z].isObject())
					{
						QJsonObject	episode	= episodes[z].toObject();

						LIST	l;
						l.szMovie	= episode["file"].toString();
						l.bWatched	= false;
						l.date		= QDateTime(QDate(1980, 1, 1), QTime(0, 0, 0));

						if(episode["lastplayed"].toString().length())
							l.bWatched	= true;
						if(episode["dateadded"].toString().length())
							l.date		= QDateTime::fromString(episode["dateadded"].toString(), "yyyy-MM-dd HH:mm:ss");

						m_EpisodeList.append(l);
					}
				}
			}
		}
	}
}

bool cXBMC::movieWatched(const QString& szPath)
{
	QString	szPath1	= szPath.left(szPath.lastIndexOf(".")).replace("\\", "/");
	szPath1			= szPath1.mid(szPath1.lastIndexOf("/", szPath1.lastIndexOf("/")-1)+1);

	for(int z = 0;z < m_MovieList.count();z++)
	{
		if(m_MovieList.at(z).szMovie.contains(szPath1, Qt::CaseInsensitive))
			return(m_MovieList.at(z).bWatched);
	}
	return(false);
}

bool cXBMC::episodeWatched(const QString& szPath)
{
	QString	szPath1	= szPath.left(szPath.lastIndexOf(".")).replace("\\", "/");
	szPath1			= szPath1.mid(szPath1.lastIndexOf("/", szPath1.lastIndexOf("/")-1)+1);

	for(int z = 0;z < m_MovieList.count();z++)
	{
		if(m_EpisodeList.at(z).szMovie.contains(szPath1, Qt::CaseInsensitive))
			return(m_EpisodeList.at(z).bWatched);
	}
	return(false);
}

QDateTime cXBMC::movieAdded(const QString& szPath)
{
	QString	szPath1	= szPath.left(szPath.lastIndexOf(".")).replace("\\", "/");
	szPath1			= szPath1.mid(szPath1.lastIndexOf("/", szPath1.lastIndexOf("/")-1)+1);

	for(int z = 0;z < m_MovieList.count();z++)
	{
		if(m_MovieList.at(z).szMovie.contains(szPath1, Qt::CaseInsensitive))
			return(m_MovieList.at(z).date);
	}
	return(QDateTime(QDate(1980, 1, 1), QTime(0, 0, 0)));
}

QDateTime cXBMC::episodeAdded(const QString& szPath)
{
	QString	szPath1	= szPath.left(szPath.lastIndexOf(".")).replace("\\", "/");
	szPath1			= szPath1.mid(szPath1.lastIndexOf("/", szPath1.lastIndexOf("/")-1)+1);

	for(int z = 0;z < m_EpisodeList.count();z++)
	{
		if(m_EpisodeList.at(z).szMovie.contains(szPath1, Qt::CaseInsensitive))
			return(m_EpisodeList.at(z).date);
	}
	return(QDateTime(QDate(1980, 1, 1), QTime(0, 0, 0)));
}
