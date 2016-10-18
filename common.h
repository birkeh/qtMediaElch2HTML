#ifndef COMMON_H
#define COMMON_H


#include <QString>
#include <QDomElement>
#include <QDomNamedNodeMap>
#include <QDomNode>


#define CHECKELEMENTS(element, check, value) if(!element.toElement().tagName().compare(check, Qt::CaseInsensitive)) value = element.toElement().text()
#define CHECKELEMENTP(element, check, value) if(!element.toElement().tagName().compare(check, Qt::CaseInsensitive)) value = QDate::fromString(element.toElement().text(), "yyyy-MM-dd")
#define CHECKELEMENTI(element, check, value) if(!element.toElement().tagName().compare(check, Qt::CaseInsensitive)) value = element.toElement().text().toInt()
#define CHECKELEMENTD(element, check, value) if(!element.toElement().tagName().compare(check, Qt::CaseInsensitive)) value = element.toElement().text().toDouble()
#define CHECKELEMENTL(element, check, value) if(!element.toElement().tagName().compare(check, Qt::CaseInsensitive)) value.append(element.toElement().text())
#define CHECKELEMENTB(element, check, value) if(!element.toElement().tagName().compare(check, Qt::CaseInsensitive)) if(!element.toElement().text().compare("true", Qt::CaseInsensitive)) value = true; else value = false
#define CHECKELEMENTF(element, check, funct) if(!element.toElement().tagName().compare(check, Qt::CaseInsensitive)) funct(child.toElement())

typedef struct tagACTOR
{
	QString	szName;
	QString	szRole;
	QString	szThumb;
}	ACTOR, *LPACTOR;

typedef struct tagTHUMB
{
	QString	szPreview;
	QString	szThumb;
}	THUMB, *LPTHUMB;

typedef struct tagVIDEO
{
	double	dAspect;
	QString	szCodec;
	qint16	iDuration;
	qint16	iHeight;
	QString	szScantype;
	qint16	iWidth;
}	VIDEO, *LPVIDEO;

typedef struct tagAUDIO
{
	qint16	iChannels;
	QString	szCodec;
	QString	szLanguage;
}	AUDIO, *LPAUDIO;

typedef struct tagSUBTITLE
{
	QString	szLanguage;
}	SUBTITLE, *LPSUBTITLE;

typedef struct tagSEASONPIC
{
	qint16	iSeason;
	QString	szBanner;
	QString	szLandscape;
	QString	szPoster;
}	SEASONPIC, *LPSEASONPIC;

QString	getAttributeStr(const QDomElement& Element, const QString& szAttribute);
qint16	getAttributeInt(const QDomElement& Element, const QString& szAttribute);


#endif // COMMON_H
