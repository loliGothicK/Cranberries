using System;

namespace Dnp.Data.Objects
{
    public class DisposableObject : Object, IDisposable
    {
        public new virtual string ToString()
        {
            return "Not implemented on DisposableObject. Implement on derived class.";
        }

        #region IDisposable

        private bool _AlreadyDisposed;

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool isDisposing)
        {
            if (_AlreadyDisposed)
                return;

            if (isDisposing)
            {
                // TODO: free managaged resources here
            }
            // TODO: free unmanaged resources here

            _AlreadyDisposed = true;
        }

        #endregion IDisposable
    }
}
