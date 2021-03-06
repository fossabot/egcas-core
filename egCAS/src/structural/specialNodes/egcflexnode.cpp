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

#include <QScopedPointer>
#include "egcflexnode.h"
#include "../egcnodecreator.h"
#include "../visitor/egcnodevisitor.h"


EgcFlexNode::EgcFlexNode() : m_childs(1)
{

}

EgcFlexNode::EgcFlexNode(const EgcFlexNode& orig) : EgcContainerNode(orig)
{
        m_childs.clear();
        EgcNode *originalChild;
        quint32 i;
        quint32 cnt = static_cast<quint32>(orig.m_childs.count());
        QScopedPointer<EgcNode> child;
        for (i = 0; i < cnt; i++) {
                originalChild = orig.getChild(i);
                if (originalChild)
                        child.reset(originalChild->copy());

                //set the parent also
                if(child.data()) {
                        child->provideParent(this);
                        m_childs.append(child.take());
                }
        }

        if (m_childs.empty()) {
                m_childs.resize(1);
                m_childs[0] = nullptr;
        }
}

EgcFlexNode::EgcFlexNode(EgcFlexNode&& orig) : EgcContainerNode(orig)
{
        m_childs.clear();
        EgcNode *originalChild;
        quint32 i;
        quint32 cnt = static_cast<quint32>(orig.m_childs.count());
        QScopedPointer<EgcNode> child;
        for (i = 0; i < cnt; i++) {
                originalChild = orig.getChild(i);
                if (originalChild)
                        child.reset(orig.takeOwnership(*originalChild));

                //set the parent also
                if(child.data()) {
                        child->provideParent(this);
                        m_childs.append(child.take());
                }
        }

        if (m_childs.empty()) {
                m_childs.resize(1);
                m_childs[0] = nullptr;
        }
}

EgcFlexNode::~EgcFlexNode()
{
        quint32 i;
        quint32 cnt = static_cast<quint32>(m_childs.count());
        EgcNode* child;

        if (!m_childs.empty()) {
                for (i = 0; i < cnt; i++) {
                        child = m_childs.at(static_cast<int>(i));
                        delete child;
                        child = nullptr;
                }
                m_childs.clear();
        }
        m_childs.resize(1);
        m_childs[0] = nullptr;
}

EgcFlexNode& EgcFlexNode::operator=(const EgcFlexNode &rhs)
{
        quint32 i;
        quint32 cnt = static_cast<quint32>(m_childs.count());
        EgcNode* child;
        quint32 cntRhs = rhs.getNumberChildNodes();

        //test if the object to be assigned to is the same as the rhs
        if (this == &rhs)
                return *this;

        //delete the old content
        if (!m_childs.empty()) {
                for (i = 0; i < cnt; i++) {
                        child = m_childs.at(static_cast<int>(i));
                        delete child;
                }
                m_childs.clear();
        }

        //and create a new one
        if (cntRhs != 0) {
                for (i = 0; i < cntRhs; i++) {
                        child = rhs.getChild(i);
                        QScopedPointer<EgcNode> childCopy;
                        if (child) {
                                childCopy.reset(child->copy());
                                child->provideParent(this);
                        }
                        m_childs.append(childCopy.take());
                }
        }

        return *this;
}

EgcFlexNode& EgcFlexNode::operator=(EgcFlexNode&& rhs)
{
        quint32 i;
        quint32 cnt = static_cast<quint32>(m_childs.count());
        EgcNode* child;
        quint32 cntRhs = rhs.getNumberChildNodes();

        //test if the object to be assigned to is the same as the rhs
        if (this == &rhs)
                return *this;

        //delete the old content
        if (!m_childs.empty()) {
                for (i = 0; i < cnt; i++) {
                        child = m_childs.at(static_cast<int>(i));
                        delete child;
                }
                m_childs.clear();
        }

        //and create a new one
        if (cntRhs != 0) {
                for (i = 0; i < cntRhs; i++) {
                        child = rhs.getChild(i);
                        QScopedPointer<EgcNode> childMove;
                        if (child) {
                                childMove.reset(rhs.takeOwnership(*child));
                                child->provideParent(this);
                        }
                        m_childs.append(childMove.take());
                }
        }

        return *this;
}

bool EgcFlexNode::valid(void)
{
        quint32 i;
        quint32 cnt = static_cast<quint32>(m_childs.count());
        EgcNode* child;

        if (m_childs.empty())
                return false;

        for (i = 0; i < cnt; i++) {
                child = m_childs.at(static_cast<int>(i));
                if (child)
                        if (!child->valid())
                                return false;
        }

        return true;
}

void EgcFlexNode::notifyContainerOnChildDeletion(EgcNode* child)
{
        int ind = m_childs.indexOf(child);
        if (ind > 0) {
                m_childs[ind] = nullptr;
        }
}

void EgcFlexNode::adjustChildPointers(EgcNode &old_child, EgcNode &new_child)
{
        int ind = m_childs.indexOf(&old_child);
        if (ind > 0) {
                m_childs[ind] = &new_child;
        }
}

EgcNode* EgcFlexNode::takeOwnership(EgcNode &child)
{
        EgcNode* retval = nullptr;

        int ind = m_childs.indexOf(&child);
        if (ind >= 0) {
                m_childs[ind] = nullptr;
                child.provideParent(nullptr);
                retval = &child;
        }

        return retval;
}

void EgcFlexNode::accept(EgcNodeVisitor *visitor)
{
        visitor->visit(this);
}

EgcNode* EgcFlexNode::getChild(quint32 index) const
{
        if (m_childs.empty())
                return nullptr;

        if (index < static_cast<quint32>(m_childs.count())) {
                return m_childs.at(static_cast<int>(index));
        } else {
                return nullptr;
        }
}

bool EgcFlexNode::setChild(quint32 index, EgcNode& expression)
{
        quint32 i;

        QScopedPointer<const EgcNode> expr(&expression);

        quint32 count = static_cast<quint32>(m_childs.count());
        if (index >= count) {
                m_childs.resize(static_cast<int>(index + 1));
        }
        quint32 countNew = static_cast<quint32>(m_childs.count());

        for (i = count; i < countNew; i++) {
                m_childs[static_cast<int>(i)] = nullptr;
        }

        if (m_childs[static_cast<int>(index)]) {
                delete m_childs[static_cast<int>(index)];
        }

        m_childs[static_cast<int>(index)] = const_cast<EgcNode*>(expr.take());

        //set the parent also
        if(m_childs[static_cast<int>(index)])
                m_childs[static_cast<int>(index)]->provideParent(this);

        return true;
}

quint32 EgcFlexNode::getNumberChildNodes(void) const
{
        return static_cast<quint32>(m_childs.count());
}

bool EgcFlexNode::isFirstChild(EgcNode &child) const
{
        if (m_childs.empty())
                return false;

        if (m_childs.at(0) == &child)
                return true;
        else
                return false;
}

bool EgcFlexNode::isLastChild(EgcNode &child) const
{
        if (m_childs.empty())
                return false;

        if (m_childs.at(m_childs.count() - 1) == &child)
                return true;
        else
                return false;
}

EgcNode* EgcFlexNode::incrementToNextChild(EgcNode &previousChild) const
{
        (void) previousChild;
        int tempIndex = m_childs.indexOf(&previousChild);
        quint32 i;
        quint32 index = static_cast<quint32>(tempIndex);
        quint32 nrChilds = static_cast<quint32>(m_childs.count());

        if (m_childs.empty() || tempIndex < 0)
                return nullptr;

        for (i = index + 1; i < nrChilds; i++) {
                if (m_childs.at(static_cast<int>(i)))
                        return m_childs.at(static_cast<int>(i));
        }

        return nullptr;
}

EgcNode* EgcFlexNode::decrementToPrevChild(EgcNode &previousChild) const
{
        (void) previousChild;
        int tempIndex = m_childs.indexOf(&previousChild);
        quint32 i;
        quint32 index = static_cast<quint32>(tempIndex);

        if (m_childs.empty() || tempIndex < 0)
                return nullptr;

        for (i = index - 1; i < index; i--) {
                if (m_childs.at(static_cast<int>(i)))
                        return m_childs.at(static_cast<int>(i));
        }

        return nullptr;
}

bool EgcFlexNode::getIndexOfChild(EgcNode& child, quint32& index) const
{
        if (child.getParent() == this) {
                int ind = m_childs.indexOf(&child);
                if (ind >= 0) {
                        index = static_cast<quint32>(ind);
                        return true;
                }
        }

        return false;
}

bool EgcFlexNode::isFlexNode(void) const
{
        return true;
}

bool EgcFlexNode::insert(quint32 index, EgcNode& node)
{
        bool retval = true;
        quint32 count = static_cast<quint32>(m_childs.count());

        if (index > count)
                return false;

        m_childs.insert(static_cast<int>(index), &node);

        count = static_cast<quint32>(m_childs.count());
        if (index < count) {
                //set the parent also
                if(m_childs[static_cast<int>(index)])
                        m_childs[static_cast<int>(index)]->provideParent(this);
        } else {
                retval = false;
        }

        return retval;
}

bool EgcFlexNode::remove(quint32 index)
{
        if (index < static_cast<quint32>(m_childs.count())) {
                delete m_childs.at(static_cast<int>(index));
                m_childs[static_cast<int>(index)] = nullptr;
                m_childs.remove(static_cast<int>(index));
        } else {
                return false;
        }

        return true;
}

bool EgcFlexNode::operator==(const EgcNode& node) const
{
        bool retval = false;

        if (node.isFlexNode() && this->getNodeType() == node.getNodeType()) {
                quint32 i;
                quint32 count = static_cast<quint32>(m_childs.count());
                const EgcFlexNode& flexNode = static_cast<const EgcFlexNode&>(node);
                if (flexNode.getNumberChildNodes() != static_cast<quint32>(m_childs.count()))
                        return false;

                retval = true;
                for(i = 0; i < count; i++) {
                        EgcNode* lhs = m_childs.at(static_cast<int>(i));
                        EgcNode* rhs = flexNode.getChild(i);
                        if (lhs && rhs) {
                                if (!(*lhs == *rhs)) {
                                        retval = false;
                                        break;
                                }
                        }
                }


        }

        return retval;

}
