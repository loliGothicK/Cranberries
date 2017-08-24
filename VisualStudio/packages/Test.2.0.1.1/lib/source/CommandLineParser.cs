using System;
using System.Reflection;
using System.Text.RegularExpressions;

namespace Dnp
{
	/// <summary>Implementation of a command-line parsing class.  Is capable of
	/// having switches registered with it directly or can examine a registered
	/// class for any properties with the appropriate attributes appended to
	/// them.</summary>
	public partial class CommandLineParser
	{
        #region Public Properties

        public string CommandLine
        {
            get { return _CommandLine; }
            set { _CommandLine = value; }
        }
        private string _CommandLine = "";

        public string WorkingString
        {
            get { return _WorkingString; }
            set { _WorkingString = value; }
        }
        private string _WorkingString = "";

        public string ApplicationName
        {
            get { return _ApplicationName; }
        }
        private string _ApplicationName = "";

        public string[] Parameters
        {
            get { return _SplitParameters; }
        }
        private string[] _SplitParameters = null;

        public OptionInfo[] Switches
        {
            get
            {
                if (_Switches == null)
                    return null;
                else
                {
                    OptionInfo[] si = new OptionInfo[_Switches.Count];
                    for (int i = 0; i < _Switches.Count; i++)
                        si[i] = new OptionInfo(_Switches[i]);
                    return si;
                }
            }
        }
        private System.Collections.ArrayList _Switches = null;

        public object this[string name]
        {
            get
            {
                if (_Switches != null)
                    for (int i = 0; i < _Switches.Count; i++)
                        if (string.Compare((_Switches[i] as SwitchRecord).Name, name, true) == 0)
                            return (_Switches[i] as SwitchRecord).Value;
                return null;
            }
        }

        /// <summary>This function returns a list of the unhandled switches
        /// that the parser has seen, but not processed.</summary>
        /// <remark>The unhandled switches are not removed from the remainder
        /// of the command-line.</remark>
        public string[] UnhandledSwitches
        {
            get
            {
                string switchPattern = @"(\s|^)(?<match>(-{1,2}|/)(.+?))(?=(\s|$))";
                Regex r = new Regex(switchPattern,
                    RegexOptions.ExplicitCapture
                    | RegexOptions.IgnoreCase);
                MatchCollection m = r.Matches(_WorkingString);

                if (m != null)
                {
                    string[] unhandled = new string[m.Count];
                    for (int i = 0; i < m.Count; i++)
                        unhandled[i] = m[i].Groups["match"].Value;
                    return unhandled;
                }
                else
                    return null;
            }
        }
        #endregion
        
        #region Private Utility Functions
		private void GetApplicationName()
		{
			Regex r = new Regex(@"((?:"")(?<commandLine>.+?)(?:"")|(?:')(?<commandLine>.+?)(?:')|(?<commandLine>\S+)) (?<remainder>.+)",
				RegexOptions.ExplicitCapture);
			Match m = r.Match(_CommandLine);			
			if ( m != null && m.Groups["commandLine"] != null )
			{
				_ApplicationName = m.Groups["commandLine"].Value;
				_ApplicationName = _ApplicationName.Trim();
				if(_ApplicationName.Length > 0)
				{
					if(_ApplicationName[0]== '\'' || _ApplicationName[0]== '"')
					{
						_ApplicationName = _ApplicationName.Remove(0,1);
						_ApplicationName = _ApplicationName.Remove(_ApplicationName.Length-1,1);
					}
				}
				else
				{
					_ApplicationName = _CommandLine;
				}
				_WorkingString = m.Groups["remainder"].Value;
			}
		}

		private void SplitParameters()
		{
			// Populate the split parameters array with the remaining parameters.
			// Note that if quotes are used, the quotes are removed.
			// e.g.   one two three "four five six"
			//						0 - one
			//						1 - two
			//						2 - three
			//						3 - four five six
			// (e.g. 3 is not in quotes).
			Regex r = new Regex(@"((\s*(""(?<param>.+?)""|'(?<param>.+?)'|(?<param>\S+))))",
				RegexOptions.ExplicitCapture);
			MatchCollection m = r.Matches( _WorkingString );

			if ( m != null )
			{
				_SplitParameters = new string[ m.Count ];
				for ( int i=0; i < m.Count; i++ )
					_SplitParameters[i] = m[i].Groups["param"].Value;
			}
		}

		private void HandleSwitches()
		{
			if ( _Switches != null )
			{
				foreach ( SwitchRecord s in _Switches )
				{
					Regex r = new Regex( s.Pattern,
						RegexOptions.ExplicitCapture
						| RegexOptions.IgnoreCase );
					MatchCollection m = r.Matches( _WorkingString );
					if ( m != null )
					{
						for ( int i=0; i < m.Count; i++ )
						{
							string value = null;
							if ( m[i].Groups != null && m[i].Groups["value"] != null )
								value = m[i].Groups["value"].Value;

							if ( s.Type == typeof(bool))
							{
								bool state = true;
								// The value string may indicate what value we want.
								if ( m[i].Groups != null && m[i].Groups["value"] != null )
								{
									switch ( value )
									{
										case "+": state = true;
											break;
										case "-": state = false;
											break;
										case "":  if ( s.ReadValue != null )
														 state = !(bool)s.ReadValue;
											break;
										default:  break;
									}
								}
								s.Notify( state );
								break;
							}
							else if ( s.Type == typeof(string) )
								s.Notify( value );
							else if ( s.Type == typeof(int) )
								s.Notify( int.Parse( value ) );
							else if ( s.Type.IsEnum )
								s.Notify( System.Enum.Parse(s.Type,value,true) );
						}
					}

					_WorkingString = r.Replace(_WorkingString, " ");
				}
			}
		}

		#endregion

		#region Public Methods
		public void AddSwitch( string name, string description )
		{
			if ( _Switches == null )
				_Switches = new System.Collections.ArrayList();

			SwitchRecord rec = new SwitchRecord( name, description );
			_Switches.Add( rec );
		}

		public void AddSwitch( string[] names, string description )
		{
			if ( _Switches == null )
				_Switches = new System.Collections.ArrayList();
			SwitchRecord rec = new SwitchRecord( names[0], description );
			for ( int s=1; s<names.Length; s++ )
				rec.AddAlias( names[s] );
			_Switches.Add( rec );
		}

		public bool Parse()
		{
			GetApplicationName();

			// Remove switches and associated info.
			HandleSwitches();

			// Split parameters.
			SplitParameters();

			return true;
		}

		/// <summary>
		/// Returns the help string based on the attributes of the commandline class.
		/// </summary>
		/// <returns></returns>
		public string GetHelpText()
		{
			System.Text.StringBuilder message = new System.Text.StringBuilder("");
			message.Append( this.ApplicationName + " ");
			foreach(CommandLineParser.OptionInfo switchInfo in this.Switches)
			{
				message.Append( "[/" );
				if(switchInfo.Aliases != null)
				{
					message.Append("{");
				}
				message.Append( switchInfo.Name);
				if(switchInfo.Aliases != null)
				{
					foreach(string alias in switchInfo.Aliases)
					{
						message.Append( "|" + alias);
					}
					message.Append("}");
				}
				if(switchInfo.IsEnum)
				{
					message.Append( ":" + string.Join("|", Enum.GetNames(switchInfo.Type)));				
				}
				message.Append( "] ");
			}

			message.Append("\n\n");
			foreach(CommandLineParser.OptionInfo switchInfo in this.Switches)
			{
				message.AppendFormat("/{0,-17}-{1}", switchInfo.Name, switchInfo.Description);
				if(switchInfo.Aliases != null)
				{
					message.AppendFormat(" (Short format: /{0})", string.Join(", /", switchInfo.Aliases));
				}
				message.Append("\n");
			}
			return message.ToString();
		}
		public object InternalValue(string name)
		{
			if ( _Switches != null )
				for ( int i=0; i<_Switches.Count; i++ )
					if ( string.Compare( (_Switches[i] as SwitchRecord).Name, name, true )==0 )
						return (_Switches[i] as SwitchRecord).InternalValue;
			return null;
		}
		#endregion

		#region Constructors
		public CommandLineParser( string commandLine )
		{
			_CommandLine = commandLine;
		}

		public CommandLineParser( string commandLine, object commandLineInfo )
		{
			_CommandLine = commandLine;

			Type type = commandLineInfo.GetType();
            PropertyInfo[] properties = type.GetProperties(
                BindingFlags.NonPublic | BindingFlags.Public | BindingFlags.Instance
                | BindingFlags.FlattenHierarchy);

			for(int i=0; i<properties.Length; i++)
			{
				object[] attributes = properties[i].GetCustomAttributes(false);
				if(attributes.Length > 0)
				{
					SwitchRecord rec = null;

					foreach ( Attribute attribute in attributes )
					{
						if ( attribute is CommandLineOptionAttribute )
						{
							CommandLineOptionAttribute switchAttrib =
								(CommandLineOptionAttribute) attribute;

							System.Reflection.PropertyInfo property = properties[i];

							rec = new SwitchRecord( switchAttrib.Name,
															switchAttrib.Description,
                                                            property.PropertyType);

							// Map in the Get/Set methods.
                            rec.SetMethod = property.GetSetMethod();
                            rec.GetMethod = property.GetGetMethod();
							rec.PropertyOwner = commandLineInfo;

							// Can only handle a single switch for each property
							// (otherwise the parsing of aliases gets silly...)
							break;
						}
					}

#if ALIAS
					// See if any aliases are required.  We can only do this after
					// a switch has been registered and the framework doesn't make
					// any guarantees about the order of attributes, so we have to
					// walk the collection a second time.
					if ( rec != null )
					{
						foreach ( Attribute attribute in attributes )
						{
							if ( attribute is CommandLineAliasAttribute )
							{
								CommandLineAliasAttribute aliasAttrib =
									(CommandLineAliasAttribute) attribute;
								rec.AddAlias( aliasAttrib.Alias );
							}
						}
					}
#endif


					// Assuming we have a switch record (that may or may not have
					// aliases), add it to the collection of switches.
					if ( rec != null )
					{
						if ( _Switches == null )
							_Switches = new System.Collections.ArrayList();
						_Switches.Add( rec );
					}
				}
			}
		}
		#endregion
	}
}
