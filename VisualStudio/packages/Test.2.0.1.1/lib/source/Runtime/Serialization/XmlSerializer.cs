#region Using Directives

using System;
using System.IO;
using System.Text;
using System.Xml;
using System.Xml.Serialization;
using Dnp.IO;

#endregion

namespace Dnp.Runtime.Serialization
{
    public class XmlSerializer<TRoot> : XmlSerializer
    {
        public XmlSerializer()
                : base( typeof (TRoot) )
        {
            Init();
        }

        public XmlSerializer( Type[] extraTypes )
                : base( typeof (TRoot), extraTypes )
        {
            Init();
        }

        public XmlSerializer( string defaultNamespace )
                : base( typeof (TRoot), defaultNamespace )
        {
            Init();
        }

        public XmlSerializer( XmlAttributeOverrides overrides )
                : base( typeof (TRoot), overrides )
        {
            Init();
        }

        public XmlSerializer( XmlRootAttribute root )
                : base( typeof (TRoot), root )
        {
            Init();
        }

        public XmlSerializer( XmlAttributeOverrides overrides,
                              Type[] extraTypes,
                              XmlRootAttribute root,
                              string defaultNamespace )
                : base( typeof (TRoot), overrides, extraTypes, root, defaultNamespace )
        {
            Init();
        }

        public XmlSerializer( XmlAttributeOverrides overrides,
                              Type[] extraTypes,
                              XmlRootAttribute root,
                              string defaultNamespace,
                              string location )
                : base( typeof (TRoot), overrides, extraTypes, root, defaultNamespace, location )
        {
            Init();
        }

        public bool Indent { get; set; }
        public string IndentChars { get; set; }

        protected virtual void Init()
        {
            Indent = true;
            IndentChars = "\t";
        }

        public string Serialize( TRoot root )
        {
            return Serialize( root, Encoding.UTF8 );
        }

        public string Serialize( TRoot root, Encoding encoding )
        {
            return Serialize( root, encoding, null );
        }

        public string Serialize( TRoot root, Encoding encoding, XmlSerializerNamespaces serializerNamespaces )
        {
            var stringBuilder = new StringBuilder();

            using ( var stringWriter = new StringWriterWithEncoding( stringBuilder, encoding ) )
            {
                using ( XmlWriter xmlWriter = GetXmlWriter( stringWriter, encoding ) )
                {
                    if ( serializerNamespaces == null )
                    {
                        Serialize( xmlWriter, root );
                    }
                    else
                    {
                        Serialize( xmlWriter, root, serializerNamespaces );
                    }
                }
            }
            string xml = stringBuilder.ToString();
            return xml;
        }

        private XmlWriter GetXmlWriter( StringWriter stringWriter, Encoding encoding )
        {
            var settings = new XmlWriterSettings
                           {
                                   Indent = Indent,
                                   IndentChars = IndentChars,
                                   Encoding = encoding
                           };
            XmlWriter xmlWriter = XmlWriter.Create( stringWriter, settings );
            return xmlWriter;
        }

        public TRoot Deserialize( string xml )
        {
            return Deserialize( xml, Encoding.UTF8 );
        }

        public TRoot Deserialize( string xml, Encoding encoding )
        {
            byte[] buffer = encoding.GetBytes( xml );
            using ( var dataStream = new MemoryStream( buffer ) )
            {
                TRoot result = Deserialize( dataStream );
                return result;
            }
        }

        public new TRoot Deserialize( Stream stream )
        {
            return (TRoot) base.Deserialize( stream );
        }

        public new TRoot Deserialize( TextReader textReader )
        {
            return (TRoot) base.Deserialize( textReader );
        }

        public new TRoot Deserialize( XmlReader xmlReader )
        {
            return (TRoot) base.Deserialize( xmlReader );
        }

        public new TRoot Deserialize( XmlReader xmlReader, string encodingStyle )
        {
            return (TRoot) base.Deserialize( xmlReader, encodingStyle );
        }

        public new TRoot Deserialize( XmlReader xmlReader, XmlDeserializationEvents events )
        {
            return (TRoot) base.Deserialize( xmlReader, events );
        }

        public new TRoot Deserialize( XmlReader xmlReader, string encodingStyle, XmlDeserializationEvents events )
        {
            return (TRoot) base.Deserialize( xmlReader, encodingStyle, events );
        }
    }
}