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
using System.Linq;
using System.Security.AccessControl;
using System.Security.Permissions;
using Microsoft.Win32;

#endregion

namespace Dnp.Configuration
{
    /// <summary>
    /// </summary>
    [RegistryPermission( SecurityAction.Demand, Unrestricted = true )]
    public class RegistryConfigurationSource : AbstractConfigurationSource, IDisposable
    {
        private const string DefaultKeyName = "(Default)";

        /// <summary>
        ///   Initializes a new instance of the <see cref = "RegistryConfigurationSource" /> class.
        /// </summary>
        /// <param name = "root">The root key to load settings from.</param>
        public RegistryConfigurationSource( RegistryKey root )
                : this( root, true )
        {
        }

        /// <summary>
        ///   Initializes a new instance of the <see cref = "RegistryConfigurationSource" /> class.
        /// </summary>
        /// <param name = "root">The root key to load settings from.</param>
        public RegistryConfigurationSource( string root )
                : this( root, true )
        {
        }

        /// <summary>
        ///   Initializes a new instance of the <see cref = "RegistryConfigurationSource" /> class.
        /// </summary>
        /// <param name = "root">The root key to load settings from.</param>
        /// <param name = "recursive">if the recursively process subkeys. Default is true.</param>
        public RegistryConfigurationSource( RegistryKey root, bool recursive )
        {
            Recursive = true;
            if ( root == null )
            {
                throw new ArgumentNullException( "root" );
            }

            Recursive = recursive;

            Root = root;
            Load( Root );
        }

        /// <summary>
        ///   Initializes a new instance of the <see cref = "RegistryConfigurationSource" /> class.
        /// </summary>
        /// <param name = "root">The root key to load settings from.</param>
        /// <param name = "recursive">if the recursively process subkeys. Default is true.</param>
        public RegistryConfigurationSource( string root, bool recursive )
        {
            Recursive = true;
            if ( root == null )
            {
                throw new ArgumentNullException( "root" );
            }

            Recursive = recursive;

            Root = OpenKey( root, RegistryKeyPermissionCheck.ReadWriteSubTree );
            Load( Root );
        }

        /// <summary>
        ///   Gets or sets the root registry key used by this source.
        /// </summary>
        /// <value>The root.</value>
        public RegistryKey Root { get; private set; }

        /// <summary>
        ///   Whether this instance processes registry keys recusively or just load the root values.
        /// </summary>
        public bool Recursive { get; private set; }

        /// <summary>
        ///   From the root key, create sections for every sub-key containing its values.
        /// </summary>
        [RegistryPermission( SecurityAction.Demand, Unrestricted = true )]
        private void Load( RegistryKey root )
        {
            {
                string pathList = root.ToString();
                var permission = new RegistryPermission( RegistryPermissionAccess.Read,
                                                         AccessControlActions.View,
                                                         pathList );
                permission.Demand();
            }

            IEnumerable<IConfigurationSection> sections = GetSections( root );
            sections.ToList().ForEach( Add );
        }

        private IEnumerable<IConfigurationSection> GetSections( RegistryKey root )
        {
            IEnumerable<string> subkeys = GetSubKeys( root ); // GetSubKeys does CAS checks internally.
            foreach ( string subKeyName in subkeys )
            {
                string sectionName = subKeyName.Replace( Root.Name, string.Empty ).Trim( '\\' );
                IConfigurationSection section = GetSection( subKeyName, sectionName );
                if ( section == null )
                {
                    continue;
                }
                yield return section;

                if ( !Recursive ||
                     string.Equals( root.Name, subKeyName ) )
                {
                    continue;
                }

                RegistryKey subKey = OpenKey( subKeyName, RegistryKeyPermissionCheck.ReadSubTree );
                IEnumerable<IConfigurationSection> sections = GetSections( subKey );
                foreach ( IConfigurationSection subSection in  sections )
                {
                    yield return subSection;
                }
            }
        }

        private IConfigurationSection GetSection( string subKey, string sectionName )
        {
            IConfigurationSection section = null;
            using ( RegistryKey key = GetKey( subKey, ref sectionName, RegistryKeyPermissionCheck.Default ) )
            {
                if ( key == null )
                {
                    return section;
                }

                section = new ConfigurationSection( sectionName );
                foreach ( string valueName in key.GetValueNames() )
                {
                    string name = valueName;
                    if ( string.IsNullOrEmpty( valueName ) )
                    {
                        name = DefaultKeyName;
                    }
                    object value = key.GetValue( valueName );
                    //string stringValue = SettingConverter.GetStringFromT( value );
                    switch ( key.GetValueKind( valueName ) )
                    {
                        case RegistryValueKind.Binary:
                            section.Set( name, (byte[]) value );
                            break;
                        case RegistryValueKind.DWord:
                            section.Set( name, (int) value );
                            break;
                        case RegistryValueKind.ExpandString:
                            section.Set( name, (string) value );
                            break;
                        case RegistryValueKind.MultiString:
                            section.Set( name, (string[]) value );
                            break;
                        case RegistryValueKind.QWord:
                            section.Set( name, (long) value );
                            break;
                        case RegistryValueKind.String:
                            section.Set( name, (string) value );
                            break;
                        case RegistryValueKind.Unknown:
                            section.Set( name, value );
                            break;
                    }
                }
            }
            return section;
        }

        /// <summary>
        ///   Recursively creates full key name for all subkeys of the root.
        /// </summary>
        private IEnumerable<string> GetSubKeys( RegistryKey registryKey )
        {
            foreach ( string subKeyName in registryKey.GetSubKeyNames() )
            {
                if ( Recursive )
                {
                    const RegistryKeyPermissionCheck permissionCheck = RegistryKeyPermissionCheck.ReadSubTree;

                    RegistryKey subTree = registryKey.OpenSubKey( subKeyName, permissionCheck );
                    foreach ( string subKey in GetSubKeys( subTree ) )
                    {
                        yield return subKey;
                    }
                }
                else
                {
                    yield return subKeyName;
                }
            }

            yield return registryKey.Name;
        }

        internal static RegistryKey CloneKey( RegistryKey source, RegistryKeyPermissionCheck permissionCheck )
        {
            RegistryKey clone = OpenKey( source.Name, permissionCheck );
            return clone;
        }

        internal static RegistryKey OpenKey( string key, RegistryKeyPermissionCheck permissionCheck )
        {
            RegistryKey root = OpenRoot( key );

            string path = key.Replace( root.Name, string.Empty ).Trim( '\\' );
            if ( string.IsNullOrEmpty( path ) )
            {
                return root;
            }

            using ( root )
            {
                return root.CreateSubKey( path, permissionCheck );
            }
        }

        internal static RegistryKey OpenRoot( string key )
        {
            string[] pathParts = key.Split( new[] { @"\" }, StringSplitOptions.None );
            RegistryKey currentKey = null;

            switch ( pathParts[0].ToUpper() )
            {
                case "HKEY_CLASSES_ROOT":
                    currentKey = Registry.ClassesRoot;
                    break;
                case "HKEY_CURRENT_CONFIG":
                    currentKey = Registry.CurrentConfig;
                    break;
                case "HKEY_CURRENT_USER":
                    currentKey = Registry.CurrentUser;
                    break;
                    //case "HKEY_DYN_DATA":
                    // 'Microsoft.Win32.Registry.DynData' is obsolete: 'The DynData registry key only works on Win9x, which is no longer supported by the CLR.  On NT-based operating systems, use the PerformanceData registry key instead.'
                    //currentKey = Registry.DynData;
                    //break;
                case "HKEY_LOCAL_MACHINE":
                    currentKey = Registry.LocalMachine;
                    break;
                case "HKEY_PERFORMANCE_DATA":
                    currentKey = Registry.PerformanceData;
                    break;
                case "HKEY_USERS":
                    currentKey = Registry.Users;
                    break;
            }
            return currentKey;
        }

        private RegistryKey GetKey( string sectionName, RegistryKeyPermissionCheck permissionCheck )
        {
            RegistryKey key;
            if ( Root.Name.EndsWith( ( sectionName ) ) )
            {
                key = CloneKey( Root, permissionCheck );
            }
            else
            {
                string trimmed = sectionName.Replace( Root.Name, string.Empty ).Trim( '\\' );
                key = Root.CreateSubKey( trimmed, permissionCheck );
            }
            return key;
        }

        private RegistryKey GetKey( string subKey, ref string sectionName, RegistryKeyPermissionCheck permissionCheck )
        {
            RegistryKey key;
            if ( string.Equals( subKey, Root.Name ) )
            {
                sectionName = Root.Name.Substring( Root.Name.LastIndexOf( '\\' ) + 1 );
                key = CloneKey( Root, permissionCheck );
            }
            else
            {
                key = Root.OpenSubKey( sectionName, permissionCheck );
            }
            return key;
        }

        #region Overrides of AbstractConfigurationSource

        /// <summary>
        ///   Saves all sections. All data merged from other merged sources will
        ///   be included.
        /// </summary>
        public override void Save()
        {
            lock ( SyncRoot )
            {
                foreach ( IConfigurationSection section in Sections.Values )
                {
                    foreach ( KeyValuePair<string, string> pair in section )
                    {
                        SaveValue( section, pair );
                    }
                }
            }
        }

        private void SaveValue( IConfigurationSection section, KeyValuePair<string, string> pair )
        {
            using ( RegistryKey key = GetKey( section.Name, RegistryKeyPermissionCheck.ReadWriteSubTree ) )
            {
                if ( key == null )
                {
                    return;
                }

                string keyName = string.Equals( pair.Key, DefaultKeyName, StringComparison.OrdinalIgnoreCase )
                                         ? string.Empty
                                         : pair.Key;

                RegistryValueKind valueKind = GetValueKind( key, keyName );
                switch ( valueKind )
                {
                    case RegistryValueKind.MultiString:
                        key.SetValue( keyName, new StringArrayConverter().ConvertFrom( pair.Value ), valueKind );
                        break;
                    case RegistryValueKind.Binary:
                        key.SetValue( keyName, new ByteArrayConverter().ConvertFrom( pair.Value ), valueKind );
                        break;
                    default:
                        key.SetValue( keyName, pair.Value, valueKind );
                        break;
                }
            }
        }

        private static RegistryValueKind GetValueKind( RegistryKey key, string keyName )
        {
            RegistryValueKind valueKind = RegistryValueKind.Unknown;

            if ( key.GetValueNames().Contains( keyName ) )
            {
                valueKind = key.GetValueKind( keyName );
            }

            if ( valueKind == RegistryValueKind.Unknown )
            {
                valueKind = RegistryValueKind.String;
            }
            return valueKind;
        }

        /// <summary>
        ///   Discards all sections and merged sources and reloads a fresh set of
        ///   settings.
        /// </summary>
        public override void Reload()
        {
            Clear();
            Load( Root );
        }

        #endregion

        #region Implementation of IDisposable

        /// <summary>
        ///   Returns whether this is has been disposed.
        /// </summary>
        public bool IsDisposed { get; private set; }

        /// <summary>
        ///   Performs application-defined tasks associated with freeing, releasing, or resetting unmanaged resources.
        /// </summary>
        /// <filterpriority>2</filterpriority>
        public void Dispose()
        {
            Dispose( true );
            GC.SuppressFinalize( this );
        }

        private void Dispose( bool disposing )
        {
            if ( disposing && !IsDisposed )
            {
                using ( Root )
                {
                }
                Root = null;
                IsDisposed = true;
            }
        }

        #endregion
    }
}