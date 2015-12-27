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


#include "egcidnodeiter.h"
#include "structural/entities/egcformulaentity.h"
#include "structural/specialNodes/egcnode.h"
#include "view/egcscreenpos.h"
#include "structural/concreteNodes/egcnumbernode.h"
#include "structural/concreteNodes/egcvariablenode.h"
#include "structural/specialNodes/egcflexnode.h"
#include "structural/iterator/egcnodeiterator.h"


EgcIdNodeIter::EgcIdNodeIter(const EgcFormulaEntity& formula) : m_nodeIterNext{new EgcNodeIterator(formula)},
                                                                m_nodeIterPrev{new EgcNodeIterator(formula)},
                                                                m_histState{EgcIteratorState::LeftIteration},
                                                                m_lookup(formula.getMathmlMappingCRef()), //gcc bug
                                                                m_histId{0}
{
        toBack();
}

EgcIdNodeIter::~EgcIdNodeIter()
{
}

bool EgcIdNodeIter::hasNext(void) const
{
        return m_nodeIterNext->hasNext();
}

bool EgcIdNodeIter::hasPrevious(void) const
{
        return m_nodeIterPrev->hasPrevious();
}

EgcNode &  EgcIdNodeIter::next(void)
{
        EgcNode* next;
        EgcNodeIterator tempIter = *m_nodeIterNext;

        next = &nextNodeWithId();
        *m_nodeIterPrev = tempIter;
        if (m_nodeIterPrev->hasNext())
                m_nodeIterPrev->next();

        return *next;
}

EgcNode & EgcIdNodeIter::previous(void)
{
        EgcNode* prev;
        EgcNodeIterator tempIter = *m_nodeIterPrev;

        prev = &prevNodeWithId();
        *m_nodeIterNext = tempIter;
        if (m_nodeIterNext->hasPrevious())
                m_nodeIterNext->previous();

        return *prev;
}

EgcNode& EgcIdNodeIter::peekNext(void) const
{
        return m_nodeIterNext->peekNext();
}

EgcNode& EgcIdNodeIter::peekPrevious(void) const
{
        return m_nodeIterPrev->peekPrevious();
}

void EgcIdNodeIter::toBack(void)
{
        m_nodeIterNext->toBack();
        m_nodeIterPrev->toBack();
        if (!mathmlIdExisting(&m_nodeIterPrev->peekPrevious(), m_nodeIterPrev->getStatePreviousNode(), nullptr, &m_nodeIterPrev->peekNext()))
                prevNodeWithId();
}

void EgcIdNodeIter::toFront(void)
{
        m_nodeIterNext->toFront();
        m_nodeIterPrev->toFront();
        if (!mathmlIdExisting(&m_nodeIterNext->peekNext(), m_nodeIterNext->getStateNextNode(), &m_nodeIterNext->peekPrevious(), nullptr))
                nextNodeWithId();
}

const quint32& EgcIdNodeIter::id(void)
{
        return m_histId;
}

EgcIteratorState EgcIdNodeIter::getLastState(void) const
{
        return m_histState;
}

bool EgcIdNodeIter::mathmlIdExisting(EgcNode* node, EgcIteratorState state, EgcNode* previousNode, EgcNode* nextNode) const
{
        if (getMathmlId(node, state, previousNode, nextNode))
                return true;
        else
                return false;
}

quint32 EgcIdNodeIter::getMathmlId(EgcNode* node, EgcIteratorState state, EgcNode* previousNode, EgcNode* nextNode) const
{
        quint32 id = 0;
        
        if (!node)
                return id;
        
        if (    state == EgcIteratorState::LeftIteration
             || state == EgcIteratorState::RightIteration) {
                return m_lookup.getIdFrame(*node);
        } else {  //EgcIteratorState::MiddleIteration
                QList<quint32> list = m_lookup.getIdsNonFrame(*node);
                quint32 childIndex = 0;

                if (node->isContainer()) {
                        if (previousNode) {
                                if (!static_cast<EgcContainerNode*>(node)->getIndexOfChild(*previousNode, childIndex))
                                        childIndex = 0;
                        } else if (nextNode) {
                                if (!static_cast<EgcContainerNode*>(node)->getIndexOfChild(*nextNode, childIndex)) {
                                        childIndex = 0;
                                } else {
                                        if (childIndex)
                                                childIndex--;
                                }
                        }
                        if (list.size() > childIndex) {
                                id = list.at(childIndex);
                        }
                } else { // is not a container, so it has no further frames
                        id = m_lookup.getIdFrame(*node);
                }
        }

        return id;
}

EgcNode& EgcIdNodeIter::prevNodeWithId(void)
{
        EgcNode* prevNode;
        EgcNode* retval;
        EgcNode* node;
        EgcNode* next;
        bool firstRun = true;

        do {
                if (firstRun)
                        next = &m_nodeIterPrev->peekNext();
                node = &m_nodeIterPrev->previous();
                if (firstRun) {
                        retval = node;
                        m_histState = m_nodeIterPrev->getLastState();
                        m_histId = getMathmlId(node, m_histState, nullptr, next);
                }
                prevNode = &m_nodeIterPrev->peekPrevious();

        } while (    !mathmlIdExisting(prevNode, m_nodeIterPrev->getStatePreviousNode(), nullptr, node)
                  && m_nodeIterPrev->hasPrevious());

        return *retval;
}

EgcNode& EgcIdNodeIter::nextNodeWithId(void)
{
        EgcNode* nextNode;
        EgcNode* retval;
        EgcNode* node;
        EgcNode* prev;
        bool firstRun = true;

        do {
                if (firstRun)
                     prev = &m_nodeIterNext->peekPrevious();
                node = &m_nodeIterNext->next();
                if (firstRun) {
                        retval = node;
                        m_histState = m_nodeIterNext->getLastState();
                        m_histId = getMathmlId(node, m_histState, prev, nullptr);
                        firstRun = false;
                }
                nextNode = &m_nodeIterNext->peekNext();

        } while (    !mathmlIdExisting(nextNode, m_nodeIterNext->getStateNextNode(), node, nullptr)
                  && m_nodeIterNext->hasNext());

        return *retval;
}
