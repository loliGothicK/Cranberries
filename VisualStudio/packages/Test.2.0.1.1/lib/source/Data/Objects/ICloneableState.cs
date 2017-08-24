using System;
using System.Collections.Generic;

namespace Dnp.Data.Objects
{
    public interface ICloneableState<T>
    {
        T Clone( Stack<Type> state );

        T Clone( Stack<Type> state, int depth );
    }
}
