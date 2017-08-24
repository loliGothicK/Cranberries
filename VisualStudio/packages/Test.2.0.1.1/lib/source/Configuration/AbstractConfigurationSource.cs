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
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using Dnp.Properties;

#endregion

namespace Dnp.Configuration
{
    /// <summary>
    ///   AbstractConfigurationSource provides a skeleton implementation of 
    ///   <see cref = "IConfigurationSource" /> which provides all merging, adding,
    ///   and expanding features. Inheritors provide implementation specific loading
    ///   and saving functionality.
    /// </summary>
    public abstract class AbstractConfigurationSource : IConfigurationSource
    {
        private readonly IList<IConfigurationSource> _configurationSources;

        private readonly IDictionary<string, IConfigurationSection> _sections;

        private readonly object _syncRoot = new object();

        /// <summary>
        ///   Initializes a new instance of the <see cref = "AbstractConfigurationSource" /> class.
        /// </summary>
        protected AbstractConfigurationSource()
        {
            _sections = new Dictionary<string, IConfigurationSection>();
            _configurationSources = new List<IConfigurationSource>();
        }

        #region Implementation of IConfigurationSource

        /// <summary>
        ///   Provides access to the base configuration sources to inheritors.
        /// </summary>
        protected IList<IConfigurationSource> ConfigurationSources
        {
            get { return _configurationSources; }
        }

        /// <summary>
        ///   Gets the sync root for modifying the internal configuration sources and sections.
        /// </summary>
        /// <value>The sync root.</value>
        protected object SyncRoot
        {
            get { return _syncRoot; }
        }

        /// <summary>
        ///   if <see cref = "AutoSave" /> is <c>true</c>, the inheritor should save
        ///   every time a key is updated; otherwise, updates should be ignored.
        /// </summary>
        /// <value></value>
        public bool AutoSave { get; set; }

        /// <summary>
        ///   Provides user access to named configuration sections.
        /// </summary>
        /// <value></value>
        public IDictionary<string, IConfigurationSection> Sections
        {
            get { return _sections; }
        }

        /// <summary>
        ///   Merges the sources into this instance. Each source's sections will
        ///   be added and merged. If the sources contain duplicate sections, they
        ///   will be merged.
        /// </summary>
        /// <param name = "configurationSources"></param>
        public void Merge( params IConfigurationSource[] configurationSources )
        {
            if ( configurationSources == null )
            {
                throw new ArgumentNullException( "configurationSources" );
            }
            lock ( _syncRoot )
            {
                foreach ( IConfigurationSource source in configurationSources )
                {
                    if ( _configurationSources.Contains( source ) )
                    {
                        continue;
                    }

                    _configurationSources.Add( source );

                    foreach ( IConfigurationSection section in source )
                    {
                        MergeSectionIntoSource( section );
                    }
                }
            }
        }

        /// <summary>
        ///   Saves all sections. All data merged from other merged sources will
        ///   be included.
        /// </summary>
        public abstract void Save();

        /// <summary>
        ///   Discards all sections and merged sources and reloads a fresh set of
        ///   settings.
        /// </summary>
        public abstract void Reload();

        /// <summary>
        ///   Adds a named configuration section to this source. If the section
        ///   name is already in this source, the values from the new source will
        ///   override and be added.
        /// </summary>
        /// <param name = "section">The named section to add.</param>
        public void Add( IConfigurationSection section )
        {
            if ( section == null )
            {
                throw new ArgumentNullException( "section" );
            }
            lock ( _syncRoot )
            {
                MergeSectionIntoSource( section );
            }
        }

        /// <summary>
        ///   Processes all sections expanding configuration variables and saving
        ///   the new values.
        /// </summary>
        public virtual void ExpandKeyValues()
        {
            lock ( _syncRoot )
            {
                foreach ( IConfigurationSection configurationSection in _sections.Values )
                {
                    IDictionary<string, string> toUpdate = new Dictionary<string, string>();
                    foreach ( KeyValuePair<string, string> keyValuePair in configurationSection )
                    {
                        string expandedValue = Expand( configurationSection, keyValuePair.Key );
                        toUpdate.Add( keyValuePair.Key, expandedValue );
                    }
                    foreach ( KeyValuePair<string, string> keyValuePair in toUpdate )
                    {
                        configurationSection.Set( keyValuePair.Key, keyValuePair.Value );
                    }
                }
            }
        }

        /// <summary>
        ///   Clears the sections and sources of this instance.
        /// </summary>
        public void Clear()
        {
            lock ( _syncRoot )
            {
                foreach ( IConfigurationSource configurationSource in _configurationSources )
                {
                    configurationSource.Clear();
                }
                _configurationSources.Clear();
                foreach ( IConfigurationSection section in _sections.Values )
                {
                    section.Clear();
                }
                _sections.Clear();
            }
        }

        #endregion

        private void MergeSectionIntoSource( IConfigurationSection section )
        {
            if ( _sections.ContainsKey( section.Name ) )
            {
                IConfigurationSection existingConfig = _sections[section.Name];
                foreach ( KeyValuePair<string, string> keyValuePair in section )
                {
                    existingConfig.Set( keyValuePair.Key, keyValuePair.Value );
                }
            }
            else
            {
                section.PropertyChanged +=
                        ( s, e ) => OnPropertyChanged( ( (IConfigurationSection) s ).Name, e.PropertyName );
                _sections.Add( section.Name, section );
            }
        }

        /// <summary>
        ///   Called when [property changed].
        /// </summary>
        /// <param name = "section">The section.</param>
        /// <param name = "key">The key.</param>
        protected virtual void OnPropertyChanged( string section, string key )
        {
            Debug.WriteLine( string.Format( Strings.Culture, "Key [{0}]'s value changed in [{1}] section.", key, section ) );
            if ( AutoSave )
            {
                Save();
            }
        }

        private string Expand( IConfigurationSection section, string key )
        {
            string result = section.Get( key, string.Empty );

            while ( true )
            {
                int startIndex = result.IndexOf( "${", 0, StringComparison.OrdinalIgnoreCase );
                if ( startIndex == -1 )
                {
                    break;
                }

                int endIndex = result.IndexOf( "}", startIndex + 2, StringComparison.OrdinalIgnoreCase );
                if ( endIndex == -1 )
                {
                    break;
                }

                string search = result.Substring( startIndex + 2, endIndex - ( startIndex + 2 ) );

                if ( string.Equals( search, key, StringComparison.OrdinalIgnoreCase ) )
                {
                    // Prevent infinite recursion
                    throw new ArgumentException( string.Format( Strings.Culture, Strings.KeyCannotExpandOnSelf0, key ) );
                }

                string replace = ExpandValue( section, search );

                result = result.Replace( "${" + search + "}", replace );
            }

            return result;
        }

        private string ExpandValue( IConfigurationSection section, string search )
        {
            string[] lookup = search.Split( '|' );

            string result = ( lookup.Length > 1 )
                                    ? GetResultFromExternalSection( lookup[0], lookup[1] )
                                    : GetResultFromCurrentSection( search, section );

            return result;
        }

        private string GetResultFromExternalSection( string sectionName, string keyName )
        {
            IConfigurationSection newConfig = _sections[sectionName];
            if ( newConfig == null )
            {
                throw new ArgumentException( string.Format( Strings.Culture, Strings.ExpandSectionMissing0, sectionName ) );
            }
            var result = newConfig.Get<string>( keyName );
            if ( result == null )
            {
                throw new ArgumentException( string.Format( Strings.Culture, Strings.ExpandKeyMissing0, keyName ) );
            }
            return result;
        }

        private static string GetResultFromCurrentSection( string keyName, IConfigurationSection section )
        {
            var result = section.Get<string>( keyName );

            if ( result == null )
            {
                throw new ArgumentException(
                        string.Format( Strings.Culture,
                                       Strings.Key0NotFoundInSection1DuringExpansion,
                                       keyName,
                                       section.Name ),
                        keyName );
            }
            return result;
        }

        #region Implementation of IEnumerable

        /// <summary>
        ///   Returns an enumerator that iterates through the collection.
        /// </summary>
        /// <returns>
        ///   A <see cref = "T:System.Collections.Generic.IEnumerator`1" /> that can
        ///   be used to iterate through the collection.
        /// </returns>
        /// <filterpriority>1</filterpriority>
        public IEnumerator<IConfigurationSection> GetEnumerator()
        {
            lock ( _syncRoot )
            {
                foreach ( KeyValuePair<string, IConfigurationSection> pair in _sections )
                {
                    yield return pair.Value;
                }
            }
        }

        /// <summary>
        ///   Returns an enumerator that iterates through a collection.
        /// </summary>
        /// <returns>
        ///   An <see cref = "T:System.Collections.IEnumerator" /> object that can
        ///   be used to iterate through the collection.
        /// </returns>
        /// <filterpriority>2</filterpriority>
        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }

        #endregion
    }
}