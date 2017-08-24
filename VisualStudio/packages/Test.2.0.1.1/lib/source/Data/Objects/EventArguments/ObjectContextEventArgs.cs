using System;

namespace Dnp.Data.Objects.EventArguments
{
    public class ObjectContextEventArgs : EventArgs
    {
        public ObjectContextEventArgs(IObjectContext objectContext)
        {
            ObjectContext = objectContext;
        }

        public IObjectContext ObjectContext { get; private set; }
    }
}
