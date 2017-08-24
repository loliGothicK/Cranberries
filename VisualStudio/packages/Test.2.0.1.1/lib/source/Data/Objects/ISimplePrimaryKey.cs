
namespace Dnp.Data.Objects
{
    public interface ISimplePrimaryKey
    {
        object KeyObject { get; set; }
    }

    public interface ISimplePrimaryKey<TKey>
    {
        // TODO: Rename to Key (involves changing edmx)
        TKey Key { get; set; }
    }
}
