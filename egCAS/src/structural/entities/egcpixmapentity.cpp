/*Copyright (c) 2014, Johannes Maier <maier_jo@gmx.de>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of egCAS nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.*/

#include <QFont>
#include <QSizeF>
#include <QByteArray>
#include <QString>
#include <QBuffer>
#include <QDir>
#include "egcpixmapentity.h"
#include "egcabstractpixmapitem.h"
#include "document/egcabstractdocument.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QCoreApplication>

EgcPixmapEntity::EgcPixmapEntity(void) : m_item(nullptr), m_fileFormat{QString("PNG")}
{
}

EgcPixmapEntity::~EgcPixmapEntity()
{
}

EgcEntityType EgcPixmapEntity::getEntityType(void) const
{
        return EgcEntityType::Picture;
}

QPointF EgcPixmapEntity::getPosition(void) const
{
        if (m_item)
                return m_item->getPosition();
        else
                return QPointF(0.0,0.0);
}

QString EgcPixmapEntity::getFilePath(void) const
{
        if (m_isEmbedded)
                return nullptr;
        else
                return m_path;
}

void EgcPixmapEntity::setFileType(QString type)
{
        if (    type == QString("PNG")
             || type == QString("JPG")
             || type == QString("BMP")
             || type == QString("JPEG"))
                m_fileFormat = type;
}

QString EgcPixmapEntity::getFileType() const
{
        return m_fileFormat;
}

QByteArray EgcPixmapEntity::getB64Encoded(void) const
{
        if (!m_item)
                return QByteArray();

        //write pixmap in bytes as png format
        QByteArray bytes;
        QBuffer buffer(&bytes);
        buffer.open(QIODevice::WriteOnly);
        m_item->getPixmap().save(&buffer, m_fileFormat.toLatin1());

        return bytes.toBase64();
}

bool EgcPixmapEntity::setB64Encoded(QByteArray &bytes)
{
        //write pixmap in bytes as png format
        QByteArray out = QByteArray::fromBase64(bytes);
        QImage image = QImage::fromData(out, m_fileFormat.toLatin1());
        m_item->setPixmap(QPixmap::fromImage(image));

        return true;
}

QSizeF EgcPixmapEntity::getSize(void) const
{
        if (!m_item)
                return QSizeF(0.0, 0.0);

        qreal scale = m_item->getScaleFactor();
        QSizeF size = m_item->getSize();
        size = QSizeF(size.width() * scale, size.height() * scale);

        return size;
}

void EgcPixmapEntity::setSize(QSizeF size)
{
        if (!m_item)
                return;

        QSizeF tmp = m_item->getSize();
        qreal xFactor = size.width()/tmp.width();
        qreal yFactor = size.height()/tmp.height();
        qreal factor = qMin(xFactor, yFactor);
        m_item->setScaleFactor(factor);
}

void EgcPixmapEntity::setItem(EgcAbstractPixmapItem* item)
{
        m_item = item;
}

EgcAbstractPixmapItem*EgcPixmapEntity::getItem()
{
        return m_item;
}

void EgcPixmapEntity::setPosition(QPointF pos)
{
        if (!m_item)
                return;

        m_item->setPos(pos);
}

void EgcPixmapEntity::setFilePath(QString file)
{
        if (!m_item)
                return;
        m_isEmbedded = false;

        m_path = file;
        QPixmap pixmap(file);
        EgcAbstractDocument* doc = getDocument();
        if (!doc)
                return;
        QSizeF size = doc->getMaxItemSize(getPosition());
        QSize pixSize = pixmap.size();
        qreal xFactor = pixSize.width() / size.width();
        qreal yFactor = pixSize.height() / size.height();
        qreal factor = qMax(xFactor, yFactor);
        if (factor > 1.0)
                m_item->setScaleFactor(1.0/factor);

        m_item->setPixmap(pixmap);
}

void EgcPixmapEntity::setIsEmbedded()
{
        m_path = "";
        m_isEmbedded = true;
}

void EgcPixmapEntity::itemChanged(EgcItemChangeType changeType)
{
}

void EgcPixmapEntity::serialize(QXmlStreamWriter& stream, SerializerProperties &properties)
{
        stream.writeStartElement("pic_entity");
        stream.writeAttribute("pos_x", QString("%1").arg(getPosition().x()));
        stream.writeAttribute("pos_y", QString("%1").arg(getPosition().y()));
        stream.writeAttribute("width", QString("%1").arg(getSize().width()));
        stream.writeAttribute("height", QString("%1").arg(getSize().height()));
        stream.writeAttribute("format", m_fileFormat);
        if (!m_isEmbedded) {
                QFileInfo info(properties.filePath);
                QDir path = QDir(info.path());
                QString file = path.relativeFilePath(m_path);
                stream.writeAttribute("path", file);

        } else {
                stream.writeCharacters(getB64Encoded());
        }
        stream.writeEndElement(); // document
}

void EgcPixmapEntity::deserialize(QXmlStreamReader& stream, SerializerProperties& properties)
{
        (void) properties;

        if (stream.name() == QLatin1String("pic_entity")) {
                QXmlStreamAttributes attr = stream.attributes();
                if (attr.hasAttribute("format")) {
                        setFileType(attr.value("format").toString());
                }
                if (attr.hasAttribute("path")) {
                        QString imageFile = attr.value("path").toString();
                        QFileInfo info(properties.filePath);
                        QDir dir(info.path());
                        QString path = QDir::cleanPath(dir.absoluteFilePath(imageFile));
                        QFileInfo file(path);
                        if (!file.exists()) {
                                QString errMsg(QCoreApplication::translate("EgcPixmapEntity"
                                                                           , "File not found while loading document:"));
                                errMsg += QString(" ");
                                errMsg += path;
                                properties.warningMessage = errMsg;
                                stream.skipCurrentElement();
                                return;
                        } else {
                                setFilePath(path);
                        }
                } else {
                        QByteArray in = stream.readElementText().toLatin1();
                        setB64Encoded(in);
                        setIsEmbedded();
                }
                if (attr.hasAttribute("height") && attr.hasAttribute("width")) {
                        qreal h = attr.value("height").toFloat();
                        qreal w = attr.value("width").toFloat();
                        setSize(QSizeF(w, h));
                }
                if (attr.hasAttribute("pos_x") && attr.hasAttribute("pos_y")) {
                        qreal x = attr.value("pos_x").toFloat();
                        qreal y = attr.value("pos_y").toFloat();
                        setPosition(QPointF(x, y));
                }
        }

        if (!stream.isEndElement())
                stream.skipCurrentElement();
}
