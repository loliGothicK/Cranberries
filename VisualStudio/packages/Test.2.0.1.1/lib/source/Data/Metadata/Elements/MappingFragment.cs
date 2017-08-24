using Dnp.Collections;

namespace Dnp.Data.Metadata.Elements
{
    public sealed class MappingFragment
    {
        public string StoreEntitySet { get; set; }

        private PropertyCollection<ScalarProperty> _ScalarProperty;
        public PropertyCollection<ScalarProperty> ScalarProperty
        {
            get { return _ScalarProperty ?? (_ScalarProperty = new PropertyCollection<ScalarProperty>()); }
        }
    }
}
