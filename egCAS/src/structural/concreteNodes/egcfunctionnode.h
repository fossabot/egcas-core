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

#ifndef EGCFUNCTIONNODE_H
#define EGCFUNCTIONNODE_H

#include <QString>
#include "egcfnccontainernode.h"

/**
 * @brief The EgcFuntionNode class is a class to model function calls. This manages e.g. something like calculation1(10, x + 3, 9.8234).
 */
class EgcFunctionNode : public EgcFncContainerNode
{
        //set the node type of this expression
        EGC_SET_EXPRESSION_TYPE(EgcFunctionNode, EgcNodeType::FunctionNode);
public:
        EgcFunctionNode();
        /**
         * @brief transferArgs transfers all arguments to the new function container
         * @param args the argument list to transfer
         * @return true if everything went well, false otherwise
         */
        virtual bool transferArgs(EgcArgumentsNode& args) override;
        /**
         * @brief setName set the function name
         * @param fncName the variable name as a string
         */
        virtual void setName(const QString& fncName);
        /**
         * @brief getName returns the function name
         * @return the function name
         */
        QString getName(void);
        /**
         * @brief valid returns true if the expression is valid and false otherwise.
         * A variable expression is valid if the value is not empty.
         * @return true if the expression is valid, false otherwise.
         */
        virtual bool valid(void) override;
        /**
         * @brief setStuffedName set the stuffed function name
         * @param fncName the variable name as a string. This can include the stuffed special signs (a "_" in the
         * variable name is stuffed into "__", and special signs are encoded with html escapes where 'ampersand'&'
         * and ';' are again encoded with '_2' and '_3'. As a result all characters will be ASCII conform then and can
         * be used by the calc kernel).
         */
        virtual void setStuffedName(const QString& fncName);
        /**
         * @brief getStuffedName returns the stuffed name (with stuffed special signs)
         * @return the stuffed variable name (a "_" in the variable name is stuffed into "__",
         * and special signs are encoded with html escapes where 'ampersand'&' and ';' are again encoded with '_2' and
         * '_3'. As a result all characters will be ASCII conform then and can be used by the calc kernel).
         */
        virtual QString getStuffedName(void);
        /**
         * @brief interface for serializing the attributes of a formula operation
         * @param stream the stream to use for serializing this class
         */
        virtual void serializeAttributes(QXmlStreamWriter& stream) override;

        /**
         * @brief deserialize interface for deserializing the attributes of a formula operation
         * @param stream the xml reader stream
         * @param version the version of the stream that is to be deserialized
         * @param attr the xml attributes provided by the parent
         */
        virtual void deserializeAttributes(QXmlStreamReader& stream, quint32 version, QXmlStreamAttributes& attr) override;


private:
        QString m_fncName;               ///< function name of the function
};

#endif // EGCFUNCTIONNODE_H
