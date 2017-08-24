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
using System.IO;
using System.IO.IsolatedStorage;
using System.Reflection;
using System.Security.Permissions;
using System.Text;
using System.Xml;
using Dnp.Properties;

#endregion

namespace Dnp.Configuration
{
    /// <summary>
    /// </summary>
    [IsolatedStorageFilePermission( SecurityAction.Demand, Unrestricted = true )]
    public class IsoStorageConfigurationSource : AbstractConfigurationSource
    {
        private const IsolatedStorageScope DefaultScope =
                IsolatedStorageScope.Application | IsolatedStorageScope.User;

        private static readonly FieldInfo FullPathAccessor = typeof (IsolatedStorageFileStream)
                .GetField( "m_FullPath", BindingFlags.Instance | BindingFlags.NonPublic );

        private readonly string _fileName;

        /// <summary>
        ///   Initializes a new instance of the <see cref = "IsoStorageConfigurationSource" /> class.
        /// </summary>
        /// <param name = "fileName">
        ///   Name of the file.
        /// </param>
        /// <remarks>
        ///   If you specify invalid scope for your application, you will get an <see cref = "IsolatedStorageException" />.
        /// </remarks>
        public IsoStorageConfigurationSource( string fileName )
                : this( DefaultScope, fileName )
        {
        }

        /// <summary>
        ///   Initializes a new instance of the <see cref = "IsoStorageConfigurationSource" /> class.
        /// </summary>
        /// <param name = "scope">
        ///   The scope for the isolated storage file.
        /// </param>
        /// <param name = "fileName">
        ///   Name of the file.
        /// </param>
        /// <remarks>
        ///   If you specify invalid scope for your application, you will get an <see cref = "IsolatedStorageException" />.
        /// </remarks>
        public IsoStorageConfigurationSource( IsolatedStorageScope scope, string fileName )
        {
            if ( scope == IsolatedStorageScope.None )
            {
                throw new ArgumentException( Strings.InvalidStorageScope );
            }

            if ( string.IsNullOrEmpty( fileName ) )
            {
                throw new ArgumentNullException( "fileName" );
            }

            Scope = scope;
            _fileName = fileName;

            Load();
        }

        #region Overrides of AbstractConfigurationSource

        /// <summary>
        ///   Saves all sections. All data merged from other merged sources will
        ///   be included.
        /// </summary>
        public override void Save()
        {
            using ( IsolatedStorageFile isolatedStorageFile = IsolatedStorageFile.GetStore( Scope, null, null ) )
            {
                using ( var isoStream =
                        new IsolatedStorageFileStream( _fileName,
                                                       FileMode.Truncate,
                                                       FileAccess.Write,
                                                       FileShare.Read,
                                                       isolatedStorageFile ) )
                {
                    string xml = XmlConfigurationSource.ToXml( Sections.Values );
                    byte[] xmlBytes = Encoding.UTF8.GetBytes( xml );
                    isoStream.Write( xmlBytes, 0, xmlBytes.Length );
                    isoStream.Flush();
                }
            }
        }

        /// <summary>
        ///   Discards all sections and merged sources and reloads a fresh set of
        ///   settings.
        /// </summary>
        public override void Reload()
        {
            Clear();
            Load();
        }

        #endregion

        /// <summary>
        ///   Gets or sets the full path to the isolated storage file.
        /// </summary>
        /// <value>The full path.</value>
        public string FullPath { get; private set; }

        /// <summary>
        ///   Gets or sets the IsolatedStorageScope for this configuration source.
        /// </summary>
        /// <value>The scope.</value>
        public IsolatedStorageScope Scope { get; private set; }

        private void Load()
        {
            using ( IsolatedStorageFile isolatedStorageFile = IsolatedStorageFile.GetStore( Scope, null, null ) )
            {
                using ( var isoStream =
                        new IsolatedStorageFileStream( _fileName,
                                                       FileMode.OpenOrCreate,
                                                       FileAccess.ReadWrite,
                                                       FileShare.Read,
                                                       isolatedStorageFile ) )
                {
                    if ( string.IsNullOrEmpty( FullPath ) )
                    {
                        FullPath = FullPathAccessor.GetValue( isoStream ).ToString();
                    }

                    if ( isoStream.Length == 0 )
                    {
                        return;
                    }

                    var document = new XmlDocument();
                    document.Load( isoStream );
                    string xml = document.OuterXml;
                    var source = new XmlConfigurationSource( xml );
                    Merge( source );
                }
            }
        }
    }
}