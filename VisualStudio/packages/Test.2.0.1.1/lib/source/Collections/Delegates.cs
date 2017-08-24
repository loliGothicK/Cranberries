namespace Dnp.Collections
{
    public delegate void DictionaryChangedEventHandler<TKey, TValue>(
            object sender, NotifyDictionaryChangedEventArgs<TKey, TValue> e );

    public delegate void DictionaryChangingEventHandler<TKey, TValue>(
            object sender, NotifyDictionaryChangingEventArgs<TKey, TValue> e );
}