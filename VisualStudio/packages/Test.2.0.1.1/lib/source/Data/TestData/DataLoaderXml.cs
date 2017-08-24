using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Xml.Linq;
using Dnp.Data.Metadata;
using Dnp.Data.Metadata.Elements;

namespace Dnp.Data.TestData
{
    /// <summary>
    /// This loads the data of an xml formatted data dump.
    /// </summary>
    public class DataLoaderXml : IDataLoader
    {
        public DataLoaderXml(string edmxFile, string dataFile)
        {
            Initialize(dataFile);

            using (EdmxReader<EntityTypeMapping> edmxReader = new EdmxReader<EntityTypeMapping>(edmxFile))
            {
                if (!edmxReader.TryLoad())
                    throw new Exception("Failed to load the edmx reader");
                EntityMapping = GetEntityTypeMapping( edmxReader );
            }
        }

        public DataLoaderXml(EdmxReader<EntityTypeMapping> edmxReader, string dataFile)
        {
            Initialize(dataFile);
            
            if (!edmxReader.HasLoaded)
                if(!edmxReader.TryLoad())
                    throw new Exception("Failed to load the edmx reader");
            EntityMapping = GetEntityTypeMapping( edmxReader );
        }

        public DataLoaderXml( EntityTypeMapping entityMapping, string dataFile )
        {
            Initialize(dataFile);

            EntityMapping = entityMapping;
        }

        private EntityTypeMapping EntityMapping { get; set; }

        private string DataFileName { get; set; }

        private string MappingName { get; set; }

        private XElement DataContents { get; set; }

        public string EntityName { get; private set; }

        public ICollection<Dictionary<string, string>> EntityPropertyValues { get; private set; }

        public bool TryLoad()
        {
            LoadEntityName();
            LoadPropertyValues();

            return true;
        }

        private void Initialize(string dataFile)
        {
            DataFileName = dataFile;
            DataContents = XElement.Load( dataFile );
            MappingName = GetMappingName( dataFile );
            EntityPropertyValues = new List<Dictionary<string, string>>();
        }

        private void LoadPropertyValues()
        {
            // Dictionary with the key being the storage mapping the value being the entity name.
            Dictionary<string, string> storageMappingEntityNames = new Dictionary<string, string>();
            foreach ( ScalarProperty s in EntityMapping.MappingFragment.ScalarProperty )
            {
                storageMappingEntityNames [s.ColumnName] = s.Name;
            }

            foreach ( var rows in DataContents.Elements( "ROW" ) )
            {
                Dictionary<string, string> entityProperty = new Dictionary<string, string>();
                foreach ( var column in rows.Elements( "COLUMN" ) )
                {
                    entityProperty [storageMappingEntityNames [column.Attribute( "NAME" ).Value]] = column.Value;
                }
                EntityPropertyValues.Add( entityProperty );
            }
        }

        private string GetMappingName(string dataFileName)
        {
            FileInfo fileInfo = new FileInfo(dataFileName);
            return fileInfo.Name.ToLower().Split('.').First();
        }

        private void LoadEntityName()
        {
            LoadDataFileName(EntityMapping.TypeName);
        }

        private EntityTypeMapping GetEntityTypeMapping(EdmxReader<EntityTypeMapping> edmxReader)
        {
            EntityTypeMapping entityTypeMapping = edmxReader.Nodes.SingleOrDefault(
                e => e.MappingFragment.StoreEntitySet.ToLower().Equals(MappingName)
            );
            if (entityTypeMapping == null)
                throw new IndexOutOfRangeException("The file specified is not apart of the mapping. File: " + DataFileName);

            return entityTypeMapping;
        }

        private void LoadDataFileName(string typeName)
        {
            string[] typeNameSplit = typeName.Split(new char[] { '.' }, StringSplitOptions.RemoveEmptyEntries);
            EntityName = typeNameSplit.Last();
        }
    }
}
