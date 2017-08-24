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
using System.Xml.Linq;

#endregion

namespace Dnp.Configuration
{
    /// <summary>
    /// </summary>
    public class XmlConfigurationSource : AbstractFileConfigurationSource
    {
        /// <summary>
        ///   Initializes a new instance of the <see cref = "XmlConfigurationSource" /> class.
        /// </summary>
        /// <param name = "xml">The XML source.</param>
        public XmlConfigurationSource( string xml )
                : this()
        {
            Parse( xml );
        }

        /// <summary>
        ///   Initializes a new instance of the <see cref = "XmlConfigurationSource" /> class.
        /// </summary>
        public XmlConfigurationSource()
        {
            DefaultEncoding = Encoding.UTF8;
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
            return FromFile<XmlConfigurationSource>( fileName );
        }

        /// <summary>
        ///   Parses the specified XML.
        /// </summary>
        /// <param name = "xml">The XML.</param>
        protected virtual void Parse( string xml )
        {
            XDocument loaded = XDocument.Parse( xml );
            Process( loaded );
        }

        /// <summary>
        ///   Loads the current instance from the given file.
        /// </summary>
        /// <param name = "fileName">The name of the file to load into this instance.</param>
        protected override void Load( string fileName )
        {
            FileName = fileName;
            string data;
            using ( FileStream fileStream = File.Open( FileName, FileMode.Open, FileAccess.Read, FileShare.Read ) )
            {
                using ( var textFile = new StreamReader( fileStream, DefaultEncoding ) )
                {
                    data = textFile.ReadToEnd();
                }
            }

            XDocument loaded = XDocument.Parse( data );
            Process( loaded );
        }

        /// <summary>
        ///   Processes the specified document.
        /// </summary>
        /// <param name = "document">The document.</param>
        protected virtual void Process( XDocument document )
        {
            var defaultNameAttribute = new XAttribute( "Name", string.Empty );
            var defaultValueAttribute = new XAttribute( "Value", string.Empty );

            var settings =
                    document.Descendants( "Section" ).SelectMany(
                            section => section.Descendants( "Key" ),
                            ( section, setting ) =>
                            {
                                XAttribute nameAttribute = setting.Attribute( "Name" ) ?? defaultNameAttribute;
                                XAttribute valueAttribute = setting.Attribute( "Value" ) ?? defaultValueAttribute;
                                XAttribute sectionNameAttribute = section.Attribute( "Name" ) ?? defaultNameAttribute;

                                return new
                                       {
                                               Key = nameAttribute.Value,
                                               valueAttribute.Value,
                                               Name = sectionNameAttribute.Value
                                       };
                            }
                            );

            IEnumerable<string> sectionList = from section in document.Descendants( "Section" )
                                              select ( section.Attribute( "Name" ) ?? defaultNameAttribute ).Value;

            foreach ( string current in sectionList )
            {
                string sectionName = current; // copy local as we pass it to to linq. We are passing a closure variable.
                var items =
                        settings.Where(
                                section =>
                                string.Equals( section.Name, sectionName, StringComparison.OrdinalIgnoreCase ) );

                var currentSection = new ConfigurationSection( current );
                foreach ( var item in items )
                {
                    currentSection.Set( item.Key, item.Value );
                }
                Add( currentSection );
            }
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
        public static XElement ToXml( IConfigurationSection section )
        {
            if ( section == null )
            {
                throw new ArgumentNullException( "section" );
            }

            var xml = new XElement( "Section",
                                    new XAttribute( "Name", section.Name ),
                                    from setting in section
                                    select new XElement( "Key",
                                                         new XAttribute( "Name", setting.Key ),
                                                         new XAttribute( "Value", setting.Value )
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
        public static string ToXml( IEnumerable<IConfigurationSection> sections )
        {
            if ( sections == null )
            {
                throw new ArgumentNullException( "sections" );
            }

            var xml = new XElement( "Configuration",
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