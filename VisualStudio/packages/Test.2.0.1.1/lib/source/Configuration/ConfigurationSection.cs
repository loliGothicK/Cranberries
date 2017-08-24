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
using System.ComponentModel;
using Dnp.Properties;

#endregion

namespace Dnp.Configuration
{
    /// <summary>
    /// </summary>
    public class ConfigurationSection : IConfigurationSection, IEquatable<ConfigurationSection>
    {
        private readonly IDictionary<string, string> _settings;

        /// <summary>
        ///   Initializes a new instance of the <see cref = "ConfigurationSection" />
        ///   class.
        /// </summary>
        /// <param name = "sectionName">Name of the section.</param>
        public ConfigurationSection( string sectionName )
        {
            if ( string.IsNullOrEmpty( sectionName ) )
            {
                throw new ArgumentNullException( "sectionName" );
            }
            Name = sectionName;

            _settings = new Dictionary<string, string>( StringComparer.OrdinalIgnoreCase );
        }

        #region Implementation of INotifyPropertyChanged

        /// <summary>
        ///   Occurs when a property value changes.
        /// </summary>
        public event PropertyChangedEventHandler PropertyChanged;

        #endregion

        #region Implementation of IConfigurationSection

        /// <summary>
        ///   The name of the section.
        /// </summary>
        /// <value></value>
        public string Name { get; set; }

        /// <summary>
        ///   Returns the value of the setting name <paramref name = "key" /> as a
        ///   <c>T</c> instance.
        /// </summary>
        /// <typeparam name = "T">The type needed by the user.</typeparam>
        /// <param name = "key">The name of the setting.</param>
        /// <returns>
        ///   returns the setting value as a <c>T</c> instance or defatult(T) if
        ///   the key does not exist.
        /// </returns>
        public T Get<T>( string key )
        {
            if ( string.IsNullOrEmpty( key ) )
            {
                throw new ArgumentNullException( "key" );
            }

            return Get( key, default( T ) );
        }

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
        public bool TryGet<T>( string key, out T value )
        {
            if ( string.IsNullOrEmpty( key ) )
            {
                throw new ArgumentNullException( "key" );
            }

            if ( _settings.ContainsKey( key ) )
            {
                value = Get<T>( key );
                return true;
            }
            value = default( T );
            return false;
        }

        /// <summary>
        ///   Returns the value of the setting name <paramref name = "key" /> as a
        ///   <c>T</c> instance.
        /// </summary>
        /// <typeparam name = "T">The type needed by the user.</typeparam>
        /// <param name = "key">The name of the setting.</param>
        /// <param name = "defaultValue">The default value.</param>
        /// <returns>
        ///   returns the setting value as a <c>T</c> instance or <paramref name = "defaultValue" /> if the key does not exist.
        /// </returns>
        public T Get<T>( string key, T defaultValue )
        {
            if ( string.IsNullOrEmpty( key ) )
            {
                throw new ArgumentNullException( "key" );
            }

            string result = null;

            if ( _settings.ContainsKey( key ) )
            {
                result = _settings[key];
            }

            return ( result == null ) ? defaultValue : SettingConverter.GetTFromString<T>( result );
        }

        /// <summary>
        ///   Removes the specified key and its value from the section.
        /// </summary>
        /// <param name = "key">The name of the setting.</param>
        /// <returns>
        ///   <c>true</c> if the setting named <paramref name = "key" /> existed and
        ///   was
        ///   removed; <c>false</c> otherwise.
        /// </returns>
        public bool Remove( string key )
        {
            if ( string.IsNullOrEmpty( key ) )
            {
                throw new ArgumentNullException( "key" );
            }

            if ( _settings.ContainsKey( key ) )
            {
                _settings.Remove( key );
                return true;
            }

            return false;
        }

        /// <summary>
        ///   Sets the value of the setting name <paramref name = "key" />. If the setting
        ///   does not exist, it is created.
        /// </summary>
        /// <typeparam name = "T">The type of the setting being saved.</typeparam>
        /// <param name = "key">The name of the setting.</param>
        /// <param name = "value">The value to save in this section.</param>
        public void Set<T>( string key, T value )
        {
            if ( string.IsNullOrEmpty( key ) )
            {
                throw new ArgumentNullException( "key" );
            }

            if ( _settings.ContainsKey( key ) )
            {
                _settings[key] = SettingConverter.GetStringFromT( value );
            }
            else
            {
                _settings.Add( key, SettingConverter.GetStringFromT( value ) );
            }
            PropertyChanged.Raise( this, new PropertyChangedEventArgs( key ) );
        }

        #endregion

        #region Implementation of IEnumerable

        /// <summary>
        ///   Returns an enumerator that iterates through the collection.
        /// </summary>
        /// <returns>
        ///   A <see cref = "T:System.Collections.Generic.IEnumerator`1" /> that can
        ///   be used to iterate through the collection.
        /// </returns>
        /// <filterpriority>1</filterpriority>
        public IEnumerator<KeyValuePair<string, string>> GetEnumerator()
        {
            return _settings.GetEnumerator();
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

        #region IEquatable<ConfigurationSection> Members

        /// <summary>
        ///   Indicates whether the current object is equal to another object of
        ///   the same type.
        /// </summary>
        /// <returns>
        ///   true if the current object is equal to the <paramref name = "other" />
        ///   parameter; otherwise, false.
        /// </returns>
        /// <param name = "other">
        ///   An object to compare with this object.
        /// </param>
        public bool Equals( ConfigurationSection other )
        {
            if ( ReferenceEquals( null, other ) )
            {
                return false;
            }
            if ( ReferenceEquals( this, other ) )
            {
                return true;
            }
            if ( other._settings.Count !=
                 _settings.Count )
            {
                return false;
            }
            foreach ( KeyValuePair<string, string> pair in _settings )
            {
                if ( !other._settings.ContainsKey( pair.Key ) )
                {
                    return false;
                }
                if (
                        !string.Equals( other._settings[pair.Key], pair.Value, StringComparison.OrdinalIgnoreCase ) )
                {
                    return false;
                }
            }
            return Equals( other.Name, Name );
        }

        #endregion

        /// <summary>
        ///   Determines whether the specified <see cref = "T:System.Object" /> is
        ///   equal to the current <see cref = "T:System.Object" />.
        /// </summary>
        /// <returns>
        ///   true if the specified <see cref = "T:System.Object" /> is equal to the
        ///   current <see cref = "T:System.Object" />; otherwise, false.
        /// </returns>
        /// <param name = "obj">
        ///   The <see cref = "T:System.Object" /> to compare with the current <see cref = "T:System.Object" />. 
        /// </param>
        /// <exception cref = "T:System.NullReferenceException">
        ///   The <paramref name = "obj" /> parameter is null.
        /// </exception>
        /// <filterpriority>2</filterpriority>
        public override bool Equals( object obj )
        {
            if ( ReferenceEquals( null, obj ) )
            {
                return false;
            }

            if ( ReferenceEquals( this, obj ) )
            {
                return true;
            }

            if ( obj.GetType() !=
                 typeof (ConfigurationSection) )
            {
                return false;
            }

            return Equals( (ConfigurationSection) obj );
        }

        /// <summary>
        ///   Serves as a hash function for a particular type.
        /// </summary>
        /// <returns>
        ///   A hash code for the current <see cref = "T:System.Object" />.
        /// </returns>
        /// <filterpriority>2</filterpriority>
        public override int GetHashCode()
        {
            unchecked
            {
                return ( ( _settings != null
                                   ? _settings.GetHashCode()
                                   : 0 ) * 397 ) ^ ( Name != null
                                                             ? Name.GetHashCode()
                                                             : 0 );
            }
        }

        /// <summary>
        ///   Implements the operator ==.
        /// </summary>
        /// <param name = "left">The left.</param>
        /// <param name = "right">The right.</param>
        /// <returns>The result of the operator.</returns>
        public static bool operator ==( ConfigurationSection left, ConfigurationSection right )
        {
            return Equals( left, right );
        }

        /// <summary>
        ///   Implements the operator !=.
        /// </summary>
        /// <param name = "left">The left.</param>
        /// <param name = "right">The right.</param>
        /// <returns>The result of the operator.</returns>
        public static bool operator !=( ConfigurationSection left, ConfigurationSection right )
        {
            return !Equals( left, right );
        }

        /// <summary>
        ///   Returns a <see cref = "T:System.String" /> that represents the current
        ///   <see cref = "T:System.Object" />.
        /// </summary>
        /// <returns>
        ///   A <see cref = "T:System.String" /> that represents the current <see cref = "T:System.Object" />.
        /// </returns>
        /// <filterpriority>2</filterpriority>
        public override string ToString()
        {
            return string.Format( Strings.Culture, "Settings: {0}, Name: {1}", _settings, Name );
        }

        #region Implementation of ICollection<KeyValuePair<string,string>>

        /// <summary>
        ///   Adds an item to the <see cref = "T:System.Collections.Generic.ICollection`1" />.
        /// </summary>
        /// <param name = "item">
        ///   The object to add to the <see cref = "T:System.Collections.Generic.ICollection`1" />.
        /// </param>
        /// <exception cref = "T:System.NotSupportedException">
        ///   The <see cref = "T:System.Collections.Generic.ICollection`1" /> is
        ///   read-only.
        /// </exception>
        void ICollection<KeyValuePair<string, string>>.Add( KeyValuePair<string, string> item )
        {
            Set( item.Key, item.Value );
        }

        /// <summary>
        ///   Removes all items from the <see cref = "T:System.Collections.Generic.ICollection`1" />.
        /// </summary>
        /// <exception cref = "T:System.NotSupportedException">
        ///   The <see cref = "T:System.Collections.Generic.ICollection`1" /> is
        ///   read-only. 
        /// </exception>
        public void Clear()
        {
            _settings.Clear();
        }

        /// <summary>
        ///   Determines whether the <see cref = "T:System.Collections.Generic.ICollection`1" /> contains a
        ///   specific value.
        /// </summary>
        /// <returns>
        ///   true if <paramref name = "item" /> is found in the <see cref = "T:System.Collections.Generic.ICollection`1" />; otherwise,
        ///   false.
        /// </returns>
        /// <param name = "item">
        ///   The object to locate in the <see cref = "T:System.Collections.Generic.ICollection`1" />.
        /// </param>
        bool ICollection<KeyValuePair<string, string>>.Contains( KeyValuePair<string, string> item )
        {
            return _settings.Contains( item );
        }

        /// <summary>
        ///   Copies the elements of the <see cref = "T:System.Collections.Generic.ICollection`1" /> to an <see cref = "T:System.Array" />, starting at a particular <see cref = "T:System.Array" /> index.
        /// </summary>
        /// <param name = "array">
        ///   The one-dimensional <see cref = "T:System.Array" /> that is the
        ///   destination of the elements copied from <see cref = "T:System.Collections.Generic.ICollection`1" />. The <see cref = "T:System.Array" /> must have zero-based indexing.
        /// </param>
        /// <param name = "arrayIndex">
        ///   The zero-based index in <paramref name = "array" /> at which copying
        ///   begins.
        /// </param>
        /// <exception cref = "T:System.ArgumentNullException"><paramref name = "array" /> is null.
        /// </exception>
        /// <exception cref = "T:System.ArgumentOutOfRangeException"><paramref name = "arrayIndex" /> is less than 0.
        /// </exception>
        /// <exception cref = "T:System.ArgumentException"><paramref name = "array" /> is multidimensional. -or- <paramref name = "arrayIndex" /> is equal
        ///   to or greater than the length of <paramref name = "array" />. -or- The
        ///   number of elements in the source <see cref = "T:System.Collections.Generic.ICollection`1" /> is greater than
        ///   the available space from <paramref name = "arrayIndex" /> to the end
        ///   of the destination <paramref name = "array" />. -or- Type <paramref name = "{KeyValuePair{string, string}}" /> cannot be cast automatically to the type of the
        ///   destination <paramref name = "array" />.
        /// </exception>
        public void CopyTo( KeyValuePair<string, string>[] array, int arrayIndex )
        {
            _settings.CopyTo( array, arrayIndex );
        }

        /// <summary>
        ///   Removes the first occurrence of a specific object from the <see cref = "T:System.Collections.Generic.ICollection`1" />.
        /// </summary>
        /// <returns>
        ///   true if <paramref name = "item" /> was successfully removed from the
        ///   <see cref = "T:System.Collections.Generic.ICollection`1" />;
        ///   otherwise, false. This method also returns false if <paramref name = "item" /> is not found in the original <see cref = "T:System.Collections.Generic.ICollection`1" />.
        /// </returns>
        /// <param name = "item">
        ///   The object to remove from the <see cref = "T:System.Collections.Generic.ICollection`1" />.
        /// </param>
        /// <exception cref = "T:System.NotSupportedException">
        ///   The <see cref = "T:System.Collections.Generic.ICollection`1" /> is
        ///   read-only.
        /// </exception>
        bool ICollection<KeyValuePair<string, string>>.Remove( KeyValuePair<string, string> item )
        {
            return _settings.Remove( item );
        }

        /// <summary>
        ///   Gets the number of elements contained in the <see cref = "T:System.Collections.Generic.ICollection`1" />.
        /// </summary>
        /// <returns>
        ///   The number of elements contained in the <see cref = "T:System.Collections.Generic.ICollection`1" />.
        /// </returns>
        public int Count
        {
            get { return _settings.Count; }
        }

        /// <summary>
        ///   Gets a value indicating whether the <see cref = "T:System.Collections.Generic.ICollection`1" /> is read-only.
        /// </summary>
        /// <returns>
        ///   true if the <see cref = "T:System.Collections.Generic.ICollection`1" /> is read-only; otherwise, false.
        /// </returns>
        public bool IsReadOnly
        {
            get { return _settings.IsReadOnly; }
        }

        #endregion
    }
}