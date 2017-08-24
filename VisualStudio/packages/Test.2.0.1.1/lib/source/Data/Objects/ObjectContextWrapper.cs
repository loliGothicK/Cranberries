using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Data.EntityClient;
using System.Data.Objects;
using System.Linq;
using System.Linq.Expressions;
using Dnp.Data.Objects.Bases;
using Dnp.Data.TestData;
using System.Collections;

namespace Dnp.Data.Objects
{
    public partial class ObjectContextWrapper : ObjectContext, IObjectContext
    {
        // Type and boxed ObjectSet dictionary.

        #region Constructor

        public ObjectContextWrapper(
                string connectionString,
                string containerName )
                : base( connectionString, containerName )
        {
            Initialize(  );
        }

        public ObjectContextWrapper(
                EntityConnection entityConnection,
                string containerName)
            : base(entityConnection, containerName)
        {
            Initialize( );
        }

        #endregion

        #region Properties

        private readonly Dictionary<Type, object> _TypeObjectSet = new Dictionary<Type, object>();

        private bool _IsDisposed;

        private Func<object> KeyGenerator { get; set; }

        #endregion

        #region Methods

        public void Initialize( )
        {
            ApplyEntityEvents();
        }

        public void SetKeyGenerator( Func<object> keyGenerator )
        {
            KeyGenerator = keyGenerator;
        }

        private static TKey CastToTKey<TKey>( object key )
        {
            var convert = TypeDescriptor.GetConverter( typeof(TKey) );
            return (TKey) convert.ConvertFrom( key.ToString() );
        }

        private ObjectSet<TEntity> GetObjectSet<TEntity>()
                where TEntity : class
        {
            if ( _TypeObjectSet.Keys.Contains( typeof (TEntity) ) )
            {
                return _TypeObjectSet[typeof (TEntity)] as ObjectSet<TEntity>;
            }

            var result = CreateObjectSet<TEntity>();
            _TypeObjectSet[typeof (TEntity)] = result;

            return result;
        }

        private ObjectSet<TEntity> GetObjectSet<TEntity>(
                string containerName
                )
                where TEntity : class
        {
            return string.IsNullOrEmpty( containerName )
                           ? CreateObjectSet<TEntity>()
                           : CreateObjectSet<TEntity>( containerName );
        }

        private void ApplyEntityEvents()
        {
            ObjectMaterialized += OnMaterialized;
            SavingChanges += OnSavingChanges;
        }

        private void OnSavingChanges( object sender, EventArgs e )
        {
            DetectChanges();
            FilterToBusinessLogic().ToList().ForEach( item => item.Item1.OnSaveChanges( this, item.Item2 ) );
        }

        private void OnMaterialized( object sender, ObjectMaterializedEventArgs e )
        {
            IObjectManager objectManager = e.Entity as IObjectManager;
            if ( ReferenceEquals( objectManager, null ) )
            {
                return;
            }
            objectManager.ObjectFinishedLoading( this );
        }

        private IEnumerable<Tuple<IBusinessLogic, IEnumerable<string>>> FilterToBusinessLogic(EntityState entityState)
        {
            var objectStateEntries = ObjectStateManager.GetObjectStateEntries( entityState ).ToList();
            foreach (ObjectStateEntry objectStateEntry in objectStateEntries.Where(item => item.Entity is IBusinessLogic))
            {
                yield return Tuple.Create((IBusinessLogic)objectStateEntry.Entity, objectStateEntry.GetModifiedProperties());
            }
        }

        private IEnumerable<Tuple<IBusinessLogic, IEnumerable<string>>> FilterToBusinessLogic()
        {
            return FilterToBusinessLogic( EntityState.Modified | EntityState.Added );
        }

        #endregion

        #region Implements IObjectContext

        public bool LazyLoading
        {
            get { return ContextOptions.LazyLoadingEnabled; }
            set { ContextOptions.LazyLoadingEnabled = value; }
        }

        public int SaveObjectChanges()
        {
            try
            {
                return SaveChanges();
            }catch(Exception ex)
            {
                throw new Exception("Exception occured in SaveObjectChanges() ", ex);
            }
        }

        public void LoadRelationship<TEntity>( TEntity entityWithRelationship, string relationshipToLoad )
        {
            LoadProperty( entityWithRelationship, relationshipToLoad, MergeOption.OverwriteChanges );
        }

        public bool IsObjectAttached<TEntity>( TEntity objectToCheck )
        {
            var objectStateEntry = ObjectStateManager.GetObjectStateEntry( objectToCheck );
            return objectStateEntry.State != EntityState.Detached;
        }

        public TKey CreateObjectKey<TKey>()
        {
            if (ReferenceEquals(KeyGenerator, null))
            {
                return default(TKey);
            }

            return CastToTKey<TKey>(KeyGenerator());
        }

        public IObjectSet<TEntity> GetObjects<TEntity>()
                where TEntity : class
        {
            return GetObjectSet<TEntity>();
        }

        public IQueryable<TEntity> GetObjects<TEntity>(
                params string[] includes
                )
                where TEntity : class
        {
            ObjectQuery<TEntity> result = GetObjectSet<TEntity>();

            return includes.Aggregate( result,
                                       ( current,
                                         i ) => current.Include( i ) );
        }

        public IQueryable<TEntity> GetObjects<TEntity>(
                Expression<Func<TEntity, bool>> predicate
                )
                where TEntity : class
        {
            return GetObjectSet<TEntity>().Where( predicate );
        }

        public IQueryable<TEntity> GetObjects<TEntity>(
                Expression<Func<TEntity, bool>> predicate,
                params string[] includes
                )
                where TEntity : class
        {
            ObjectQuery<TEntity> result = GetObjectSet<TEntity>();
            result = includes.Aggregate( result,
                                         ( current,
                                           i ) => current.Include( i ) );

            return result.Where( predicate );
        }

        public TEntity GetObject<TEntity, TKey>(
                TKey key
                )
                where TEntity : class, ISimplePrimaryKey<TKey>
                where TKey : IEquatable<TKey>
        {
            return GetObjectSet<TEntity>().Where( e => e.Key.Equals( key ) ).FirstOrDefault();
        }

        public TEntity GetObjectEntityName<TEntity, TKey>(
                TKey key,
                string containerName
                )
                where TEntity : class, ISimplePrimaryKey<TKey>
                where TKey : IEquatable<TKey>
        {
            return GetObjectSet<TEntity>().Where( e => e.Key.Equals( key ) ).FirstOrDefault();
        }

        public TEntity GetObject<TEntity, TKey>(
                TKey key,
                params string[] includes
                )
                where TEntity : class, ISimplePrimaryKey<TKey>
                where TKey : IEquatable<TKey>
        {
            return GetObjects<TEntity>( includes ).Where( e => e.Key.Equals( key ) ).FirstOrDefault();
        }

        public bool AddObject<TEntity>( TEntity entity )
                where TEntity : class
        {
            GetObjectSet<TEntity>().AddObject( entity );
            IBusinessLogic businessLogic = entity as IBusinessLogic;
            if (!ReferenceEquals(businessLogic, null))
            {
                businessLogic.OnAddObject(this);
            }

            return true;
        }

        public bool DeleteObject<TEntity, TKey>( TEntity entity )
                where TEntity : class, ISimplePrimaryKey<TKey>, INavigations
                where TKey : IEquatable<TKey>
        {
            GetObjectSet<TEntity>().DeleteObject( entity );

            return true;
        }

        public bool DeleteObject<TEntity, TKey>( TKey key )
                where TEntity : class, ISimplePrimaryKey<TKey>, INavigations
                where TKey : IEquatable<TKey>
        {
            ObjectSet<TEntity> objectSet = GetObjectSet<TEntity>();
            TEntity entity = objectSet.Where( e => e.Key.Equals( key ) ).ToList().FirstOrDefault();
            if ( entity == null )
            {
                return false;
            }

            objectSet.DeleteObject( entity );

            return true;
        }

        public bool DeleteAllObjects<TEntity>() where TEntity : class
        {
            if ( GetObjectSet<TEntity>().Count() == 0 )
            {
                return true;
            }
            foreach ( var @objects in GetObjectSet<TEntity>().ToList() )
            {
                GetObjectSet<TEntity>().DeleteObject( @objects );
            }

            return true;
        }

        public bool UpdateObject<TEntity>( TEntity entity )
                where TEntity : class
        {
            // Entity should be tracked by the ObjectContext so there is no need to
            // do much of anything here.

            return true;
        }

        public void InitializeObjectKey<TEntity, TKey>(
                TEntity entity
                )
                where TEntity : class, ISimplePrimaryKey<TKey>
                where TKey : IEquatable<TKey>
        {
            if ( ReferenceEquals( KeyGenerator, null ) )
            {
                return;
            }

            entity.Key = CastToTKey<TKey>( KeyGenerator() );
        }

        public bool RefreshObject<TEntity>( TEntity entity )
        {
            var objectStateEntry = ObjectStateManager.GetObjectStateEntry( entity );
            // Refresh( RefreshMode.ClientWins, entity );
            Refresh( RefreshMode.StoreWins, entity );
            if(objectStateEntry.State == EntityState.Detached || objectStateEntry.State == EntityState.Deleted)
            {
                return false;
            }
            return true;
        }

        public void RefreshObjects<TEntity>(IEnumerable<TEntity> entities)
        {
            Refresh( RefreshMode.StoreWins, (IEnumerable) entities );
        }

        public void RefreshAllObjects( bool ignoreExceptions = true )
        {
            IEnumerable<ObjectStateEntry> objectStates = 
                ObjectStateManager.GetObjectStateEntries( EntityState.Added | EntityState.Modified | EntityState.Deleted );
            Refresh( RefreshMode.StoreWins, objectStates.Where( item => !item.IsRelationship ).Select( item => item.Entity ) );
        }

        public void RefreshAllModifiedObjects(bool ignoreExceptions = true)
        {
            IEnumerable<ObjectStateEntry> objectStates =
                ObjectStateManager.GetObjectStateEntries( EntityState.Modified );
            Refresh( RefreshMode.StoreWins, objectStates.Where( item => !item.IsRelationship ).Select( item => item.Entity ) );
        }

        public void DetachObject( ref object objectToDetach )
        {
            Detach( objectToDetach );
        }

        #endregion

        #region Implements IDisposableObject

        public bool IsDisposed
        {
            get { return _IsDisposed; }
        }

        protected override void Dispose( bool disposing )
        {
            base.Dispose( disposing );
            if ( disposing )
            {
                _IsDisposed = true;
            }
        }

        #endregion

        #region Implementation of IUserIdentifier

        string IUserIdentifier.Identifier {get; set; }

        #endregion
    }

    public partial class ObjectContextWrapper
    {
        #region Implements IObjectContext

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

        #endregion
    }
}