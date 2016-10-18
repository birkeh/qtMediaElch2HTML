#ifndef CXBMC_H
#define CXBMC_H


#include <QStringList>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDateTime>


typedef struct tagLIST
{
	QString		szMovie;
	QDateTime	date;
	bool		bWatched;
}	LIST, *LPLIST;

class cXBMC
{
public:
	cXBMC();

	void			loadMovies(const QString& szServer, const QString& szPort, const QString& szUser, const QString& szPass);
	void			loadTVShows(const QString& szServer, const QString& szPort, const QString& szUser, const QString& szPass);

	bool			movieWatched(const QString& szPath);
	bool			episodeWatched(const QString& szPath);

	QDateTime		movieAdded(const QString& szPath);
	QDateTime		episodeAdded(const QString& szPath);
protected:
	QJsonDocument	getJson(QJsonObject& request, const QString& szServer, const QString& szPort, const QString& szUser, const QString& szPass);

private:
	QList<LIST>		m_MovieList;
	QList<LIST>		m_EpisodeList;
};

#endif // CXBMC_H
