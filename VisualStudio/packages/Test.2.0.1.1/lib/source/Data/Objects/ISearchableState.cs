using System;
using System.Collections.Generic;
using System.Threading;

namespace Dnp.Data.Objects
{
    public interface ISearchableState
    {
        /// <summary>
        /// Searchable but maintains state so there will be no infinite loops.  This is meant for
        /// internal use by the entities themselves.
        /// </summary>
        /// <param name="criteria">The criteria to search on.</param>
        /// <param name="state">A stack that keeps track of previous types that have been checked.</param>
        /// <param name="result">The Type, object tuple result.</param>
        /// <returns>True if the search criteria matched something, false otherwise.</returns>
        bool Search(
            string criteria,
            Stack<Type> state,
            ref Tuple<Type, object> result);

        /// <summary>
        /// Searchable but maintains state so there will be no infinite loops.  This is meant for
        /// internal use by the entities themselves.
        /// </summary>
        /// <param name="criteria">The criteria to search on.</param>
        /// <param name="state">A stack that keeps track of previous types that have been checked.</param>
        /// <param name="cancelToken">CancellationToken to cancel the search.</param>
        /// <param name="result">The Type, object tuple result.</param>
        /// <returns>True if the search criteria matched something or cancellation was requested, false otherwise.</returns>
        bool Search(
            string criteria,
            Stack<Type> state,
            CancellationToken cancelToken,
            ref Tuple<Type, object> result);
    }
}
