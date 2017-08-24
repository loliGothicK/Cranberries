#region Using Directives

using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Linq;

#endregion

namespace Dnp.Collections
{
    public class NotifyDictionaryChangedEventArgs<TKey, TValue> : EventArgs
    {
        private readonly NotifyCollectionChangedAction _Action;
        private readonly IDictionary<TKey, TValue> _NewItems;
        private readonly IDictionary<TKey, TValue> _OldItems;

        /// <summary>
        ///   Initializes a new instance of the <see cref = "NotifyDictionaryChangedEventArgs&lt;TKey, TValue&gt;" /> class.
        /// </summary>
        /// <param name = "action">The action.</param>
        public NotifyDictionaryChangedEventArgs( NotifyCollectionChangedAction action )
        {
            _Action = action;
            if ( action != NotifyCollectionChangedAction.Reset )
            {
                throw new ArgumentException( "Only Reset is supported for this .ctor." );
            }
        }

        /// <summary>
        ///   Initializes a new instance of the <see cref = "NotifyDictionaryChangedEventArgs&lt;TKey, TValue&gt;" /> class.
        /// </summary>
        /// <param name = "action">The action.</param>
        /// <param name = "items">The items.</param>
        public NotifyDictionaryChangedEventArgs( NotifyCollectionChangedAction action, IDictionary<TKey, TValue> items )
        {
            _Action = action;
            if ( ( action == NotifyCollectionChangedAction.Reset ) ||
                 ( action == NotifyCollectionChangedAction.Remove ) )
            {
                _OldItems = items;
            }
            else
            {
                _NewItems = items;
            }
        }

        /// <summary>
        ///   Initializes a new instance of the <see cref = "NotifyDictionaryChangedEventArgs&lt;TKey, TValue&gt;" /> class.
        /// </summary>
        /// <param name = "action">The action.</param>
        /// <param name = "item">The item.</param>
        public NotifyDictionaryChangedEventArgs( NotifyCollectionChangedAction action, KeyValuePair<TKey, TValue> item )
                : this( action, item.Key, item.Value )
        {
        }

        /// <summary>
        ///   Initializes a new instance of the <see cref = "NotifyDictionaryChangedEventArgs&lt;TKey, TValue&gt;" /> class.
        /// </summary>
        /// <param name = "action">The action.</param>
        /// <param name = "newItems">The new items.</param>
        /// <param name = "oldItems">The old items.</param>
        public NotifyDictionaryChangedEventArgs( NotifyCollectionChangedAction action,
                                                 IDictionary<TKey, TValue> newItems,
                                                 IDictionary<TKey, TValue> oldItems )
        {
            _Action = action;
            _OldItems = oldItems;
            _NewItems = newItems;
        }

        /// <summary>
        ///   Initializes a new instance of the <see cref = "NotifyDictionaryChangedEventArgs&lt;TKey, TValue&gt;" /> class.
        /// </summary>
        /// <param name = "action">The action.</param>
        /// <param name = "newItem">The new item.</param>
        /// <param name = "oldItem">The old item.</param>
        public NotifyDictionaryChangedEventArgs( NotifyCollectionChangedAction action,
                                                 KeyValuePair<TKey, TValue> newItem,
                                                 KeyValuePair<TKey, TValue> oldItem )
        {
            _Action = action;
            _NewItems = new Dictionary<TKey, TValue>( 1 );
            _NewItems.Add( newItem );
            _OldItems = new Dictionary<TKey, TValue>( 1 );
            _OldItems.Add( oldItem );
        }

        /// <summary>
        ///   Initializes a new instance of the <see cref = "NotifyDictionaryChangedEventArgs&lt;TKey, TValue&gt;" /> class.
        /// </summary>
        /// <param name = "action">The action.</param>
        /// <param name = "key">The key.</param>
        /// <param name = "value">The value.</param>
        public NotifyDictionaryChangedEventArgs( NotifyCollectionChangedAction action, TKey key, TValue value )
                : this( action, new Dictionary<TKey, TValue>( 1 ) )
        {
            IDictionary<TKey, TValue> dictionary = ( ( action == NotifyCollectionChangedAction.Reset ) |
                                                     ( action == NotifyCollectionChangedAction.Remove ) )
                                                           ? OldItems
                                                           : NewItems;
            dictionary[key] = value;
        }

        /// <summary>
        ///   Gets the action.
        /// </summary>
        /// <value>The action.</value>
        public NotifyCollectionChangedAction Action
        {
            get { return _Action; }
        }

        /// <summary>
        ///   Gets the new item.
        /// </summary>
        /// <value>The new item.</value>
        public KeyValuePair<TKey, TValue> NewItem
        {
            get
            {
                return NewItems == null
                               ? new KeyValuePair<TKey, TValue>( default( TKey ), default( TValue ) )
                               : NewItems.ElementAtOrDefault( 0 );
            }
        }

        /// <summary>
        ///   Gets the new items.
        /// </summary>
        /// <value>The new items.</value>
        public IDictionary<TKey, TValue> NewItems
        {
            get { return _NewItems; }
        }

        /// <summary>
        ///   Gets the old item.
        /// </summary>
        /// <value>The old item.</value>
        public KeyValuePair<TKey, TValue> OldItem
        {
            get
            {
                return OldItems == null
                               ? new KeyValuePair<TKey, TValue>( default( TKey ), default( TValue ) )
                               : OldItems.ElementAtOrDefault( 0 );
            }
        }

        /// <summary>
        ///   Gets the old items.
        /// </summary>
        /// <value>The old items.</value>
        public IDictionary<TKey, TValue> OldItems
        {
            get { return _OldItems; }
        }
    }
}