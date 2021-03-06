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

#ifndef EGCTEXTITEM_H
#define EGCTEXTITEM_H

#include <QGraphicsTextItem>
#include "egcabstracttextitem.h"
#include "egcabstractitem.h"

class EgcAbstractTextEntity;

class EgcTextItem: public QGraphicsTextItem, public EgcAbstractTextItem, public EgcAbstractItem
{
public:
        ///std constructor
        explicit EgcTextItem(QGraphicsItem *parent = 0);
        ///constructor for text message
        EgcTextItem(const QString & text, QGraphicsItem * parent = 0);
        /// point constructor
        explicit EgcTextItem(const QPointF point, QGraphicsItem *parent = 0);
        ///std destructor
        virtual ~EgcTextItem();
        /**
         * @brief setEntity set a pointer to the entity that contains the logical structure / frontend for the view
         * @param entity a pointer to the entity that is associated with this object
         */
        void setEntity(EgcAbstractTextEntity* entity);
        /**
         * @brief getPosItemIface needs to be overwritten by subclasses to get the position of the item
         * @return the Position of the item
         */
        virtual QPointF getPosition( void ) const override;
        /**
         * @brief setPosItemIface needs to be overwritten by subclasses to set the position of the item
         * @param point the position to set.
         */
        virtual void setPos(const QPointF& point) override;
        /**
         * @brief setText set the given text
         * @param text the text to set
         */
        virtual void setText(QString text) override;
        /**
         * @brief getText returns the text of the associated item
         * @return the text of this item
         */
        virtual QString getText(void) override;
        /**
         * @brief setEditMode sets this item in edit mode, so that the user can start to write text
         */
        virtual void setEditMode(void) override;
        /**
         * @brief override paint from QGraphicsItem
         * @param painter pointer to painter
         * @param option pointer to style options
         * @param widget pointer to parent widget
         */
        void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
        /**
         * @brief getEnity returns a pointer to the entity that contains the logical structure for the text
         * @return the entity that is associated with the text item
         */
        EgcAbstractTextEntity* getEnity(void) const;

protected:
        /**
         * @brief getEgcScene needs to be implemented by the subclasses since we cannot inherit from QGraphicsitem (the
         * subclasses already inherit from it - and we don't want to make it complicated)
         * @return pointer to EgCasScene
         */
        virtual EgCasScene* getEgcScene(void) override;
        /**
         * @brief itemChange reimplements change function of QGraphicsItem to be able to realize a grid
         * @param change enum that describes state changes that are notified
         * @param value the value that has changed
         * @return the value that has been adjusted
         */
        QVariant itemChange(GraphicsItemChange change, const QVariant &value);
        /**
         * @brief mouseDoubleClickEvent overrides mousePressEvent from QGraphicsItem
         * @param event pointer to QGraphicsSceneMouseEvent
         */
        void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
        /**
         * @brief focusOutEvent overrides
         * @param event
         */
        virtual void focusOutEvent(QFocusEvent*event) override;
        /**
         * @brief focusInEvent overwrite focus in event to be able to work with the position iterator
         * @param event focus event
         */
        virtual void focusInEvent(QFocusEvent * event) override;
        /**
         * @brief keyPressEvent overwrites key events
         * @param keyEvent the key event to react on
         */
        virtual void keyPressEvent(QKeyEvent *keyEvent) override;
        /**
         * @brief isAtLeftEnd checks if cursor is at the left end of the text field
         * @return true if cursor is at the left end, false otherwise
         */
        bool isAtLeftEnd(void);
        /**
         * @brief isAtRightEnd checks if cursor is at the right end of the text field
         * @return true if cursor is at the right end, false otherwise
         */
        bool isAtRightEnd(void);
        /**
         * @brief isAtTop checks if cursor is at the toop of the text field
         * @return true if cursor is at the top, false otherwise
         */
        bool isAtTop(void);
        /**
         * @brief isAtBottom checks if cursor is at the bottom of the text field
         * @return true if cursor is at the bottom, false otherwise
         */
        bool isAtBottom(void);
        /**
         * @brief type returns the type of the item
         * @return item type
         */
        virtual int type() const override;
        /**
         * @brief bRect returns the bounding rect of the abstract item (interface to concrete item)
         * @return bounding rect rectangle
         */
        virtual QRectF bRect(void) const override;
        /**
         * @brief getPos return the current position of the item
         * @return the current position of the item
         */
        virtual QPointF getPos(void) override;

signals:


private:        
        Q_DISABLE_COPY(EgcTextItem)
        EgcAbstractTextEntity* m_entity;        ///< pointer to text entity (no ownership)
        bool m_editingActivated;                ///< editing has already been activated
};

#endif // EGCTEXTITEM_H


