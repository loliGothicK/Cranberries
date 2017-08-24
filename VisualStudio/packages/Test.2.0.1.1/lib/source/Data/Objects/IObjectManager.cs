namespace Dnp.Data.Objects
{
    public interface IObjectManager
    {
        void SetContext(IObjectContext objectContext);

        void ObjectFinishedLoading();

        void ObjectFinishedLoading(IObjectContext objectContext);

        void ResetUpdateValues();

        void ResetValuesToOldValues();
    }
}
