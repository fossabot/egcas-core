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

#include "egcnodecreator.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QScopedPointer>
#include <QLatin1String>
#include "egccontainernode.h"
#include "egcbinaryoperator.h"

EgcContainerNode::EgcContainerNode()
{
}

EgcContainerNode::EgcContainerNode(const EgcContainerNode& orig)
{
        m_parent = nullptr;
}

EgcContainerNode::~EgcContainerNode()
{
}

bool EgcContainerNode::isContainer(void) const
{
        return true;
}

void EgcContainerNode::adjustChildPointers(EgcNode &old_child, EgcNode &new_child)
{
        (void) old_child;
        (void) new_child;
}

EgcNode* EgcContainerNode::getChild(quint32 index) const
{
        (void) index;

        return nullptr;
}

bool EgcContainerNode::setChild(quint32 index, EgcNode& expression)
{
        (void) index;
        (void) expression;
}

quint32 EgcContainerNode::getNumberChildNodes(void) const
{
        return 0;
}

bool EgcContainerNode::isFirstChild(EgcNode &child) const
{
        (void) child;

        return false;
}

bool EgcContainerNode::isLastChild(EgcNode &child) const
{
        (void) child;

        return false;
}

bool EgcContainerNode::getIndexOfChild(EgcNode& child, quint32& index) const
{
        (void) child;
        (void) index;

        return false;
}

bool EgcContainerNode::transferProperties(EgcContainerNode &from)
{
        bool retval = false;
        quint32 nrChildNodes = this->getNumberChildNodes();
        quint32 nrChildsFrom = from.getNumberChildNodes();
        quint32 i;

        if (!this->isFlexNode()) {
                if (from.getNumberChildNodes() != nrChildNodes)
                        return false;
        }

        //test if all childs are null
        EgcNode* nd;
        for (i = 0; i < nrChildNodes; i++) {
                nd = this->getChild(i);
                //check also if there is a reordering protector
                if (nd != nullptr) {
                        return false;
                }
        }

        EgcNode* child;
        QScopedPointer<EgcNode> tempChild;
        for (i = 0; i < nrChildsFrom; i++) {
                child = from.getChild(i);
                if (child) {
                        tempChild.reset(from.takeOwnership(*child));
                        this->setChild(i, *tempChild.take());
                }
        }
        m_parent = from.getParent();
        m_parent->adjustChildPointers(from, *this);
        from.m_parent = nullptr;
        retval = true;

        return retval;
}

bool EgcContainerNode::isOperation(void) const
{
        return true;
}

bool EgcContainerNode::hasSubNode(const EgcNode& node, quint32 &index) const
{
        EgcNode* parent = nullptr;
        EgcNode* node_l = const_cast<EgcNode*>(&node);

        while (parent != this) {
                parent = node_l->getParent();
                if (parent) {
                        if (parent != this)
                                node_l = parent;
                } else {
                        return false;
                }
        }

        const EgcContainerNode* container = static_cast<const EgcContainerNode*>(parent);
        (void) container->getIndexOfChild(*node_l, index);

        return true;
}

void EgcContainerNode::serialize(QXmlStreamWriter& stream, SerializerProperties &properties)
{
        quint32 i;
        EgcNode* node;
        quint32 n = getNumberChildNodes();
        QLatin1String str = EgcNodeCreator::stringize(getNodeType());
        if (str.size() != 0) {
                stream.writeStartElement(str);
                serializeAttributes(stream);
        }

        for(i = 0; i < n; i++) {
                node = getChild(i);
                if (node)
                        node->serialize(stream, properties);
        }

        if (str.size() != 0) {
                stream.writeEndElement(); // document
        }
}

void EgcContainerNode::deserialize(QXmlStreamReader& stream, SerializerProperties &properties)
{
        if (stream.name() == EgcNodeCreator::stringize(getNodeType())) {
                QXmlStreamAttributes attr = stream.attributes();
                deserializeAttributes(stream, properties.version, attr);
                quint32 i = 0;
                while (stream.readNextStartElement()) {
                        QString str(stream.name().toLatin1());
                        QScopedPointer<EgcNode> node(EgcNodeCreator::create(QLatin1String(str.toLatin1())));
                        EgcNode* n = nullptr;
                        if (node.isNull()) {
                                stream.skipCurrentElement();
                        } else {
                                if (    (i < getNumberChildNodes())
                                     || (isFlexNode())) {
                                        setChild(i, *node.take());
                                        n = getChild(i);
                                }
                        }

                        if (n != nullptr)
                                n->deserialize(stream, properties);

                        i++;
                }


        } else {
                stream.skipCurrentElement();
        }
}

void EgcContainerNode::serializeAttributes(QXmlStreamWriter& stream)
{
        (void) stream;
}

void EgcContainerNode::deserializeAttributes(QXmlStreamReader& stream, quint32 version, QXmlStreamAttributes& attr)
{
        (void) stream;
        (void) version;
        (void) attr;
}

