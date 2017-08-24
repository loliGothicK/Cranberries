using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;

namespace Dnp
{
    partial class CommandLineParser
    {
        /// <summary>
        /// The SwitchRecord is stored within the parser's collection of registered
        /// switches.  This class is private to the outside world.
        /// </summary>
        private class SwitchRecord
        {
            #region Private Variables

            #endregion

            #region Private Utility Functions
            private void Initialize(string name, string description)
            {
                _Name = name;
                _Description = description;

                BuildPattern();
            }

            private void BuildPattern()
            {
                string matchString = Regex.Escape(Name);

                if (Aliases != null && Aliases.Length > 0)
                {
                    foreach (string s in Aliases)
                    {
                        matchString += "|" + Regex.Escape(s);
                    }
                }

                string strPatternStart = @"(\s|^)(?<match>(-{1,2}|/)(";
                string strPatternEnd;  // To be defined below.

                // The common suffix ensures that the switches are followed by
                // a white-space OR the end of the string.  This will stop
                // switches such as /help matching /helpme
                //
                string strCommonSuffix = @"(?=(\s|$))";

                if (Type == typeof(bool))
                    strPatternEnd = @")(?<value>(\+|-){0,1}))";
                else if (Type == typeof(string))
                    strPatternEnd = @")(?::|\s+))((?:"")(?<value>.+?)(?:"")|(?:')(?<value>.+?)(?:')|(?<value>\S+))";
                else if (Type == typeof(int))
                    strPatternEnd = @")(?::|\s+))((?<value>(-|\+)[0-9]+)|(?<value>[0-9]+))";
                else if (Type.IsEnum)
                {
                    string[] enumNames = Enumerations;
                    string e_str = enumNames[0];
                    for (int e = 1; e < enumNames.Length; e++)
                        e_str += "|" + enumNames[e];
                    strPatternEnd = @")(?::|\s+))(?<value>" + e_str + @")";
                }
                else
                    throw new System.ArgumentException();

                // Set the internal regular expression pattern.
                _Pattern = strPatternStart + matchString + strPatternEnd + strCommonSuffix;
            }
            #endregion

            #region Public Properties
            public object Value
            {
                get
                {
                    if (ReadValue != null)
                        return ReadValue;
                    else
                        return _Value;
                }
            }
            private object _Value = null;

            public object InternalValue
            {
                get { return _Value; }
            }

            public string Name
            {
                get { return _Name; }
                set { _Name = value; }
            }
            private string _Name = "";

            public string Description
            {
                get { return _Description; }
                set { _Description = value; }
            }
            private string _Description = "";

            public System.Type Type
            {
                get { return _SwitchType; }
            }
            private System.Type _SwitchType = typeof(bool);

            public string[] Aliases
            {
                get { return (_Aliases != null) ? (string[])_Aliases.ToArray(typeof(string)) : null; }
            }
            private System.Collections.ArrayList _Aliases = null;

            public string Pattern
            {
                get { return _Pattern; }
            }
            private string _Pattern = "";

            // The following advanced functions allow for callbacks to be
            // made to manipulate the associated data type.
            public System.Reflection.MethodInfo SetMethod
            {
                set { _SetMethod = value; }
            }
            private System.Reflection.MethodInfo _SetMethod = null;

            public System.Reflection.MethodInfo GetMethod
            {
                set { _GetMethod = value; }
            }
            private System.Reflection.MethodInfo _GetMethod = null;

            public object PropertyOwner
            {
                set { _PropertyOwner = value; }
            }
            private object _PropertyOwner = null;

            public object ReadValue
            {
                get
                {
                    object o = null;
                    if (_PropertyOwner != null && _GetMethod != null)
                        o = _GetMethod.Invoke(_PropertyOwner, null);
                    return o;
                }
            }

            public string[] Enumerations
            {
                get
                {
                    if (_SwitchType.IsEnum)
                        return System.Enum.GetNames(_SwitchType);
                    else
                        return null;
                }
            }
            #endregion

            #region Constructors
            public SwitchRecord(string name, string description)
            {
                Initialize(name, description);
            }

            public SwitchRecord(string name, string description, System.Type type)
            {
                if (type == typeof(bool) ||
                      type == typeof(string) ||
                      type == typeof(int) ||
                      type.IsEnum)
                {
                    _SwitchType = type;
                    Initialize(name, description);
                }
                else
                {
                    throw new ArgumentException("Currently only Ints, Bool and Strings are supported");
                }
            }
            #endregion

            #region Public Methods
            public void AddAlias(string alias)
            {
                if (_Aliases == null)
                    _Aliases = new System.Collections.ArrayList();
                _Aliases.Add(alias);

                BuildPattern();
            }

            public void Notify(object value)
            {
                if (_PropertyOwner != null && _SetMethod != null)
                {
                    object[] parameters = new object[1];
                    parameters[0] = value;
                    _SetMethod.Invoke(_PropertyOwner, parameters);
                }
                _Value = value;
            }

            #endregion
        }
    }
}
