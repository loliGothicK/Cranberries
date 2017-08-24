
namespace Dnp.Security.Cryptography
{
    public interface IPasswordDeriver
    {
        byte[] DeriveKey( string password );
    }
}
