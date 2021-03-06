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

#ifndef EGCASSCENE_H
#define EGCASSCENE_H

#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QScopedPointer>
#include <QLine>
#include "entities/egcabstractformulaentity.h"
#include "entities/egcabstractpixmapentity.h"
#include "entities/egcabstracttextentity.h"
#include "document/egcabstractdocument.h"
#include "egcworksheet.h"
#include "grid.h"

class EgcFormulaItem;
class EgcPixmapItem;
class EgcTextItem;
class EgcCrossItem;

/**
 * @brief The SnapDirection enum specifies the snap direction in which direction the cursor should snap comming from an
 * item
 */
enum class EgcSceneSnapDirection{up, down, left, right};

class EgCasScene : public QGraphicsScene
{
        Q_OBJECT
public:        
        ///constructor of EgCasScene
        explicit EgCasScene(EgcAbstractDocument& doc, QObject *parent = 0);
        virtual ~EgCasScene();
        /**
         * @brief grid return a reference to the grid object
         * @return the grid size as reference
         */
        egcas::Grid& grid(void);
        /**
         * @brief setGrid set the grid size to be used as background
         * @param grid the grid size to be set
         */
        void setGrid(QSizeF grid);
        /**
         * @brief addText add text to the scene (overrides the standard function)
         * @param text the text to add
         * @param font the font to use
         * @return a pointer to the text item
         */
        QGraphicsTextItem * addText(const QString & text, const QFont & font = QFont());
        /**
         * @brief addText add a text to the graphicsscene
         * @param text the text entity to be rendered
         * @param point  the point where (position) to add the text on the scene
         * @return a pointer to the text added
         */
        EgcTextItem* addText(EgcAbstractTextEntity& text, QPointF point = QPointF(0.0,0.0));
        /**
         * @brief addPixmap add a pixmap to the scene (overrides the standard function and adds a egcaspixmapitem)
         * @param pixmap the pixmap to add to the scene
         * @return a pointer to the pixmap item
         */
        QGraphicsPixmapItem * addPixmap(const QPixmap & pixmap);
        /**
         * @brief addPixmap  add a pixmap to the graphicsscene
         * @param pixmap the pixmap entity to be rendered
         * @param point  the point where (position) to add the pixmap on the scene
         * @return a pointer to the pixmap added
         */
        EgcPixmapItem* addPixmap(EgcAbstractPixmapEntity& pixmap, QPointF point = QPointF(0.0,0.0));
        /**
         * @brief addFormula add a formula to the graphicsscene
         * @param formula the formula to be rendered
         * @param point the point where (position) to add the formula on the scene
         * @return a pointer to the formula added
         */        
        EgcFormulaItem * addFormula(const QString & formula, QPointF point);
        /**
         * @brief addFormula  add a formula to the graphicsscene
         * @param formula the formula entity to be rendered
         * @param point  the point where (position) to add the formula on the scene
         * @return a pointer to the formula added
         */
        EgcFormulaItem* addFormula(EgcAbstractFormulaEntity& formula, QPointF point = QPointF(0.0,0.0));
        /**
         * @brief setFormulaCursor shows the cursor for modifying a formula at the given position
         * @param line the position where to paint the cursor
         */
        void setFormulaCursor(const QLineF& line);
        /**
         * @brief setUnderlineCursor shows the formula underline cursor at the given position
         * @param line the position where to paint the underline cursor
         */
        void setUnderlineCursor(const QLineF& line);
        /**
         * @brief hideFormulaCursors hides all formula cursors
         */
        void hideFormulaCursors( void );
        /**
         * @brief itemYieldsFocus is called from an item that wants to hand over its focus and give it back to the scene.
         * @param direction the direction in which the cursor should snap in
         * @param item the item which hands over the focus
         */
        void itemYieldsFocus(EgcSceneSnapDirection direction, QGraphicsItem& item);
        /**
         * @brief triggerFormulaCreation
         * @param point the position where to create a formula
         * @param action the action emitted when a key has been pressed
         */
        void triggerFormulaCreation(QPointF point, EgcAction action);
        /**
         * @brief deleteItem removes the given item from the scene and deletes it
         * @param item item to delete
         * @return true if deleting the item was successful, false otherwise
         */
        bool deleteItem(EgcAbstractFormulaItem* item);
        /**
         * @brief deleteItem removes the given item from the scene and deletes it
         * @param item item to delete
         * @return true if deleting the item was successful, false otherwise
         */
        bool deleteItem(EgcAbstractPixmapItem* item);
        /**
         * @brief deleteItem removes the given item from the scene and deletes it
         * @param item item to delete
         * @return true if deleting the item was successful, false otherwise
         */
        bool deleteItem(EgcAbstractTextItem* item);
        /**
         * @brief moveItems moves all following (from the given point onwards) items up or down by the gridsize
         * @param moveDwn if true this moves the items downwards, upwards otherwise
         * @param point the point where onwards the movement shall be started
         */
        void moveItems(bool moveDwn, QPointF point);
        /**
         * @brief worksheet returns a reference to the worksheet
         * @return the worksheet associated with this scene
         */
        EgcWorksheet& worksheet(void);
        /**
         * @brief addPage adds space for one page to the scene (items are not moved on the scene)
         * @param pageIndex add page before given page index. The new page will have the given index.
         */
        void addPage(quint32 pageIndex);
        /**
         * @brief removePage removes space for one page from the scene (items are not moved on the scene). If scene is
         * only one size big, the space for that page will NOT be removed.
         * @param pageIndex remove the page at given page index
         */
        void removePage(quint32 pageIndex);
        /**
         * @brief moveDown move given item downwards on the page. If the item collides with the worksheet borders, the
         * item will be moved onto the next page. If there is no page anymore, one will be added
         * @param item the item to move
         * @param useItemPos normally the items bounding rectangle will be used to calculate if the item collides with
         * the border. However if true only the items position will be used to do the calculations.
         */
        void moveDown(QGraphicsItem* item, bool useItemPos = false);
        /**
         * @brief moveUp move given item upwards on the page. If the item collides with the worksheet borders, the
         * item will be moved onto the previous page. If there is no item on the last page, the page will be deleted.
         * @param item the item to move
         * @param useItemPos normally the items bounding rectangle will be used to calculate if the item collides with
         * the border. However if true only the items position will be used to do the calculations.
         */
        void moveUp(QGraphicsItem* item, bool useItemPos = false);
        /**
         * @brief anyItemOnPage checks if any item is left on the given page
         * @param pageIndex the index of the page to check
         * @return true if there is any item, false otherwise
         */
        bool anyItemOnPage(quint32 pageIndex) const;
        /**
         * @brief document returns a reference to the internal document
         * @return the document that is associated with this scene
         */
        EgcAbstractDocument& document(void) const;
        /**
         * @brief getLastCursorPositon returns the last cursor position
         * @return the last cursor position
         */
        QPointF getLastCursorPositon(void);
        /**
         * @brief deleteAll removes all items from the scene
         */
        void deleteAll(void);

public slots:
        /**
         * @brief routeAction the given action (from any source) is routed to an active formula or the crosshair to
         * trigger a formula creation
         * @param action the action to route
         */
        void routeAction(EgcAction action);

protected:
        virtual void drawBackground(QPainter *painter, const QRectF &rect);
        /**
         * @brief mouseReleaseEvent reimplements mouse release event to show cursor
         * @param event mouse event
         */
        virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event) override;
        /**
         * @brief deleteItem delete a QGraphicsItem from the scene
         * @param item item to delete
         * @return true if successful, false otherwise
         */
        bool deleteItem(QGraphicsItem* item);
        /**
         * @brief keyPressEvent overrides key press event from QGraphicsScene
         * @param event contains the key pressed
         */
        virtual void keyPressEvent(QKeyEvent *keyEvent);
signals:
        /**
         * @brief createFormula signal that is emitted if a empty formula shall be created at the given position.
         * @param point the position where to create a formula
         * @param event pointer to key press event
         */
        void createFormula(QPointF point, EgcAction action);

private:
        /**
         * @brief drawHorizontalLines draws the horizontal lines of the grid on each worksheet
         * @param painter painter of the scene
         * @param rect active rectangle of the scene
         * @param leftX left x coordinate where active area begins
         * @param rightX right x coordinate where active area ends
         */
        void drawHorizontalLines(QPainter* painter, const QRectF&rect, qreal leftX, qreal rightX);
        /**
         * @brief drawVerticalLines draws the vertical lines of the grid on each worksheet
         * @param painter painter of the scene
         * @param rect active rectangle of the scene
         * @param leftX left x coordinate where active area begins
         * @param rightX right x coordinate where active area ends
         */
        void drawVerticalLines(QPainter* painter, const QRectF&rect, qreal leftX, qreal rightX);
        /**
         * @brief drawActiveAreaBorder draws the border on the worksheet around the active area
         * @param painter painter of the scene
         * @param rect active rectangle of the scene
         */
        void drawActiveAreaBorder(QPainter* painter, const QRectF&rect);
        /**
         * @brief deleteFocusedItem check if focused item shall be deleted
         * @return true if it shall be deleted, false otherwise
         */
        bool deleteFocusedItem(void);

        egcas::Grid m_grid;                     ///< returns the grid
        EgcWorksheet m_worksheet;               ///< the worksheet we are working on
        QGraphicsLineItem* m_cursor;            ///< formula cursor for modifying formula
        QGraphicsLineItem* m_nodeUnderline;     ///< node cursor to show user the context of changes in a formula
        EgcCrossItem* m_cross;                  ///< crosshair (cursor) to be able to see enter position
        EgcAbstractDocument& m_document;        ///< reference to document that contains the scene
};

#endif // EGCASSCENE_H
