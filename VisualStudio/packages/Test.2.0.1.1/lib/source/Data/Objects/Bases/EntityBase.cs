using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Runtime.Serialization;

namespace Dnp.Data.Objects.Bases
{
    [Serializable]
    [DataContract]
    public abstract class EntityBase<TEntity> : IBusinessLogic, IObjectManager, IObjectCreated
            where TEntity : class, INotifyPropertyChanged, INotifyPropertyChanging
    {
        [NonSerialized]
        protected readonly Dictionary<string, object> PropertyUpdatedValue = new Dictionary<string, object>();

        [NonSerialized]
        protected readonly Dictionary<string, object> PropertyOriginalValue = new Dictionary<string, object>();

        [NonSerialized]
        private static readonly PropertyDescriptorCollection PropertyDescriptors;

        [NonSerialized]
        private IObjectContext _ObjectContext;
        protected IObjectContext ObjectContext
        {
            get { return _ObjectContext; }
            set { _ObjectContext = value; }
        }

        static EntityBase()
        {
            PropertyDescriptors = TypeDescriptor.GetProperties( typeof (TEntity) );
        }

        private void OnPropertyChanging( object sender, PropertyChangingEventArgs e )
        {
            object value;
            if ( PropertyOriginalValue.TryGetValue( e.PropertyName, out value ) )
            {
                return;
            }
            PropertyOriginalValue[e.PropertyName] = GetPropertyValue(e.PropertyName);
        }

        private void OnPropertyChanged( object sender, PropertyChangedEventArgs e )
        {
            var propertyValue = GetPropertyValue(e.PropertyName);
            object value;
            if (!PropertyOriginalValue.TryGetValue(e.PropertyName, out value))
            {
                return;
            }
            if (value == propertyValue)
            {
                PropertyUpdatedValue.Remove(e.PropertyName);
                return;
            }

            PropertyUpdatedValue[e.PropertyName] = propertyValue;
        }

        private object GetPropertyValue( string propertyName )
        {
            PropertyDescriptor propertyDescriptor = PropertyDescriptors[propertyName];
            return propertyDescriptor.GetValue( this );
        }

        private void ResetOriginalValues()
        {
            PropertyOriginalValue.Clear();
        }

        protected virtual void IsNewChanged()
        {
        }

        private void ListenForChangingNotifications()
        {
            TEntity entity = this as TEntity;
            entity.PropertyChanged += OnPropertyChanged;
            entity.PropertyChanging += OnPropertyChanging;
        }

        #region Implementation of IObjectManager

        public virtual void SetContext(IObjectContext objectContext)
        {
            ObjectFinishedLoading();
            ObjectContext = objectContext;
        }

        public virtual void ObjectFinishedLoading()
        {
            ListenForChangingNotifications();
        }

        public virtual void ObjectFinishedLoading(IObjectContext objectContext)
        {
            ObjectFinishedLoading();
            ObjectContext = objectContext;
        }

        public virtual void ResetUpdateValues()
        {
            PropertyUpdatedValue.Clear();
        }

        public virtual void ResetValuesToOldValues()
        {
            foreach (KeyValuePair<string, object> @property in PropertyOriginalValue)
            {
                PropertyDescriptor propertyDescriptor = PropertyDescriptors[@property.Key];
                propertyDescriptor.SetValue(this, @property.Value);
            }
        }

        #endregion

        #region Implementation of IBusinessLogic

        public virtual void OnSaveChanges( IObjectContext objectContext )
        {
            ResetOriginalValues();
            IsNew = false;
        }

        public virtual void OnSaveChanges( IObjectContext objectContext, IEnumerable<string> modifiedProperties )
        {
            ResetOriginalValues();
            IsNew = false;
        }

        public virtual void OnAddObject( IObjectContext objectContext )
        {
            IsNew = true;
        }

        #endregion

        #region Implementation of IObjectCreated

        private bool _IsNew;

        public virtual bool IsNew
        {
            get { return _IsNew; }
            protected set
            {
                _IsNew = value;
                IsNewChanged();
            }
        }

        #endregion
    }
}