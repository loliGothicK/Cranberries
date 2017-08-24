#region Using Directives

using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.Specialized;

#endregion

namespace Dnp.Collections
{
    /// <summary>
    ///   Represents a dictionary of key value pairs that implement an observable model.
    /// </summary>
    /// <typeparam name = "TKey">The type of the key.</typeparam>
    /// <typeparam name = "TValue">The type of the value.</typeparam>
    public class ObservableDictionary<TKey, TValue>
            : IDictionary<TKey, TValue>,
              INotifyDictionaryChanged<TKey, TValue>,
              INotifyDictionaryChanging<TKey, TValue>,
              INotifyCollectionChanged
    {
        private readonly Dictionary<TKey, TValue> _Dictionary;

        /// <summary>
        ///   Initializes a new instance of the <see cref = "ObservableDictionary&lt;TKey, TValue&gt;" /> class.
        /// </summary>
        public ObservableDictionary()
        {
            _Dictionary = new Dictionary<TKey, TValue>();
        }

        /// <summary>
        ///   Initializes a new instance of the <see cref = "ObservableDictionary&lt;TKey, TValue&gt;" /> class.
        /// </summary>
        /// <param name = "dictionary">The dictionary.</param>
        public ObservableDictionary( IEnumerable<KeyValuePair<TKey, TValue>> dictionary )
                : this()
        {
            foreach ( KeyValuePair<TKey, TValue> value in dictionary )
            {
                _Dictionary.Add( value.Key, value.Value );
            }
        }

        /// <summary>
        ///   Initializes a new instance of the <see cref = "ObservableDictionary&lt;TKey, TValue&gt;" /> class.
        /// </summary>
        /// <param name = "dictionary">The dictionary.</param>
        public ObservableDictionary( Dictionary<TKey, TValue> dictionary )
                : this( (IEnumerable<KeyValuePair<TKey, TValue>>) dictionary )
        {
        }

        /// <summary>
        ///   Initializes a new instance of the <see cref = "ObservableDictionary&lt;TKey, TValue&gt;" /> class.
        /// </summary>
        /// <param name = "item">The item.</param>
        public ObservableDictionary( KeyValuePair<TKey, TValue> item )
                : this( item.Key, item.Value )
        {
        }

        /// <summary>
        ///   Initializes a new instance of the <see cref = "ObservableDictionary&lt;TKey, TValue&gt;" /> class.
        /// </summary>
        /// <param name = "key">The key.</param>
        /// <param name = "value">The value.</param>
        public ObservableDictionary( TKey key, TValue value )
                : this()
        {
            Add( key, value );
        }

        /// <summary>
        ///   Gets the dictionary.
        /// </summary>
        /// <value>The dictionary.</value>
        protected Dictionary<TKey, TValue> Dictionary
        {
            get { return _Dictionary; }
        }

        #region IDictionary<TKey,TValue> Members

        /// <summary>
        ///   Adds an item to the <see cref = "T:System.Collections.Generic.ICollection`1" />.
        /// </summary>
        /// <param name = "item">The object to add to the <see cref = "T:System.Collections.Generic.ICollection`1" />.</param>
        /// <exception cref = "T:System.NotSupportedException">The <see cref = "T:System.Collections.Generic.ICollection`1" /> is read-only.</exception>
        public void Add( KeyValuePair<TKey, TValue> item )
        {
            Add( item.Key, item.Value );
        }

        /// <summary>
        ///   Adds an element with the provided key and value to the <see cref = "T:System.Collections.Generic.IDictionary`2" />.
        /// </summary>
        /// <param name = "key">The object to use as the key of the element to add.</param>
        /// <param name = "value">The object to use as the value of the element to add.</param>
        /// <exception cref = "T:System.ArgumentNullException"><paramref name = "key" /> is null.</exception>
        /// <exception cref = "T:System.ArgumentException">An element with the same key already exists in the <see cref = "T:System.Collections.Generic.IDictionary`2" />.</exception>
        /// <exception cref = "T:System.NotSupportedException">The <see cref = "T:System.Collections.Generic.IDictionary`2" /> is read-only.</exception>
        public void Add( TKey key, TValue value )
        {
            var e = new NotifyDictionaryChangingEventArgs<TKey, TValue>( NotifyCollectionChangedAction.Add, key, value );
            OnDictionaryChanging( e );
            Dictionary.Add( key, value );
            var item = new KeyValuePair<TKey, TValue>( key, value );
            OnDictionaryChanged( new NotifyDictionaryChangedEventArgs<TKey, TValue>( NotifyCollectionChangedAction.Add,
                                                                                     item ) );
            OnCollectionChanged( new NotifyCollectionChangedEventArgs( NotifyCollectionChangedAction.Add, item ) );
        }

        /// <summary>
        ///   Removes all items from the <see cref = "T:System.Collections.Generic.ICollection`1" />.
        /// </summary>
        /// <exception cref = "T:System.NotSupportedException">The <see cref = "T:System.Collections.Generic.ICollection`1" /> is read-only. </exception>
        public void Clear()
        {
            var e = new NotifyDictionaryChangingEventArgs<TKey, TValue>( NotifyCollectionChangedAction.Reset );
            OnDictionaryChanging( e );
            Dictionary.Clear();
            OnDictionaryChanged( new NotifyDictionaryChangedEventArgs<TKey, TValue>(
                                         NotifyCollectionChangedAction.Reset ) );
            OnCollectionChanged( new NotifyCollectionChangedEventArgs( NotifyCollectionChangedAction.Reset ) );
        }

        /// <summary>
        ///   Determines whether the <see cref = "T:System.Collections.Generic.ICollection`1" /> contains a specific value.
        /// </summary>
        /// <returns>
        ///   true if <paramref name = "item" /> is found in the <see cref = "T:System.Collections.Generic.ICollection`1" />; otherwise, false.
        /// </returns>
        /// <param name = "item">The object to locate in the <see cref = "T:System.Collections.Generic.ICollection`1" />.</param>
        public bool Contains( KeyValuePair<TKey, TValue> item )
        {
            return ( Dictionary.ContainsKey( item.Key ) && Dictionary.ContainsValue( item.Value ) );
        }

        /// <summary>
        ///   Determines whether the <see cref = "T:System.Collections.Generic.IDictionary`2" /> contains an element with the specified key.
        /// </summary>
        /// <returns>
        ///   true if the <see cref = "T:System.Collections.Generic.IDictionary`2" /> contains an element with the key; otherwise, false.
        /// </returns>
        /// <param name = "key">The key to locate in the <see cref = "T:System.Collections.Generic.IDictionary`2" />.</param>
        /// <exception cref = "T:System.ArgumentNullException"><paramref name = "key" /> is null.</exception>
        public bool ContainsKey( TKey key )
        {
            return Dictionary.ContainsKey( key );
        }

        /// <summary>
        ///   Copies the elements of the <see cref = "T:System.Collections.Generic.ICollection`1" /> to an <see cref = "T:System.Array" />, starting at a particular <see cref = "T:System.Array" /> index.
        /// </summary>
        /// <param name = "array">The one-dimensional <see cref = "T:System.Array" /> that is the destination of the elements copied from <see cref = "T:System.Collections.Generic.ICollection`1" />. The <see cref = "T:System.Array" /> must have zero-based indexing.</param>
        /// <param name = "arrayIndex">The zero-based index in <paramref name = "array" /> at which copying begins.</param>
        /// <exception cref = "T:System.ArgumentNullException"><paramref name = "array" /> is null.</exception>
        /// <exception cref = "T:System.ArgumentOutOfRangeException"><paramref name = "arrayIndex" /> is less than 0.</exception>
        /// <exception cref = "T:System.ArgumentException"><paramref name = "array" /> is multidimensional.-or-The number of elements in the source <see cref = "T:System.Collections.Generic.ICollection`1" /> is greater than the available space from <paramref name = "arrayIndex" /> to the end of the destination <paramref name = "array" />.-or-Type <paramref name = "T" /> cannot be cast automatically to the type of the destination <paramref name = "array" />.</exception>
        public void CopyTo( KeyValuePair<TKey, TValue>[] array, int arrayIndex )
        {
            throw new NotSupportedException();
        }

        /// <summary>
        ///   Returns an enumerator that iterates through the collection.
        /// </summary>
        /// <returns>
        ///   A <see cref = "T:System.Collections.Generic.IEnumerator`1" /> that can be used to iterate through the collection.
        /// </returns>
        /// <filterpriority>1</filterpriority>
        public IEnumerator<KeyValuePair<TKey, TValue>> GetEnumerator()
        {
            return Dictionary.GetEnumerator();
        }

        /// <summary>
        ///   Returns an enumerator that iterates through a collection.
        /// </summary>
        /// <returns>
        ///   An <see cref = "T:System.Collections.IEnumerator" /> object that can be used to iterate through the collection.
        /// </returns>
        /// <filterpriority>2</filterpriority>
        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }

        /// <summary>
        ///   Removes the element with the specified key from the <see cref = "T:System.Collections.Generic.IDictionary`2" />.
        /// </summary>
        /// <returns>
        ///   true if the element is successfully removed; otherwise, false.  This method also returns false if <paramref name = "key" /> was not found in the original <see cref = "T:System.Collections.Generic.IDictionary`2" />.
        /// </returns>
        /// <param name = "key">The key of the element to remove.</param>
        /// <exception cref = "T:System.ArgumentNullException"><paramref name = "key" /> is null.</exception>
        /// <exception cref = "T:System.NotSupportedException">The <see cref = "T:System.Collections.Generic.IDictionary`2" /> is read-only.</exception>
        public bool Remove( TKey key )
        {
            if ( !Keys.Contains( key ) )
            {
                return false;
            }
            TValue local = this[key];
            var item = new KeyValuePair<TKey, TValue>( key, local );
            var e = new NotifyDictionaryChangingEventArgs<TKey, TValue>( NotifyCollectionChangedAction.Remove, item );
            OnDictionaryChanging( e );
            bool flag = Dictionary.Remove( key );

            OnDictionaryChanged(
                    new NotifyDictionaryChangedEventArgs<TKey, TValue>( NotifyCollectionChangedAction.Remove, item ) );
            OnCollectionChanged( new NotifyCollectionChangedEventArgs( NotifyCollectionChangedAction.Remove, item ) );

            return flag;
        }

        /// <summary>
        ///   Removes the first occurrence of a specific object from the <see cref = "T:System.Collections.Generic.ICollection`1" />.
        /// </summary>
        /// <returns>
        ///   true if <paramref name = "item" /> was successfully removed from the <see cref = "T:System.Collections.Generic.ICollection`1" />; otherwise, false. This method also returns false if <paramref name = "item" /> is not found in the original <see cref = "T:System.Collections.Generic.ICollection`1" />.
        /// </returns>
        /// <param name = "item">The object to remove from the <see cref = "T:System.Collections.Generic.ICollection`1" />.</param>
        /// <exception cref = "T:System.NotSupportedException">The <see cref = "T:System.Collections.Generic.ICollection`1" /> is read-only.</exception>
        public bool Remove( KeyValuePair<TKey, TValue> item )
        {
            bool flag = Remove( item.Key );
            return flag;
        }

        /// <summary>
        ///   Gets the value associated with the specified key.
        /// </summary>
        /// <returns>
        ///   true if the object that implements <see cref = "T:System.Collections.Generic.IDictionary`2" /> contains an element with the specified key; otherwise, false.
        /// </returns>
        /// <param name = "key">The key whose value to get.</param>
        /// <param name = "value">When this method returns, the value associated with the specified key, if the key is found; otherwise, the default value for the type of the <paramref name = "value" /> parameter. This parameter is passed uninitialized.</param>
        /// <exception cref = "T:System.ArgumentNullException"><paramref name = "key" /> is null.</exception>
        public bool TryGetValue( TKey key, out TValue value )
        {
            return Dictionary.TryGetValue( key, out value );
        }

        /// <summary>
        ///   Gets the number of elements contained in the <see cref = "T:System.Collections.Generic.ICollection`1" />.
        /// </summary>
        /// <returns>
        ///   The number of elements contained in the <see cref = "T:System.Collections.Generic.ICollection`1" />.
        /// </returns>
        public int Count
        {
            get { return Dictionary.Count; }
        }

        /// <summary>
        ///   Gets a value indicating whether the <see cref = "T:System.Collections.Generic.ICollection`1" /> is read-only.
        /// </summary>
        /// <returns>
        ///   true if the <see cref = "T:System.Collections.Generic.ICollection`1" /> is read-only; otherwise, false.
        /// </returns>
        public bool IsReadOnly
        {
            get { return false; }
        }

        /// <summary>
        ///   Gets or sets the element with the specified key.
        /// </summary>
        /// <returns>
        ///   The element with the specified key.
        /// </returns>
        /// <param name = "key">The key of the element to get or set.</param>
        /// <exception cref = "T:System.ArgumentNullException"><paramref name = "key" /> is null.</exception>
        /// <exception cref = "T:System.Collections.Generic.KeyNotFoundException">The property is retrieved and <paramref name = "key" /> is not found.</exception>
        /// <exception cref = "T:System.NotSupportedException">The property is set and the <see cref = "T:System.Collections.Generic.IDictionary`2" /> is read-only.</exception>
        public TValue this[ TKey key ]
        {
            get
            {
                TValue value;
                TryGetValue( key, out value );
                return value;
            }
            set
            {
                var oldItem = new KeyValuePair<TKey, TValue>( key, this[key] );
                var newItem = new KeyValuePair<TKey, TValue>( key, value );
                var e = new NotifyDictionaryChangingEventArgs<TKey, TValue>( NotifyCollectionChangedAction.Replace,
                                                                             newItem,
                                                                             oldItem );
                OnDictionaryChanging( e );
                Dictionary[key] = value;
                OnDictionaryChanged(
                        new NotifyDictionaryChangedEventArgs<TKey, TValue>( NotifyCollectionChangedAction.Replace,
                                                                            newItem,
                                                                            oldItem ) );
                OnCollectionChanged( new NotifyCollectionChangedEventArgs( NotifyCollectionChangedAction.Replace,
                                                                           newItem,
                                                                           oldItem ) );
            }
        }

        /// <summary>
        ///   Gets an <see cref = "T:System.Collections.Generic.ICollection`1" /> containing the keys of the <see cref = "T:System.Collections.Generic.IDictionary`2" />.
        /// </summary>
        /// <returns>
        ///   An <see cref = "T:System.Collections.Generic.ICollection`1" /> containing the keys of the object that implements <see cref = "T:System.Collections.Generic.IDictionary`2" />.
        /// </returns>
        public ICollection<TKey> Keys
        {
            get { return Dictionary.Keys; }
        }

        /// <summary>
        ///   Gets an <see cref = "T:System.Collections.Generic.ICollection`1" /> containing the values in the <see cref = "T:System.Collections.Generic.IDictionary`2" />.
        /// </summary>
        /// <returns>
        ///   An <see cref = "T:System.Collections.Generic.ICollection`1" /> containing the values in the object that implements <see cref = "T:System.Collections.Generic.IDictionary`2" />.
        /// </returns>
        public ICollection<TValue> Values
        {
            get { return Dictionary.Values; }
        }

        #endregion

        #region Implementation of INotifyCollectionChanged

        /// <summary>
        ///   Occurs when the collection changes.
        /// </summary>
        public event NotifyCollectionChangedEventHandler CollectionChanged;

        #endregion

        #region INotifyDictionaryChanged<TKey,TValue> Members

        /// <summary>
        ///   Occurs when [dictionary changed].
        /// </summary>
        public event DictionaryChangedEventHandler<TKey, TValue> DictionaryChanged;

        #endregion

        #region INotifyDictionaryChanging<TKey,TValue> Members

        /// <summary>
        ///   Occurs when [dictionary changing].
        /// </summary>
        public event DictionaryChangingEventHandler<TKey, TValue> DictionaryChanging;

        #endregion

        /// <summary>
        ///   Adds a range of items to this instance.
        /// </summary>
        /// <param name = "dictionary">The dictionary.</param>
        public void AddRange( IDictionary<TKey, TValue> dictionary )
        {
            var e = new NotifyDictionaryChangingEventArgs<TKey, TValue>( NotifyCollectionChangedAction.Add, dictionary );
            OnDictionaryChanging( e );
            foreach ( KeyValuePair<TKey, TValue> pair in dictionary )
            {
                Dictionary.Add( pair.Key, pair.Value );
            }
            OnDictionaryChanged( new NotifyDictionaryChangedEventArgs<TKey, TValue>( NotifyCollectionChangedAction.Add,
                                                                                     dictionary ) );
            OnCollectionChanged( new NotifyCollectionChangedEventArgs( NotifyCollectionChangedAction.Add, dictionary ) );
        }

        /// <summary>
        ///   Raises the <see cref = "DictionaryChanged" /> event.
        /// </summary>
        /// <param name = "e">The <see cref = "Dnp.Collections.NotifyDictionaryChangedEventArgs&lt;TKey,TValue&gt;" /> instance containing the event data.</param>
        protected void OnDictionaryChanged( NotifyDictionaryChangedEventArgs<TKey, TValue> e )
        {
            DictionaryChangedEventHandler<TKey, TValue> handler = DictionaryChanged;
            if ( handler != null )
            {
                handler( this, e );
            }
        }

        /// <summary>
        ///   Raises the <see cref = "DictionaryChanging" /> event.
        /// </summary>
        /// <param name = "e">The <see cref = "Dnp.Collections.NotifyDictionaryChangingEventArgs&lt;TKey,TValue&gt;" /> instance containing the event data.</param>
        protected void OnDictionaryChanging( NotifyDictionaryChangingEventArgs<TKey, TValue> e )
        {
            DictionaryChangingEventHandler<TKey, TValue> handler = DictionaryChanging;
            if ( handler != null )
            {
                handler( this, e );
            }
        }

        /// <summary>
        ///   Raises the <see cref = "CollectionChanged" /> event.
        /// </summary>
        /// <param name = "e">The <see cref = "System.Collections.Specialized.NotifyCollectionChangedEventArgs" /> instance containing the event data.</param>
        protected void OnCollectionChanged( NotifyCollectionChangedEventArgs e )
        {
            NotifyCollectionChangedEventHandler handler = CollectionChanged;
            if ( handler != null )
            {
                handler( this, e );
            }
        }

        /// <summary>
        ///   Removes the range of items specified.
        /// </summary>
        /// <param name = "dictionary">The dictionary.</param>
        public void RemoveRange( IDictionary<TKey, TValue> dictionary )
        {
            var e = new NotifyDictionaryChangingEventArgs<TKey, TValue>( NotifyCollectionChangedAction.Remove,
                                                                         dictionary );
            OnDictionaryChanging( e );
            foreach ( TKey local in dictionary.Keys )
            {
                Dictionary.Remove( local );
            }
            OnDictionaryChanged(
                    new NotifyDictionaryChangedEventArgs<TKey, TValue>( NotifyCollectionChangedAction.Remove, dictionary ) );
            OnCollectionChanged( new NotifyCollectionChangedEventArgs( NotifyCollectionChangedAction.Remove, dictionary ) );
        }
    }
}