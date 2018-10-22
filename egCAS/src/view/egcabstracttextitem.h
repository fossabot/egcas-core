/*
Copyright (c) 2015, Johannes Maier <maier_jo@gmx.de>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of the egCAS nor the names of its
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

#ifndef EGCABSTRACTTEXTITEM_H
#define EGCABSTRACTTEXTITEM_H

#include <QString>
#include <QFont>
#include "egcasiteminterface.h"

class EgcAbstractTextItem : public EgcasItemInterface
{
public:
        /**
         * @brief ~EgcAbstractFormulaItem virtual destructor in order to be able to delete subclasses
         */
        virtual ~EgcAbstractTextItem() {}
        /**
         * @brief setText set the given text
         * @param text the text to set
         */
        virtual void setText(QString text) = 0;
        /**
         * @brief getText returns the text of the associated item
         * @return the text of this item
         */
        virtual QString getText(void) = 0;
        /**
         * @brief setHtmlText set text that is already in html style
         * @param text the text to set
         */
        virtual void setHtmlText(QString text) = 0;
        /**
         * @brief getHtmlText get text that is already in html style
         * @return text in html format
         */
        virtual QString getHtmlText(void) = 0;
        /**
         * @brief setEditMode sets this item in edit mode, so that the user can start to write text
         * @param activateEditing activate the edition if true, deactivate it if false
         */
        virtual void setEditMode(bool activateEditing = true) = 0;
};

#endif // EGCABSTRACTTEXTITEM_H
