using System;
using System.Collections.Generic;
using Dnp.Data.TestData;

namespace Dnp.Data.Objects.Factory
{
    internal class MockObjectContextWithDataFactory
    {
        public static HashSet<Type> ContainerInitialized = new HashSet<Type>();
    }

    public class MockObjectContextWithDataFactory<TContainer> : ObjectContextFactoryBase<TContainer>
        where TContainer : ObjectContainer
    {
        public ITestDataLoader TestDataLoader { get; set; }

        public MockObjectContextWithDataFactory(ITestDataLoader testDataLoader)
        {
            TestDataLoader = testDataLoader;
            if (ReferenceEquals(TestDataLoader, null))
            {
				throw new ArgumentNullException("testDataLoader");
            }
        }

        #region Implementation of IObjectContextFactory

        public override IObjectContainer CreateObjectContext( string entityConnectionString,
                                                     string entityContainerName,
                                                     string userId = null,
                                                     string password = null,
                                                     string server = null,
                                                     string sid = null)
        {
            var mockObjectContext = MockObjectContext.CreateObjectContextWithTestData();
            if ( !MockObjectContextWithDataFactory.ContainerInitialized.Contains( typeof( TContainer ) ) )
            {
                // TestDataLoader.RemoveAllRows(mockObjectContext);
                TestDataLoader.Load( mockObjectContext );
            }
            MockObjectContextWithDataFactory.ContainerInitialized.Add( typeof( TContainer ) );

            return InvokeObjectContainerConstructor<IObjectContainer>(mockObjectContext);
        }

        #endregion
    }
}
