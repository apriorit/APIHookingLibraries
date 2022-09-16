/*
 * Copyright (C) 2010-2015 Nektra S.A., Buenos Aires, Argentina.
 * All rights reserved. Contact: http://www.nektra.com
 *
 *
 * This file is part of Deviare
 *
 *
 * Commercial License Usage
 * ------------------------
 * Licensees holding valid commercial Deviare licenses may use this file
 * in accordance with the commercial license agreement provided with the
 * Software or, alternatively, in accordance with the terms contained in
 * a written agreement between you and Nektra.  For licensing terms and
 * conditions see http://www.nektra.com/licensing/. Use the contact form
 * at http://www.nektra.com/contact/ for further information.
 *
 *
 * GNU General Public License Usage
 * --------------------------------
 * Alternatively, this file may be used under the terms of the GNU General
 * Public License version 3.0 as published by the Free Software Foundation
 * and appearing in the file LICENSE.GPL included in the packaging of this
 * file.  Please visit http://www.gnu.org/copyleft/gpl.html and review the
 * information to ensure the GNU General Public License version 3.0
 * requirements will be met.
 *
 **/

using System;
using Nektra.Deviare2;

namespace CSharpConsole.Model.Parsing
{
    public class PointerDescriptor : ParameterDescriptor
    {
        #region Parameter Formatting

        private string DescriptionForStructurePointee(NktParam pointedParameter, string functionName)
        {
            return Describe(pointedParameter, functionName);
        }

        private string DescriptionForNonStructurePointee(NktParam aParameter)
        {
            return FormatAsPointer(aParameter.PointerVal);
        }

        private string FormatAsPointer(object aValue)
        {
            return string.Format("0x{0}X", aValue);
        }

        #endregion

        public override bool CanHandle(NktParam aParameter, string functionName)
        {
            return aParameter.IsPointer;
        }

        public override string GenerateDescription(NktParam aParameter, string functionName)
        {
            if (aParameter.PointerVal == IntPtr.Zero) return "0x0";
            var pointedParameter = aParameter.Evaluate();

            return pointedParameter.FieldsCount == 0 ? DescriptionForNonStructurePointee(aParameter) : DescriptionForStructurePointee(pointedParameter, functionName);
        }
    }
}
