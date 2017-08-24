using System;

namespace Dnp.Data.Objects
{
    public class ObjectContainer : IObjectContainer
    {
        public ObjectContainer(IObjectContext objectContext)
        {
            ObjectContext = objectContext;
        }

        #region Implementation of IDisposable

        public virtual void Dispose( )
        {
            Dispose( true );
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing)
        {
            if (IsDisposed)
                return;
            if(disposing && !ReferenceEquals( ObjectContext, null ))
            {
                ObjectContext.Dispose();
                ObjectContext = null;
            }
            IsDisposed = true;
        }

        #endregion

        #region Implementation of IDisposableObject

        public virtual bool IsDisposed { get; set; }

        #endregion

        #region Implementation of IObjectContainer

        public virtual IObjectContext ObjectContext { get; private set; }

        #endregion
    }
}
