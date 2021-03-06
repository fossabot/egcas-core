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

#ifndef EGCOPERATIONS_H
#define EGCOPERATIONS_H

/**
 * @brief The EgcOperations enum These are operations that can be sent from the item classes to the entity classes to
 * describe any sort of action to take for any specific entity
 */
enum class EgcOperations
{
        // view based operations (e.g. s.th. happened on the view)
        mathCharOperator,               ///< math operator that can be determined by a char only
        mathFunction,                   ///< a math function that can have an additional name
        internalFunction,               ///< a internal math function that has an additional name
        formulaActivated,               ///< formula was activated (e.g. got focus on screen)
        formulaDeactivated,             ///< formula was deactivated (e.g. lost focus on screen)
        cursorForward,                  ///< user wants to iterate forward in a formula
        cursorBackward,                 ///< user wants to iterate backward in a formula
        spacePressed,                   ///< user pressed space (wants to highlight parent element)
        backspacePressed,               ///< user pressed backspace (wants to delete the element before the cursor)
        delPressed,                     ///< user pressed space (wants to delete the element behind the cursor)
        homePressed,                    ///< user pressed home key
        endPressed,                     ///< user pressed End key
        alnumKeyPressed,                ///< user pressed any digit or letter key
        createSubscript,                ///< create subid of a variable
        noAction,

};

/**
 * @brief The OpModificators enum These are modificators for the above operations
 */
enum class OpModificators
{
        standard,
        definiteIntegral,                       ///< if the integral is a definite one (with limits)
};

/**
 * @brief The LookModificators enum These are modificators for the above operations
 */
enum class LookModificators
{
        standard,
        differential_lagrange_notation_1,       ///< default is leibnitz notation dn(f(x))/dxn
        differential_lagrange_notation_2,       ///< default is leibnitz notation dn(f(x))/dxn
        differential_lagrange_notation_3,       ///< default is leibnitz notation dn(f(x))/dxn

};

/**
 * @brief The InternalFunctionType enum denotes the type of the internal function
 */
enum class InternalFunctionType
{
        undefined,
        logarithm,
        natLogarithm,
        integral,
        differential,
};

#endif // EGCOPERATIONS_H
