using System;

namespace Dnp
{
	/// <summary>Implements a basic command-line switch by taking the
	/// switching name and the associated description.</summary>
	/// <remark>Only currently is implemented for properties, so all
	/// auto-switching variables should have a get/set method supplied.</remark>
	[AttributeUsage( AttributeTargets.Property )]
	public class CommandLineOptionAttribute : System.Attribute
	{
		#region Private Variables
		#endregion

		#region Public Properties
		/// <summary>Accessor for retrieving the switch-name for an associated
		/// property.</summary>
		public string Name 			
        {
            get 
            {
                return _Name; 
            } 
        }
		private string _Name = "";

		/// <summary>Accessor for retrieving the description for a switch of
		/// an associated property.</summary>
		public string Description	
        { 
            get 
            { 
                return _Description; 
            } 
        }
		private string _Description = "";

        public string Alias
        {
            get { return _Alias; }
        }
        private string _Alias;

		#endregion

		#region Constructors
		/// <summary>Attribute constructor.</summary>
		public CommandLineOptionAttribute( string name, string description )
		{
			_Name = name;
			_Description = description;
		}

        public CommandLineOptionAttribute(string name, string description, string aliases)
        {
            _Name = name;
            _Description = description;
            _Alias = aliases;
        }
        #endregion
	}

#if ALIAS
	/// <summary>
	/// This class implements an alias attribute to work in conjunction
	/// with the <see cref="CommandLineSwitchAttribute">CommandLineSwitchAttribute</see>
	/// attribute.  If the CommandLineSwitchAttribute exists, then this attribute
	/// defines an alias for it.
	/// </summary>
	[AttributeUsage( AttributeTargets.Property )]
	public class CommandLineAliasAttribute : System.Attribute
	{
		#region Private Variables
        #pragma warning disable 3008 // Identifier 'Dnp.CommandLineAliasAttribute._Alias' is not CLS-compliant
		protected string _Alias = "";
        #pragma warning restore 3008 // Identifier 'Dnp.CommandLineAliasAttribute._Alias' is not CLS-compliant
		#endregion

		#region Public Properties
		public string Alias 
		{
			get { return _Alias; }
		}

		#endregion

		#region Constructors
		public CommandLineAliasAttribute( string alias ) 
		{
			_Alias = alias;
		}
		#endregion
	}
#endif
}
