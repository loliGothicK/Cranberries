using System;
using System.Collections.Generic;

namespace Dnp.Data.Objects
{
    public interface INavigations
    {
        string[] NavigationPropertyNames { get; }
        Dictionary<string, Type> NavigationPropertyTypes { get; }
    }
}
