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

#ifndef EGCVARIABLENODE_H
#define EGCVARIABLENODE_H

#include <QString>
#include <QRegularExpression>
#include "../specialNodes/egcnode.h"
#include "structural/specialNodes/egcflexnode.h"

/**
 * @brief The EgcVariableNode class is a class that holds the leafes with variable names.
 *
 * It is also possible to store mathml symbol codes e.g. &Alpha; inside the varible node. Following codes for ampersand
 * '&' and ";" are used:
 * _1  : as delimeter for value and subscript
 * _2  : as code for the ampersand '&'
 * _3  : as code for ";"
 * These symbols cannot be represented by a cas kernel or have different meaning, therefore the are stuffed.
 */
class EgcVariableNode : public EgcNode
{
        //set the node type of this expression
        EGC_SET_EXPRESSION_TYPE(EgcVariableNode, EgcNodeType::VariableNode);

public:
        ///std constructor
        EgcVariableNode();
        ///destructor
        virtual ~EgcVariableNode();
        /**
         * @brief setValue set the variable name (value)
         * @param varName the variable name as a string
         * @param subscript the subscript of the variable if any
         */
        virtual void setValue(const QString& varName, const QString& subscript = QString::null);
        /**
         * @brief setStuffedVar set the raw variable name (value) maybe including stuffed subscript
         * @param varName the variable name as a string. This can include the stuffed subscript
         */
        virtual void setStuffedVar(const QString& varName);
        /**
         * @brief getValue returns the variable name (without subscript)
         * @return the variable name
         */
        virtual QString getValue(void) const;
        /**
         * @brief getSubscript returns the subscript of a variable name
         * @return the subscript of this variable object
         */
        virtual QString getSubscript(void) const;
        /**
         * @brief getStuffedValue returns the stuffed variable name (with subscript)
         * @return the stuffed variable name (a "_" in the variable name is stuffed into "__",
         * and variable name and subscript is seperated via "_1").
         */
        virtual QString getStuffedValue(void);
        /**
         * @brief getStuffedVarSeparator returns the stuffed var separator
         * @return returns the variable separator
         */
        static QString getStuffedVarSeparator(void);
        /**
         * @brief valid returns true if the expression is valid and false otherwise.
         * A variable expression is valid if the value is not empty.
         * @return true if the expression is valid, false otherwise.
         */
        virtual bool valid(void) override;
        /**
         * @brief operator== comparison operator overload
         * @param node the node to compare against
         * @return true if the trees are equal
         */
        virtual bool operator==(const EgcNode& node) const override;
        /**
         * @brief isOperation checks if the node is a operation. There are also nodes that are containers, but no operations
         * @return true if the node is an operation, false otherwise
         */
        virtual bool isOperation(void) const override;
        /**
         * @brief isSubscriptEmptyElement checks if subscript contains an empty element. This means not that the string
         * is empty!
         * @return true if it contains an empty element, false otherwise
         */
        bool isSubscriptEmptyElement(void);
        /**
         * @brief interface for serializing a class
         * @param stream the stream to use for serializing this class
         * @param properties object with all neccessary information for serializing
         */
        virtual void serialize(QXmlStreamWriter& stream, SerializerProperties& properties) override;
        /**
         * @brief deserialize interface for deserializing a class
         * @param stream the stream to use for deserializing this class
         * @param properties object with all neccessary information for deserializing
         */
        virtual void deserialize(QXmlStreamReader& stream, SerializerProperties &properties) override;

protected:

        static QRegularExpression s_varSubSeparator; ///< regex for separating variable and subscript
        static bool s_initializeRegex;    ///< initialize regex?
        QString m_value;                  ///< value of the variable
        QString m_subscript;              ///< subscript of the variable
        bool m_subscrIsEmpty;             ///< subscript is an empty element
};

#endif // EGCVARIABLENODE_H
