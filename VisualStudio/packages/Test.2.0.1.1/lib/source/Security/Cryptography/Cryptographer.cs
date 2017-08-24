using System;
using System.Text;
using System.Security.Cryptography;
using System.IO;

namespace Dnp.Security.Cryptography
{
    public class Cryptographer : IDisposable
    {
        public SymmetricAlgorithm CryptoAlgorithm { get; set; }

        public Cryptographer(SymmetricAlgorithm cryptoAlgorithm)
        {
            CryptoAlgorithm = cryptoAlgorithm;
        }

        public Cryptographer()
            : this(new AesManaged())
        {
        }


        public string Encrypt(string text)
        {
            byte[] bytes = Encrypt(CryptoAlgorithm.CreateEncryptor(), Encoding.Default.GetBytes(text));
            return Encoding.Default.GetString(bytes);
        }

        public string Encrypt(ICryptoTransform encryptor, string text)
        {
            byte[] bytes = Encrypt(encryptor, Encoding.Default.GetBytes(text));
            return Encoding.Default.GetString(bytes);
        }
        public byte[] Encrypt(ICryptoTransform encryptor, byte[] data)
        {
            if (encryptor == null)
                throw new ArgumentNullException("encryptor");
            if (data == null)
                throw new ArgumentNullException("data");

            //Encrypt the data.
            using (MemoryStream msEncrypt = new MemoryStream())
            using (CryptoStream csEncrypt = new CryptoStream(msEncrypt,
            encryptor, CryptoStreamMode.Write))
            {
                //Write all data to the crypto stream and flush it.
                csEncrypt.Write(data, 0, data.Length);
                csEncrypt.FlushFinalBlock();

                //Get encrypted array of bytes.
                byte[] encrypted = msEncrypt.ToArray();
                return encrypted;
            }
        }

        public string Decrypt(string encryptedText)
        {
            byte[] decrypt = Decrypt(CryptoAlgorithm.CreateDecryptor(), Encoding.Default.GetBytes(encryptedText));
            return Encoding.Default.GetString(decrypt);
        }


        public string Decrypt(ICryptoTransform decryptor, string encryptedText)
        {
            byte[] decrypt = Decrypt(decryptor, Encoding.Default.GetBytes(encryptedText));
            return Encoding.Default.GetString(decrypt);
        }

        public byte[] Decrypt(ICryptoTransform decryptor, byte[] encrypted)
        {
            if (decryptor == null)
                throw new ArgumentNullException("decryptor");
            if (encrypted == null)
                throw new ArgumentNullException("encrypted");

            using (MemoryStream msDecrypt = new MemoryStream(encrypted))
            using (CryptoStream csDecrypt = new CryptoStream(msDecrypt,
            decryptor, CryptoStreamMode.Read))
            {
                byte[] fromEncrypt = new byte[encrypted.Length];

                int read = csDecrypt.Read(fromEncrypt, 0,
                fromEncrypt.Length);
                if (read < fromEncrypt.Length)
                {
                    byte[] clearBytes = new byte[read];
                    Buffer.BlockCopy(fromEncrypt, 0, clearBytes, 0, read);
                    return clearBytes;
                }
                return fromEncrypt;
            }
        }

        #region IDisposable Members

        public void Dispose()
        {
            CryptoAlgorithm.Clear();
        }

        #endregion
    }
}
