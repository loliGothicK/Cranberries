#region License

// 
// Copyright (c) 2009-2011, Ian Davis <ian@innovatian.com>
// 
// Dual-licensed under the Apache License, Version 2.0, and the Microsoft Public License (Ms-PL).
// See the file LICENSE.txt for details.
// 

#endregion

#region Using Directives

using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Linq;
using System.Text.RegularExpressions;
using Dnp.Properties;

#endregion

namespace Dnp.Configuration
{
    /// <summary>
    /// </summary>
    public class CommandLineConfigurationSource : AbstractConfigurationSource
    {
        private readonly string[] _arguments;
        private StringDictionary _parameters;

        /// <summary>
        ///   Initializes a new instance of the <see cref = "CommandLineConfigurationSource" /> class.
        /// </summary>
        /// <param name = "args">
        ///   The arguments to procces.
        /// </param>
        public CommandLineConfigurationSource( string[] args )
        {
            if ( args == null )
            {
                throw new ArgumentNullException( "args" );
            }

            _arguments = new string[args.Length];
            args.CopyTo( _arguments, 0 );

            Extract( _arguments );
        }

        /// <summary>
        ///   Initializes a new instance of the <see cref = "CommandLineConfigurationSource" /> class.
        /// </summary>
        /// <param name = "args">
        ///   The arguments to procces.
        /// </param>
        /// <param name = "sectionName">
        ///   Name of the section to add the settings into.
        /// </param>
        /// <param name = "switches">
        ///   The arguments switches to match against the arguments. The values
        ///   may be accessed through these switches.
        /// </param>
        public CommandLineConfigurationSource( string[] args, string sectionName, IEnumerable<string> switches )
                : this( args )
        {
            if ( string.IsNullOrEmpty( sectionName ) )
            {
                throw new ArgumentNullException( "sectionName" );
            }

            if ( switches == null )
            {
                throw new ArgumentNullException( "switches" );
            }

            AddSwitch( sectionName, switches );
        }

        /// <summary>
        ///   Initializes a new instance of the <see cref = "CommandLineConfigurationSource" /> class.
        /// </summary>
        /// <param name = "args">
        ///   The arguments to procces.
        /// </param>
        /// <param name = "sectionName">
        ///   Name of the section to add the settings into.
        /// </param>
        /// <param name = "switches">
        ///   The arguments switches to match against the arguments. The values
        ///   may be accessed through these switches.
        /// </param>
        public CommandLineConfigurationSource( string[] args, string sectionName, params string[] switches )
                : this( args )
        {
            if ( string.IsNullOrEmpty( sectionName ) )
            {
                throw new ArgumentNullException( "sectionName" );
            }

            if ( switches == null )
            {
                throw new ArgumentNullException( "switches" );
            }

            AddSwitch( sectionName, switches );
        }

        /// <summary>
        ///   Adds the switch to match against the arguments mapping the short
        ///   name value to the long name (and short name) keys if possible.
        /// </summary>
        /// <param name = "sectionName">
        ///   Name of the section to add the settings into.
        /// </param>
        /// <param name = "longName">
        ///   The long name of the switch.
        /// </param>
        /// <param name = "shortName">
        ///   The short name of the switch. This must be 1 or 2 characters in
        ///   length.
        /// </param>
        public void AddSwitch( string sectionName, string longName, string shortName )
        {
            if ( string.IsNullOrEmpty( sectionName ) )
            {
                throw new ArgumentNullException( "sectionName" );
            }

            if ( string.IsNullOrEmpty( longName ) )
            {
                throw new ArgumentNullException( "longName" );
            }

            if ( string.IsNullOrEmpty( shortName ) )
            {
                throw new ArgumentNullException( "shortName" );
            }

            if ( shortName.Length > 2 )
            {
                throw new ArgumentException( Strings.InvalidShortNameLength );
            }

            IConfigurationSection section;
            if ( Sections.ContainsKey( sectionName ) )
            {
                section = Sections[sectionName];
            }
            else
            {
                section = new ConfigurationSection( sectionName );
                Add( section );
            }

            if ( _parameters.ContainsKey( longName ) )
            {
                section.Set( longName, _parameters[longName] );
            }

            if ( _parameters.ContainsKey( shortName ) )
            {
                section.Set( longName, _parameters[shortName] );
                section.Set( shortName, _parameters[shortName] );
            }
        }

        /// <summary>
        ///   Adds the switch to match against the arguments mapping each switch.
        /// </summary>
        /// <param name = "sectionName">
        ///   Name of the section to add the settings into.
        /// </param>
        /// <param name = "switches">
        ///   The arguments switches to match against the arguments. The values
        ///   may be accessed through these switches.
        /// </param>
        public void AddSwitch( string sectionName, IEnumerable<string> switches )
        {
            if ( string.IsNullOrEmpty( sectionName ) )
            {
                throw new ArgumentNullException( "sectionName" );
            }

            if ( switches == null )
            {
                throw new ArgumentNullException( "switches" );
            }

            AddSwitch( sectionName, switches.ToArray() );
        }

        /// <summary>
        ///   Adds the switch to match against the arguments mapping each switch.
        /// </summary>
        /// <param name = "sectionName">
        ///   Name of the section to add the settings into.
        /// </param>
        /// <param name = "switches">
        ///   The arguments switches to match against the arguments. The values
        ///   may be accessed through these switches.
        /// </param>
        public void AddSwitch( string sectionName, params string[] switches )
        {
            if ( string.IsNullOrEmpty( sectionName ) )
            {
                throw new ArgumentNullException( "sectionName" );
            }

            if ( switches == null )
            {
                throw new ArgumentNullException( "switches" );
            }

            IConfigurationSection section;
            if ( Sections.ContainsKey( sectionName ) )
            {
                section = Sections[sectionName];
            }
            else
            {
                section = new ConfigurationSection( sectionName );
                Add( section );
            }

            foreach ( string switchName in switches )
            {
                if ( !string.IsNullOrEmpty( switchName ) &&
                     _parameters.ContainsKey( switchName ) )
                {
                    section.Set( switchName, _parameters[switchName] );
                }
            }
        }

        /// <summary>
        ///   Determins whether the specified switch name has a corresponding value in a section.
        /// </summary>
        /// <param name = "switchName">
        ///   Name of the switch.
        /// </param>
        /// <returns>
        ///   <c>true</c> if any section contains the switch; <c>false</c> otherwise.
        /// </returns>
        public bool Exists( string switchName )
        {
            if ( string.IsNullOrEmpty( switchName ) )
            {
                return false;
            }

            foreach ( IConfigurationSection section in Sections.Values )
            {
                if ( section.Get<string>( switchName, null ) != null )
                {
                    return true;
                }
            }
            return false;
        }

        // Extract command line parameters and values stored in a string array
        private void Extract( IEnumerable<string> args )
        {
            _parameters = new StringDictionary();
            var splitter = new Regex( @"^([/-]|--){1}(?<name>\w+)([:=])?(?<value>.+)?$", RegexOptions.Compiled );
            char[] trimChars = { '"', '\'' };
            string parameter = null;
            Match part;

            // Valid parameters forms: {-,/,--}param{ , = ,:}((",')value(",'))
            // Examples: -param1 value1 --param2 /param3:"Test-:-work" /param4=happy -param5 '-- = nice = --'
            foreach ( string arg in args )
            {
                part = splitter.Match( arg );
                if ( !part.Success )
                {
                    // Found a value (for the last parameter found (space
                    // separator))
                    if ( parameter != null )
                    {
                        _parameters[parameter] = arg.Trim( trimChars );
                    }
                }
                else
                {
                    // Matched a name, optionally with inline value
                    parameter = part.Groups["name"].Value;
                    _parameters.Add( parameter, part.Groups["value"].Value.Trim( trimChars ) );
                }
            }
        }

        #region Overrides of AbstractConfigurationSource

        /// <summary>
        ///   Saves this instance.
        /// </summary>
        public override void Save()
        {
            throw new NotSupportedException( string.Format( Strings.Culture, Strings.TypeCantBeSaved0, GetType().Name ) );
        }

        /// <summary>
        ///   Reloads this instance.
        /// </summary>
        public override void Reload()
        {
            throw new NotSupportedException( string.Format( Strings.Culture, Strings.TypeCantBeReloaded0, GetType().Name ) );
        }

        #endregion
    }
}