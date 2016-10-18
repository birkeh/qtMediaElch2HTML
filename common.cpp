#include "common.h"


QString getAttributeStr(const QDomElement& Element, const QString& szAttribute)
{
	QDomNamedNodeMap	Attributes	= Element.attributes();

	for(int z = 0;z < Attributes.count();z++)
	{
		QDomNode	Node	= Attributes.item(z);
		if(!Node.toAttr().name().compare(szAttribute, Qt::CaseInsensitive))
			return(Node.toAttr().nodeValue());
	}
	return("");
}

qint16 getAttributeInt(const QDomElement& Element, const QString& szAttribute)
{
	QDomNamedNodeMap	Attributes	= Element.attributes();

	for(int z = 0;z < Attributes.count();z++)
	{
		QDomNode	Node	= Attributes.item(z);
		if(!Node.toAttr().name().compare(szAttribute, Qt::CaseInsensitive))
			return(Node.toAttr().nodeValue().toInt());
	}
	return(-1);
}
