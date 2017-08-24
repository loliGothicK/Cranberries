using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Dnp.Data.TestData
{
    public interface IDataLoader
    {
        /// <summary>
        /// Collections of all rows for the entity specified, they are mapped by Property to Value.
        /// </summary>
        ICollection<Dictionary<string, string>> EntityPropertyValues { get; }

        /// <summary>
        /// The name of the entity that is apart of this data loader.
        /// </summary>
        string EntityName { get; }

        /// <summary>
        /// Loads the appropriate values into EntityPropertyValues.
        /// </summary>
        /// <returns>True if succeeded false otherwise.</returns>
        bool TryLoad();
    }
}
