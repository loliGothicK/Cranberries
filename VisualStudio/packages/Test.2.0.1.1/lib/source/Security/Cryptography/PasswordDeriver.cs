using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography;

namespace Dnp.Security.Cryptography
{
    public class PasswordDeriver : IPasswordDeriver
    {
        private static readonly List<Type> AllowedSymmetricAlgorithms = new List<Type>
                                                               { typeof (TripleDES), typeof (RC2), typeof (DES) };
        private static readonly byte[] DefaultSalt = new byte[] { 1, 1, 1, 2, 2, 2, 3, 3, 3 };

        public PasswordDeriver( SymmetricAlgorithm symmetricAlgorithm,
                                byte[] initialVector,
                                HashAlgorithm passwordHashAlgorithm,
                                byte[] passwordSalt )
        {
            if ( !IsSupportedSymmetricAlgorithm( symmetricAlgorithm ) )
            {
                throw new NotSupportedException( "SymmetricAlgorithm" );
            }
            InitialVector = initialVector;
            SymmetricAlgorithm = symmetricAlgorithm;
            PasswordHashAlgorithm = passwordHashAlgorithm;
            PasswordSalt = passwordSalt;
        }

        public PasswordDeriver( SymmetricAlgorithm symmetricAlgorithm, byte[] initialVector )
                : this( symmetricAlgorithm, initialVector, new SHA1Managed(), DefaultSalt )
        {
        }

        public byte[] InitialVector { get; set; }
        public SymmetricAlgorithm SymmetricAlgorithm { get; set; }
        public HashAlgorithm PasswordHashAlgorithm { get; set; }
        public byte[] PasswordSalt { get; set; }

        protected bool IsSupportedSymmetricAlgorithm( SymmetricAlgorithm symmetricAlgorithm )
        {
            Type baseType = symmetricAlgorithm.GetType().BaseType;
            return AllowedSymmetricAlgorithms.Contains( baseType );
        }

        #region Implementation of IPassword

        public byte[] DeriveKey( string password )
        {
            var passwordDeriveBytes = new PasswordDeriveBytes( password, PasswordSalt );
            var initialVector = new byte[InitialVector.Length];
            InitialVector.CopyTo( initialVector, 0 );
            return passwordDeriveBytes.CryptDeriveKey( SymmetricAlgorithm.GetType().BaseType.Name,
                                                       PasswordHashAlgorithm.GetType().BaseType.Name,
                                                       SymmetricAlgorithm.LegalKeySizes.First().MaxSize,
                                                       initialVector );
        }

        #endregion
    }
}