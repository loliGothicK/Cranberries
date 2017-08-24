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

#endregion

namespace Dnp.Configuration
{
    internal static class EventHandlerExtension
    {
        public static void Raise<T>( this EventHandler<T> handler, object sender, T args )
                where T : EventArgs
        {
            if ( handler != null )
            {
                handler( sender, args );
            }
        }

        public static void Raise( this EventHandler handler, object sender, EventArgs args )
        {
            if ( handler != null )
            {
                handler( sender, args );
            }
        }

        public static void Raise( this PropertyChangedEventHandler handler, object sender, PropertyChangedEventArgs args )
        {
            if ( handler != null )
            {
                handler( sender, args );
            }
        }
    }
}