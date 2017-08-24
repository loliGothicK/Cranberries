#region Using Directives

using System;
using System.IO;
using System.Text;

#endregion

namespace Dnp.IO
{
    public class StringWriterWithEncoding : StringWriter
    {
        private readonly Encoding _Encoding;

        /// <summary>
        ///   Initializes a new instance of the <see cref = "StringWriterWithEncoding" /> class.
        /// </summary>
        /// <param name = "encoding">The encoding.</param>
        public StringWriterWithEncoding( Encoding encoding )
        {
            _Encoding = encoding;
        }

        /// <summary>
        ///   Initializes a new instance of the <see cref = "StringWriterWithEncoding" /> class.
        /// </summary>
        /// <param name = "formatProvider">The format provider.</param>
        /// <param name = "encoding">The encoding.</param>
        public StringWriterWithEncoding( IFormatProvider formatProvider, Encoding encoding )
                : base( formatProvider )
        {
            _Encoding = encoding;
        }

        /// <summary>
        ///   Initializes a new instance of the <see cref = "StringWriterWithEncoding" /> class.
        /// </summary>
        /// <param name = "sb">The sb.</param>
        /// <param name = "encoding">The encoding.</param>
        public StringWriterWithEncoding( StringBuilder sb, Encoding encoding )
                : base( sb )
        {
            _Encoding = encoding;
        }

        /// <summary>
        ///   Initializes a new instance of the <see cref = "StringWriterWithEncoding" /> class.
        /// </summary>
        /// <param name = "sb">The sb.</param>
        /// <param name = "formatProvider">The format provider.</param>
        /// <param name = "encoding">The encoding.</param>
        public StringWriterWithEncoding( StringBuilder sb, IFormatProvider formatProvider, Encoding encoding )
                : base( sb, formatProvider )
        {
            _Encoding = encoding;
        }

        /// <summary>
        ///   Gets the <see cref = "T:System.Text.Encoding" /> in which the output is written.
        /// </summary>
        /// <value></value>
        /// <returns>The Encoding in which the output is written.</returns>
        public override Encoding Encoding
        {
            get { return _Encoding; }
        }
    }
}