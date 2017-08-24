using System;
using System.Linq;
using System.Security.Cryptography;

namespace Dnp.Security.Cryptography
{
    public class SymmetricAlgorithmHelper
    {
        public static byte[] GenerateConstantIV(SymmetricAlgorithm symmetricAlgorithm, int seed = 7)
        {
            int initialVectorSize = symmetricAlgorithm.LegalBlockSizes.First().MaxSize / 8;
            byte[] result = new byte[initialVectorSize];
            Random random = new Random(seed);
            random.NextBytes( result );
            return result;
        }
    }
}
