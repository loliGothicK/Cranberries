using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Linq.Expressions;
using System.Data.EntityClient;
using System.Data.Objects;
using System.Reflection;
using Dnp.Data.TestData;

namespace Dnp.Data.Objects
{
    public partial class MockObjectContext
    {
        // These are meant for debugging purposes so the developer can
        // pass in the same parameters to this object context as they would
        // for the primary object context.

        #region Constructors

        static MockObjectContext()
        {
            StaticObjectSets = new Dictionary<Type, object>();
        }

        public MockObjectContext()
        {
            Initialize();
        }

        public MockObjectContext( EntityConnection connection )
        {
            Initialize( connection );
        }

        public MockObjectContext( string connectionString )
        {
            Initialize( connectionString : connectionString );
        }

        public MockObjectContext( EntityConnection connection, string connectionString )
        {
            Initialize( connection, connectionString );
        }

        public MockObjectContext( string connectionString, string defaultContainerName )
        {
            Initialize( connectionString : connectionString, defaultContainerName : defaultContainerName );
        }

        private MockObjectContext( bool useTestData )
        {
            InitializeLazyLoading();
            if ( !useTestData )
            {
                return;
            }
            ApplyTestDataLoaderFinishedLoading();
        }

        #endregion

        // Dictionary having the Type of the entity class as the key and the boxed MockObjectSet as the value.

        #region Properties

        private static readonly Dictionary<Type, object> StaticObjectSets;

        public bool ObjectSetsHasObjects
        {
            get { return StaticObjectSets.Count > 0; }
        }

        public string ConnectionString { get; private set; }

        public EntityConnection Connection { get; private set; }

        public string DefaultContainerName { get; private set; }

        public Dictionary<Type, object> ObjectSets
        {
            get { return StaticObjectSets; }
        }

        private bool ShouldDeferObjectManagement { get; set; }

        #endregion

        #region Methods

        public static void ClearMockObjectSets()
        {
            StaticObjectSets.Clear();
        }

        public static MockObjectContext CreateObjectContextWithoutTestData()
        {
            return new MockObjectContext( false );
        }

        public static MockObjectContext CreateObjectContextWithTestData()
        {
            return new MockObjectContext( true );
        }

        private void Initialize( EntityConnection connection = null,
                                 string connectionString = null,
                                 string defaultContainerName = null
                )
        {
            InitializeLazyLoading();
            Connection = connection;
            ConnectionString = connectionString;
            DefaultContainerName = defaultContainerName;
            ApplyTestDataLoaderFinishedLoading();
        }

        private void InitializeLazyLoading()
        {
            LazyLoading = true;
        }

        private void ApplyTestDataLoaderFinishedLoading()
        {
            ShouldDeferObjectManagement = true;
            TestDataLoader.FinishedLoading +=
                    ( sender, e ) =>
                    {
                        if ( !ReferenceEquals( e.ObjectContext, this ) )
                        {
                            return;
                        }
                        FilterObjectSetsAgainst<IObjectManager>().ToList().ForEach(
                                objectManager => objectManager.ObjectFinishedLoading() );
                        ShouldDeferObjectManagement = false;
                    };
        }

        private IEnumerable<TType> FilterObjectSetsAgainst<TType>()
        {
        	return ObjectSets.SelectMany(typeObjectSet => ( (IEnumerable) typeObjectSet.Value ).OfType<TType>());
        }

    	private static MockObjectSet<TEntity> GetMockObjectSet<TEntity>()
                where TEntity : class
        {
            MockObjectSet<TEntity> objectSet;
            if ( StaticObjectSets.Keys.Contains( typeof (TEntity) ) )
            {
                objectSet = (MockObjectSet<TEntity>) StaticObjectSets[typeof (TEntity)];
            }
            else
            {
                objectSet = new MockObjectSet<TEntity>();
                StaticObjectSets[typeof (TEntity)] = objectSet;
            }

            return objectSet;
        }

        private static void CleanupRelationships<TEntity, TKey>(
                TEntity entity
                )
                where TEntity : class, ISimplePrimaryKey<TKey>, INavigations
        {
            foreach ( string navigationPropertyName in entity.NavigationPropertyNames )
            {
                PropertyInfo propertyInfo = typeof (TEntity).GetProperty( navigationPropertyName );
                object navigation = propertyInfo.GetValue( entity, null );
                if ( navigation == null )
                {
                    continue;
                }

                if ( navigation is INavigations )
                {
                    CleanupRelationship<TEntity, TKey>( entity,
                                                        navigation as INavigations,
                                                        entity.NavigationPropertyTypes[navigationPropertyName] );
                    continue;
                }

                IList navigationCollection = (IList) navigation;
                foreach (object t in navigationCollection)
                {
                	CleanupRelationship<TEntity, TKey>( entity,
                	                                    (INavigations) t,
                	                                    entity.NavigationPropertyTypes[navigationPropertyName] );
                }
            }
        }

        private static void CleanupRelationship<TEntity, TKey>(
                TEntity entity,
                INavigations navigation,
                Type navigationType
                )
                where TEntity : class, ISimplePrimaryKey<TKey>, INavigations
        {
            ICollection<KeyValuePair<string, Type>> relationshipBack =
                    navigation.NavigationPropertyTypes.Where(
                            kv => kv.Value.Equals( typeof (TEntity) ) ).ToList();
            foreach ( KeyValuePair<string, Type> navigationPropertyName in relationshipBack )
            {
                PropertyInfo propertyInfo = navigationType.GetProperty( navigationPropertyName.Key );
                object navigationBack = propertyInfo.GetValue( navigation, null );
                if ( navigationBack is ISimplePrimaryKey<TKey> )
                {
                    ISimplePrimaryKey<TKey> navigationBackKey = (ISimplePrimaryKey<TKey>) navigationBack;
                    if ( !entity.Key.Equals( navigationBackKey.Key ) )
                    {
                        continue;
                    }

                    propertyInfo.SetValue( navigation, null, null );
                    continue;
                }

                IList navigationBackCollection = (IList) navigationBack;
                for ( int i = 0; i < navigationBackCollection.Count; i++ )
                {
                    ISimplePrimaryKey<TKey> element = (ISimplePrimaryKey<TKey>) navigationBackCollection[i];
                    if ( !entity.Key.Equals( element.Key ) )
                    {
                        continue;
                    }

                    navigationBackCollection.Remove( element );
                }
            }
        }

        private static IEnumerable<long> KeyValues( Type type, IEnumerable objectSet )
        {
        	return from object @object in objectSet let keyProperty = type.GetProperties().First( p => p.Name.Equals( "Key" ) ) let keyType = keyProperty.PropertyType select keyType == typeof (long) ? (long) keyProperty.GetValue( @object, null ) : 0 + 1;
        }

    	private static TKey CastTo<TKey>( object key )
        {
            return (TKey) key;
        }

        private static TKey Increment<TKey>( object keyToIncrement, Type keyType )
        {
            if ( keyType == typeof (long) ||
                 keyType == typeof (int) )
            {
                return Increment<TKey>( (long) keyToIncrement );
            }

            return default( TKey );
        }

        private static TKey Increment<TKey>( long keyToIncrement )
        {
            object result = keyToIncrement + 1;
            return (TKey) result;
        }

        #endregion

        #region Implementation of IDisposableObject

        private bool _IsDisposed;

        public bool IsDisposed
        {
            get { return _IsDisposed; }
        }

        public void Dispose()
        {
            _IsDisposed = true;
        }

        #endregion

        #region Implementation of IObjectContext

        public bool LazyLoading { get; set; }

        public void SetKeyGenerator( Func<object> keyGenerator )
        {
        }

        public int ExecuteFunction( string functionName,
                                    params ObjectParameter[] parameters )
        {
            return 0;
        }

        public void InitializeObjectKey<TEntity, TKey>(
                TEntity entity
                )
                where TEntity : class, ISimplePrimaryKey<TKey>
                where TKey : IEquatable<TKey>
        {
            var lastEntity = GetMockObjectSet<TEntity>().OrderBy(e => e.Key).LastOrDefault() ?? entity;
            entity.Key = CastTo<TKey>( Increment<TKey>( lastEntity.Key, typeof (TKey) ) );
        }

        public TKey CreateObjectKey<TKey>()
        {
            return ObjectSets.Count <= 0
                           ? default( TKey )
                           : CastTo<TKey>( ObjectSets.Max<KeyValuePair<Type, object>, long>(
                                   item =>
                                   {
                                       IEnumerable objectSet = item.Value as IEnumerable;
                                       if ( ReferenceEquals( objectSet, null ) )
                                       {
                                           return 0;
                                       }
                                       var keyValues = KeyValues( item.Key, objectSet );
                                       if ( keyValues.Count() <= 0 )
                                       {
                                           return 0;
                                       }
                                       return keyValues.Max();
                                   }
                                                   ) + 1 );
        }

        public IObjectSet<TEntity> GetObjects<TEntity>()
                where TEntity : class
        {
            return GetMockObjectSet<TEntity>();
        }

        public IQueryable<TEntity> GetObjects<TEntity>(
                Expression<Func<TEntity, bool>> predicate
                )
                where TEntity : class
        {
            return GetMockObjectSet<TEntity>().Where( predicate );
        }

        public TEntity GetObject<TEntity, TKey>(
                TKey key
                )
                where TEntity : class, ISimplePrimaryKey<TKey>
                where TKey : IEquatable<TKey>
        {
            return GetMockObjectSet<TEntity>().Where( e => e.Key.Equals( key ) ).FirstOrDefault();
        }

        public TEntity GetObject<TEntity, TKey>(
                TKey key,
                params string[] includes
                )
                where TEntity : class, ISimplePrimaryKey<TKey>
                where TKey : IEquatable<TKey>
        {
            // Currently includes is not supported for the mock object database.
            return GetObject<TEntity, TKey>( key );
        }

        public bool AddObject<TEntity>(
                TEntity entity
                )
                where TEntity : class
        {
            GetMockObjectSet<TEntity>().AddObject( entity );
            if ( ShouldDeferObjectManagement )
            {
                return true;
            }
            IObjectManager objectManager = entity as IObjectManager;
            if ( !ReferenceEquals( objectManager, null ) )
            {
                objectManager.ObjectFinishedLoading();
            }
            IBusinessLogic businessLogic = entity as IBusinessLogic;
            if ( !ReferenceEquals( businessLogic, null ) )
            {
                businessLogic.OnAddObject( this );
            }

            return true;
        }

        public bool DeleteObject<TEntity, TKey>(
                TEntity entity
                )
                where TEntity : class, ISimplePrimaryKey<TKey>, INavigations
                where TKey : IEquatable<TKey>
        {
            CleanupRelationships<TEntity, TKey>( entity );
            GetMockObjectSet<TEntity>().DeleteObject( entity );
            return true;
        }

        public bool DeleteObject<TEntity, TKey>( TKey key )
                where TEntity : class, ISimplePrimaryKey<TKey>, INavigations
                where TKey : IEquatable<TKey>
        {
            TEntity entity = GetMockObjectSet<TEntity>().Where( e => e.Key.Equals( key ) ).FirstOrDefault();
            if ( entity != null )
            {
                DeleteObject<TEntity, TKey>( entity );
            }

            return true;
        }

        public bool DeleteAllObjects<TEntity>() where TEntity : class
        {
            foreach ( var @object in GetMockObjectSet<TEntity>().ToList() )
            {
                GetMockObjectSet<TEntity>().DeleteObject( @object );
            }

            return true;
        }

        public bool UpdateObject<TEntity>(
                TEntity entity
                )
                where TEntity : class
        {
            // Currently do not do anything, may want this implemented later.
            return true;
        }

        public int SaveObjectChanges()
        {
            if ( ShouldDeferObjectManagement )
            {
                return 0;
            }
            //FilterObjectSetsAgainst<IBusinessLogic>().ToList().ForEach(
            //        businessLogic => businessLogic.OnSaveChanges( this ) );
            //FilterObjectSetsAgainst<IObjectManager>().ToList().ForEach(
            //        objectManager => objectManager.ResetUpdateValues() );
            return 0;
        }

        public void LoadRelationship<TEntity>( TEntity entityWithRelationship, string relationshipToLoad )
        {
        }

        public bool IsObjectAttached<TEntity>( TEntity objectToCheck )
        {
            return true;
        }

        public IQueryable<TEntity> GetObjects<TEntity>(
                params string[] includes
                )
                where TEntity : class
        {
            // Currently not using includes
            return GetMockObjectSet<TEntity>();
        }

        public IQueryable<TEntity> GetObjects<TEntity>(
                Expression<Func<TEntity, bool>> predicate,
                params string[] includes
                )
                where TEntity : class
        {
            // Currently not using includes
            return GetMockObjectSet<TEntity>().Where( predicate );
        }

        public TEntity GetObjectEntityName<TEntity, TKey>(
                TKey key,
                string containerName
                )
                where TEntity : class, ISimplePrimaryKey<TKey>
                where TKey : IEquatable<TKey>
        {
            return null;
        }

        public bool RefreshObject<TEntity>( TEntity entity )
        {
            return true;
        }

        public void RefreshObjects<TEntity>(IEnumerable<TEntity> entities)
        {
        }

        public void RefreshAllObjects( bool ignoreExceptions = true )
        {
        }

        public void RefreshAllModifiedObjects( bool ignoreExceptions = true )
        {
        }

        string IUserIdentifier.Identifier { get; set; }

        #endregion
    }

    public partial class MockObjectContext : IObjectContext
    {
        public TEntity GetObject<TEntity>(
                long key
                )
                where TEntity : class, ISimplePrimaryKey<long>
        {
            return GetObject<TEntity, long>( key );
        }

        public TEntity GetObject<TEntity>(
                string key
                )
                where TEntity : class, ISimplePrimaryKey<string>
        {
            return GetObject<TEntity, string>( key );
        }

        public TEntity GetObject<TEntity>(
                long key,
                params string[] includes
                )
                where TEntity : class, ISimplePrimaryKey<long>
        {
            return GetObject<TEntity, long>( key, includes );
        }

        public TEntity GetObject<TEntity>(
                string key,
                params string[] includes
                )
                where TEntity : class, ISimplePrimaryKey<string>
        {
            return GetObject<TEntity, string>( key, includes );
        }

        public bool DeleteObject<TEntity>(
                long key
                )
                where TEntity : class, ISimplePrimaryKey<long>, INavigations
        {
            return DeleteObject<TEntity, long>( key );
        }

        public bool DeleteObject<TEntity>(
                string key
                )
                where TEntity : class, ISimplePrimaryKey<string>, INavigations
        {
            return DeleteObject<TEntity, string>( key );
        }

        public void DetachObject( ref object objectToDetach )
        { 
            // TODO: implement cloning on the entity so when requrested to detach we can
            // just clone the entity and reasign that object.
        }
    }
}