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

#include "../entities/egcformulaentity.h"
#include "egcnodevisitor.h"



EgcNodeVisitor::EgcNodeVisitor(EgcFormulaEntity& formula) : m_result{QString::null}, m_formula{&formula},
                                                            m_state{EgcIteratorState::LeftIteration},
                                                            m_childIndex{0}
{
        m_stack.clear();
}

QString EgcNodeVisitor::getResult(void)
{
        EgcNodeIterator iter(*m_formula);
        EgcNode *node;
        EgcNode *previousChildNode;
        QString result;

        //clear stack
        m_stack.clear();

        while(iter.hasNext()) {
                previousChildNode = &iter.peekPrevious();
                node = &iter.next();
                m_state = iter.getLastState();
                if (node->isContainer()) {
                        if (m_state == EgcIteratorState::LeftIteration) {
                                m_childIndex = 0;
                        } else if (   m_state == EgcIteratorState::RightIteration) {
                                m_childIndex = static_cast<EgcContainerNode*>(node)->getNumberChildNodes();
                        } else {
                                quint32 ind;
                                if (static_cast<EgcContainerNode*>(node)->getIndexOfChild(*previousChildNode, ind))
                                      m_childIndex = ind;
                                else
                                      m_childIndex = 0;
                        }
                } else {
                        m_childIndex = 0;
                }
                node->accept(this);
                result += m_result;
                m_result = QString::null;
        };

        //add the result from the stack
        if (!m_stack.isEmpty())
                result += m_stack.pop();

        return result;
}

