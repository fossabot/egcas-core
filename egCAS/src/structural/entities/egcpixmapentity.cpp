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
#include "egcpixmapentity.h"
#include "egcabstractpixmapitem.h"
#include "document/egcabstractdocument.h"
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

EgcPixmapEntity::EgcPixmapEntity(void) : m_item(nullptr)
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

QByteArray EgcPixmapEntity::getB64Encoded(void) const
{
        if (!m_item)
                return QByteArray();

        //write pixmap in bytes as png format
        QByteArray bytes;
        QBuffer buffer(&bytes);
        buffer.open(QIODevice::WriteOnly);
        m_item->getPixmap().save(&buffer, "PNG");

        return bytes.toBase64();
}

bool EgcPixmapEntity::setB64Encoded(QByteArray &bytes)
{
        //write pixmap in bytes as png format
        QByteArray out = QByteArray::fromBase64(bytes);
        QImage image = QImage::fromData(out, "PNG");
        m_item->setPixmap(QPixmap::fromImage(image));

        return true;
}

QSizeF EgcPixmapEntity::getSize(void) const
{
        if (!m_item)
                return QSizeF(0.0, 0.0);

        return m_item->getSize();
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

void EgcPixmapEntity::itemChanged(EgcItemChangeType changeType)
{
}

void EgcPixmapEntity::serialize(QXmlStreamWriter& stream)
{
        stream.writeStartElement("pic_entity");
        stream.writeAttribute("pos_x", QString("%1").arg(getPosition().x()));
        stream.writeAttribute("pos_y", QString("%1").arg(getPosition().y()));
        stream.writeAttribute("width", QString("%1").arg(getSize().width()));
        stream.writeAttribute("height", QString("%1").arg(getSize().height()));
        if (!m_isEmbedded)
                stream.writeAttribute("path", m_path);
        else
                stream.writeCharacters(getB64Encoded());
        stream.writeEndElement(); // document
}

void EgcPixmapEntity::deserialize(QXmlStreamReader& stream, quint32 version)
{
        (void) version;

        if (stream.name() == QLatin1String("pic_entity")) {
                QXmlStreamAttributes attr = stream.attributes();
                if (attr.hasAttribute("path")) {
                        QString path = attr.value("path").toString();
                        setFilePath(path);
                } else {
                        QByteArray in = stream.readElementText().toLatin1();
                        setB64Encoded(in);
                }
                if (attr.hasAttribute("pos_x") && attr.hasAttribute("pos_y")) {
                        qreal x = attr.value("pos_x").toFloat();
                        qreal y = attr.value("pos_y").toFloat();
                        setPosition(QPointF(x, y));
                }
                if (attr.hasAttribute("height") && attr.hasAttribute("width")) {
                        qreal h = attr.value("height").toFloat();
                        qreal w = attr.value("width").toFloat();
                        setSize(QSizeF(w, h));
                }
        }

        stream.skipCurrentElement();
}
