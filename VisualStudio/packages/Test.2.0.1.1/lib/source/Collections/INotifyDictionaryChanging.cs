namespace Dnp.Collections
{
    public interface INotifyDictionaryChanging<TKey, TValue>
    {
        event DictionaryChangingEventHandler<TKey, TValue> DictionaryChanging;
    }
}