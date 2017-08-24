using System.Collections.Generic;

namespace Dnp.Data.Objects
{
    public interface IBusinessLogic
    {
        void OnSaveChanges( IObjectContext objectContext, IEnumerable<string> modifiedProperties );

        void OnAddObject( IObjectContext objectContext );
    }
}
