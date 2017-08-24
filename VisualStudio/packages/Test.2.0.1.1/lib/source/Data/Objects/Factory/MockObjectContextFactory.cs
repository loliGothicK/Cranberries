namespace Dnp.Data.Objects.Factory
{
    public class MockObjectContextFactory<TContainer> : ObjectContextFactoryBase<TContainer>
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
            return
                    InvokeObjectContainerConstructor<IObjectContainer>(
                            MockObjectContext.CreateObjectContextWithoutTestData() );
        }

        #endregion
    }
}