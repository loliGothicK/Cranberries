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
using System.ComponentModel;

#endregion

namespace Dnp.Configuration
{
    /// <summary>
    ///   Defines a named section of a configuration exposing key value setting
    ///   pairs. All settings are stored as strings until requested.
    /// </summary>
    public interface IConfigurationSection : INotifyPropertyChanged, ICollection<KeyValuePair<string, string>>
    {
        /// <summary>
        ///   The name of the section.
        /// </summary>
        string Name { get; set; }

        /// <summary>
        ///   Returns the value of the setting name <paramref name = "key" /> as a
        ///   <c>T</c> instance.
        /// </summary>
        /// <typeparam name = "T">
        ///   The type needed by the user.
        /// </typeparam>
        /// <param name = "key">
        ///   The name of the setting.
        /// </param>
        /// <returns>returns the setting value as a <c>T</c> instance or
        ///   defatult(T) if the key does not exist.
        /// </returns>
        T Get<T>( string key );

        /// <summary>
        ///   Returns <c>true</c> if the value of the setting name <paramref name = "key" /> 
        ///   exists and sets the <c>T</c> instance via the out parameter. If the key
        ///   was not found, <c>false</c> is returned and the out value is set to default(T).
        /// </summary>
        /// <typeparam name = "T">
        ///   The type needed by the user.
        /// </typeparam>
        /// <param name = "key">
        ///   The name of the setting.
        /// </param>
        /// <param name = "value">
        ///   The value to set if the key exists.
        /// </param>
        /// <returns>returns <c>true</c> if the setting exists and sets the value as a <c>T</c> instance or
        ///   <c>false</c> if the key does not exist and sets the out value to default(T).
        /// </returns>
        bool TryGet<T>( string key, out T value );

        /// <summary>
        ///   Returns the value of the setting name <paramref name = "key" /> as a
        ///   <c>T</c> instance.
        /// </summary>
        /// <typeparam name = "T">
        ///   The type needed by the user.
        /// </typeparam>
        /// <param name = "key">
        ///   The name of the setting.
        /// </param>
        /// <param name = "defaultValue">
        ///   The default value.
        /// </param>
        /// <returns>returns the setting value as a <c>T</c> instance or <paramref name = "defaultValue" /> if the key does not exist.
        /// </returns>
        T Get<T>( string key, T defaultValue );

        /// <summary>
        ///   Removes the specified key and its value from the section.
        /// </summary>
        /// <param name = "key">
        ///   The name of the setting.
        /// </param>
        /// <returns>
        ///   <c>true</c> if the setting named <paramref name = "key" /> existed and was
        ///   removed; <c>false</c> otherwise.
        /// </returns>
        bool Remove( string key );

        /// <summary>
        ///   Sets the value of the setting name <paramref name = "key" />. If the setting
        ///   does not exist, it is created.
        /// </summary>
        /// <typeparam name = "T">
        ///   The type of the setting being saved.
        /// </typeparam>
        /// <param name = "key">
        ///   The name of the setting.
        /// </param>
        /// <param name = "value">
        ///   The value to save in this section.
        /// </param>
        void Set<T>( string key, T value );
    }
}