using System;

namespace Dnp.Data.Objects
{
    public interface IDisposableObject : IDisposable
    {
        bool IsDisposed { get; }
    }
}
