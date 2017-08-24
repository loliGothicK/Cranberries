using System;
using System.Data.Objects;
using System.Linq;
using System.Linq.Expressions;
using System.Collections.Generic;

namespace Dnp.Data.Objects
{
    public partial interface IObjectContext : IUserIdentifier
    {
        bool LazyLoading { get; set; }

        void SetKeyGenerator( Func<object> keyGenerator );

        int ExecuteFunction(
            string functionName,
            params ObjectParameter[] parameters);

        void InitializeObjectKey<TEntity, TKey>(
            TEntity entity
            )
            where TEntity : class, ISimplePrimaryKey<TKey>
            where TKey : IEquatable<TKey>;

        TKey CreateObjectKey<TKey>();

        IObjectSet<TEntity> GetObjects<TEntity>( )
            where TEntity : class;

        IQueryable<TEntity> GetObjects<TEntity>(
            params string[] includes
            )
            where TEntity : class;

        IQueryable<TEntity> GetObjects<TEntity>(
            Expression<Func<TEntity, bool>> predicate
            )
            where TEntity : class;

        IQueryable<TEntity> GetObjects<TEntity>(
            Expression<Func<TEntity, bool>> predicate,
            params string[] includes
            )
            where TEntity : class;

        TEntity GetObject<TEntity, TKey>(
            TKey key
            )
            where TEntity : class, ISimplePrimaryKey<TKey>
            where TKey : IEquatable<TKey>;

        TEntity GetObjectEntityName<TEntity, TKey>(
            TKey key,
            string containerName
            )
            where TEntity : class, ISimplePrimaryKey<TKey>
            where TKey : IEquatable<TKey>;

        TEntity GetObject<TEntity, TKey>(
            TKey key,
            params string[] includes
            )
            where TEntity : class, ISimplePrimaryKey<TKey>
            where TKey : IEquatable<TKey>;

        bool AddObject<TEntity>(
            TEntity entity
            )
            where TEntity : class;

        bool DeleteObject<TEntity, TKey>(
            TEntity entity
            )
            where TEntity : class, ISimplePrimaryKey<TKey>, INavigations
            where TKey : IEquatable<TKey>;

        bool DeleteObject<TEntity, TKey>(
            TKey key
            )
            where TEntity : class, ISimplePrimaryKey<TKey>, INavigations
            where TKey : IEquatable<TKey>;

        bool DeleteAllObjects<TEntity>( )
            where TEntity : class;

        bool UpdateObject<TEntity>(
            TEntity entity
            )
            where TEntity : class;

        int SaveObjectChanges( );

        void LoadRelationship<TEntity>( TEntity entityWithRelationship, string relationshipToLoad );

        bool IsObjectAttached<TEntity>( TEntity objectToCheck );

        bool RefreshObject<TEntity>(TEntity entity);

        void RefreshObjects<TEntity>(IEnumerable<TEntity> entities);

        void RefreshAllObjects(bool ignoreExceptions = true);

        void RefreshAllModifiedObjects(bool ignoreExceptions = true);

        void DetachObject( ref object objectToDetach );
    }

    public partial interface IObjectContext : IDisposableObject
    {
        #region GetObject

        TEntity GetObject<TEntity>(
            long key
            )
            where TEntity : class, ISimplePrimaryKey<long>;

        TEntity GetObject<TEntity>(
            string key
            )
            where TEntity : class, ISimplePrimaryKey<string>;

        #endregion

        #region GetObject With Includes

        TEntity GetObject<TEntity>(
            long key,
            params string[] includes
            )
            where TEntity : class, ISimplePrimaryKey<long>;

        TEntity GetObject<TEntity>(
            string key,
            params string[] includes
            )
            where TEntity : class, ISimplePrimaryKey<string>;

        #endregion

        #region DeleteObject With Key

        bool DeleteObject<TEntity>(
            long key
            )
            where TEntity : class, ISimplePrimaryKey<long>, INavigations;

        bool DeleteObject<TEntity>(
            string key
            )
            where TEntity : class, ISimplePrimaryKey<string>, INavigations;

        #endregion DeleteObject With Key
    }

    // TODO: This shoud be made into a generic.
    public interface IUserIdentifier
    {
        string Identifier { get; set; }
    }
}