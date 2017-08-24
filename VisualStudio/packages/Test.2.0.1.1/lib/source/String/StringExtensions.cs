using System.Collections.Generic;
using System.Text.RegularExpressions;

namespace Dnp.String
{
    public static class StringExtensions
    {
        public static string Replace( this string stringToReplace, string criteria )
        {
            if ( ReferenceEquals( stringToReplace, null ) )
            {
                return null;
            }

            return stringToReplace.Replace( criteria, string.Empty );
        }

        public static string ReplaceWithRegex( this string stringToReplace, string regexCriteria, string replaceWith )
        {
            if ( ReferenceEquals( stringToReplace, null ) )
            {
                return null;
            }

            Regex regex = new Regex( regexCriteria );
            return regex.Replace( stringToReplace, replaceWith );
        }

        public static string TrimInternal( this string stringToTrim )
        {
            if ( ReferenceEquals( stringToTrim, null ) )
            {
                return null;
            }

            return stringToTrim.Replace( " " ).Replace( "\r" ).Replace( "\n" ).Replace( "\t" );
        }

        public static string[] SplitAndKeepSeparator( this string stringToSplit, params char [] separator )
        {
            int[] sepList = new int[stringToSplit.Length];
            int numReplaces = MakeSeparatorList( stringToSplit, separator, ref sepList );
            if(numReplaces == 0)
            {
                return new [] { stringToSplit };
            }

            List<string> result = new List<string>();
            int startIndex = 0;
            for ( int i = 0; i < numReplaces + 1 && startIndex < stringToSplit.Length; i++)
            {
                result.Add( stringToSplit.Substring( startIndex, sepList[i] - startIndex ) );
                startIndex = sepList[i];
                result.Add( stringToSplit.Substring( startIndex++, 1 ) );
            }
            if(startIndex < stringToSplit.Length)
            {
                result.Add(stringToSplit.Substring( startIndex )  );
            }
            if(result.Count == (numReplaces * 2) + 1)
            {
                result.Add(stringToSplit.Substring( stringToSplit.Length - 1, 1 )  );
            }
            return result.ToArray();
        }

        private static int MakeSeparatorList( string stringToSplit, char [] separator, ref int [] sepList )
        {
            int numberOfSplits = 0;
            if ( ( separator == null ) ||
                    ( separator.Length == 0 ) )
            {
                for ( int i = 0; ( i < stringToSplit.Length ) && ( numberOfSplits < sepList.Length ); i++ )
                {
                    if ( char.IsWhiteSpace( stringToSplit [i] ) )
                    {
                        sepList [numberOfSplits++] = i;
                    }
                }
                return numberOfSplits;
            }
            int length = sepList.Length;
            int separatorLength = separator.Length;
            for ( int j = 0; ( j < stringToSplit.Length ) && ( numberOfSplits < length ); j++ )
            {
                for ( int i = 0; i < separatorLength; i++ )
                {
                    if ( stringToSplit [j] == separator[i] )
                    {
                        sepList [numberOfSplits++] = j;
                        break;
                    }
                }
            }
            return numberOfSplits;
        }
    }
}
