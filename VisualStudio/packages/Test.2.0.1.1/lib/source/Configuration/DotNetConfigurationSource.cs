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
using System.Diagnostics;
using System.Linq;
using System.Xml.Linq;
using Dnp.Properties;

#endregion

namespace Dnp.Configuration
{
    /// <summary>
    /// </summary>
    public class DotNetConfigurationSource : XmlConfigurationSource
    {
        /// <summary>
        ///   Initializes a new instance of the <see cref = "DotNetConfigurationSource" /> class.
        /// </summary>
        /// <param name = "xml">The XML source.</param>
        public DotNetConfigurationSource( string xml )
                : base( xml )
        {
        }

        /// <summary>
        ///   Initializes a new instance of the <see cref = "DotNetConfigurationSource" /> class.
        /// </summary>
        public DotNetConfigurationSource()
        {
        }

        /// <summary>
        ///   Froms the file.
        /// </summary>
        /// <param name = "fileName">Name of the file.</param>
        /// <returns></returns>
        public new static IConfigurationSource FromFile( string fileName )
        {
            return FromFile<DotNetConfigurationSource>( fileName );
        }

        /// <summary>
        ///   Processes the specified document.
        /// </summary>
        /// <param name = "document">The document.</param>
        protected override void Process( XDocument document )
        {
            if ( document == null )
            {
                throw new ArgumentNullException( "document" );
            }

            if ( document.Root == null )
            {
                throw new InvalidOperationException( Strings.DocumentRootMustNotBeNull );
            }

            IEnumerable<string> sections = GetSections( document );
            foreach ( string current in sections )
            {
                string sectionName = current; // copy local as we pass it to to linq. We are passing a closure variable.
                IEnumerable<KeyValuePair<string, string>> items = GetSettings( document, sectionName );
                CreateConfigurationSection( sectionName, items );
            }
        }

        private static IEnumerable<string> GetSections( XDocument document )
        {
            IEnumerable<string> sections = GetSectionsImpl( document );
            return sections;
        }

        private static IEnumerable<string> GetSectionsImpl( XDocument document )
        {
            // we already know this from the caller validating. But resharper complains.
            Debug.Assert( document.Root != null );
            return document.Root.Descendants( "configSections" ).Descendants( "section" ).Select(
                    section => ( section.Attribute( "name" ) ?? new XAttribute( "name", string.Empty ) ).Value );
        }

        private void CreateConfigurationSection( string current, IEnumerable<KeyValuePair<string, string>> items )
        {
            var currentSection = new ConfigurationSection( current );
            foreach ( KeyValuePair<string, string> item in items )
            {
                currentSection.Set( item.Key, item.Value );
            }
            Add( currentSection );
        }

        private static IEnumerable<KeyValuePair<string, string>> GetSettings( XDocument document, string sectionName )
        {
            // we already know this from the caller validating. But resharper complains.
            Debug.Assert( document.Root != null );

            return document.Root.Descendants( sectionName ).SelectMany(
                    section => section.Descendants(),
                    ( section, setting ) =>
                    {
                        XAttribute keyAttribute = setting.Attribute( "key" ) ?? new XAttribute( "key", string.Empty );
                        XAttribute valueAttribute = setting.Attribute( "value" ) ??
                                                    new XAttribute( "value", string.Empty );
                        return new KeyValuePair<string, string>( keyAttribute.Value, valueAttribute.Value );
                    } );
        }

        /// <summary>
        ///   Converts the given <see cref = "IConfigurationSection" /> to an XML representation.
        /// </summary>
        /// <param name = "section">
        ///   The section to convert to XML.
        /// </param>
        /// <returns>
        ///   A XML representation of the <see cref = "IConfigurationSection" />.
        /// </returns>
        public new static XElement ToXml( IConfigurationSection section )
        {
            if ( section == null )
            {
                throw new ArgumentNullException( "section" );
            }
            var xml = new XElement( section.Name,
                                    from setting in section
                                    select new XElement( "add",
                                                         new XAttribute( "key", setting.Key ),
                                                         new XAttribute( "value", setting.Value )
                                            )
                    );
            return xml;
        }

        /// <summary>
        ///   Converts the given <see cref = "IConfigurationSection" />s to an XML representation.
        /// </summary>
        /// <param name = "sections">
        ///   The sections to convert to XML.
        /// </param>
        /// <returns>
        ///   A XML representation of the <see cref = "IConfigurationSection" />s.
        /// </returns>
        public new static string ToXml( IEnumerable<IConfigurationSection> sections )
        {
            if ( sections == null )
            {
                throw new ArgumentNullException( "sections" );
            }

            var xml = new XElement( "configuration",
                                    new XElement( "configSections",
                                                  from section in sections
                                                  select
                                                          new XElement( "section",
                                                                        new XAttribute( "name", section.Name ),
                                                                        new XAttribute( "type",
                                                                                        "System.Configuration.NameValueSectionHandler" ) ) ),
                                    from section in sections
                                    select ToXml( section ) );
            return xml.ToString();
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
            return ToXml( this );
        }
    }
}