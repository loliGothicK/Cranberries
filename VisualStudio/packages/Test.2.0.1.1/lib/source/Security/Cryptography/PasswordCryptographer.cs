using System.IO;
using System.Security.Cryptography;
using System.Text;

namespace Dnp.Security.Cryptography
{
    public class PasswordCryptographer
    {
        public PasswordCryptographer( SymmetricAlgorithm symmetricAlgorithm,
                                      IPasswordDeriver passwordDeriver,
                                      byte[] initialVector,
                                      string password = null )
        {
            SymmetricAlgorithm = symmetricAlgorithm;
            PasswordDeriver = passwordDeriver;
            InitialVector = initialVector;
            Password = password;
        }

        public PasswordCryptographer( SymmetricAlgorithm symmetricAlgorithm, byte [] initialVector, string password = null )
            : this(symmetricAlgorithm, new PasswordDeriver( symmetricAlgorithm, initialVector ), initialVector, password)
        {
        }

        public PasswordCryptographer( SymmetricAlgorithm symmetricAlgorithm, string password = null )
                : this(
                        symmetricAlgorithm,
                        SymmetricAlgorithmHelper.GenerateConstantIV( symmetricAlgorithm ),
                        password )
        {
        }

        public PasswordCryptographer(string password = null)
                : this( new TripleDESCryptoServiceProvider(), password )
        {
        }

        public string Password { get; set; }
        protected SymmetricAlgorithm SymmetricAlgorithm { get; set; }
        public IPasswordDeriver PasswordDeriver { get; set; }
        public byte[] InitialVector { get; set; }

        public byte[] Encrypt( byte[] data, string password = null )
        {
            using ( var encryptedStream = new MemoryStream() )
            {
                using(var dataStream = new MemoryStream(data))
                {
                    Encrypt( dataStream, encryptedStream, password );
                }
                return encryptedStream.ToArray();
            }
        }

        public void Encrypt( Stream dataStream, Stream encryptedStream, string password = null )
        {
            byte [] key = PasswordDeriver.DeriveKey( Password ?? password );
            ICryptoTransform cryptoTransform = SymmetricAlgorithm.CreateEncryptor( key, InitialVector );
            using ( var cryptoStream = new CryptoStream( encryptedStream, cryptoTransform, CryptoStreamMode.Write ) )
            {
                dataStream.CopyTo( cryptoStream );
            }
        }

        public byte[] Decrypt( byte[] encryptedData, string password = null )
        {
            using ( var memoryStream = new MemoryStream( encryptedData ) )
            {
                return Decrypt( memoryStream, password );
            }
        }

        public byte[] Decrypt( Stream encryptedDataStream, string password = null )
        {
            byte [] key = PasswordDeriver.DeriveKey( Password ?? password );
            ICryptoTransform cryptoTransform = SymmetricAlgorithm.CreateDecryptor( key, InitialVector );
            using ( var cryptoStream = new CryptoStream( encryptedDataStream, cryptoTransform, CryptoStreamMode.Read ) )
            {
                using ( var streamReader = new StreamReader( cryptoStream, Encoding.Default ) )
                {
                    string decryptedText = streamReader.ReadToEnd();
                    return Encoding.Default.GetBytes( decryptedText );
                }
            }
        }
    }
}