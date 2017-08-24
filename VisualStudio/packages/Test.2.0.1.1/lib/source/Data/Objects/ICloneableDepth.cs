
namespace Dnp.Data.Objects
{
    public interface ICloneableDepth<T>
    {
        T Clone( int depth );
    }
}
