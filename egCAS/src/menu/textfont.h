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
#ifndef TEXTFONT_H
#define TEXTFONT_H

#include <QScopedPointer>
#include <QWidget>
#include "entities/egcformulaentity.h"

class QSpinBox;
class EgcDocument;
class QToolBar;
class QFontComboBox;


class TextFont : public QWidget
{
        Q_OBJECT
public:
        TextFont(EgcDocument* doc, QToolBar* toolbar, QWidget* parent = nullptr);
        ~TextFont();
private slots:
        /**
         * @brief changeSize change the font size of the text
         */
        void changeSize(int size);
        /**
         * @brief changeFont change the font of the text
         */
        void changeFont(QFont font);
        /**
         * @brief onSelectionChange is called if the selection of a formula or any other entity changes
         */
        void onSelectionChange(void);
        /**
         * @brief change the font of the text to bold style
         */
        void boldText(bool bold);
        /**
         * @brief change the font of the text to underlined style
         */
        void underlinedText(bool underlined);
        /**
         * @brief change the font of the text to italic style
         */
        void italicText(bool italic);

private:

        Q_DISABLE_COPY(TextFont)

        QSpinBox* m_box;
        EgcDocument* m_document;
        QFontComboBox* m_fontBox;
        QAction *mnu_bold_text;
        QAction *mnu_italic_text;
        QAction *mnu_underlined_text;
};

#endif // TEXTFONT_H
