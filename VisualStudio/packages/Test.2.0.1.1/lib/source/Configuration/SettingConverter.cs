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
using Dnp.Properties;

#endregion

namespace Dnp.Configuration
{
    internal static class SettingConverter
    {
        private static readonly Type StringType = typeof (string);

        /// <summary>
        ///   Gets an instance of T from string.
        /// </summary>
        /// <typeparam name = "T">
        /// </typeparam>
        /// <param name = "value">The value to convert.</param>
        /// <returns>
        ///   <paramref name = "value" /> as an instance of <c>T</c> or an exception if
        ///   the conversion is not possible.
        /// </returns>
        internal static T GetTFromString<T>( string value )
        {
            Type typeOfT = typeof (T);

            try
            {
                if ( IsBool<T>() )
                {
                    // Need to use aliasing to get around this.
                    return ConvertToBool<T>( value );
                }

                if ( IsStringArray<T>() )
                {
                    var stringArrayConverter = new StringArrayConverter();
                    return (T) stringArrayConverter.ConvertFrom( value );
                }

                if ( IsByteArray<T>() )
                {
                    var byteArrayConverter = new ByteArrayConverter();
                    return (T) byteArrayConverter.ConvertFrom( value );
                }

                TypeConverter converter = TypeDescriptor.GetConverter( typeOfT );
                if ( converter.CanConvertFrom( StringType ) )
                {
                    return (T) converter.ConvertFrom( value );
                }

                throw new InvalidOperationException( string.Format( Strings.Culture,
                                                                    Strings.NoTypeConverterForRestore0,
                                                                    typeOfT.Name ) );
            }
            catch ( Exception ex )
            {
                string message = string.Format( Strings.Culture, Strings.FailedToConvert0T1, value, typeOfT.Name );
                throw new Exception( message, ex );
            }
        }

        /// <summary>
        ///   Gets a string from an instance of <c>T</c>.
        /// </summary>
        /// <typeparam name = "T"></typeparam>
        /// <param name = "value">The value.</param>
        /// <returns>
        ///   <c>T</c> as a string.
        /// </returns>
        /// <remarks>
        ///   If you are using a custom or special type, you should make sure that
        ///   there is a <see cref = "TypeConverter" /> registered for it.
        /// </remarks>
        internal static string GetStringFromT<T>( T value )
        {
            if ( ReferenceEquals( value, default( T ) ) )
            {
                return string.Empty;
            }

            Type typeOfT = typeof (T);

            string stringValue;

            TypeConverter converter = TypeDescriptor.GetConverter( typeOfT );
            // verify that we can eventually round-trip the object.
            if ( converter.CanConvertTo( StringType ) &&
                 converter.CanConvertFrom( StringType ) )
            {
                stringValue = converter.ConvertTo( value, StringType ) as string;
            }
            else
            {
                if ( IsStringArray<T>() )
                {
                    var stringArrayConverter = new StringArrayConverter();
                    stringValue = stringArrayConverter.ConvertTo( value, StringType ) as string;
                }
                else if ( IsByteArray<T>() )
                {
                    var stringArrayConverter = new ByteArrayConverter();
                    stringValue = stringArrayConverter.ConvertTo( value, StringType ) as string;
                }
                else
                {
                    TypeConverter stringConverter = TypeDescriptor.GetConverter( StringType );
                    if ( stringConverter.CanConvertTo( typeOfT ) &&
                         stringConverter.CanConvertFrom( typeOfT ) )
                    {
                        stringValue = stringConverter.ConvertTo( value, StringType ) as string;
                    }
                    else
                    {
                        if ( typeOfT == typeof (object) )
                        {
                            stringValue = value.ToString();
                        }
                        else
                        {
                            string message = string.Format( Strings.Culture,
                                                            Strings.CannotRoundTripType0,
                                                            value.GetType() );
                            throw new InvalidOperationException( message );
                        }
                    }
                }
            }
            return stringValue;
        }

        /// <summary>
        ///   Converts the string value to a boolean. Int32 values are able to be
        ///   parsed.
        /// </summary>
        /// <typeparam name = "T"></typeparam>
        /// <param name = "value"></param>
        /// <returns></returns>
        private static T ConvertToBool<T>( string value )
        {
            // see if an int was used. 0 = false, anything else is true.
            int result;
            if ( Int32.TryParse( value, out result ) )
            {
                return (T) (object) ( result != 0 );
            }

            // didn't use an int, pass off the the BooleanConverter. The
            // BooleanConverter can't handle numbers and it is faster to check
            // if it is an int than deal with the exception that the
            // BooleanConverter would throw and then deal with converting from
            // int.
            var converter = new BooleanConverter();
            return (T) converter.ConvertFrom( value );
        }

        /// <summary>
        ///   Determines if <c>T</c> is of type <see cref = "bool" />.
        /// </summary>
        /// <typeparam name = "T"></typeparam>
        /// <returns></returns>
        public static bool IsBool<T>()
        {
            return typeof (T) == typeof (bool);
        }

        /// <summary>
        ///   Determines if <c>T</c> is of type <see cref = "string[]" />.
        /// </summary>
        /// <typeparam name = "T"></typeparam>
        /// <returns></returns>
        public static bool IsStringArray<T>()
        {
            return typeof (T) == typeof (string[]);
        }

        /// <summary>
        ///   Determines if <c>T</c> is of type <see cref = "byte[]" />.
        /// </summary>
        /// <typeparam name = "T"></typeparam>
        /// <returns></returns>
        public static bool IsByteArray<T>()
        {
            return typeof (T) == typeof (byte[]);
        }
    }
}