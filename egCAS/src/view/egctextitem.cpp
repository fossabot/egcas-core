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

#include <QTextCursor>
#include "egctextitem.h"
#include "egcasscene.h"

EgcTextItem::EgcTextItem(QGraphicsItem*parent) : QGraphicsTextItem{parent}, m_entity{nullptr}
{
        setFlags(ItemIsMovable | ItemClipsToShape | ItemIsSelectable | ItemIsFocusable | ItemSendsScenePositionChanges);
}

EgcTextItem::EgcTextItem(const QString & text, QGraphicsItem * parent) : EgcTextItem{parent}
{
        QGraphicsTextItem::setPlainText(text);
}

EgcTextItem::EgcTextItem(const QPointF point, QGraphicsItem *parent) : EgcTextItem{parent}
{
        QGraphicsTextItem::setPos(point);
}

QVariant EgcTextItem::itemChange(GraphicsItemChange change, const QVariant &value)
 {
     if (change == ItemPositionChange && scene()) {
         // value is the new position.
         QPointF newPos = value.toPointF();
         QSizeF grid = qobject_cast<EgCasScene*>(this->scene())->grid();
         newPos.setX(qRound(newPos.x()/grid.width()) * grid.width() );
         newPos.setY(qRound(newPos.y()/grid.height()) * grid.height() );

         return newPos;
     }
     return QGraphicsItem::itemChange(change, value);
 }

void EgcTextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent*event)
{
        setTextInteractionFlags(Qt::TextEditable | Qt::TextEditorInteraction);
        QTextCursor cursor = textCursor();
        cursor.movePosition(QTextCursor::End);
        setTextCursor(cursor);
        setFocus();
        QGraphicsItem::mouseDoubleClickEvent(event);
}

void EgcTextItem::focusOutEvent(QFocusEvent *event)
{
        QTextCursor cursor(textCursor());
        cursor.clearSelection();
        setTextCursor(cursor);
        setTextInteractionFlags(Qt::NoTextInteraction);
        QGraphicsItem::focusOutEvent(event);
}

void EgcTextItem::setEntity(EgcAbstractTextEntity* entity)
{
        m_entity = entity;
}

QPointF EgcTextItem::getPosition( void ) const
{
        return pos();
}

void EgcTextItem::setPosition(const QPointF &point)
{
        QGraphicsTextItem::setPos(snapGrid(point));
}

void EgcTextItem::setFont(const QFont& font)
{
        QGraphicsTextItem::setFont(font);
}

QFont EgcTextItem::getFont(void) const
{
        return font();
}

void EgcTextItem::setText(QString text)
{
        setPlainText(text);
}

QString EgcTextItem::getText(void)
{
        return toPlainText();
}

QSizeF EgcTextItem::getGrid(void)
{
        QSizeF grid;

        QGraphicsScene *scene = this->scene();
        if (scene) {
                grid = static_cast<EgCasScene*>(scene)->grid();
        }

        return grid;
}
