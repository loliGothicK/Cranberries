#region License

// 
// Copyright (c) 2009-2011, Ian Davis <ian@innovatian.com>
// 
// Dual-licensed under the Apache License, Version 2.0, and the Microsoft Public License (Ms-PL).
// See the file LICENSE.txt for details.
// 

#endregion

#region Using Directives

using System.Collections.Generic;

#endregion

namespace Dnp.Configuration
{
    /// <summary>
    ///   Defines methods and properties representing a configuration data source.
    /// </summary>
    public interface IConfigurationSource : IEnumerable<IConfigurationSection>
    {
        /// <summary>
        ///   if <see cref = "AutoSave" /> is <c>true</c>, the inheritor should save
        ///   every time a key is updated; otherwise, updates should be ignored.
        /// </summary>
        bool AutoSave { get; set; }

        /// <summary>
        ///   Provides user access to named configuration sections.
        /// </summary>
        IDictionary<string, IConfigurationSection> Sections { get; }

        /// <summary>
        ///   Adds a named configuration section to this source. If the section
        ///   name is already in this source, the values from the new source will
        ///   override and be added.
        /// </summary>
        /// <param name = "section">The named section to add.</param>
        void Add( IConfigurationSection section );

        /// <summary>
        ///   Processes all sections expanding configuration variables and saving
        ///   the new values.
        /// </summary>
        void ExpandKeyValues();

        /// <summary>
        ///   Merges the sources into this instance. Each source's sections will
        ///   be added and merged. If the sources contain duplicate sections, they
        ///   will be merged.
        /// </summary>
        /// <param name = "configurationSources"></param>
        void Merge( params IConfigurationSource[] configurationSources );

        /// <summary>
        ///   Discards all sections and merged sources and reloads a fresh set of
        ///   settings.
        /// </summary>
        void Reload();

        /// <summary>
        ///   Saves all sections. All data merged from other merged sources will
        ///   be included.
        /// </summary>
        void Save();

        /// <summary>
        ///   Clears all sections, their values, and calls Clear on all internal
        ///   configuration sources.
        /// </summary>
        void Clear();
    }
}