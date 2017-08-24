using System.Collections.Generic;

namespace Dnp.Collections
{
    public interface IPropertyCollection
    {
        void Add(object o);
    }

    public class PropertyCollection<T> : List<T>, IPropertyCollection
    {
        public void Add(object o)
        {
            base.Add((T)o);
        }
    }
}
