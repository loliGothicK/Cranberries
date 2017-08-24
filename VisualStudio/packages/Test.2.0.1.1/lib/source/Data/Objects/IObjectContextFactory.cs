
namespace Dnp.Data.Objects
{
    /// <summary>
    /// The factory that creates the appropriate object context and passes it back in a container.
    /// </summary>
    public interface IObjectContextFactory
    {
        /// <summary>
        /// Creates an object container and an appropriate object context for the container then passes back the container.
        /// </summary>
        /// <returns>An initialized object container.</returns>
        IObjectContainer CreateObjectContext(
            string entityConnectionString,
            string entityContainerName,
            string userId = null,
            string password = null,
            string server = null,
            string sid = null );

        ///// <summary>
        ///// Creates an object container and an appropriate object context for the container then passes back the container.
        ///// </summary>
        ///// <returns>An initialized object container.</returns>
        //IObjectContainer CreateObjectContext(string userID, string password, string server, string sid);

        ///// <summary>
        ///// Creates an object container with an object context and loads the object context with data.  This object context
        ///// is attributed to a database either passed in or specified elsewhere.
        ///// </summary>
        ///// <returns>An initialized object container.</returns>
        //IObjectContainer CreateObjectContextWithData( string userID = null, string password = null );

        ///// <summary>
        ///// Creates an object container with an object context and loads the object context with data.  This object context
        ///// is attributed to a database either passed in or specified elsewhere.  This method will first ensure that there
        ///// is nothing in the database.
        ///// </summary>
        ///// <returns>An initialized object container.</returns>
        //IObjectContainer CreateOjectContextWithDataEnsureDatabaseIsEmpty( string userID = null, string password = null );

        ///// <summary>
        ///// Creates an object container and a mock object context for the container and passes back this newly initialized container.
        ///// </summary>
        ///// <returns>An initialized object container.</returns>
        //IObjectContainer CreateMockObjectContext( string userID = null, string password = null );

        ///// <summary>
        ///// Creates an object container and a mock object context that's initialized with data for the container.
        ///// </summary>
        ///// <returns>An initialized object container with data.</returns>
        //IObjectContainer CreateMockObjectContextWithData( string userID = null, string password = null );
    }
}