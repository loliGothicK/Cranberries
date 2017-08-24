namespace Dnp.Data.Metadata.Elements
{
    public sealed class EntityTypeMapping
    {
        public string TypeName { get; set; }

        private MappingFragment _MappingFragment;
        public MappingFragment MappingFragment 
        {
            get { return _MappingFragment ?? (_MappingFragment = new MappingFragment()); }
        }
    }
}
