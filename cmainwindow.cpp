#include "cmainwindow.h"
#include "ui_cmainwindow.h"

#include "csourcescombodelegate.h"
#include "csourcesbuttondelegate.h"

#include <QHeaderView>
#include <QItemSelectionModel>
#include <QMessageBox>
#include <QTableView>
#include <QFileDialog>
#include <QImage>
#include <QInputDialog>

#include <QSettings>


#define	FANART_W	1920
#define FANART_H	1080
#define POSTER_W	 200
#define POSTER_H	 300


cMainWindow::cMainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::cMainWindow)
{
	m_bTVShowCheck		= false;
	m_lpXBMC			= 0;
	m_bGenreToSelecting	= false;

	init();
	load();

	m_htmlList << HTML("&",		"&amp;");
	m_htmlList << HTML("\"",	"&quot;");
	m_htmlList << HTML("<",		"&lt;");
	m_htmlList << HTML(">",		"&gt;");
	m_htmlList << HTML("¡",		"&iexcl;");
	m_htmlList << HTML("¢",		"&cent;");
	m_htmlList << HTML("£",		"&pound;");
	m_htmlList << HTML("¤",		"&curren;");
	m_htmlList << HTML("¥",		"&yen;");
	m_htmlList << HTML("¦",		"&brvbar;");
	m_htmlList << HTML("§",		"&sect;");
	m_htmlList << HTML("¨",		"&uml;");
	m_htmlList << HTML("©",		"&copy;");
	m_htmlList << HTML("ª",		"&ordf;");
	m_htmlList << HTML("«",		"&laquo;");
	m_htmlList << HTML("¬",		"&not;");
	m_htmlList << HTML("­",		"&shy;");
	m_htmlList << HTML("®",		"&reg;");
	m_htmlList << HTML("¯",		"&macr;");
	m_htmlList << HTML("°",		"&deg;");
	m_htmlList << HTML("±",		"&plusmn;");
	m_htmlList << HTML("²",		"&sup2;");
	m_htmlList << HTML("³",		"&sup3;");
	m_htmlList << HTML("´",		"&acute;");
	m_htmlList << HTML("µ",		"&micro;");
	m_htmlList << HTML("¶",		"&para;");
	m_htmlList << HTML("·",		"&middot;");
	m_htmlList << HTML("¸",		"&cedil;");
	m_htmlList << HTML("¹",		"&sup1;");
	m_htmlList << HTML("º",		"&ordm;");
	m_htmlList << HTML("»",		"&raquo;");
	m_htmlList << HTML("¼",		"&frac14;");
	m_htmlList << HTML("½",		"&frac12;");
	m_htmlList << HTML("¾",		"&frac34;");
	m_htmlList << HTML("¿",		"&iquest;");
	m_htmlList << HTML("À",		"&Agrave;");
	m_htmlList << HTML("Á",		"&Aacute;");
	m_htmlList << HTML("Â",		"&Acirc;");
	m_htmlList << HTML("Ã",		"&Atilde;");
	m_htmlList << HTML("Ä",		"&Auml;");
	m_htmlList << HTML("Å",		"&Aring;");
	m_htmlList << HTML("Æ",		"&AElig;");
	m_htmlList << HTML("Ç",		"&Ccedil;");
	m_htmlList << HTML("È",		"&Egrave;");
	m_htmlList << HTML("É",		"&Eacute;");
	m_htmlList << HTML("Ê",		"&Ecirc;");
	m_htmlList << HTML("Ë",		"&Euml;");
	m_htmlList << HTML("Ì",		"&Igrave;");
	m_htmlList << HTML("Í",		"&Iacute;");
	m_htmlList << HTML("Î",		"&Icirc;");
	m_htmlList << HTML("Ï",		"&Iuml;");
	m_htmlList << HTML("Ð",		"&ETH;");
	m_htmlList << HTML("Ñ",		"&Ntilde;");
	m_htmlList << HTML("Ò",		"&Ograve;");
	m_htmlList << HTML("Ó",		"&Oacute;");
	m_htmlList << HTML("Ô",		"&Ocirc;");
	m_htmlList << HTML("Õ",		"&Otilde;");
	m_htmlList << HTML("Ö",		"&Ouml;");
	m_htmlList << HTML("×",		"&times;");
	m_htmlList << HTML("Ø",		"&Oslash;");
	m_htmlList << HTML("Ù",		"&Ugrave;");
	m_htmlList << HTML("Ú",		"&Uacute;");
	m_htmlList << HTML("Û",		"&Ucirc;");
	m_htmlList << HTML("Ü",		"&Uuml;");
	m_htmlList << HTML("Ý",		"&Yacute;");
	m_htmlList << HTML("Þ",		"&THORN;");
	m_htmlList << HTML("ß",		"&szlig;");
	m_htmlList << HTML("à",		"&agrave;");
	m_htmlList << HTML("á",		"&aacute;");
	m_htmlList << HTML("â",		"&acirc;");
	m_htmlList << HTML("ã",		"&atilde;");
	m_htmlList << HTML("ä",		"&auml;");
	m_htmlList << HTML("å",		"&aring;");
	m_htmlList << HTML("æ",		"&aelig;");
	m_htmlList << HTML("ç",		"&ccedil;");
	m_htmlList << HTML("è",		"&egrave;");
	m_htmlList << HTML("é",		"&eacute;");
	m_htmlList << HTML("ê",		"&ecirc;");
	m_htmlList << HTML("ë",		"&euml;");
	m_htmlList << HTML("ì",		"&igrave;");
	m_htmlList << HTML("í",		"&iacute;");
	m_htmlList << HTML("î",		"&icirc;");
	m_htmlList << HTML("ï",		"&iuml;");
	m_htmlList << HTML("ð",		"&eth;");
	m_htmlList << HTML("ñ",		"&ntilde;");
	m_htmlList << HTML("ò",		"&ograve;");
	m_htmlList << HTML("ó",		"&oacute;");
	m_htmlList << HTML("ô",		"&ocirc;");
	m_htmlList << HTML("õ",		"&otilde;");
	m_htmlList << HTML("ö",		"&ouml;");
	m_htmlList << HTML("÷",		"&divide;");
	m_htmlList << HTML("ø",		"&oslash;");
	m_htmlList << HTML("ù",		"&ugrave;");
	m_htmlList << HTML("ú",		"&uacute;");
	m_htmlList << HTML("û",		"&ucirc;");
	m_htmlList << HTML("ü",		"&uuml;");
	m_htmlList << HTML("ý",		"&yacute;");
	m_htmlList << HTML("þ",		"&thorn;");
	m_htmlList << HTML("ÿ",		"&yuml;");
}

cMainWindow::~cMainWindow()
{
	saveSettings();

	if(m_lpSourcesModel)
		delete m_lpSourcesModel;

	if(m_lpStatusProgress)
		delete m_lpStatusProgress;

	if(m_lpXBMC)
		delete m_lpXBMC;

	if(m_lpGenreFromModel)
		delete m_lpGenreFromModel;

	if(m_lpGenreToModel)
		delete m_lpGenreToModel;

	delete ui;
}

void cMainWindow::init()
{
	ui->setupUi(this);

	m_lpSourcesModel			= new QStandardItemModel(0, 3);
	QStringList	headerLabels	= QStringList() << tr("Type") << tr("Path") << tr("");
	m_lpSourcesModel->setHorizontalHeaderLabels(headerLabels);
	ui->m_lpSources->setModel(m_lpSourcesModel);

	m_lpMoviesModel				= new QStandardItemModel(0, 1);
	ui->m_lpMovies->setModel(m_lpMoviesModel);

	m_lpTVShowsModel			= new QStandardItemModel(0, 1);
	ui->m_lpTVShows->setModel(m_lpTVShowsModel);

	m_lpGenreFromModel			= new QStandardItemModel(0, 1);
	ui->m_lpGenreFrom->setModel(m_lpGenreFromModel);

	m_lpGenreToModel			= new QStandardItemModel(0, 1);
	ui->m_lpGenreTo->setModel(m_lpGenreToModel);

	cSourcesComboDelegate*	lpSourcesComboDelegate	= new cSourcesComboDelegate(this);
	ui->m_lpSources->setItemDelegateForColumn(0, lpSourcesComboDelegate);
	ui->m_lpSources->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
	ui->m_lpSources->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);

	ui->m_lpSources->resizeColumnToContents(0);
	ui->m_lpSources->setColumnWidth(2, 25);
	ui->m_lpSources->resizeRowsToContents();

	ui->m_lpMainTab->setCurrentIndex(0);

	m_lpStatusProgress  = new QProgressBar(this);
	m_lpStatusProgress->setMinimum(0);
	m_lpStatusProgress->setMaximum(100);
	ui->m_lpStatusBar->addPermanentWidget(m_lpStatusProgress);
	m_lpStatusProgress->hide();

	loadSettings();

	connect(m_lpTVShowsModel, SIGNAL(itemChanged(QStandardItem*)), SLOT(onTVShowsItemChanged(QStandardItem*)));
	connect(ui->m_lpGenreTo->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(on_m_lpGenreToSelectionChanged(QItemSelection,QItemSelection)));
	connect(ui->m_lpGenreFrom->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(on_m_lpGenreFromSelectionChanged(QItemSelection,QItemSelection)));
}

void cMainWindow::load()
{
	m_lpXBMC	= new cXBMC;
#ifdef WITHDB
	m_MovieList.open();
	displayMovieList();
#endif
}

void cMainWindow::loadSettings()
{
	QSettings	settings;

	for(int z = 0;;z++)
	{
		Sources	t;
		QString	szPath;

		t		= (Sources)settings.value(QString("sources/type%1").arg(z)).toInt();
		szPath	= settings.value(QString("sources/path%1").arg(z)).toString();
		if(szPath.isEmpty())
			break;
		addSourcesLine(t, szPath);
	}

	QString	szOutputPath	= settings.value("output/path").toString();
	ui->m_lpOutputPath->setText(szOutputPath);

	qint16	iMaxActor		= settings.value("output/maxactor", QVariant((int)15)).toInt();
	ui->m_lpMaxActor->setValue(iMaxActor);

	QString	szTitle			= settings.value("output/title", "Movie Collection").toString();
	ui->m_lpTitle->setText(szTitle);

	if(settings.value("xbmc/enabled", "false").toBool() == true)
		ui->m_lpXBMC->setChecked(true);
	else
		ui->m_lpXBMC->setChecked(false);

	ui->m_lpXBMCServer->setText(settings.value("xbmc/server").toString());
	ui->m_lpXBMCPort->setText(settings.value("xbmc/port").toString());
	ui->m_lpXBMCUser->setText(settings.value("xbmc/user").toString());
	ui->m_lpXBMCPass->setText(settings.value("xbmc/pass").toString());

	ui->m_lpPosterW->setValue(settings.value("poster/w", QVariant(POSTER_W)).toInt());
	ui->m_lpPosterH->setValue(settings.value("poster/h", QVariant(POSTER_H)).toInt());
	ui->m_lpFanartW->setValue(settings.value("fanart/w", QVariant(FANART_W)).toInt());
	ui->m_lpFanartH->setValue(settings.value("fanart/h", QVariant(FANART_H)).toInt());

	QStringList	genres;
	settings.beginGroup("genre");
	genres	= settings.allKeys();
	settings.endGroup();

	genres.removeDuplicates();
	genres.sort(Qt::CaseInsensitive);

	for(int z = 0;z < genres.count();z++)
	{
		cGenreCombine*	lpGenreCombine	= m_GenreCombineList.add(genres.at(z));
		if(lpGenreCombine)
			lpGenreCombine->addFrom(settings.value(QString("genre/%1").arg(genres.at(z))).toStringList());
	}
	showGenres();
}

bool genreCompare(cGenreCombine* g1, cGenreCombine* g2)
{
	return(g1->genreTo() < g2->genreTo());
}

void cMainWindow::showGenres()
{
	m_lpGenreToModel->clear();
	m_lpGenreFromModel->clear();

	qSort(m_GenreCombineList.begin(), m_GenreCombineList.end(), genreCompare);

	QStringList	szGenresFrom;
	for(int z = 0;z < m_GenreCombineList.count();z++)
	{
		QStandardItem*	lpItem	= new QStandardItem(m_GenreCombineList.at(z)->genreTo());
		m_lpGenreToModel->appendRow(lpItem);
		QModelIndex		iIndex	= m_lpGenreToModel->indexFromItem(lpItem);
		m_lpGenreToModel->setData(iIndex, QVariant::fromValue(m_GenreCombineList.at(z)), Qt::UserRole);
		szGenresFrom.append(m_GenreCombineList.at(z)->genreFrom());
	}

	szGenresFrom.removeDuplicates();
	szGenresFrom.sort(Qt::CaseInsensitive);
	for(int z = 0;z < szGenresFrom.count();z++)
	{
		QStandardItem*	lpItem	= new QStandardItem(szGenresFrom.at(z));
		m_lpGenreFromModel->appendRow(lpItem);
	}
}

void cMainWindow::saveSettings()
{
	QSettings	settings;

	settings.remove("sources");
	settings.remove("output");
	settings.remove("xbmc");
	settings.remove("poster");
	settings.remove("fanart");
	settings.remove("genre");

	int	i	= 0;
	for(int z = 0;z < m_lpSourcesModel->rowCount();z++)
	{
		QStandardItem*	type	= m_lpSourcesModel->item(z, 0);
		QStandardItem*	path	= m_lpSourcesModel->item(z, 1);

		Sources			t		= (Sources)type->data(Qt::EditRole).toInt();
		QString			szPath	= path->text();

		if(!szPath.isEmpty())
		{
			settings.setValue(QString("sources/type%1").arg(i), QVariant(t));
			settings.setValue(QString("sources/path%1").arg(i), QVariant(szPath));
			i++;
		}
	}

	settings.setValue("output/path", QVariant(ui->m_lpOutputPath->text()));
	settings.setValue("output/maxactor", QVariant(ui->m_lpMaxActor->value()));
	settings.setValue("output/title", QVariant(ui->m_lpTitle->text()));

	settings.setValue("xbmc/enabled", QVariant(ui->m_lpXBMC->isChecked()));
	settings.setValue("xbmc/server", QVariant(ui->m_lpXBMCServer->text()));
	settings.setValue("xbmc/port", QVariant(ui->m_lpXBMCPort->text()));
	settings.setValue("xbmc/user", QVariant(ui->m_lpXBMCUser->text()));
	settings.setValue("xbmc/pass", QVariant(ui->m_lpXBMCPass->text()));

	settings.setValue("poster/w", QVariant(ui->m_lpPosterW->value()));
	settings.setValue("poster/h", QVariant(ui->m_lpPosterH->value()));
	settings.setValue("fanart/w", QVariant(ui->m_lpFanartW->value()));
	settings.setValue("fanart/h", QVariant(ui->m_lpFanartH->value()));

	for(int z = 0;z < m_GenreCombineList.count();z++)
	{
		cGenreCombine* lpGenreCombine	= m_GenreCombineList.at(z);
		settings.setValue(QString("genre/%1").arg(lpGenreCombine->genreTo()), QVariant(lpGenreCombine->genreFrom()));
	}
}

void cMainWindow::addSourcesLine(Sources type, const QString& szText)
{
	SOURCES	s;

	s.lpTypeItem	= new QStandardItem;
	m_lpSourcesModel->appendRow(s.lpTypeItem);
	s.index			= m_lpSourcesModel->indexFromItem(s.lpTypeItem);
	m_lpSourcesModel->setData(s.index, QVariant((int)type));
	ui->m_lpSources->openPersistentEditor(s.index);
	s.lpPathItem	= new QStandardItem(szText);
	m_lpSourcesModel->setItem(s.index.row(), 1, s.lpPathItem);
	s.lpButton		= new cPushButton("...");
	ui->m_lpSources->setIndexWidget(m_lpSourcesModel->index(s.index.row(), 2), s.lpButton);

	m_SourcesList.append(s);

	connect(s.lpButton, SIGNAL(buttonClicked(cPushButton*)), this, SLOT(sourcesButtonClicked(cPushButton*)));

	ui->m_lpSources->resizeColumnToContents(0);
	ui->m_lpSources->setColumnWidth(2, 25);
	ui->m_lpSources->resizeRowsToContents();
}

void cMainWindow::sourcesButtonClicked(cPushButton* lpButton)
{
	for(int z = 0;z < m_SourcesList.count();z++)
	{
		if(lpButton == m_SourcesList.at(z).lpButton)
		{
			QString		szPath		= m_SourcesList.at(z).lpPathItem->text();
			if(szPath.isEmpty())
				szPath	= m_szLastPath;

			QFileDialog	dlg(this, "select Path", szPath);
			dlg.setFileMode(QFileDialog::Directory);
			if(dlg.exec())
			{
				QString	szNewPath	= dlg.selectedFiles().at(0);
				if(szNewPath.isEmpty())
					return;
				m_SourcesList.at(z).lpPathItem->setText(szNewPath);
				m_szLastPath	= szNewPath;
			}
		}
	}
}

void cMainWindow::on_m_lpAddSources_clicked()
{
	addSourcesLine(SourcesNone, "");
}

void cMainWindow::on_m_lpDeleteSources_clicked()
{
	if(ui->m_lpSources->selectionModel()->selectedIndexes().count() == 0)
		return;

	for(int z = 0;z < ui->m_lpSources->selectionModel()->selectedIndexes().count();z++)
	{
		QModelIndex	i	= ui->m_lpSources->selectionModel()->selectedIndexes().at(z);
		m_lpSourcesModel->removeRow(i.row());
	}
}

void cMainWindow::on_m_lpMoviesScan_clicked()
{
	if(ui->m_lpXBMC->isChecked())
	{
		ui->m_lpStatusBar->showMessage(QString("Loading XBMC ..."));
		m_lpXBMC->loadMovies(ui->m_lpXBMCServer->text(), ui->m_lpXBMCPort->text(), ui->m_lpXBMCUser->text(), ui->m_lpXBMCPass->text());
	}

	m_lpMoviesModel->clear();
	m_MovieList.clear();

	for(int z = 0;z < m_lpSourcesModel->rowCount();z++)
	{
		QStandardItem*	type	= m_lpSourcesModel->item(z, 0);
		QStandardItem*	path	= m_lpSourcesModel->item(z, 1);

		Sources			t		= (Sources)type->data(Qt::EditRole).toInt();
		QString			szPath	= path->text();

		if(!szPath.isEmpty() && t == SourcesMovie)
			m_MovieList.parse(szPath, m_lpXBMC, ui->m_lpStatusBar);
	}
	ui->m_lpStatusBar->showMessage(QString(""));

	displayMovieList();

	QStringList	szGenreList	= m_MovieList.genreList();
	addGenreList(szGenreList);
	showGenres();
}

bool tvshowSort(cTVShow* left, cTVShow* right)
{
	return(left->title() < right->title());
}

void cMainWindow::on_m_lpTVShowsScan_clicked()
{
	if(ui->m_lpXBMC->isChecked())
	{
		ui->m_lpStatusBar->showMessage(QString("Loading XBMC ..."));
		m_lpXBMC->loadTVShows(ui->m_lpXBMCServer->text(), ui->m_lpXBMCPort->text(), ui->m_lpXBMCUser->text(), ui->m_lpXBMCPass->text());
	}

	m_bTVShowCheck	= true;

	m_lpTVShowsModel->clear();
	m_TVShowList.clear();

	for(int z = 0;z < m_lpSourcesModel->rowCount();z++)
	{
		QStandardItem*	type	= m_lpSourcesModel->item(z, 0);
		QStandardItem*	path	= m_lpSourcesModel->item(z, 1);

		Sources			t		= (Sources)type->data(Qt::EditRole).toInt();
		QString			szPath	= path->text();

		if(!szPath.isEmpty() && t == SourcesTVShow)
			m_TVShowList.parse(szPath, m_lpXBMC, ui->m_lpStatusBar);
	}
	ui->m_lpStatusBar->showMessage(QString(""));

	qSort(m_TVShowList.begin(), m_TVShowList.end(), tvshowSort);

	for(int z = 0;z < m_TVShowList.count();z++)
	{
		cTVShow*	lpTVShow	= m_TVShowList.at(z);

		QStandardItem*	lpTVShowItem	= new QStandardItem(lpTVShow->title());
		lpTVShowItem->setCheckable(true);
		lpTVShowItem->setCheckState(Qt::Checked);
		QVariant	v	= qVariantFromValue(lpTVShow);
		lpTVShowItem->setData(v, Qt::UserRole);
		m_lpTVShowsModel->appendRow(lpTVShowItem);

		qint16			iSeason		= -999;
		QStandardItem*	lpSeasonItem	= 0;

		for(int z = 0;z < lpTVShow->episodeList().count();z++)
		{
			cTVShowEpisode*	lpEpisode	= lpTVShow->episodeList().at(z);
			if(lpEpisode->season() != iSeason)
			{
				lpSeasonItem	= new QStandardItem(QString(tr("Season %1").arg(lpEpisode->season())));
				lpSeasonItem->setCheckable(true);
				lpSeasonItem->setCheckState(Qt::Checked);
				lpTVShowItem->appendRow(lpSeasonItem);
				iSeason			= lpEpisode->season();
			}
			QStandardItem*	lpItem	= new QStandardItem(QString("%1 - %2").arg(lpEpisode->episode(), 2, 10, QChar('0')).arg(lpEpisode->title()));
			lpItem->setCheckable(true);
			lpItem->setCheckState(Qt::Checked);
			lpSeasonItem->appendRow(lpItem);
		}
	}

	QStringList	szGenreList	= m_TVShowList.genreList();
	addGenreList(szGenreList);
	showGenres();

	m_bTVShowCheck = false;
}

void cMainWindow::on_m_lpMoviesSelectAll_clicked()
{
	for(int z = 0;z < m_lpMoviesModel->rowCount();z++)
	{
		QStandardItem*	lpItem	= m_lpMoviesModel->item(z, 0);
		lpItem->setCheckState(Qt::Checked);
	}
}

void cMainWindow::on_m_lpTVShowsSelectAll_clicked()
{
	m_bTVShowCheck	= true;

	for(int z = 0;z < m_lpTVShowsModel->rowCount();z++)
	{
		QStandardItem*	lpItem	= m_lpTVShowsModel->item(z, 0);
		lpItem->setCheckState(Qt::Checked);

		for(int y = 0;y < lpItem->rowCount();y++)
		{
			QStandardItem*	lpSeasonItem	= lpItem->child(y, 0);
			lpSeasonItem->setCheckState(Qt::Checked);

			for(int x = 0;x < lpSeasonItem->rowCount();x++)
			{
				QStandardItem*	lpEpisodeItem	= lpSeasonItem->child(x, 0);
				lpEpisodeItem->setCheckState(Qt::Checked);
			}
		}
	}

	m_bTVShowCheck	= false;
}

void cMainWindow::on_m_lpMoviesDeselectAll_clicked()
{
	for(int z = 0;z < m_lpMoviesModel->rowCount();z++)
	{
		QStandardItem*	lpItem	= m_lpMoviesModel->item(z, 0);
		lpItem->setCheckState(Qt::Unchecked);
	}
}

void cMainWindow::on_m_lpTVShowsDeselectAll_clicked()
{
	m_bTVShowCheck	= true;

	for(int z = 0;z < m_lpTVShowsModel->rowCount();z++)
	{
		QStandardItem*	lpItem	= m_lpTVShowsModel->item(z, 0);
		lpItem->setCheckState(Qt::Unchecked);

		for(int y = 0;y < lpItem->rowCount();y++)
		{
			QStandardItem*	lpSeasonItem	= lpItem->child(y, 0);
			lpSeasonItem->setCheckState(Qt::Unchecked);

			for(int x = 0;x < lpSeasonItem->rowCount();x++)
			{
				QStandardItem*	lpEpisodeItem	= lpSeasonItem->child(x, 0);
				lpEpisodeItem->setCheckState(Qt::Unchecked);
			}
		}
	}

	m_bTVShowCheck	= false;
}

void cMainWindow::on_m_lpMoviesInvertSelection_clicked()
{
	for(int z = 0;z < m_lpMoviesModel->rowCount();z++)
	{
		QStandardItem*	lpItem	= m_lpMoviesModel->item(z, 0);
		if(lpItem->checkState() == Qt::Unchecked)
			lpItem->setCheckState(Qt::Checked);
		else
			lpItem->setCheckState(Qt::Unchecked);
	}
}

void cMainWindow::on_m_lpTVShowsInvertSelection_clicked()
{
	m_bTVShowCheck	= true;

	for(int z = 0;z < m_lpTVShowsModel->rowCount();z++)
	{
		QStandardItem*	lpItem	= m_lpTVShowsModel->item(z, 0);
		bool			bSeasonChecked	= true;

		for(int y = 0;y < lpItem->rowCount();y++)
		{
			QStandardItem*	lpSeasonItem	= lpItem->child(y, 0);
			bool			bEpisodeChecked	= true;

			for(int x = 0;x < lpSeasonItem->rowCount();x++)
			{
				QStandardItem*	lpEpisodeItem	= lpSeasonItem->child(x, 0);
				if(lpEpisodeItem->checkState() == Qt::Checked)
				{
					bEpisodeChecked	= false;
					lpEpisodeItem->setCheckState(Qt::Unchecked);
				}
				else
					lpEpisodeItem->setCheckState(Qt::Checked);
			}
			if(bEpisodeChecked)
				lpSeasonItem->setCheckState(Qt::Checked);
			else
			{
				bSeasonChecked	= false;
				lpSeasonItem->setCheckState(Qt::Unchecked);
			}
		}
		if(bSeasonChecked)
			lpItem->setCheckState(Qt::Checked);
		else
			lpItem->setCheckState(Qt::Unchecked);
	}

	m_bTVShowCheck	= false;
}

void cMainWindow::on_m_lpMoviesClear_clicked()
{
	m_lpMoviesModel->clear();
}

void cMainWindow::on_m_lpTVShowsClearList_clicked()
{
	m_bTVShowCheck = true;
	m_lpTVShowsModel->clear();
	m_bTVShowCheck = false;
}

QString cMainWindow::string2HTML(const QString& sz)
{
	QString	sz1	= sz;

	for(int z = 0;z < m_htmlList.count();z++)
		sz1	= sz1.replace(m_htmlList.at(z).m_str, m_htmlList.at(z).m_html);

	return(sz1);
}

void cMainWindow::copyResource(const QString& szResource, const QString& szPath)
{
	QFile::copy(QString(":/template/%1").arg(szResource), QString("%1/%2").arg(szPath).arg(szResource));
}

QString cMainWindow::preparePath()
{
	if(ui->m_lpOutputPath->text().isEmpty())
		return("");

	QString		szDestDir	= ui->m_lpOutputPath->text();
	QDir		dir(szDestDir);

	dir.removeRecursively();
	dir.mkpath(szDestDir);

	dir.setCurrent(QString("%1/poster").arg(szDestDir));
	dir.removeRecursively();
	dir.setCurrent(QString("%1/fanart").arg(szDestDir));
	dir.removeRecursively();

	dir.mkpath(szDestDir);
	dir.mkpath(QString("%1/poster").arg(szDestDir));
	dir.mkpath(QString("%1/fanart").arg(szDestDir));
	dir.mkpath(QString("%1/Flags").arg(szDestDir));
	dir.mkpath(QString("%1/images").arg(szDestDir));

	copyResource("function.js", szDestDir);
	copyResource("style.css", szDestDir);
	copyResource("Flags/a_aac.png", szDestDir);
	copyResource("Flags/achan_1.png", szDestDir);
	copyResource("Flags/achan_2.png", szDestDir);
	copyResource("Flags/achan_6.png", szDestDir);
	copyResource("Flags/achan_7.png", szDestDir);
	copyResource("Flags/achan_8.png", szDestDir);
	copyResource("Flags/achan_defaultsound.png", szDestDir);
	copyResource("Flags/acodec_a_vorbis.png", szDestDir);
	copyResource("Flags/acodec_aac.png", szDestDir);
	copyResource("Flags/acodec_ac3.png", szDestDir);
	copyResource("Flags/acodec_aif.png", szDestDir);
	copyResource("Flags/acodec_aifc.png", szDestDir);
	copyResource("Flags/acodec_aiff.png", szDestDir);
	copyResource("Flags/acodec_ape.png", szDestDir);
	copyResource("Flags/acodec_dca.png", szDestDir);
	copyResource("Flags/acodec_dd.png", szDestDir);
	copyResource("Flags/acodec_defaultsound.png", szDestDir);
	copyResource("Flags/acodec_dolbydigital.png", szDestDir);
	copyResource("Flags/acodec_dts.png", szDestDir);
	copyResource("Flags/acodec_dtshd_hra.png", szDestDir);
	copyResource("Flags/acodec_dtshd_ma.png", szDestDir);
	copyResource("Flags/acodec_dtshr.png", szDestDir);
	copyResource("Flags/acodec_dtsma.png", szDestDir);
	copyResource("Flags/acodec_flac.png", szDestDir);
	copyResource("Flags/acodec_mp1.png", szDestDir);
	copyResource("Flags/acodec_mp2.png", szDestDir);
	copyResource("Flags/acodec_mp3.png", szDestDir);
	copyResource("Flags/acodec_ogg.png", szDestDir);
	copyResource("Flags/acodec_truehd.png", szDestDir);
	copyResource("Flags/acodec_vorbis.png", szDestDir);
	copyResource("Flags/acodec_wma.png", szDestDir);
	copyResource("Flags/acodec_wmahd.png", szDestDir);
	copyResource("Flags/acodec_wmapro.png", szDestDir);
	copyResource("Flags/vcodec_3iv2.png", szDestDir);
	copyResource("Flags/vcodec_3ivd.png", szDestDir);
	copyResource("Flags/vcodec_3ivx.png", szDestDir);
	copyResource("Flags/vcodec_8bps.png", szDestDir);
	copyResource("Flags/vcodec_advj.png", szDestDir);
	copyResource("Flags/vcodec_avc.png", szDestDir);
	copyResource("Flags/vcodec_avc1.png", szDestDir);
	copyResource("Flags/vcodec_avrn.png", szDestDir);
	copyResource("Flags/vcodec_dca.png", szDestDir);
	copyResource("Flags/vcodec_defaultscreen.png", szDestDir);
	copyResource("Flags/vcodec_div1.png", szDestDir);
	copyResource("Flags/vcodec_div2.png", szDestDir);
	copyResource("Flags/vcodec_div3.png", szDestDir);
	copyResource("Flags/vcodec_div4.png", szDestDir);
	copyResource("Flags/vcodec_div5.png", szDestDir);
	copyResource("Flags/vcodec_div6.png", szDestDir);
	copyResource("Flags/vcodec_divx.png", szDestDir);
	copyResource("Flags/vcodec_dm4v.png", szDestDir);
	copyResource("Flags/vcodec_dx50.png", szDestDir);
	copyResource("Flags/vcodec_em2v.png", szDestDir);
	copyResource("Flags/vcodec_flv.png", szDestDir);
	copyResource("Flags/vcodec_geox.png", szDestDir);
	copyResource("Flags/vcodec_h264.png", szDestDir);
	copyResource("Flags/vcodec_lmp2.png", szDestDir);
	copyResource("Flags/vcodec_m4s2.png", szDestDir);
	copyResource("Flags/vcodec_mmes.png", szDestDir);
	copyResource("Flags/vcodec_mp4.png", szDestDir);
	copyResource("Flags/vcodec_mpeg.png", szDestDir);
	copyResource("Flags/vcodec_mpeg2.png", szDestDir);
	copyResource("Flags/vcodec_mpeg-2.png", szDestDir);
	copyResource("Flags/vcodec_mpeg4.png", szDestDir);
	copyResource("Flags/vcodec_mpeg-4.png", szDestDir);
	copyResource("Flags/vcodec_nds.png", szDestDir);
	copyResource("Flags/vcodec_ndx.png", szDestDir);
	copyResource("Flags/vcodec_pim1.png", szDestDir);
	copyResource("Flags/vcodec_pvmm.png", szDestDir);
	copyResource("Flags/vcodec_qt.png", szDestDir);
	copyResource("Flags/vcodec_rle.png", szDestDir);
	copyResource("Flags/vcodec_rpza.png", szDestDir);
	copyResource("Flags/vcodec_smc.png", szDestDir);
	copyResource("Flags/vcodec_sv10.png", szDestDir);
	copyResource("Flags/vcodec_svq.png", szDestDir);
	copyResource("Flags/vcodec_vc1.png", szDestDir);
	copyResource("Flags/vcodec_wmv.png", szDestDir);
	copyResource("Flags/vcodec_wmva.png", szDestDir);
	copyResource("Flags/vcodec_wvc1.png", szDestDir);
	copyResource("Flags/vcodec_xvid.png", szDestDir);
	copyResource("Flags/vcodec_xvix.png", szDestDir);
	copyResource("Flags/vcodec_zygo.png", szDestDir);
	copyResource("Flags/vres_480.png", szDestDir);
	copyResource("Flags/vres_480i.png", szDestDir);
	copyResource("Flags/vres_480p.png", szDestDir);
	copyResource("Flags/vres_540.png", szDestDir);
	copyResource("Flags/vres_540i.png", szDestDir);
	copyResource("Flags/vres_540p.png", szDestDir);
	copyResource("Flags/vres_576.png", szDestDir);
	copyResource("Flags/vres_576i.png", szDestDir);
	copyResource("Flags/vres_576p.png", szDestDir);
	copyResource("Flags/vres_720.png", szDestDir);
	copyResource("Flags/vres_720i.png", szDestDir);
	copyResource("Flags/vres_720p.png", szDestDir);
	copyResource("Flags/vres_768.png", szDestDir);
	copyResource("Flags/vres_768i.png", szDestDir);
	copyResource("Flags/vres_768p.png", szDestDir);
	copyResource("Flags/vres_1080.png", szDestDir);
	copyResource("Flags/vres_1080i.png", szDestDir);
	copyResource("Flags/vres_1080p.png", szDestDir);
	copyResource("Flags/vres_defaultscreen.png", szDestDir);
	copyResource("Flags/vres_sd.png", szDestDir);
	copyResource("Flags/vres_sdi.png", szDestDir);
	copyResource("Flags/vres_sdp.png", szDestDir);
	copyResource("Flags/vsource_bluray.png", szDestDir);
	copyResource("Flags/vsource_defaultscreen.png", szDestDir);
	copyResource("Flags/vsource_dvd.png", szDestDir);
	copyResource("Flags/vsource_hddvd.png", szDestDir);
	copyResource("Flags/vsource_hdtv.png", szDestDir);
	copyResource("Flags/vsource_sdtv.png", szDestDir);
	copyResource("images/default.jpg", szDestDir);
	copyResource("images/dvd.jpg", szDestDir);
	copyResource("images/haken.png", szDestDir);
	copyResource("images/hd.jpg", szDestDir);
	copyResource("images/imdb.jpg", szDestDir);
	copyResource("images/movie_folder.png", szDestDir);
	copyResource("images/youtube.jpg", szDestDir);

	return(szDestDir);
}

void cMainWindow::writeHeader(QTextStream &out)
{
	QStringList	szHeader;

	szHeader << "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">";
	szHeader << "<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\" lang=\"en\">";
	szHeader << "";
	szHeader << "<head>";
	szHeader << "<meta http-equiv=\"content-type\" content=\"text/html; charset=iso-8859-1\" />";
	szHeader << "<title>:.%PAGETITLE%.:</title>";
	szHeader << "<meta name=\"description\" content=\"Personal Movie Collection\" />";
	szHeader << "<meta name=\"keywords\" content=\"Movie Collection, Database, Movies, Collection\" />";
	szHeader << "<meta http-equiv=\"Content-Script-Type\" content=\"text/javascript\" />";
	szHeader << "<link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\" media=\"screen\" />";
	szHeader << "<script src=\"function.js\" type=\"text/javascript\"></script>";
	szHeader << "<script type=\"text/javascript\">";
	szHeader << "function navigate(e) {";
	szHeader << "    ";
	szHeader << "if(event.keyCode == 13)  //enter pressed";
	szHeader << "   ";
	szHeader << "    doSend();";
	szHeader << "  }";
	szHeader << "  ";
	szHeader << "document.onkeypress=navigate;";
	szHeader << "";
	szHeader << "function doSend()";
	szHeader << "{";
	szHeader << "func_SearchActor(document.getElementById('actorform').value);";
	szHeader << "}";
	szHeader << "</script>";
	szHeader << "</head>";
	szHeader << "";
	szHeader << "<body>";
	szHeader << "	";
	szHeader << "";

	for(int z = 0;z < szHeader.count();z++)
	{
		QString	sz	= szHeader.at(z);

		if(szHeader.at(z).contains("%PAGETITLE%", Qt::CaseInsensitive))
			sz	= sz.replace("%PAGETITLE%", ui->m_lpTitle->text(), Qt::CaseInsensitive);

		out << sz << "\n";
	}
}

void cMainWindow::writeFooter(QTextStream &out, const QStringList& szGenreList)
{
	QStringList	szFooter1;
	QStringList	szFooter2;
	QStringList	szFooterGenre;

	szFooter1 << "<!-- NAVIGATION L -->";
	szFooter1 << "<div id=\"sidebar-left\">";
	szFooter1 << "<div id=\"navigation\" class=\"paddingwrap\">";
	szFooter1 << "";
	szFooter1 << "</div></div>";
	szFooter1 << "<!-- NAVIGATION L END -->";
	szFooter1 << "";
	szFooter1 << "<!-- NAVIGATION R -->";
	szFooter1 << "<div id=\"sidebar-right\">";
	szFooter1 << "<div class=\"paddingwrap\">";
	szFooter1 << "<div id=\"div_moviewall\"></div>";
	szFooter1 << "</div></div>";
	szFooter1 << "<!-- NAVIGATION R END -->";
	szFooter1 << "";
	szFooter1 << "<!-- HEADER -->";
	szFooter1 << "<div id=\"header\">";
	szFooter1 << "<p><h1 style= \"display:inline-block;\"  id=\"pagetitleh1\">%PAGETITLE%</h1><div style= \"float:right;margin-top:10px;\" id=\"div_displaylastmodified\"></div></p>";
	szFooter1 << "<div style= \"float:right;\" id=\"div_moviecounter\"></div>";
	szFooter1 << "<!--<h2 id=\"pagetitleh2\">...and still growing</h2>-->";
	szFooter1 << "<br />";
	szFooter1 << "";
	szFooter1 << "<div class=\"headernavigation\">";
	szFooter1 << "<ul class=\"nav\">";
	szFooter1 << "<li><a class=\"menu2\" href=\"javascript:{}\" onclick=\"func_SearchTitel('^[^A-Za-z]')\">#</a></li>";
	szFooter1 << "<li><a class=\"menu2\" href=\"javascript:{}\" onclick=\"func_SearchTitel('^[A|a]')\">A</a></li>";
	szFooter1 << "<li><a class=\"menu2\" href=\"javascript:{}\" onclick=\"func_SearchTitel('^[B|b]')\">B</a></li>";
	szFooter1 << "<li><a class=\"menu2\" href=\"javascript:{}\" onclick=\"func_SearchTitel('^[C|c]')\">C</a></li>";
	szFooter1 << "<li><a class=\"menu2\" href=\"javascript:{}\" onclick=\"func_SearchTitel('^[D|d]')\">D</a></li>";
	szFooter1 << "<li><a class=\"menu2\" href=\"javascript:{}\" onclick=\"func_SearchTitel('^[E|e]')\">E</a></li>";
	szFooter1 << "<li><a class=\"menu2\" href=\"javascript:{}\" onclick=\"func_SearchTitel('^[F|f]')\">F</a></li>";
	szFooter1 << "<li><a class=\"menu2\" href=\"javascript:{}\" onclick=\"func_SearchTitel('^[G|g]')\">G</a></li>";
	szFooter1 << "<li><a class=\"menu2\" href=\"javascript:{}\" onclick=\"func_SearchTitel('^[H|h]')\">H</a></li>";
	szFooter1 << "<li><a class=\"menu2\" href=\"javascript:{}\" onclick=\"func_SearchTitel('^[I|i]')\">I</a></li>";
	szFooter1 << "<li><a class=\"menu2\" href=\"javascript:{}\" onclick=\"func_SearchTitel('^[J|j]')\">J</a></li>";
	szFooter1 << "<li><a class=\"menu2\" href=\"javascript:{}\" onclick=\"func_SearchTitel('^[K|k]')\">K</a></li>";
	szFooter1 << "<li><a class=\"menu2\" href=\"javascript:{}\" onclick=\"func_SearchTitel('^[L|l]')\">L</a></li>";
	szFooter1 << "<li><a class=\"menu2\" href=\"javascript:{}\" onclick=\"func_SearchTitel('^[M|m]')\">M</a></li>";
	szFooter1 << "<li><a class=\"menu2\" href=\"javascript:{}\" onclick=\"func_SearchTitel('^[N|n]')\">N</a></li>";
	szFooter1 << "<li><a class=\"menu2\" href=\"javascript:{}\" onclick=\"func_SearchTitel('^[O|o]')\">O</a></li>";
	szFooter1 << "<li><a class=\"menu2\" href=\"javascript:{}\" onclick=\"func_SearchTitel('^[P|p]')\">P</a></li>";
	szFooter1 << "<li><a class=\"menu2\" href=\"javascript:{}\" onclick=\"func_SearchTitel('^[Q|q]')\">Q</a></li>";
	szFooter1 << "<li><a class=\"menu2\" href=\"javascript:{}\" onclick=\"func_SearchTitel('^[R|r]')\">R</a></li>";
	szFooter1 << "<li><a class=\"menu2\" href=\"javascript:{}\" onclick=\"func_SearchTitel('^[S|s]')\">S</a></li>";
	szFooter1 << "<li><a class=\"menu2\" href=\"javascript:{}\" onclick=\"func_SearchTitel('^[T|t]')\">T</a></li>";
	szFooter1 << "<li><a class=\"menu2\" href=\"javascript:{}\" onclick=\"func_SearchTitel('^[U|u]')\">U</a></li>";
	szFooter1 << "<li><a class=\"menu2\" href=\"javascript:{}\" onclick=\"func_SearchTitel('^[V|v]')\">V</a></li>";
	szFooter1 << "<li><a class=\"menu2\" href=\"javascript:{}\" onclick=\"func_SearchTitel('^[W|w]')\">W</a></li>";
	szFooter1 << "<li><a class=\"menu2\" href=\"javascript:{}\" onclick=\"func_SearchTitel('^[X|x]')\">X</a></li>";
	szFooter1 << "<li><a class=\"menu2\" href=\"javascript:{}\" onclick=\"func_SearchTitel('^[Y|y]')\">Y</a></li>";
	szFooter1 << "<li><a class=\"menu2\" href=\"javascript:{}\" onclick=\"func_SearchTitel('^[Z|z]')\">Z</a></li>";
	szFooter1 << "</ul><br>";
	szFooter1 << "<ul class=\"nav\">";
	szFooter1 << "<li><a class=\"menu\" href=\"javascript:{}\" title=\"Alle Filme anzeigen\" onclick=\"func_SearchGenre('ALLE')\"><strong>ALLE</strong></a></li>";

	szFooterGenre << "<li><a class=\"menu\" href=\"javascript:{}\" title=\"%GENRE%\" onclick=\"func_SearchGenre('%GENRE%')\"><strong>%GENRE1%</strong></a></li>";

/*
	szFooter << "<li><a class=\"menu\" href=\"javascript:{}\" title=\"Alle Abenteuerfilme anzeigen\" onclick=\"func_SearchGenre('Abenteuer')\">Abenteuer</a></li>";
	szFooter << "<li><a class=\"menu\" href=\"javascript:{}\" title=\"Alle Actionfilme anzeigen\" onclick=\"func_SearchGenre('Action')\">Action</a></li>";
	szFooter << "<li><a class=\"menu\" href=\"javascript:{}\" title=\"Alle Dramenfilme anzeigen\" onclick=\"func_SearchGenre('Drama')\">Drama</a></li>";
	szFooter << "<li><a class=\"menu\" href=\"javascript:{}\" title=\"Alle Familienfilme anzeigen\" onclick=\"func_SearchGenre('Famil')\">Familie</a></li>";
	szFooter << "<li><a class=\"menu\" href=\"javascript:{}\" title=\"Alle Fantasyfilme anzeigen\" onclick=\"func_SearchGenre('Fantasy')\">Fantasy</a></li>";
	szFooter << "<li><a class=\"menu\" href=\"javascript:{}\" title=\"Alle Gruselfilme anzeigen\" onclick=\"func_SearchGenre('Grusel')\">Grusel</a></li>";
	szFooter << "<li><a class=\"menu\" href=\"javascript:{}\" title=\"Alle Historienfilme anzeigen\" onclick=\"func_SearchGenre('Histor')\">Historienfilm</a></li>";
	szFooter << "<li><a class=\"menu\" href=\"javascript:{}\" title=\"Alle Horror-Filme anzeigen\"  onclick=\"func_SearchGenre('Horror')\">Horror</a></li>";
	szFooter << "<li><a class=\"menu\" href=\"javascript:{}\" title=\"Alle Kinder-Filme anzeigen\" onclick=\"func_SearchGenre('Kinder')\">Kinder</a></li>";
	szFooter << "<li><a class=\"menu\" href=\"javascript:{}\" title=\"Alle Kom&#246;dien anzeigen\" onclick=\"func_SearchGenre('Kom')\">Kom&#246;die</a></li>";
	szFooter << "<li><a class=\"menu\" href=\"javascript:{}\" title=\"Alle Kriegsfilme anzeigen\" onclick=\"func_SearchGenre('Krieg')\">Krieg</a></li>";
	szFooter << "<li><a class=\"menu\" href=\"javascript:{}\" title=\"Alle Krimis anzeigen\" onclick=\"func_SearchGenre('Krimi')\">Krimi</a></li>";
	szFooter << "<li><a class=\"menu\" href=\"javascript:{}\" title=\"Alle Liebesfilme anzeigen\" onclick=\"func_SearchGenre('Liebe')\">Liebe</a></li>";
	szFooter << "<li><a class=\"menu\" href=\"javascript:{}\" title=\"Alle Musikfilme anzeigen\" onclick=\"func_SearchGenre('Musik')\">Musik</a></li>";
	szFooter << "<li><a class=\"menu\" href=\"javascript:{}\" title=\"Alle Mysteryfilme anzeigen\" onclick=\"func_SearchGenre('Mystery')\">Mystery</a></li>";
	szFooter << "<li><a class=\"menu\" href=\"javascript:{}\" title=\"Alle Romantikfilme anzeigen\" onclick=\"func_SearchGenre('Roman')\">Romantik</a></li>";
	szFooter << "<li><a class=\"menu\" href=\"javascript:{}\" title=\"Alle Science-Fiction-Filme anzeigen\" onclick=\"func_SearchGenre('Sci')\">Science Fiction</a></li>";
	szFooter << "<li><a class=\"menu\" href=\"javascript:{}\" title=\"Alle Sportfilme anzeigen\" onclick=\"func_SearchGenre('Sport')\">Sport</a></li>";
	szFooter << "<li><a class=\"menu\" href=\"javascript:{}\" title=\"Alle Thriller anzeigen\" onclick=\"func_SearchGenre('Thriller')\">Thriller</a></li>";
*/
	//szFooter << "<li><a class=\"menu\" href=\"javascript:{}\" title=\"Alle Moviesets anzeigen\" style=\"color:#CCFFCC;\" onclick=\"func_BuildMoviesets()\">Moviesets</a></li>";
	//szFooter << "<li><a class=\"menu\" href=\"javascript:{}\" title=\"Alle Serien anzeigen\" style=\"color:#CCFFCC;\" onclick=\"func_BuildTvShows()\">Serien</a></li>";

	szFooter2 << "<li><a class=\"menu\" href=\"javascript:{}\" title=\"Filme nach Erscheinungsjahr sortieren\" onclick=\"func_SearchYear()\">Erscheinungsjahr</a></li>";
	szFooter2 << "<li><a class=\"menu\" href=\"javascript:{}\" title=\"Zuletzt hinzugef&#252;gte Filme anzeigen\" onclick=\"func_SearchDate()\">Neueste Filme</a></li>";
	szFooter2 << "<li><a class=\"menu\" href=\"javascript:{}\" title=\"Bestbewertete Filme (IMDB-Wertung) anzeigen\" onclick=\"func_SearchWertung()\">Wertung</a></li>";
	szFooter2 << "<li><a class=\"menu\" href=\"javascript:{}\" title=\"Bestes Bild anzeigen\" onclick=\"func_SearchQualiVideo()\">Bestes Bild</a></li>";
	szFooter2 << "<li><a class=\"menu\" href=\"javascript:{}\" title=\"Bester Ton anzeigen\" onclick=\"func_SearchQualiAudio()\">Bester Ton</a></li>";
	szFooter2 << "<li><a class=\"menu\" href=\"javascript:{}\" title=\"3D Filme anzeigen\" onclick=\"func_SearchThreeD()\">3D</a></li>";
	szFooter2 << "</ul>";
	szFooter2 << "</div>";
	szFooter2 << "";
	szFooter2 << "<table style= \"float:right;margin-top:10px;\">";
	szFooter2 << "	<tr>";
	szFooter2 << "		<td>";
	szFooter2 << "			<form name=\"seen\" id=\"seen\" >";
	szFooter2 << "				<label>Gesehen: </label>";
	szFooter2 << "				<input type=\"checkbox\" id=\"watched\" name=\"watched\" onclick=\"func_WatchedMovies()\" value=\"gesehen\" checked>";
	szFooter2 << "				<label>Ungesehen: </label>";
	szFooter2 << "				<input type=\"checkbox\" id=\"unwatched\" name=\"unwatched\" onclick=\"func_WatchedMovies()\" value=\"ungesehen\" checked>";
	szFooter2 << "			</form>";
	szFooter2 << "       </td>";
	szFooter2 << "	   <td width=\"50px\">";
	szFooter2 << "	   </td>";
	szFooter2 << "	   <td>";
	szFooter2 << "		   <form action=\"javascript:{}\">	";
	szFooter2 << "			   <label>Schauspielersuche:  </label>";
	szFooter2 << "			   <input type=\"text\" name=\"name\" id=\"actorform\" size=\"10\" maxlength=\"30\" onchange=\"doSend()\" />";
	szFooter2 << "			   <input type=\"submit\" value=\"Suchen\" onclick=\"func_SearchActor(document.getElementById('actorform').value)\" />";
	szFooter2 << "		   </form> ";
	szFooter2 << "		</td>";
/*
	szFooter2 << "	<!--			   <td>";
	szFooter2 << "		  <form name=\"Genreauswahl\" action=\"\">";
	szFooter2 << "		  <label>Genre: </label>";
	szFooter2 << "           <select name=\"Genreliste\" size=1 onchange=\"func_SearchGenre(document.Genreauswahl.Genreliste.selectedIndex)\">";
	szFooter2 << "              <option value=\"0\">ALLE</option>";
	szFooter2 << "              <option value=\"1\">Abenteuer</option>";
	szFooter2 << "              <option value=\"2\">Action</option>";
	szFooter2 << "              <option value=\"3\">Drama</option>  ";
	szFooter2 << "              <option value=\"4\">Familie</option>  ";
	szFooter2 << "              <option value=\"5\">Familie</option> ";
	szFooter2 << "              <option value=\"6\">Grusel</option>  ";
	szFooter2 << "              <option value=\"7\">Historienfilm</option>  ";
	szFooter2 << "              <option value=\"8\">Horror</option> ";
	szFooter2 << "              <option value=\"9\">Kinder</option>";
	szFooter2 << "              <option value=\"10\">Kom&#246;die</option>";
	szFooter2 << "              <option value=\"11\">Krieg</option>";
	szFooter2 << "              <option value=\"12\">Krimi</option>  ";
	szFooter2 << "              <option value=\"13\">Liebe</option>  ";
	szFooter2 << "              <option value=\"14\">Musik</option>  ";
	szFooter2 << "              <option value=\"15\">Mystery</option>  ";
	szFooter2 << "              <option value=\"16\">Romantik</option>  ";
	szFooter2 << "               <option value=\"17\">Science Fiction</option>";
	szFooter2 << "              <option value=\"18\">Sport</option>";
	szFooter2 << "              <option value=\"19\">Thriller</option>  ";
	szFooter2 << "              <option value=\"20\">Western</option>  ";
	szFooter2 << "            </select>";
	szFooter2 << "			</fieldset>";
	szFooter2 << "         </form>";
	szFooter2 << "		</td>";
	szFooter2 << "-->		";
*/
	szFooter2 << "		";
	szFooter2 << "	</tr>";
	szFooter2 << "</table>";
	szFooter2 << "";
	szFooter2 << "";
	szFooter2 << "</div>";
	szFooter2 << "<!-- HEADER END -->";
	szFooter2 << "";
	szFooter2 << "<!-- FOOTER -->";
	szFooter2 << "<div id=\"footer\">";
	szFooter2 << "<div style=\"padding:0.2em;float:right;\" >";
	szFooter2 << "<p style=\"color:white;font-size:0.6em;float:left; padding:0.1em;\">";
	szFooter2 << "    Hilfe/Support: <a class=\"menu\" href=\"http://www.german-yamj-board.org/viewtopic.php?f=4&t=297\">Ember auf german-yamj-board</a><br />  ";
	szFooter2 << "	Template v1.80 design by Cocotus</p>";
	szFooter2 << "	<a href=\"http://validator.w3.org/check?uri=referer\"> <img style=\"padding:0.1em; height:20px; width:58px; border:0;float:right; \"";
	szFooter2 << " src=\"http://www.w3.org/Icons/valid-xhtml10\" alt=\"Valid XHTML 1.0 Strict\"   /></a>     ";
	szFooter2 << "</div>";
	szFooter2 << "</div>";
	szFooter2 << "<!-- FOOTER END -->";
	szFooter2 << "";
	szFooter2 << "<script type=\"text/javascript\">";
	szFooter2 << "function addListener(obj, evt, handler)";
	szFooter2 << "{";
	szFooter2 << "   if (obj.addEventListener)";
	szFooter2 << "   {";
	szFooter2 << "      obj.addEventListener(evt, handler, false);";
	szFooter2 << "   }";
	szFooter2 << "   else if (obj.attachEvent)";
	szFooter2 << "   {";
	szFooter2 << "      obj.attachEvent('on' + evt, handler);";
	szFooter2 << "   }";
	szFooter2 << "}";
	szFooter2 << "";
	szFooter2 << "function init()";
	szFooter2 << "{";
	szFooter2 << "func_BuildNavigation(0);";
	szFooter2 << "func_DisplayLastModified();";
	szFooter2 << "}";
	szFooter2 << "";
	szFooter2 << "addListener(window, 'load', init);";
	szFooter2 << "</script>";
	szFooter2 << "</body>";
	szFooter2 << "</html>";

	for(int z = 0;z < szFooter1.count();z++)
	{
		QString	sz	= szFooter1.at(z);

		if(sz.contains("%PAGETITLE%", Qt::CaseInsensitive))
			sz	= sz.replace(QString("%PAGETITLE%"), ui->m_lpTitle->text(), Qt::CaseInsensitive);

		out << sz << "\n";
	}

	for(int x = 0;x < szGenreList.count();x++)
	{

		for(int z = 0;z < szFooterGenre.count();z++)
		{
			QString	sz	= szFooterGenre.at(z);

			if(sz.contains("%GENRE%", Qt::CaseInsensitive))
				sz	= sz.replace(QString("%GENRE%"), szGenreList.at(x), Qt::CaseInsensitive);

			if(sz.contains("%GENRE1%", Qt::CaseInsensitive))
				sz	= sz.replace(QString("%GENRE1%"), string2HTML(szGenreList.at(x)), Qt::CaseInsensitive);

			out << sz << "\n";
		}
	}

	for(int z = 0;z < szFooter2.count();z++)
	{
		QString	sz	= szFooter2.at(z);

		if(szFooter2.at(z).contains("%PAGETITLE%", Qt::CaseInsensitive))
			sz	= sz.replace(QString("%PAGETITLE%"), ui->m_lpTitle->text(), Qt::CaseInsensitive);

		out << sz << "\n";
	}
}

void cMainWindow::writeMovie(QTextStream& out, const qint16& iMovie, const QString& szTitle, const qint16& iYear, const QString& szTagline, const double& dRating,
							 const QStringList& szDirectorList, const QList<ACTOR>& ActorList, const QStringList& szGenreList, const qint16& iRuntime, const QString& szMPAA,
							 const QString& szPlot, const QList<VIDEO>& videoList, const QList<AUDIO>& audioList, const QString& szIMDBID, const bool& bWatched, const QDateTime& added, const QList<cTVShowEpisode*>& episodeList)
{
	QStringList	szMovie1;
	QStringList	szRegisseur;
	QStringList	szEpisode1;
	QStringList	szEpisode2;
	QStringList	szEpisode2t;
	QStringList	szEpisode3;
	QStringList	szActor1;
	QStringList	szActor2;
	QStringList	szGenre;
	QStringList	szMovie2;
/*
 * %MOVIECOUNT%
 * %TITLEYEAR%
 * %TAGLINE%
 * %RATING%
 */
	szMovie1 << "<div id=\"movie%MOVIECOUNT%\" class=\"divmain\">";
	szMovie1 << "<div id=\"backgrounddiv%MOVIECOUNT%\" class=\"page-background\"></div> ";
	szMovie1 << "<div class=\"paddingwrap\">";
	szMovie1 << "";
	szMovie1 << "<div class=\"divmovietopbar\"> ";
	szMovie1 << "<h2 style=\"margin-bottom:0px; margin-top:0px; display:inline;\">%TITLEYEAR%</h2>";
	szMovie1 << "<p class=\"fontstil\">%TAGLINE%</p>";
	szMovie1 << "<div style=\"text-align: right; position:absolute; top:0px; right:0px;\">";
	szMovie1 << "<p class=\"fontstil\" style= \"padding: 2em;\">Wertung: &nbsp;<b>%RATING%</b> &nbsp;(89,279 Stimmen)</p>";
	szMovie1 << "</div>";
	szMovie1 << "</div>";
	szMovie1 << "";
	szMovie1 << "<div class=\"divmoviedetails\"> ";
	szMovie1 << "<table style=\"border:0px; empty-cells:hide\">";

	szEpisode1 << "  <tr class=\"tabellenlinie\">";
	szEpisode1 << "    <td rowspan=\"%EPISODEROWS%\" valign=\"top\">";
	szEpisode1 << "      <div class=\"inner-table\">";
	szEpisode2 << "        <div title=\"%PLOT%\"><p>&nbsp;&nbsp;%EPISODE%</p></div>";
	szEpisode2t << "        <div><p style=\"color: #000000; background-color: #ffffff\"><b>%EPISODE%</b></p></div>";
	szEpisode3 << "      </div>";
	szEpisode3 << "    </td>";

/*
 * %DIRECTOR%
 */
	szRegisseur << "  <tr>";
	szRegisseur << "    <td><p>Regisseur:</p></td>";
	szRegisseur << "    <td class=\"tab\"><p>%DIRECTOR%</p></td>";
	szRegisseur << "  </tr>";

	szActor1 << "  <tr>";
	szActor1 << "    <td><p>Actor:</p></td>";
	szActor1 << "    <td class=\"tab\"><p>%ACTOR%</p></td>";
	szActor1 << "  </tr>";

	szActor2 << "  <tr>";
	szActor2 << "    <td><p></p></td>";
	szActor2 << "    <td class=\"tab\"><p>%ACTOR%</p></td>";
	szActor2 << "  </tr>";

/*
 * %GENRE% (Genre / Genre / ...)
 */
	szGenre << "  <tr class=\"tabellenlinie\">";
	szGenre << "    <td><p>Genre:</p></td>";
	szGenre << "    <td class=\"tab\"><p>%GENRE%</p></td>";
	szGenre << "  </tr>";

/*
 * %RUNTIME% (min)
 * %VBITRATE%
 * %ABITRATE%
 * %MPAA%
 * %POSTER% = movienr.jpg
 * %PLOT%
 * %YEAR%
 * %THUMB%
 * %TITLE%
 * %GENRE% (Genre / Genre / ...)
 * %FANART% = movienr-fanart.jpg
 * %RATING%
 * %ACTORS% (Actor,Actor, ...)
 * %VRES%
 * %OURCE% ???
 * %VCODEC%
 * %ACODEC%
 * %ACHANNEL%
 * %MOVIECOUNT%
 * %IMDBID%
 * %DATADATE%
 */
	szMovie2 << "  <tr class=\"tabellenlinie\">";
	szMovie2 << "    <td><p>L&#228;nge:</p></td>";
	szMovie2 << "    <td class=\"tab\"><p>%RUNTIME% min</p></td>";
	szMovie2 << "  </tr>";
	// TO IMPLEMENT szMovie2 << "  <tr class=\"tabellenlinie\">";
	// TO IMPLEMENT szMovie2 << "    <td><p>VideoBitrate:</p></td>";
	// TO IMPLEMENT szMovie2 << "    <td class=\"tab\"><p>%VBITRATE%</p></td>";
	// TO IMPLEMENT szMovie2 << "  </tr>";
	// TO IMPLEMENT szMovie2 << "  <tr class=\"tabellenlinie\">";
	// TO IMPLEMENT szMovie2 << "    <td><p>AudioBitrate:</p></td>";
	// TO IMPLEMENT szMovie2 << "    <td class=\"tab\"><p>%ABITRATE%</p></td>";
	// TO IMPLEMENT szMovie2 << "  </tr>";
	szMovie2 << "  <tr class=\"tabellenlinie\">";
	szMovie2 << "    <td><p>FSK:</p></td>";
	szMovie2 << "    <td class=\"tab\"><p>%MPAA%</p></td>";
	szMovie2 << "  </tr>";
	szMovie2 << "</table>";
	szMovie2 << "</div>";
	szMovie2 << "";
	szMovie2 << "<div class=\"divmoviebottompicture\">";
	szMovie2 << " <img class=\"poster\" src=\"%POSTER%\" width=\"133\" height=\"200\" alt=\"\" />";
	szMovie2 << "</div>";
	szMovie2 << "";
	szMovie2 << "<div class=\"divmoviebottominfo\"> ";
	szMovie2 << "<p> Links:";
	szMovie2 << "<a href=\"http://www.imdb.de/title/%IMDBID%/\" target=\"_blank\"><img style=\"border-style:none\" src=\"images/imdb.jpg\" title=\"IMDB-Link\" alt=\"IMDB-Eintrag anzeigen\"/></a>";
	szMovie2 << "<a href=\"http://www.imdb.com/title/%IMDBID%/trailers\" target=\"_blank\"><img style=\"border-style:none\" src=\"images/youtube.jpg\" title=\"Trailer-Link\" alt=\"Trailer anzeigen\"/></a>";
	// TO IMPLEMENT szMovie2 << "<a href=\"javascript:{}\" onclick=\"func_OpenFolder(1)\"><img style=\"border-style:none\" src=\"images/movie_folder.png\" title=\"Film &#246;ffnen (nur InternetExplorer)\" alt=\"Film &#246;ffnen (nur InternetExplorer)\"/></a>  ";
	szMovie2 << "</p><br />  ";
	szMovie2 << "<p class=\"fontstil\">%PLOT%</p>";
	szMovie2 << "<br />";
	szMovie2 << "<img class=\"flags\" alt=\"\" src=\"%VRES%\"/>";
	szMovie2 << "<img class=\"flags\" alt=\"\" src=\"Flags/vsource_defaultscreen.png\"/>";
	szMovie2 << "<img class=\"flags\" alt=\"\" src=\"%VCODEC%\"/>";
	szMovie2 << "<img class=\"flags\" alt=\"\" src=\"%ACODEC%\"/>";
	szMovie2 << "<img class=\"flags\" alt=\"\" src=\"%ACHANNEL%\"/>";
	szMovie2 << "</div>";
	szMovie2 << "";
	szMovie2 << "<script type=\"text/javascript\">";
	szMovie2 << "iCount=%MOVIECOUNT%;";
	// TO IMPLEMENT szMovie2 << "DataURL[%MOVIECOUNT%] = \"E:/Video1/10.000 BC\";";
	szMovie2 << "DataYear[%MOVIECOUNT%] = \"%YEAR%\";";
	szMovie2 << "DataBild[%MOVIECOUNT%] = \"%THUMB%\";";
	szMovie2 << "DataName[%MOVIECOUNT%] = \"%TITLE%\";";
	// TO IMPLEMENT szMovie2 << "DataFilename[%MOVIECOUNT%] = \"10.000 B.C. (2008).mkv\";";
	szMovie2 << "DataGenre[%MOVIECOUNT%] = \"%GENRE%\";";
	szMovie2 << "DataDate[%MOVIECOUNT%] = \"%DATADATE%\";";
	szMovie2 << "DataFanart[%MOVIECOUNT%] = \"%FANART%\";";
	szMovie2 << "DataSource[%MOVIECOUNT%] = \"%VRES%\";";
	szMovie2 << "DataRating[%MOVIECOUNT%] = \"%RATING%\";";
	szMovie2 << "DataActors[%MOVIECOUNT%] = \"%ACTORS%\";";
	szMovie2 << "DataWatched[%MOVIECOUNT%] = \"%WATCHED%\";";
	szMovie2 << "DataVidBitrate[%MOVIECOUNT%] = \"%VBITRATE%\";";
	szMovie2 << "DataAudBitrate[%MOVIECOUNT%] = \"%ABITRATE%\";";
	szMovie2 << "DataThreed[%MOVIECOUNT%] = \"\";";
	szMovie2 << "DataSet[%MOVIECOUNT%] = \"\";";
	szMovie2 << QString("DataNow = \"%1\";").arg(QDate::currentDate().toString("dd.MM.yyyy"));
	szMovie2 << "DataMoviesets = \"\";";
	// TO IMPLEMENT szMovie2 << "DataTVShows = \"53*24*53s1*53s3*53s6*53s7*53s8|54*4400*54s1*54s2*54s3*54s4|1*Alias*1s1*1s2*1s3*1s4*1s5|55*Alphas*55s1|2*Andromeda*2s1*2s2*2s3*2s4*2s5|56*Arrow*56s1|3*Babylon 5*3s1*3s2*3s3*3s4*3s5|4*Battlestar Galactica*4s0*4s1*4s2*4s3*4s4|5*Baywatch*5s1*5s2*5s3*5s4*5s5*5s6*5s7*5s8*5s9*5s10*5s11|6*Beverly Hills*6s1*6s2*6s3*6s4*6s5*6s6*6s7*6s8*6s9*6s10|7*Bionic Woman*7s1|8*Caprica*8s1|9*Champagner Dynastie*9s1|57*Charmed*57s1*57s2*57s3*57s4*57s5*57s6*57s7*57s8|10*Columbo*10s1*10s2*10s3*10s4*10s5*10s6*10s7*10s8*10s9*10s10|58*Continuum*58s1|11*Countdown - Die Jagd Beginnt*11s1*11s2*11s3|12*Crusade*12s1|13*Dallas*13s0*13s1*13s2*13s3*13s4*13s5*13s6*13s7*13s8*13s9*13s10*13s11*13s12*13s13*13s14|14*Dallas*14s1*14s2|59*Defiance*59s1|15*Der Denver-Clan*15s1*15s2*15s3*15s4*15s5*15s6*15s7*15s8*15s9*15s10|16*Der Seewolf*16s1|17*Der Sonne Entgegen*17s1|18*Die Lottosieger*18s1|19*Eureka*19s1*19s2*19s3*19s4*19s5|60*Falling Skies*60s1*60s2*60s3|20*Farscape*20s1*20s2*20s3*20s4*20s5|21*FC R&#252;ckpass*21s1|22*Firefly*22s1|23*Fringe*23s1*23s2*23s3*23s5|24*Galactica*24s1|61*Grimm*61s1*61s2|62*Haven*62s1*62s2*62s3*62s4|25*Kampfstern Galactica*25s1|26*Knight Rider*26s1*26s2*26s3|27*Kottan Ermittelt*27s1|28*Legend Of The Seeker*28s1*28s2|29*Lost*29s0*29s1*29s2*29s3*29s4*29s5*29s6|63*Lost Girl*63s1*63s2|64*Once Upon A Time*64s1*64s2|30*Primeval*30s1*30s2*30s4*30s5|31*Primeval - New World*31s1|32*Raumschiff Enterprise*32s1*32s2*32s3|33*Raumschiff Enterprise - Das N&#228;chste Jahrhundert*33s1*33s2*33s3*33s4*33s5*33s6*33s7|34*Roswell*34s1|35*Sanctuary*35s1*35s2*35s3*35s4|36*Scarlett*36s1|37*Space*37s1|38*Spartacus*38s1*38s2*38s3|39*Spartacus - Gods Of The Arena*39s0|40*Star Trek - Deep Space Nine*40s1*40s2*40s3*40s4*40s5*40s6*40s7|41*Star Trek - Enterprise*41s1*41s2*41s3*41s4|42*Star Trek - Raumschiff Voyager*42s1*42s2*42s3*42s4*42s5*42s6*42s7|43*Stargate Atlantis*43s1*43s2*43s3*43s4*43s5|44*Stargate SG-1*44s1*44s2*44s3*44s4*44s5*44s6*44s7*44s8*44s9*44s10|45*Stargate Universe*45s1*45s2|46*Superman - Die Abenteuer Von Lois &amp; Clark*46s1*46s2*46s3*46s4|47*Supernatural*47s1*47s2*47s3*47s4*47s5*47s6*47s7*47s8|65*Terminator - Sarah Connor Chronicles*65s1*65s2|48*The Colbys*48s1*48s2|66*The Walking Dead*66s1*66s2*66s3*66s4|49*Twin Peaks*49s1*49s2|67*Under The Dome*67s1|50*Unsere Kleine Farm*50s0*50s1*50s2*50s3*50s4*50s5*50s6*50s7*50s8*50s9*50s10|51*V - Die Au&#223;erirdischen Besucher Kommen*51s0*51s1|52*V - Die Besucher*52s1*52s2|68*Warehouse 13*68s1*68s2*68s3*68s4|69*White Collar*69s1*69s2*69s3|70*Wild Palms*70s1\";";
	szMovie2 << "</script>";
	szMovie2 << "					";
	szMovie2 << "";
	szMovie2 << "</div></div>";
	szMovie2 << "<!-- MAIN CONTENT END -->";

	for(int z = 0;z < szMovie1.count();z++)
	{
		QString	sz	= szMovie1.at(z);
		sz	= sz.replace("%MOVIECOUNT%", QString("%1").arg(iMovie), Qt::CaseInsensitive);

		if(!szTitle.isEmpty())
		{
			if(iYear > 1000)
				sz	= sz.replace("%TITLEYEAR%", QString("%1 (%2)").arg(string2HTML(szTitle)).arg(iYear), Qt::CaseInsensitive);
			else
				sz	= sz.replace("%TITLEYEAR%", string2HTML(szTitle));
		}
		else
			sz	= sz.replace("%TITLEYEARS%", "", Qt::CaseInsensitive);

		if(!szTagline.isEmpty())
			sz	= sz.replace("%TAGLINE%", string2HTML(szTagline), Qt::CaseInsensitive);
		else
			sz	= sz.replace("%TAGLINE%", "", Qt::CaseInsensitive);

		if(dRating > 0)
			sz	= sz.replace("%RATING%", QString("%1").arg(dRating), Qt::CaseInsensitive);
		else
			sz	= sz.replace("%RATING%", "", Qt::CaseInsensitive);

		out << sz << "\n";
	}

	int iStart	= 0;
	if(episodeList.count())
	{
		iStart	= 1;

		for(int y = 0;y < szEpisode1.count();y++)
		{
			QString	sz	= szEpisode1.at(y);
			sz	= sz.replace("%EPISODEROWS%", QString("%1").arg((ui->m_lpMaxActor->value()+3)*18));

			out << sz << "\n";
		}

		qint16	iOldSeason	= -1;

		for(int y = 0;y < episodeList.count();y++)
		{
			if(episodeList.at(y)->season()	!= iOldSeason)
			{
				iOldSeason	= episodeList.at(y)->season();
				QString	szEpisode	= QString("Season %1").arg(iOldSeason);
				for(int z = 0;z < szEpisode2t.count();z++)
				{
					QString	sz	= szEpisode2t.at(z);
					sz	= sz.replace("%EPISODE%", string2HTML(szEpisode));
					sz	= sz.replace("%PLOT%", "");

					out << sz << "\n";
				}
			}

			QString	szEpisode	= QString("  %1 - %2").arg(episodeList.at(y)->episode(), 2, 10, QChar('0')).arg(episodeList.at(y)->title());
			for(int z = 0;z < szEpisode2.count();z++)
			{
				QString	sz	= szEpisode2.at(z);
				sz	= sz.replace("%EPISODE%", string2HTML(szEpisode));
				sz	= sz.replace("%PLOT%", string2HTML(episodeList.at(y)->plot()));

				out << sz << "\n";
			}
		}

		for(int y = 0;y < szEpisode3.count();y++)
		{
			QString	sz	= szEpisode3.at(y);
			out << sz << "\n";
		}
	}

	for(int y = iStart;y < szDirectorList.count();y++)
	{
		QString	szDirector	= szDirectorList.at(y);

		for(int z = 0;z < szRegisseur.count();z++)
		{
			QString	sz	= szRegisseur.at(z);
			sz	= sz.replace("%DIRECTOR%", string2HTML(szDirector), Qt::CaseInsensitive);

			out << sz << "\n";
		}
	}

	if(ActorList.count())
	{
		for(int z = 0;z < ActorList.count();z++)
		{
			if(z >= ui->m_lpMaxActor->value())
				break;

			ACTOR	actor	= ActorList.at(z);

			if(z == 0)
			{
				for(int i = iStart;i < szActor1.count();i++)
				{
					QString	sz	= szActor1.at(i);
					if(actor.szRole.isEmpty())
						sz	= sz.replace("%ACTOR%", string2HTML(actor.szName), Qt::CaseInsensitive);
					else
						sz	= sz.replace("%ACTOR%", QString("%1 (%2)").arg(string2HTML(actor.szName)).arg(string2HTML(actor.szRole)));
					out << sz << "\n";
				}
			}
			else
			{
				for(int i = 0;i < szActor2.count();i++)
				{
					QString	sz	= szActor2.at(i);
					if(actor.szRole.isEmpty())
						sz	= sz.replace("%ACTOR%", string2HTML(actor.szName), Qt::CaseInsensitive);
					else
						sz	= sz.replace("%ACTOR%", QString("%1 (%2)").arg(string2HTML(actor.szName)).arg(string2HTML(actor.szRole)));
					out << sz << "\n";
				}
			}
		}
	}

	if(szGenreList.count())
	{
		for(int z = 0;z < szGenre.count();z++)
		{
			QString	sz	= szGenre.at(z);
			sz	= sz.replace("%GENRE%", string2HTML(szGenreList.join(" / ")), Qt::CaseInsensitive);

			out << sz << "\n";
		}
	}

	for(int z = 0;z < szMovie2.count();z++)
	{
		QString	sz	= szMovie2.at(z);

		if(iRuntime > 0)
			sz	= sz.replace("%RUNTIME%", QString("%1").arg(iRuntime), Qt::CaseInsensitive);
		else
			sz	= sz.replace("%RUNTIME%", "", Qt::CaseInsensitive);

		if(!szMPAA.isEmpty())
			sz	= sz.replace("%MPAA%", szMPAA, Qt::CaseInsensitive);
		else
			sz	= sz.replace("%MPAA%", "", Qt::CaseInsensitive);

		if(!szPlot.isEmpty())
			sz	= sz.replace("%PLOT%", string2HTML(szPlot), Qt::CaseInsensitive);
		else
			sz	= sz.replace("%PLOT%", "", Qt::CaseInsensitive);

		if(iYear > 1000)
			sz	= sz.replace("%YEAR%", QString("%1").arg(iYear), Qt::CaseInsensitive);
		else
			sz	= sz.replace("%YEAR%", "", Qt::CaseInsensitive);

		if(!szTitle.isEmpty())
			sz	= sz.replace("%TITLE%", string2HTML(szTitle), Qt::CaseInsensitive);
		else
			sz	= sz.replace("%TITLE%", "", Qt::CaseInsensitive);

		if(szGenreList.count())
			sz	= sz.replace("%GENRE%", szGenreList.join(" / "), Qt::CaseInsensitive);

		if(dRating > 0)
			sz	= sz.replace("%RATING%", QString("%1").arg(dRating), Qt::CaseInsensitive);
		else
			sz	= sz.replace("%RATING%", "", Qt::CaseInsensitive);

		if(bWatched)
			sz	= sz.replace("%WATCHED%", QString("true"), Qt::CaseInsensitive);
		else
			sz	= sz.replace("%WATCHED%", "", Qt::CaseInsensitive);

		if(ActorList.count())
		{
			QString	szTmp;
			for(int i = 0;i < ActorList.count();i++)
			{
				if(!szTmp.isEmpty())
					szTmp.append(",");
				szTmp.append(string2HTML(ActorList.at(i).szName));
			}
			sz	= sz.replace("%ACTORS%", szTmp, Qt::CaseInsensitive);
		}
		else
			sz.replace("%ACTORS%", "", Qt::CaseInsensitive);

		if(videoList.count())
		{
			QString	szTmp;
			VIDEO	video	= videoList.at(0);
			QString	szScantype(" ");

			if(!video.szScantype.compare("progressive", Qt::CaseInsensitive))
				szScantype	= "p";
			else if(!video.szScantype.compare("interleave", Qt::CaseInsensitive))
				szScantype	= "i";

			if(video.iWidth == 1920)
				szTmp	= QString("Flags/vres_1080%1.png").arg(szScantype);
			else
			{
				switch(video.iHeight)
				{
				case 480:
					szTmp	= QString("Flags/vres_480%1.png").arg(szScantype);
					break;
				case 540:
					szTmp	= QString("Flags/vres_540%1.png").arg(szScantype);
					break;
				case 576:
					szTmp	= QString("Flags/vres_576%1.png").arg(szScantype);
					break;
				case 720:
					szTmp	= QString("Flags/vres_720%1.png").arg(szScantype);
					break;
				case 768:
					szTmp	= QString("Flags/vres_768%1.png").arg(szScantype);
					break;
				case 1080:
					szTmp	= QString("Flags/vres_1080%1.png").arg(szScantype);
					break;
				default:
					szTmp	= "Flags/defaultscreen.png";
					break;
				}
			}
			sz	= sz.replace("%VRES%", szTmp, Qt::CaseInsensitive);

			if(!video.szCodec.compare("h264", Qt::CaseInsensitive))
				szTmp	= "Flags/vcodec_h264.png";
			else
				szTmp	= "Flags/vcodec_defaultscreen.png";
			sz	= sz.replace("%VCODEC%", szTmp, Qt::CaseInsensitive);
		}

		if(audioList.count())
		{
			QString	szTmp;
			AUDIO	audio	= audioList.at(0);

			if(!audio.szCodec.compare("dtshd_ma", Qt::CaseInsensitive))
				szTmp	= "Flags/acodec_dtshd_ma.png";
			else if(!audio.szCodec.compare("DTS", Qt::CaseInsensitive))
				szTmp	= "Flags/acodec_dts.png";
			else
				szTmp	= "Flags/acodec_defaultsound.png";
			sz	= sz.replace("%ACODEC%", szTmp, Qt::CaseInsensitive);

			switch(audio.iChannels)
			{
			case 1:
				szTmp	= "Flags/achan_1.png";
				break;
			case 2:
				szTmp	= "Flags/achan_2.png";
				break;
			case 6:
				szTmp	= "Flags/achan_6.png";
				break;
			case 7:
				szTmp	= "Flags/achan_7.png";
				break;
			case 8:
				szTmp	= "Flags/achan_8.png";
				break;
			default:
				szTmp	= "Flags/achan_defaultsound.png";
				break;
			}
			sz	= sz.replace("%ACHANNEL%", szTmp, Qt::CaseInsensitive);
		}

		if(sz.contains("%IMDBID%", Qt::CaseInsensitive))
		{
			if(szIMDBID.isEmpty())
				sz	= "";
			else
				sz	= sz.replace("%IMDBID%", szIMDBID, Qt::CaseInsensitive);
		}

		sz	= sz.replace("%MOVIECOUNT%", QString("%1").arg(iMovie), Qt::CaseInsensitive);
		sz	= sz.replace("%THUMB%", QString("poster/%1.jpg").arg(iMovie), Qt::CaseInsensitive);
		sz	= sz.replace("%POSTER%", QString("poster/%1.jpg").arg(iMovie), Qt::CaseInsensitive);
		sz	= sz.replace("%FANART%", QString("fanart/%1.jpg").arg(iMovie), Qt::CaseInsensitive);

		if(sz.contains("%DATADATE%") && added.date().year() != 1980)
			sz.replace("%DATADATE%", added.toString("dd.MM.yyyy"));

		out << sz << "\n";
	}
}

void cMainWindow::copyImages(const qint16& iMovie, const QString& szDestDir, const QString& szFanart, const QString& szPoster)
{
	if(!szFanart.isEmpty())
	{
		QImage	i(szFanart);
		if(!i.isNull())
		{
			if(i.width() > ui->m_lpFanartW->value() || i.height() > ui->m_lpFanartH->value())
				i.scaled(QSize(ui->m_lpFanartW->value(), ui->m_lpFanartH->value()), Qt::KeepAspectRatio, Qt::SmoothTransformation).save(QString("%1/fanart/%2.jpg").arg(szDestDir).arg(iMovie));
			else
				i.save(QString("%1/fanart/%2.jpg").arg(szDestDir).arg(iMovie));
		}
	}

	// 200x300
	if(!szPoster.isEmpty())
	{
		QImage	i(szPoster);
		if(!i.isNull())
		{
			if(i.width() > ui->m_lpPosterW->value() || i.height() > ui->m_lpPosterH->value())
				i.scaled(QSize(ui->m_lpPosterW->value(), ui->m_lpPosterH->value()), Qt::KeepAspectRatio, Qt::SmoothTransformation).save(QString("%1/poster/%2.jpg").arg(szDestDir).arg(iMovie));
			else
				i.save(QString("%1/poster/%2.jpg").arg(szDestDir).arg(iMovie));
		}
	}
}

void cMainWindow::on_m_lpGenerateHTML_clicked()
{
	QString	szDestDir	= preparePath();
	if(szDestDir.isEmpty())
		return;

	QFile		file(QString("%1/index.html").arg(szDestDir));
	file.open(QIODevice::WriteOnly | QIODevice::Text);
	if(!file.isOpen())
		return;

	QStringList	szGenreList;
	qint16		iCount	= 0;

	for(int x = 0;x < m_lpTVShowsModel->rowCount();x++)
	{
		QStandardItem*	lpItem	= m_lpTVShowsModel->item(x, 0);
		if(lpItem->checkState() == Qt::Unchecked)
			continue;

		QVariant	v			= lpItem->data(Qt::UserRole);
		cTVShow*	lpTVShow	= v.value<cTVShow*>();

		szGenreList.append(lpTVShow->genreList());
		iCount++;
	}

	for(int x = 0;x < m_lpMoviesModel->rowCount();x++)
	{
		QStandardItem*	lpItem	= m_lpMoviesModel->item(x, 0);
		if(lpItem->checkState() == Qt::Unchecked)
			continue;

		QVariant	v		= lpItem->data(Qt::UserRole);
		cMovie*		lpMovie	= v.value<cMovie*>();

		szGenreList.append(lpMovie->genreList());
		iCount++;
	}

	m_lpStatusProgress->show();
	m_lpStatusProgress->setMinimum(0);
	m_lpStatusProgress->setMaximum(iCount);
	iCount	= 0;

	szGenreList.removeDuplicates();
	szGenreList	= convertGenre(szGenreList);
	szGenreList.sort(Qt::CaseInsensitive);

	QTextStream	out(&file);

	writeHeader(out);
	int	iMovie	= 1;

	m_lpStatusProgress->setValue(iMovie);

	for(int x = 0;x < m_lpTVShowsModel->rowCount();x++)
	{
		QStandardItem*	lpItem	= m_lpTVShowsModel->item(x, 0);
		if(lpItem->checkState() == Qt::Unchecked)
			continue;

		QVariant		v			= lpItem->data(Qt::UserRole);
		cTVShow*		lpTVShow	= v.value<cTVShow*>();

		ui->m_lpStatusBar->showMessage(QString("preparing %1 ...").arg(lpTVShow->title()));

		cTVShowEpisode*	lpEpisode	= 0;
		bool			bWatched	= true;

		if(lpTVShow->episodeList().count())
		{
			for(int z = 0;z < lpTVShow->episodeList().count();z++)
			{
				if(!lpTVShow->episodeList().at(z)->watched())
				{
					bWatched	= false;
					break;
				}
			}
			lpEpisode	= lpTVShow->episodeList().at(0);
		}

		if(lpEpisode)
			writeMovie(out, iMovie, lpTVShow->title(), lpTVShow->year(), QString(""), lpTVShow->rating(), QStringList(), lpTVShow->actorList(), convertGenre(lpTVShow->genreList()),
					   lpTVShow->runtime(), lpTVShow->mpaa(), lpTVShow->plot(), lpEpisode->videoList(), lpEpisode->audioList(), QString(""), bWatched, QDateTime(QDate(1980, 1, 1), QTime(0, 0, 0)), lpTVShow->episodeList());
		else
			writeMovie(out, iMovie, lpTVShow->title(), lpTVShow->year(), QString(""), lpTVShow->rating(), QStringList(), lpTVShow->actorList(), convertGenre(lpTVShow->genreList()),
					   lpTVShow->runtime(), lpTVShow->mpaa(), lpTVShow->plot(), QList<VIDEO>(), QList<AUDIO>(), QString(""), bWatched, QDateTime(QDate(1980, 1, 1), QTime(0, 0, 0)), lpTVShow->episodeList());

		copyImages(iMovie, szDestDir, lpTVShow->fanart(), lpTVShow->poster());
		iMovie++;
		m_lpStatusProgress->setValue(iMovie);
	}

	for(int x = 0;x < m_lpMoviesModel->rowCount();x++)
	{
		QStandardItem*	lpItem	= m_lpMoviesModel->item(x, 0);
		if(lpItem->checkState() == Qt::Unchecked)
			continue;

		QVariant	v		= lpItem->data(Qt::UserRole);
		cMovie*		lpMovie	= v.value<cMovie*>();

		ui->m_lpStatusBar->showMessage(QString("preparing %1 ...").arg(lpMovie->title()));

		writeMovie(out, iMovie, lpMovie->title(), lpMovie->year(), lpMovie->tagline(), lpMovie->rating(), lpMovie->directorList(), lpMovie->actorList(), convertGenre(lpMovie->genreList()),
				   lpMovie->runtime(), lpMovie->mpaa(), lpMovie->plot(), lpMovie->videoList(), lpMovie->audioList(), lpMovie->imdbID(), lpMovie->watched(), lpMovie->added(), QList<cTVShowEpisode*>());

		copyImages(iMovie, szDestDir, lpMovie->fanart(), lpMovie->poster());
		iMovie++;
		m_lpStatusProgress->setValue(iMovie);
	}

	writeFooter(out, szGenreList);
	file.close();

	ui->m_lpOutput->load(QUrl(QString("file:///%1/index.html").arg(szDestDir)));
	ui->m_lpOutput->show();

	ui->m_lpStatusBar->showMessage("");
	m_lpStatusProgress->hide();
}

void cMainWindow::onTVShowsItemChanged(QStandardItem* lpItem)
{
	if(m_bTVShowCheck)
		return;

	m_bTVShowCheck	= true;

	Qt::CheckState	s		= lpItem->checkState();

	if(lpItem->rowCount())
	{
		for(int z = 0;z < lpItem->rowCount();z++)
		{
			QStandardItem*	lpItem1	= lpItem->child(z, 0);
			lpItem1->setCheckState(s);

			if(lpItem1->rowCount())
			{
				for(int y = 0;y < lpItem1->rowCount();y++)
					lpItem1->child(y, 0)->setCheckState(s);
			}
		}
	}


	for(int z = 0;z < m_lpTVShowsModel->rowCount();z++)
	{
		QStandardItem*	lpItem	= m_lpTVShowsModel->item(z, 0);
		bool			bSeasonChecked	= true;

		for(int y = 0;y < lpItem->rowCount();y++)
		{
			QStandardItem*	lpSeasonItem	= lpItem->child(y, 0);
			bool			bEpisodeChecked	= true;

			for(int x = 0;x < lpSeasonItem->rowCount();x++)
			{
				QStandardItem*	lpEpisodeItem	= lpSeasonItem->child(x, 0);
				if(lpEpisodeItem->checkState() == Qt::Unchecked)
					bEpisodeChecked	= false;
			}
			if(bEpisodeChecked)
				lpSeasonItem->setCheckState(Qt::Checked);
			else
			{
				bSeasonChecked	= false;
				lpSeasonItem->setCheckState(Qt::Unchecked);
			}
		}
		if(bSeasonChecked)
			lpItem->setCheckState(Qt::Checked);
		else
			lpItem->setCheckState(Qt::Unchecked);
	}

	m_bTVShowCheck = false;
}

void cMainWindow::on_m_lpOutputPathSelect_clicked()
{
	QString		szPath		= ui->m_lpOutputPath->text();
	if(szPath.isEmpty())
		szPath	= m_szLastOutputPath;

	QFileDialog	dlg(this, "select Path", szPath);
	dlg.setFileMode(QFileDialog::Directory);
	if(dlg.exec())
	{
		QString	szNewPath	= dlg.selectedFiles().at(0);
		if(szNewPath.isEmpty())
			return;
		ui->m_lpOutputPath->setText(szNewPath);
		m_szLastOutputPath	= szNewPath;
	}
}

void cMainWindow::displayMovieList()
{
	for(int z = 0;z < m_MovieList.count();z++)
	{
		cMovie*	lpMovie	= m_MovieList.at(z);
		QString	szTitle;

		if(lpMovie->year() > 1000)
			szTitle	= QString("%1 (%2)").arg(lpMovie->title()).arg(lpMovie->year());
		else
			szTitle	= lpMovie->title();

		QStandardItem*	lpItem	= new QStandardItem(szTitle);
		lpItem->setCheckable(true);
		lpItem->setCheckState(Qt::Checked);
		QVariant	v	= qVariantFromValue(lpMovie);
		lpItem->setData(v, Qt::UserRole);
		m_lpMoviesModel->appendRow(lpItem);
	}
	ui->m_lpMovies->sortByColumn(0, Qt::AscendingOrder);
}

void cMainWindow::on_m_lpScanAll_clicked()
{
	on_m_lpMoviesScan_clicked();
	on_m_lpTVShowsScan_clicked();
}

void cMainWindow::on_m_lpXBMC_clicked()
{
	bool	bEnabled	= false;

	if(ui->m_lpXBMC->isChecked())
		bEnabled	= true;

	ui->m_lpXBMCServerLabel->setEnabled(bEnabled);
	ui->m_lpXBMCServer->setEnabled(bEnabled);
	ui->m_lpXBMCPortLabel->setEnabled(bEnabled);
	ui->m_lpXBMCPort->setEnabled(bEnabled);
	ui->m_lpXBMCUserLabel->setEnabled(bEnabled);
	ui->m_lpXBMCUser->setEnabled(bEnabled);
	ui->m_lpXBMCPassLabel->setEnabled(bEnabled);
	ui->m_lpXBMCPass->setEnabled(bEnabled);
}

void cMainWindow::addGenreList(const QStringList& szGenreList)
{
	for(int z = 0;z < szGenreList.count();z++)
	{
		if(!m_GenreCombineList.existFrom(szGenreList.at(z), Qt::CaseInsensitive))
		{
			cGenreCombine*	lpGenreCombine	= m_GenreCombineList.add(szGenreList.at(z));
			lpGenreCombine->addFrom(szGenreList.at(z));
		}
	}
}

QStandardItem* cMainWindow::selectedGenreTo()
{
	if(ui->m_lpGenreTo->selectionModel()->selectedIndexes().count())
		return(m_lpGenreToModel->itemFromIndex(ui->m_lpGenreTo->selectionModel()->selectedIndexes().at(0)));
	return(0);
}

void cMainWindow::on_m_lpGenreToSelectionChanged(const QItemSelection& selected, const QItemSelection& /*deselected*/)
{
	QStandardItem*	lpItem;

	if(selected.indexes().count())
		lpItem	= m_lpGenreToModel->itemFromIndex(selected.indexes().at(0));
	else
		return;

	QVariant		v				= lpItem->data(Qt::UserRole);
	cGenreCombine*	lpGenreCombine	= v.value<cGenreCombine*>();
	QStringList		szGenres		= lpGenreCombine->genreFrom();

	m_bGenreToSelecting	= true;

	ui->m_lpGenreFrom->clearSelection();

	for(int z = 0;z < szGenres.count();z++)
	{
		QList<QStandardItem*>items	= m_lpGenreFromModel->findItems(szGenres.at(z));
		if(items.count())
		{
			QModelIndex		i	= m_lpGenreFromModel->indexFromItem(items.at(0));
			ui->m_lpGenreFrom->selectionModel()->select(i, QItemSelectionModel::Select);
		}
	}

	m_bGenreToSelecting	= false;
}

void cMainWindow::on_m_lpGenreFromSelectionChanged(const QItemSelection& /*selected*/, const QItemSelection& /*deselected*/)
{
	if(	m_bGenreToSelecting)
		return;

	QStandardItem*	lpItem	= selectedGenreTo();
	if(!lpItem)
		return;

	QVariant		v				= lpItem->data(Qt::UserRole);
	cGenreCombine*	lpGenreCombine	= v.value<cGenreCombine*>();
	QStringList		szGenres		= lpGenreCombine->genreFrom();

	lpGenreCombine->clearFrom();

	for(int z = 0;z < ui->m_lpGenreFrom->selectionModel()->selectedRows().count();z++)
	{
		QStandardItem*	lpItem		= m_lpGenreFromModel->item(ui->m_lpGenreFrom->selectionModel()->selectedRows().at(z).row());
		if(lpItem)
			lpGenreCombine->addFrom(lpItem->text());
	}
}

void cMainWindow::on_m_lpGenreToPlus_clicked()
{
	bool	bOK;
	QString	szGenre	= QInputDialog::getText(this, tr("Genre"), tr("Enter new genre:"), QLineEdit::Normal, "", &bOK);

	if(!bOK || szGenre.isEmpty())
		return;

	if(m_GenreCombineList.existTo(szGenre, Qt::CaseInsensitive))
	{
		QMessageBox::information(this, "Warning", "Genre already exists.");
		return;
	}
	cGenreCombine*	lpGenreCombine	= m_GenreCombineList.add(szGenre);
	lpGenreCombine->addFrom(szGenre);
	showGenres();
}

void cMainWindow::on_m_lpGenreToMinus_clicked()
{
	if(!m_lpGenreToItem)
		return;

	QVariant		v				= m_lpGenreToItem->data(Qt::UserRole);
	cGenreCombine*	lpGenreCombine	= v.value<cGenreCombine*>();
	if(!lpGenreCombine)
		return;

	m_GenreCombineList.removeOne(lpGenreCombine);
	showGenres();
}

void cMainWindow::on_m_lpGenreFromPlus_clicked()
{
	bool	bOK;
	QString	szGenre	= QInputDialog::getText(this, tr("Genre"), tr("Enter new genre:"), QLineEdit::Normal, "", &bOK);

	if(!bOK || szGenre.isEmpty())
		return;

	if(m_GenreCombineList.existFrom(szGenre, Qt::CaseInsensitive))
	{
		QMessageBox::information(this, "Warning", "Genre already exists.");
		return;
	}

	m_lpGenreToItem = selectedGenreTo();
	if(!m_lpGenreToItem)
		return;

	QVariant		v				= m_lpGenreToItem->data(Qt::UserRole);
	cGenreCombine*	lpGenreCombine	= v.value<cGenreCombine*>();
	lpGenreCombine->addFrom(szGenre);
	showGenres();
}

void cMainWindow::on_m_lpGenreFromMinus_clicked()
{
	if(!m_lpGenreFromItem)
		return;

	QStandardItem*	lpItem			= selectedGenreTo();
	if(!lpItem)
		return;

	QVariant		v				= lpItem->data(Qt::UserRole);
	cGenreCombine*	lpGenreCombine	= v.value<cGenreCombine*>();
	if(!lpGenreCombine)
		return;
	lpGenreCombine->removeFrom(m_lpGenreFromItem->text());
	showGenres();
}

void cMainWindow::on_m_lpGenreTo_customContextMenuRequested(const QPoint &pos)
{
	m_lpGenreToItem = m_lpGenreToModel->itemFromIndex(ui->m_lpGenreTo->indexAt(pos));

	QMenu*	lpMenu	= new QMenu;
	lpMenu->addAction(QString("Add"), this, SLOT(on_m_lpGenreToPlus_clicked()));

	if(m_lpGenreToItem)
		lpMenu->addAction(QString("Delete"), this, SLOT(on_m_lpGenreToMinus_clicked()));

	lpMenu->exec(QCursor::pos());
}

void cMainWindow::on_m_lpGenreFrom_customContextMenuRequested(const QPoint &pos)
{
	m_lpGenreFromItem	= m_lpGenreFromModel->itemFromIndex(ui->m_lpGenreFrom->indexAt(pos));
	m_lpGenreToItem		= selectedGenreTo();

	if(!m_lpGenreToItem)
		return;

	QMenu*	lpMenu	= new QMenu;
	lpMenu->addAction(QString("Add"), this, SLOT(on_m_lpGenreFromPlus_clicked()));

	if(m_lpGenreFromItem)
		lpMenu->addAction(QString("Delete"), this, SLOT(on_m_lpGenreFromMinus_clicked()));

	lpMenu->exec(QCursor::pos());
}

QStringList cMainWindow::convertGenre(const QStringList& szGenreList)
{
	QStringList	szGenreListNew;

	for(int z = 0;z < szGenreList.count();z++)
		szGenreListNew.append(m_GenreCombineList.to(szGenreList.at(z)));

	szGenreListNew.removeDuplicates();
	szGenreListNew.sort(Qt::CaseInsensitive);
	return(szGenreListNew);
}
