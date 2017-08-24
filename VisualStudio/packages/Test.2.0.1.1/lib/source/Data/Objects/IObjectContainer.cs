

namespace Dnp.Data.Objects
{
    /// <summary>
    /// Defines methods for the container of the object model.  The container will hold an object context.
    /// </summary>
    public interface IObjectContainer : IDisposableObject
    {
        /// <summary>
        /// Retrieves the appropriate object context.
        /// </summary>
        IObjectContext ObjectContext { get; }
    }
}
