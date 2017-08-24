using System;
using System.Collections.Generic;
using System.Linq;
using System.Data.Objects;

namespace Dnp.Data.Objects
{
    public class MockObjectSet<TEntity> : IMockObjectSet, IObjectSet<TEntity>
        where TEntity : class
    {
    	readonly HashSet<TEntity> _Set;
    	readonly IQueryable<TEntity> _Query;

        public MockObjectSet()
        {
            _Set = new HashSet<TEntity>();
            _Query = _Set.AsQueryable();
        }

        public MockObjectSet(IEnumerable<TEntity> set)
        {
            _Set = new HashSet<TEntity>(set);
            _Query = _Set.AsQueryable();
        }

        public void AddObject(TEntity entity)
        {
            _Set.Add(entity);
        }

        public void AddObject(object entity)
        {
			TEntity entityAsT = entity as TEntity;
			if (entityAsT != null) _Set.Add(entityAsT);
        }

        public void Attach(TEntity entity)
        {
            _Set.Add(entity);
        }

        public void DeleteObject(TEntity entity)
        {
            _Set.Remove(entity);
        }

        public void DeleteObject(object entity)
        {
        	TEntity entityAsT = entity as TEntity;
			if (entityAsT != null) _Set.Remove(entityAsT);
        }

        public void Detach(TEntity entity)
        {
            _Set.Remove(entity);
        }

        public IEnumerator<TEntity> GetEnumerator()
        {
            return _Set.GetEnumerator();
        }

        System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
        {
            return _Set.GetEnumerator();
        }

        public Type ElementType
        {
            get { return _Query.ElementType; }
        }

        public System.Linq.Expressions.Expression Expression
        {
            get { return _Query.Expression; }
        }

        public IQueryProvider Provider
        {
            get { return _Query.Provider; }
        }
    }
}
