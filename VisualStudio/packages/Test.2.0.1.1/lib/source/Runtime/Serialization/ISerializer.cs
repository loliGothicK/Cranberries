using System.IO;

namespace Dnp.Runtime.Serialization
{
    public interface ISerializer
    {
        byte[] SerializeObjects<T>( T objectsToSerialize );

        T DeserializeObjects<T>(Stream stream);
    }
}
