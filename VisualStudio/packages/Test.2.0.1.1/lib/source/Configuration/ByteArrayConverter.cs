#region License

// 
// Copyright (c) 2009-2011, Ian Davis <ian@innovatian.com>
// 
// Dual-licensed under the Apache License, Version 2.0, and the Microsoft Public License (Ms-PL).
// See the file LICENSE.txt for details.
// 

#endregion

#region Using Directives

using System;
using System.ComponentModel;
using System.Globalization;

#endregion

namespace Dnp.Configuration
{
    internal class ByteArrayConverter : TypeConverter
    {
        /// <summary>
        ///   Returns whether this converter can convert an object of the given type to the type of this converter, using the specified context.
        /// </summary>
        /// <param name = "context">An <see cref = "T:System.ComponentModel.ITypeDescriptorContext" /> that provides a format context.</param>
        /// <param name = "sourceType">A <see cref = "T:System.Type" /> that represents the type you want to convert from.</param>
        /// <returns>
        ///   true if this converter can perform the conversion; otherwise, false.
        /// </returns>
        public override bool CanConvertFrom( ITypeDescriptorContext context, Type sourceType )
        {
            return ( sourceType == typeof (string) );
        }

        /// <summary>
        ///   Converts the given object to the type of this converter, using the specified context and culture information.
        /// </summary>
        /// <param name = "context">An <see cref = "T:System.ComponentModel.ITypeDescriptorContext" /> that provides a format context.</param>
        /// <param name = "culture">The <see cref = "T:System.Globalization.CultureInfo" /> to use as the current culture.</param>
        /// <param name = "value">The <see cref = "T:System.Object" /> to convert.</param>
        /// <returns>
        ///   An <see cref = "T:System.Object" /> that represents the converted value.
        /// </returns>
        /// <exception cref = "T:System.NotSupportedException">The conversion cannot be performed. </exception>
        public override object ConvertFrom( ITypeDescriptorContext context, CultureInfo culture, object value )
        {
            if ( !( value is string ) )
            {
                throw GetConvertFromException( value );
            }
            if ( ( (string) value ).Length == 0 )
            {
                return new byte[0];
            }

            string[] stringValue = ( (string) value ).Split( '-' );
            var byteArray = new byte[stringValue.Length];
            for ( int i = 0; i < byteArray.Length; i++ )
            {
                byte val = byte.Parse( stringValue[i], NumberStyles.HexNumber );
                byteArray[i] = val;
            }
            return byteArray;
        }

        /// <summary>
        ///   Converts the given value object to the specified type, using the specified context and culture information.
        /// </summary>
        /// <param name = "context">An <see cref = "T:System.ComponentModel.ITypeDescriptorContext" /> that provides a format context.</param>
        /// <param name = "culture">A <see cref = "T:System.Globalization.CultureInfo" />. If null is passed, the current culture is assumed.</param>
        /// <param name = "value">The <see cref = "T:System.Object" /> to convert.</param>
        /// <param name = "destinationType">The <see cref = "T:System.Type" /> to convert the <paramref name = "value" /> parameter to.</param>
        /// <returns>
        ///   An <see cref = "T:System.Object" /> that represents the converted value.
        /// </returns>
        /// <exception cref = "T:System.ArgumentNullException">The <paramref name = "destinationType" /> parameter is null. </exception>
        /// <exception cref = "T:System.NotSupportedException">The conversion cannot be performed. </exception>
        public override object ConvertTo( ITypeDescriptorContext context,
                                          CultureInfo culture,
                                          object value,
                                          Type destinationType )
        {
            if ( destinationType != typeof (string) )
            {
                throw GetConvertToException( value, destinationType );
            }
            if ( value == null )
            {
                return string.Empty;
            }
            string retVal = BitConverter.ToString( ( (byte[]) value ), 0 );
            return retVal;
        }
    }
}