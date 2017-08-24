using System.IO;
using System.Runtime.Serialization.Formatters.Binary;

namespace Dnp.Runtime.Serialization
{
    public class BinaryFormatterSerializer : ISerializer
    {
        private readonly BinaryFormatter _BinaryFormatter = new BinaryFormatter();

        #region Implementation of ISerializer

        public byte[] SerializeObjects<T>( T objectsToSerialize )
        {
            byte[] result;

            using ( var memoryStream = new MemoryStream() )
            {
                _BinaryFormatter.Serialize( memoryStream, objectsToSerialize );
                result = memoryStream.ToArray();
            }

            return result;
        }

        public T DeserializeObjects<T>( Stream stream )
        {
            return (T) _BinaryFormatter.Deserialize( stream );
        }

        #endregion
    }
}