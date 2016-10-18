#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H


#include "cmovie.h"
#include "ctvshow.h"
#include "cpushbutton.h"
#include "cgenrecombine.h"
#include "cxbmc.h"

#include <QMainWindow>
#include <QPushButton>
#include <QList>
#include <QStandardItemModel>
#include <QTextStream>
#include <QProgressBar>
#include <QDateTime>
#include <QItemSelection>


namespace Ui {
class cMainWindow;
}

class HTML
{
public:
	HTML(const QString& str, const QString& html)
	{
		m_str	= str;
		m_html	= html;
	}
	QString	m_str;
	QString	m_html;
};

class cMainWindow : public QMainWindow
{
	Q_OBJECT

	enum Sources
	{
		SourcesNone		= 0,
		SourcesMovie	= 1,
		SourcesTVShow	= 2,
	};

	typedef struct tagSOURCES
	{
		QStandardItem*	lpTypeItem;
		QStandardItem*	lpPathItem;
		cPushButton*	lpButton;
		QModelIndex		index;
		Sources			type;
	}	SOURCES, *LPSOURCES;


public:
	explicit cMainWindow(QWidget *parent = 0);
	~cMainWindow();

protected:
	void				init();
	void				load();

	void				loadSettings();
	void				saveSettings();

	void				addSourcesLine(Sources type, const QString& szText);

	QString				string2HTML(const QString& sz);
	QString				preparePath();
	void				writeHeader(QTextStream& out);
	void				writeFooter(QTextStream& out, const QStringList& szGenreList);
	void				writeMovie(QTextStream& out, const qint16& iMovie, const QString& szTitle, const qint16& iYear, const QString& szTagline, const double& dRating,
								   const QStringList& szDirectorList, const QList<ACTOR>& ActorList, const QStringList& szGenreList, const qint16& iRuntime,
								   const QString& szMPAA, const QString& szPlot, const QList<VIDEO>& videoList, const QList<AUDIO>& audioList, const QString& szIMDBID,
								   const bool& bWatched, const QDateTime& added, const QList<cTVShowEpisode*>& episodeList);
	void				copyImages(const qint16& iMovie, const QString& szDestDir, const QString& szFanart, const QString& szPoster);
	void				copyResource(const QString& szResource, const QString& szPath);

	void				displayMovieList();
private:
	Ui::cMainWindow*	ui;
	QStandardItemModel*	m_lpSourcesModel;
	QStandardItemModel*	m_lpMoviesModel;
	QStandardItemModel*	m_lpTVShowsModel;
	QStandardItemModel*	m_lpGenreFromModel;
	QStandardItemModel*	m_lpGenreToModel;

	QProgressBar*		m_lpStatusProgress;

	QList<SOURCES>		m_SourcesList;
	QString				m_szLastPath;
	QString				m_szLastOutputPath;

	cMovieList			m_MovieList;
	cTVShowList			m_TVShowList;

	QList<HTML>			m_htmlList;

	bool				m_bTVShowCheck;

	cXBMC*				m_lpXBMC;

	cGenreCombineList	m_GenreCombineList;
	bool				m_bGenreToSelecting;

	QStandardItem*		m_lpGenreToItem;
	QStandardItem*		m_lpGenreFromItem;

	void				addGenreList(const QStringList& szGenreList);
	void				showGenres();
	QStandardItem*		selectedGenreTo();
	QStringList			convertGenre(const QStringList& szGenreList);
public slots:
	void				sourcesButtonClicked(cPushButton* lpButton);
private slots:
	void				on_m_lpAddSources_clicked();
	void				on_m_lpDeleteSources_clicked();
	void				on_m_lpMoviesScan_clicked();
	void				on_m_lpTVShowsScan_clicked();
	void				on_m_lpMoviesSelectAll_clicked();
	void				on_m_lpTVShowsSelectAll_clicked();
	void				on_m_lpMoviesDeselectAll_clicked();
	void				on_m_lpTVShowsDeselectAll_clicked();
	void				on_m_lpMoviesInvertSelection_clicked();
	void				on_m_lpTVShowsInvertSelection_clicked();
	void				on_m_lpMoviesClear_clicked();
	void				on_m_lpTVShowsClearList_clicked();
	void				on_m_lpGenerateHTML_clicked();
	void				on_m_lpOutputPathSelect_clicked();

	void				onTVShowsItemChanged(QStandardItem *);
	void				on_m_lpScanAll_clicked();
	void				on_m_lpXBMC_clicked();

	void				on_m_lpGenreToSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);
	void				on_m_lpGenreFromSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);

	void				on_m_lpGenreToPlus_clicked();
	void				on_m_lpGenreToMinus_clicked();
	void				on_m_lpGenreFromPlus_clicked();
	void				on_m_lpGenreFromMinus_clicked();
	void				on_m_lpGenreTo_customContextMenuRequested(const QPoint &pos);
	void				on_m_lpGenreFrom_customContextMenuRequested(const QPoint &pos);
};

#endif // CMAINWINDOW_H
