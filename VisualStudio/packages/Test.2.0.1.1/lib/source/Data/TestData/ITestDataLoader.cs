using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Dnp.Data.Objects;

namespace Dnp.Data.TestData
{
    public interface ITestDataLoader
    {
        void Load( IObjectContext objectContext );

        void RemoveAllRows( IObjectContext objectContext );
    }
}
