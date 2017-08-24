using System;
using System.Threading;

namespace Dnp.Data.Objects
{
    public interface ISearchable
    {
        /// <summary>
        /// Searches through the object where this chunk of criteria exists.  If this information
        /// is not in this object then the criteria is passed to other relationships that
        /// implement ISearchableState.
        /// </summary>
        /// <param name="criteria">The criteria to check for.</param>
        /// <param name="result">Type and object, Type specifies the type of the object.</param>
        /// <returns>True if the criteria was matched in this object.</returns>
        bool Search(
            string criteria,
            out Tuple<Type, object> result );

        /// <summary>
        /// Searches through the object where this chunk of criteria exists.  If this information
        /// is not in this object then the criteria is passed to other relationships that
        /// implement ISearchableState. If the cancelToken is set to cancel then this search is cancelled.
        /// </summary>
        /// <param name="criteria">The criters to check for.</param>
        /// <param name="cancelToken">The cancellation token to cancel the search.</param>
        /// <param name="result">Type and object, Type specifies the type of the object.</param>
        /// <returns>True if the criteria was mathched in this object or cancellation was requested.</returns>
        bool Search(
            string criteria,
            CancellationToken cancelToken,
            out Tuple<Type, object> result);
    }
}
