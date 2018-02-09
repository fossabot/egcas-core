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
#include "formulascrelement.h"
#include "../entities/egcformulaentity.h"
#include "../egcnodes.h"
#ifdef DEBUG_KERNEL_COMMAND_GENERATION
#include <QDebug>
#endif //#ifdef DEBUG_KERNEL_COMMAND_GENERATION
#include "../egcnodes.h"
#include "formulascrvisitor.h"
#include "../entities/egcformulaentity.h"
#include <iterator/egcscrpositerator.h>
#include <structural/iterator/formulascriter.h>
#include <QStringBuilder>




FormulaScrVisitor::FormulaScrVisitor(EgcFormulaEntity& formula, FormulaScrIter& iter) :  EgcNodeVisitor(formula),
        m_iter{iter}, m_currNode{nullptr}
{
}

void FormulaScrVisitor::visit(EgcBinaryNode* binary)
{
        m_currNode = binary;
        EgcNode* lnode = binary->getChild(0);
        if (!lnode)
                lnode = binary;
        EgcNode* rnode = binary->getChild(1);
        if (!rnode)
                rnode = binary;

        switch (binary->getNodeType()) {
        case EgcNodeType::RootNode: {
                bool isSquaredRoot = false;
                if (lnode->getNodeType() == EgcNodeType::NumberNode) {
                        if (static_cast<EgcNumberNode*>(lnode)->getValue() == QString("2"))
                                isSquaredRoot = true;
                }
                if (m_state == EgcIteratorState::LeftIteration) {
                        if (isSquaredRoot) {
                                m_suppressList.insert(lnode); //exponent of squared roots are not shown
                                appendSegmented("sqrt(", binary, CursorAdhesion::low, 0, true, rnode, 0, true);
                        } else {
                                appendSegmented("_root(_{", binary, CursorAdhesion::low, 0, true, lnode, 0, true);
                        }
                } else if (m_state == EgcIteratorState::MiddleIteration) {
                        if (!isSquaredRoot)
                                appendSegmented("_},", lnode, CursorAdhesion::low, 0, false, rnode, 0, true);
                } else {
                        appendSegmented(")", rnode, CursorAdhesion::low, 0, false, binary, 0, false);
                }
                break;
        }
        case EgcNodeType::PlusNode:
                if (m_state == EgcIteratorState::MiddleIteration)
                        append("+", lnode, CursorAdhesion::low, 0, false, rnode, 0, true);
                break;
        case EgcNodeType::MinusNode:
                if (m_state == EgcIteratorState::MiddleIteration)
                        append("-", lnode, CursorAdhesion::low, 0, false, rnode, 0, true);
                break;
        case EgcNodeType::MultiplicationNode:
                if (m_state == EgcIteratorState::MiddleIteration)
                        append("*", lnode, CursorAdhesion::low, 0, false, rnode, 0, true);
                break;
        case EgcNodeType::DivisionNode:
                if (m_state == EgcIteratorState::LeftIteration)
                        appendSegmented("_{", binary, CursorAdhesion::low, 0, true, lnode, 0, true);
                else if (m_state == EgcIteratorState::MiddleIteration)
                        appendSegmented("_}/_{", lnode, CursorAdhesion::low, 0, true, rnode, 0, true);
                else
                        appendSegmented("_}", rnode, CursorAdhesion::low, 0, false, binary, 0, false);
                break;
        case EgcNodeType::BinEmptyNode:
                if (m_state == EgcIteratorState::MiddleIteration)
                        append("_emptybinop", lnode, CursorAdhesion::low, 0, false, rnode, 0, true);
                break;
        case EgcNodeType::ExponentNode:
                if (m_state == EgcIteratorState::MiddleIteration)
                        appendSegmented("^_{", lnode, CursorAdhesion::low, 0, false, rnode, 0, true);
                else if (m_state == EgcIteratorState::RightIteration)
                        appendSegmented("_}", rnode, CursorAdhesion::low, 0, false, binary, 0, false);
                break;
        case EgcNodeType::EqualNode: {
                if (m_state == EgcIteratorState::MiddleIteration) {
                        append("=", lnode, CursorAdhesion::low, 0, false, rnode, 0, true);
                }
                break;
        }
        case EgcNodeType::DefinitionNode:
                if (m_state == EgcIteratorState::MiddleIteration)
                        append(":", lnode, CursorAdhesion::low, 0, false, rnode, 0, true);
                break;
        default:
                qDebug("No visitor code for maxima defined for this type: %d", static_cast<int>(binary->getNodeType())) ;
                break;
        }
}

void FormulaScrVisitor::visit(EgcUnaryNode* unary)
{
        m_currNode = unary;
        EgcNode* node = unary->getChild(0);
        if (!node)
                node = unary;

        switch (unary->getNodeType()) {
        case EgcNodeType::ParenthesisNode:
                if (m_state == EgcIteratorState::LeftIteration)
                        append("(", unary, CursorAdhesion::low, 0, true, node, 0, true);
                else if (m_state == EgcIteratorState::RightIteration)
                        append(")", node, CursorAdhesion::low, 0, false, unary, 0, false);
                break;
        case EgcNodeType::LogNode:
                if (m_state == EgcIteratorState::LeftIteration) {
                        append("_log", unary, CursorAdhesion::low, 0, true, unary, 3, false);
                        append("(", unary, CursorAdhesion::low, 4, true, node, 0, true);
                } else if (m_state == EgcIteratorState::RightIteration) {
                        append(")", node, CursorAdhesion::low, 0, false, unary, 5, false);
                }
                break;
        case EgcNodeType::NatLogNode:
                if (m_state == EgcIteratorState::LeftIteration) {
                        append("log", unary, CursorAdhesion::low, 0, true, unary, 2, false);
                        append("(", unary, CursorAdhesion::low, 3, true, node, 0, true);
                } else if (m_state == EgcIteratorState::RightIteration) {
                        append(")", node, CursorAdhesion::low, 0, false, unary, 4, false);
                }
                break;
        case EgcNodeType::LParenthesisNode:
                if (m_state == EgcIteratorState::LeftIteration)
                        append("(", unary, CursorAdhesion::low, 0, true, node, 0, true);
                break;
        case EgcNodeType::RParenthesisNode:
                if (m_state == EgcIteratorState::RightIteration)
                        append(")", node, CursorAdhesion::low, 0, false, unary, 0, false);
                break;
        case EgcNodeType::UnaryMinusNode:
                if (m_state == EgcIteratorState::LeftIteration)
                        append("-", unary, CursorAdhesion::normal);
                break;
        default:
                qDebug("No visitor code for maxima defined for this type: %d", static_cast<int>(unary->getNodeType())) ;
                break;
        }
}

void FormulaScrVisitor::visit(EgcFlexNode* flex)
{
        m_currNode = flex;
        switch (flex->getNodeType()) {
        case EgcNodeType::FunctionNode:
                if (m_state == EgcIteratorState::LeftIteration) {
                        QString name = static_cast<EgcFunctionNode*>(flex)->getName();
                        appendSigns(name, flex, CursorAdhesion::strong);
                        appendSegmented("(", flex, CursorAdhesion::low, name.length() + 1, true, flex->getChild(0), 0, true);
                } else if (m_state == EgcIteratorState::MiddleIteration) {
                        append(",", flex->getChild(m_childIndex), CursorAdhesion::low, 0, false, flex->getChild(m_childIndex + 1), 0, true);
                } else {
                        appendSegmented(")", flex->getChild(flex->getNumberChildNodes() - 1), CursorAdhesion::low, 0, false, flex, 0, false);
                }
                break;
        case EgcNodeType::IntegralNode:
                if (m_state == EgcIteratorState::LeftIteration) {
                        appendSegmented("_integrate_{", flex, CursorAdhesion::low, 0, true, flex->getChild(0), 0, true);
                } else if (m_state == EgcIteratorState::MiddleIteration) {
                        append(",", flex->getChild(m_childIndex), CursorAdhesion::low, 0, false, flex->getChild(m_childIndex + 1), 0, true);
                } else {
                        appendSegmented("_}", flex->getChild(flex->getNumberChildNodes() - 1), CursorAdhesion::low, 0, false, flex, 0, false);
                }
                break;
        case EgcNodeType::DifferentialNode:
                if (m_state == EgcIteratorState::LeftIteration) {
                        if (static_cast<EgcDifferentialNode*>(flex)->getNrDerivative() <= 3) {
                                appendSegmented("_diff_{", flex, CursorAdhesion::low, 0, true, flex->getChild(0), 0, true);
                        } else {
                                appendSegmented("_diff_{", flex, CursorAdhesion::ultra, 0, true, flex, 1, true);
                                appendSigns(QString::number(static_cast<EgcDifferentialNode*>(flex)->getNrDerivative()), flex, CursorAdhesion::strong);
                                appendSegmented(",", flex, CursorAdhesion::ultra, 1, false, flex->getChild(0), 0, true);
                        }
                } else if (m_state == EgcIteratorState::MiddleIteration) {
                        appendSegmented(",", flex->getChild(m_childIndex), CursorAdhesion::ultra, 0, false, flex->getChild(m_childIndex + 1), 0, true);
                } else {
                        appendSegmented("_}", flex->getChild(flex->getNumberChildNodes() - 1), CursorAdhesion::ultra, 0, false, flex, 0, false);
                }
                break;
        case EgcNodeType::VariableNode:
                if (m_state == EgcIteratorState::MiddleIteration) {
                        if (flex->getNumberChildNodes() == 2) {
                                EgcVariableNode *var = static_cast<EgcVariableNode*>(flex);
                                append(var->getStuffedVarSeparator(), flex);
                        }
                }
                break;
        default:
                qDebug("No visitor code for maxima defined for this type: %d", static_cast<int>(flex->getNodeType())) ;
                break;
        }
}

void FormulaScrVisitor::visit(EgcNode* node)
{
        m_currNode = node;
        switch (node->getNodeType()) {
        case EgcNodeType::EmptyNode:
                append("_empty", node, CursorAdhesion::strong);
                break;
        case EgcNodeType::AlnumNode:
                appendSigns(static_cast<EgcAlnumNode*>(node)->getValue(), node, CursorAdhesion::strong);
                break;
        case EgcNodeType::NumberNode:
                appendSigns(static_cast<EgcNumberNode*>(node)->getValue(), node, CursorAdhesion::strong);
                break;
        default:
                qDebug("No visitor code for maxima defined for this type: %d", static_cast<int>(node->getNodeType())) ;
                break;
        }
}

void FormulaScrVisitor::appendSegmented(QString str, EgcNode* node, CursorAdhesion cursorAdhesion, quint32 subpos, bool leftSide,
                               EgcNode* rNode, quint32 rSubpos, bool rLeftSide)
{
        append(str, node, cursorAdhesion, subpos, leftSide, rNode, rSubpos, rLeftSide);
        FormulaScrElement& tmp = m_iter.peekPrevious();
        tmp.m_isSegmented = true;
}

void FormulaScrVisitor::append(QString str, EgcNode* node, CursorAdhesion cursorAdhesion, quint32 subpos, bool leftSide,
                               EgcNode* rNode, quint32 rSubpos, bool rLeftSide)
{
        FormulaScrElement el;
        el.m_value = str;
        el.m_cAdh = cursorAdhesion;
        el.m_node = m_currNode;
        el.lTemp.m_node = node;
        el.lTemp.m_subpos = subpos;
        el.lTemp.m_left_side = leftSide;
        if (rNode) {
                el.rTemp.m_node = rNode;
                el.rTemp.m_subpos = rSubpos;
                el.rTemp.m_left_side = rLeftSide;
        } else {
                el.rTemp.m_left_side = false;
                el.rTemp.m_node = node;
                el.rTemp.m_subpos = subpos;
        }
        if (m_state == EgcIteratorState::LeftIteration)
                el.m_sideNode = FormulaScrElement::nodeLeftSide;
        else if (m_state == EgcIteratorState::MiddleIteration)
                el.m_sideNode = FormulaScrElement::nodeMiddle;
        else // (m_state == EgcIteratorState::RightIteration)
                el.m_sideNode = FormulaScrElement::nodeRightSide;
        m_iter.insert(el);
}

void FormulaScrVisitor::appendSigns(QString str, EgcNode* node, CursorAdhesion cursorAdhesion)
{
        QString i;
        quint32 n = 1;
        foreach (i, str) {
                append(EgcAlnumNode::encode(i), node, cursorAdhesion);
                FormulaScrElement& tmp = m_iter.peekPrevious();
                tmp.lTemp.m_subpos = n;
                tmp.rTemp.m_subpos = n;
                n++;
        }
}

void FormulaScrVisitor::updateVector(void)
{
        EgcNodeIterator iter(*m_formula);
        EgcNode *node;
        EgcNode *previousChildNode;

        m_iter.clear();

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
                if (!m_suppressList.contains(node))
                        node->accept(this);
        };

        // do post processing lookup id's from node's
        doPostprocessing();
}

void FormulaScrVisitor::doPostprocessing(void)
{
        const EgcMathmlLookup lookup = m_formula->getMathmlMappingCRef();
        quint32 id;
        FormulaScrIter iter = m_iter;
        iter.toFront();
        while(iter.hasNext()) {
                FormulaScrElement& i = iter.next();
                if (i.lTemp.m_node) {
                        id = lookup.getIdFrame(*i.lTemp.m_node);
                        if (id)
                                i.lTemp.m_id = id;
                }
                if (i.rTemp.m_node) {
                        id = lookup.getIdFrame(*i.rTemp.m_node);
                        if (id)
                                i.rTemp.m_id = id;
                }
        }
}

QString FormulaScrVisitor::getResult(void)
{
        QString result;
        FormulaScrElement i;
        FormulaScrIter iter = m_iter;
        iter.toFront();
        while(iter.hasNext())
                result = result % iter.next().m_value;

        return result;
}

void FormulaScrVisitor::suppressNode(const EgcNode* node)
{
        m_suppressList.insert(node);
}
