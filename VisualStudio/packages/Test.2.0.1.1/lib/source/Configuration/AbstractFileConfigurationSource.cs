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
using System.Diagnostics;
using System.IO;
using System.Security.Permissions;
using System.Text;
using Dnp.Properties;

#endregion

namespace Dnp.Configuration
{
    /// <summary>
    ///   AbstractFileConfigurationSource add helper functions to load configuration sources from file
    ///   that are not needed in general implementations of <see cref = "IConfigurationSource" />.
    /// </summary>
    public abstract class AbstractFileConfigurationSource : AbstractConfigurationSource
    {
        /// <summary>
        ///   Initializes a new instance of the <see cref = "AbstractFileConfigurationSource" /> class.
        /// </summary>
        protected AbstractFileConfigurationSource()
        {
            DefaultEncoding = Encoding.Default;
        }

        /// <summary>
        ///   Gets or sets the default file encoding.
        /// </summary>
        /// <value>The default encoding.</value>
        public Encoding DefaultEncoding { get; set; }

        /// <summary>
        ///   The file used to store the configuration on disk.
        /// </summary>
        public string FileName { get; set; }

        /// <summary>
        ///   Loads the current instance from the given file.
        /// </summary>
        /// <param name = "fileName">
        ///   The name of the file to load into this instance.
        /// </param>
        [FileIOPermission( SecurityAction.Demand, AllFiles = FileIOPermissionAccess.Read,
                AllLocalFiles = FileIOPermissionAccess.Read, Unrestricted = true )]
        protected abstract void Load( string fileName );

        /// <summary>
        ///   Loads an <see cref = "IConfigurationSource" /> from the given file.
        /// </summary>
        /// <param name = "fileName">
        ///   The name of the file to load into a new instance.
        /// </param>
        /// <returns></returns>
        protected static IConfigurationSource FromFile<T>( string fileName )
                where T : AbstractFileConfigurationSource
        {
            Type typeToLoad = typeof (T);
            var instance =
                    Activator.CreateInstance( typeToLoad, true ) as
                    AbstractFileConfigurationSource;
            Debug.Assert( instance != null );
            instance.Load( fileName );
            return instance;
        }

        /// <summary>
        ///   Saves all sections. All data merged from other merged sources will
        ///   be included.
        /// </summary>
        [FileIOPermission( SecurityAction.Demand, AllFiles = FileIOPermissionAccess.Write,
                AllLocalFiles = FileIOPermissionAccess.Write, Unrestricted = true )]
        public override void Save()
        {
            if ( string.IsNullOrEmpty( FileName ) )
            {
                throw new InvalidOperationException( Strings.DestinationFileNameNotSet );
            }

            if ( !File.Exists( FileName ) )
            {
                using ( File.Create( FileName ) )
                {
                    // just making sure it exists.
                }
            }

            using ( FileStream fileStream = File.Open( FileName, FileMode.Truncate, FileAccess.Write, FileShare.Read ) )
            {
                using ( var textFile = new StreamWriter( fileStream, DefaultEncoding ) )
                {
                    string text = ToString();
                    textFile.Write( text );
                    textFile.Flush();
                }
            }
        }

        /// <summary>
        ///   If this instance has a file associated with it 
        ///   then the settings are cleared and reloaded from file.
        ///   If, however, there is no <see cref = "FileName" /> set, then
        ///   nothing is done.
        /// </summary>
        public override void Reload()
        {
            if ( string.IsNullOrEmpty( FileName ) )
            {
                return;
            }

            Clear();
            Load( FileName );
        }
    }
}