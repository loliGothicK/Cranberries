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
using System.IO;
using System.Linq;
using System.Text;
using Dnp.Properties;

#endregion

namespace Dnp.Configuration
{
    /// <summary>
    /// </summary>
    public class IniConfigurationSource : AbstractFileConfigurationSource
    {
        private readonly string _comment = ";";
        private readonly string _delimiter = "=";

        /// <summary>
        ///   Initializes a new instance of the <see cref = "IniConfigurationSource" /> class.
        /// </summary>
        /// <param name = "data">The data.</param>
        public IniConfigurationSource( string data )
        {
            if ( string.IsNullOrEmpty( data ) )
            {
                throw new ArgumentNullException( "data" );
            }
            Parse( data );
        }

        /// <summary>
        ///   Initializes a new instance of the <see cref = "IniConfigurationSource" /> class.
        /// </summary>
        /// <param name = "data">The data.</param>
        /// <param name = "comment"></param>
        /// <param name = "delimiter"></param>
        public IniConfigurationSource( string data, string comment, string delimiter )
        {
            if ( string.IsNullOrEmpty( data ) )
            {
                throw new ArgumentNullException( "data" );
            }

            if ( string.IsNullOrEmpty( comment ) )
            {
                throw new ArgumentNullException( "comment" );
            }

            if ( string.IsNullOrEmpty( delimiter ) )
            {
                throw new ArgumentNullException( "delimiter" );
            }

            _comment = comment;
            _delimiter = delimiter;
            Parse( data );
        }

        /// <summary>
        ///   Initializes a new instance of the <see cref = "IniConfigurationSource" /> class.
        /// </summary>
        public IniConfigurationSource()
        {
        }

        /// <summary>
        ///   Gets the comment string for the ini file.
        /// </summary>
        /// <value>The comment.</value>
        protected virtual string Comment
        {
            get { return _comment; }
        }

        /// <summary>
        ///   Gets the key/value pair delimiter used in the ini file.
        /// </summary>
        /// <value>The delimiter.</value>
        protected virtual string Delimiter
        {
            get { return _delimiter; }
        }

        /// <summary>
        ///   Creates a new <see cref = "IConfigurationSource" /> instance from the file specified.
        /// </summary>
        /// <param name = "fileName">
        ///   The file to load.
        /// </param>
        /// <returns></returns>
        public static IConfigurationSource FromFile( string fileName )
        {
            return FromFile<IniConfigurationSource>( fileName );
        }

        /// <summary>
        ///   Loads the current instance from the given file.
        /// </summary>
        /// <param name = "fileName">The name of the file to load into this instance.</param>
        protected override void Load( string fileName )
        {
            if ( string.IsNullOrEmpty( fileName ) )
            {
                throw new ArgumentNullException( "fileName" );
            }

            FileName = fileName;
            using ( FileStream fileStream = File.Open( FileName, FileMode.Open, FileAccess.Read, FileShare.Read ) )
            {
                using ( var textFile = new StreamReader( fileStream, DefaultEncoding ) )
                {
                    string data = textFile.ReadToEnd();
                    Parse( data );
                }
            }
        }

        private void Parse( string data )
        {
            List<string> sectionData = GetFileSections( data );
            IEnumerable<IConfigurationSection> sections = GetSections( sectionData );

            lock ( SyncRoot )
            {
                foreach ( IConfigurationSection section in sections )
                {
                    Add( section );
                }
            }
        }

        private static List<string> GetFileSections( string data )
        {
            List<string> lines = data.Split( new[] { Environment.NewLine }, StringSplitOptions.None ).ToList();
            List<string> sectionHeaders =
                    lines.Where( line => line.StartsWith( "[", StringComparison.OrdinalIgnoreCase ) &&
                                         line.EndsWith( "]", StringComparison.OrdinalIgnoreCase ) ).ToList();
            var sections = new List<string>( sectionHeaders.Count );

            for ( int i = 0; i < sectionHeaders.Count; i++ )
            {
                int start = lines.IndexOf( sectionHeaders[i] );
                int end = ( i == ( sectionHeaders.Count - 1 ) )
                                  ? ( lines.Count - 1 )
                                  : ( lines.IndexOf( sectionHeaders[i + 1] ) - 1 );
                var sectionBuilder = new StringBuilder();
                for ( int j = start; j <= end; j++ )
                {
                    sectionBuilder.AppendLine( lines[j] );
                }
                string section = sectionBuilder.ToString().Trim();
                sections.Add( section );
            }
            return sections;
        }

        /// <summary>
        ///   Returns a <see cref = "T:System.String" /> that represents the current <see cref = "T:System.Object" />.
        /// </summary>
        /// <returns>
        ///   A <see cref = "T:System.String" /> that represents the current <see cref = "T:System.Object" />.
        /// </returns>
        /// <filterpriority>2</filterpriority>
        public override string ToString()
        {
            var builder = new StringBuilder();
            IEnumerable<string> sections = GetSections();
            foreach ( string section in sections )
            {
                builder.AppendLine( section );
                builder.AppendLine();
            }
            return builder.ToString();
        }

        private IEnumerable<string> GetSections()
        {
            lock ( SyncRoot )
            {
                foreach ( KeyValuePair<string, IConfigurationSection> pair in Sections )
                {
                    yield return GetSection( pair.Value );
                }
            }
        }

        private string GetSection( IConfigurationSection section )
        {
            var sectionBuilder = new StringBuilder();
            sectionBuilder.AppendLine( string.Format( Strings.Culture, "[{0}]", section.Name ) );
            foreach ( KeyValuePair<string, string> pair in section )
            {
                sectionBuilder.AppendLine( string.Format( Strings.Culture, "{0}{1}{2}", pair.Key, Delimiter, pair.Value ) );
            }
            return sectionBuilder.ToString();
        }

        private IEnumerable<IConfigurationSection> GetSections( IEnumerable<string> sectionData )
        {
            foreach ( string section in sectionData )
            {
                yield return GetSection( section );
            }
        }

        private IConfigurationSection GetSection( string sectionData )
        {
            IConfigurationSection section = new ConfigurationSection( "Temp" );
            string[] lines = sectionData.Split( new[] { Environment.NewLine }, StringSplitOptions.None );
            foreach ( string line in lines )
            {
                string workingLine = line;
                if ( line.Contains( Comment ) )
                {
                    workingLine = line.Substring( 0, line.IndexOf( Comment, StringComparison.OrdinalIgnoreCase ) );
                }
                if ( workingLine.StartsWith( "[", StringComparison.OrdinalIgnoreCase ) &&
                     workingLine.EndsWith( "]", StringComparison.OrdinalIgnoreCase ) )
                {
                    string sectionName = workingLine.Substring( 1, workingLine.Length - 2 ).Trim();
                    if ( string.IsNullOrEmpty( sectionName ) )
                    {
                        throw new InvalidOperationException( Strings.NoEmptySectionNames );
                    }
                    section.Name = sectionName;
                    continue;
                }

                string[] pair = workingLine.Split( new[] { Delimiter }, StringSplitOptions.None );
                if ( pair.Length != 2 )
                {
                    continue;
                }
                string key = pair[0].Trim();
                string value = pair[1].Trim();
                if ( string.IsNullOrEmpty( key ) )
                {
                    continue;
                }
                section.Set( key, value );
            }
            return section;
        }
    }
}