using System.Data.EntityClient;
using Dnp.Data.Connection;

namespace Dnp.Data.Objects.Factory
{
    public class ObjectContextFactory<TContainer> : ObjectContextFactoryBase<TContainer>
            where TContainer : ObjectContainer
    {
        #region Implementation of IObjectContextFactory

        public override IObjectContainer CreateObjectContext(
                string entityConnectionString,
                string entityContainerName,
                string userId = null,
                string password = null,
                string server = null,
                string sid = null )
        {
            var entityConnection = new EntityConnection( entityConnectionString );
            entityConnection.StoreConnection.ConnectionString += ConnectionString.Create(
                    userId, password, server, sid : sid );
            return
                    InvokeObjectContainerConstructor<IObjectContainer>(
                            new ObjectContextWrapper( entityConnection, entityContainerName ) );
        }

        #endregion
    }
}