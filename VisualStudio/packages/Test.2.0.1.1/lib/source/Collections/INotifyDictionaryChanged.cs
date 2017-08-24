namespace Dnp.Collections
{
    public interface INotifyDictionaryChanged<TKey, TValue>
    {
        event DictionaryChangedEventHandler<TKey, TValue> DictionaryChanged;
    }
}