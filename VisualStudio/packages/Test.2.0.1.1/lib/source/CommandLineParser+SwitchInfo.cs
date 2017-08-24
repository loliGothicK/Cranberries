using System;
using System.Collections.Generic;
using System.Text;

namespace Dnp
{
    partial class CommandLineParser
    {
        /// <summary>A simple internal class for passing back to the caller
        /// some information about the switch.  The internals/implementation
        /// of this class has privillaged access to the contents of the
        /// SwitchRecord class.</summary>
        public class OptionInfo
        {
            #region Private Variables
            private object _Switch = null;
            #endregion

            #region Public Properties
            public string Name { get { return (_Switch as SwitchRecord).Name; } }
            public string Description { get { return (_Switch as SwitchRecord).Description; } }
            public string[] Aliases { get { return (_Switch as SwitchRecord).Aliases; } }
            public Type Type { get { return (_Switch as SwitchRecord).Type; } }
            public object Value { get { return (_Switch as SwitchRecord).Value; } }
            public object InternalValue { get { return (_Switch as SwitchRecord).InternalValue; } }
            public bool IsEnum { get { return (_Switch as SwitchRecord).Type.IsEnum; } }
            public string[] Enumerations { get { return (_Switch as SwitchRecord).Enumerations; } }
            #endregion

            /// <summary>
            /// Constructor for the SwitchInfo class.  Note, in order to hide to the outside world
            /// information not necessary to know, the constructor takes a System.Object (aka
            /// object) as it's registering type.  If the type isn't of the correct type, an exception
            /// is thrown.
            /// </summary>
            /// <param name="record">The SwitchRecord for which this class store information.</param>
            /// <exception cref="ArgumentException">Thrown if the rec parameter is not of
            /// the type SwitchRecord.</exception>
            public OptionInfo(object record)
            {
                if (record is SwitchRecord)
                {
                    _Switch = record;
                }
                else
                {
                    throw new ArgumentException();
                }
            }
        }

    }
}
