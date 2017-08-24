using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Dnp.Data.Objects;
using Dnp.Data.Objects.EventArguments;

namespace Dnp.Data.TestData
{
    public abstract class TestDataLoader : ITestDataLoader
    {
        public static EventHandler<ObjectContextEventArgs> BeginLoading;

        public static EventHandler<ObjectContextEventArgs> FinishedLoading;

        protected static void OnBeginLoading(object sender, IObjectContext objectContext)
        {
            EventHandler<ObjectContextEventArgs> beginLoading = BeginLoading;
            if (!ReferenceEquals(beginLoading, null))
            {
                beginLoading(sender, new ObjectContextEventArgs(objectContext));
            }
        }

        protected static void OnFinishedLoading(object sender, IObjectContext objectContext)
        {
            EventHandler<ObjectContextEventArgs> finishedLoading = FinishedLoading;
            if (!ReferenceEquals(finishedLoading, null))
            {
                finishedLoading(sender, new ObjectContextEventArgs(objectContext));
            }
        }

        #region Implementation of ITestDataLoader

        public virtual void Load( IObjectContext objectContext )
        {
        }

        public virtual void RemoveAllRows( IObjectContext objectContext )
        {
        }

        #endregion
    }
}
