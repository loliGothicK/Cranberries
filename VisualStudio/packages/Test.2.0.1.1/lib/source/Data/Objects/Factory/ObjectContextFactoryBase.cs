using System.Reflection;

namespace Dnp.Data.Objects.Factory
{
    public abstract class ObjectContextFactoryBase<TContainer> : IObjectContextFactory
        where TContainer : ObjectContainer
    {
        protected virtual ConstructorInfo ObjectContainerConstructor
        {
            get { return typeof ( TContainer ).GetConstructor( new[] {typeof ( IObjectContext )} ); }
        }

        protected virtual TObjectContainer InvokeObjectContainerConstructor<TObjectContainer>(
            params object[] parameters )
        {
            return ( TObjectContainer ) ObjectContainerConstructor.Invoke( parameters );
        }

        #region Implementation of IObjectContextFactory

        public abstract IObjectContainer CreateObjectContext(
            string entityConnectionString,
            string entityContainerName,
            string userId = null,
            string password = null,
            string server = null,
            string sid = null );

        #endregion
    }
}