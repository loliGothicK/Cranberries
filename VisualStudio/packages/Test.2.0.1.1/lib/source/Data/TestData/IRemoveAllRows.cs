using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Dnp.Data.Objects;

namespace Dnp.Data.TestData
{
    public interface IRemoveAllRows : IDisposable
    {
        void Execute( IObjectContext objectContext );
    }
}
