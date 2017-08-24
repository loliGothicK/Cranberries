using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Dnp.Data.Metadata;
using Dnp.Data.Metadata.Elements;

namespace Dnp.Data.TestData
{
    public class DirectoryDataLoader
    {
        public DirectoryDataLoader(string directory, string edmxFile)
        {
            Files = Directory.GetFiles(directory);
            EdmxFile = edmxFile;
        }

        private string EdmxFile { get; set; }
        public IEnumerable<string> Files { get; set; }

        public IEnumerable<IDataLoader> GetData()
        {
            EdmxReader<EntityTypeMapping> edmxReader = new EdmxReader<EntityTypeMapping>(EdmxFile);
            foreach (string file in Files)
            {
                IDataLoader dataLoader = LoadData( edmxReader, file );
                if ( !ReferenceEquals( dataLoader, null ) )
                {
                    yield return dataLoader;
                }
            }
        }

        public IDataLoader LoadData(EdmxReader<EntityTypeMapping> edmxReader, string dataFile)
        {
            IDataLoader dataLoader = null;

            try
            {
                dataLoader = new DataLoaderXml( edmxReader, dataFile );
                if ( !dataLoader.TryLoad() )
                    throw new InvalidOperationException( "Failed to load data for the file specifed. File: " + dataFile );
            }
            catch ( Exception ) { return null; }

            return dataLoader;
        }
    }
}
