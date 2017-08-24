using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using Dnp.Data.Metadata;
using Dnp.Data.Metadata.Elements;

namespace Dnp.Data.TestData
{
    public class DataLoader : IDataLoader
    {
        EntityTypeMapping _EntityMapping;
        string[][] _DataFileContents;
        string _DataFileName;
        string _MappingName;

        public DataLoader(string edmxFile, string dataFile)
        {
            Initialize(dataFile);
            using (EdmxReader<EntityTypeMapping> edmxReader = new EdmxReader<EntityTypeMapping>(edmxFile))
            {
                if (!edmxReader.TryLoad())
                    throw new Exception("Failed to load the edmx reader");
                _EntityMapping = GetEntityTypeMapping(edmxReader);
            }
        }

        public DataLoader(EdmxReader<EntityTypeMapping> edmxReader, string dataFile)
        {
            Initialize(dataFile);
            
            if (!edmxReader.HasLoaded)
                if(!edmxReader.TryLoad())
                    throw new Exception("Failed to load the edmx reader");
            _EntityMapping = GetEntityTypeMapping(edmxReader);
        }

        public DataLoader(EntityTypeMapping entityMapping, string dataFile)
        {
            Initialize(dataFile);
            _EntityMapping = entityMapping;
        }

        private void Initialize(string dataFile)
        {
            _DataFileName = dataFile;

            LoadDataFileContents();

            _MappingName = GetMappingName(_DataFileName);
            _EntityPropertyValues = new List<Dictionary<string, string>>();
        }

        private void LoadDataFileContents()
        {
            string [] dataFileContents = File.ReadAllLines( _DataFileName );
            _DataFileContents = new string[dataFileContents.Length][];
            _DataFileContents[0] = dataFileContents[0].Replace("\"", string.Empty).Split( ',' );

            for (int i = 1; i < dataFileContents.Length; i++)
            {
                _DataFileContents[i] = ParseDataContents(dataFileContents[i]).ToArray();
            }
        }

        private IEnumerable<string> BuildRowsOfData( string dataContents )
        {
            List<string> result = new List<string>();

            for ( int start = 0, end = 0; start != dataContents.Length; start = end )
            {
                string[] row = new string[] {};
                int numQoutes = 0;
                do
                {
                    end = dataContents.IndexOf( Environment.NewLine, start );
                    row = dataContents.Substring( start, end - start ).Split( ',' );
                    numQoutes = row.Last().Count( c => c == '\"' );
                } while ( numQoutes != 2 && numQoutes != 0 );
            }

            return result;
        }

        private IEnumerable<string> ParseDataContents(string dataFileContent)
        {
            int endIndex = 0;
            int indexInc = 1;
            bool hasReachedTheEnd = false;
            for (
                int startIndex = 0;
                startIndex < dataFileContent.Length;
                startIndex = endIndex + indexInc
            )
            {
                if (dataFileContent[startIndex] == '"')
                {
                    startIndex += 1;
                    endIndex = dataFileContent.IndexOf("\",", startIndex);
                    indexInc = 2;
                    if (endIndex < 0)
                    {
                        endIndex = dataFileContent.Length - 1;
                        hasReachedTheEnd = true;
                    }
                }
                else
                {
                    endIndex = dataFileContent.IndexOf(",", startIndex);
                    indexInc = 1;
                }

                if (endIndex < 0)
                {
                    endIndex = dataFileContent.Length;
                    hasReachedTheEnd = true;
                }
                yield return dataFileContent.Substring(startIndex, endIndex - startIndex);
            }
            if (!hasReachedTheEnd)
                yield return string.Empty;
        }

        // The name of the entity class that represents the storage mapping.
        string _EntityName;
        public string EntityName { get { return _EntityName; } }

        // A collection of entity property with the associated values.
        ICollection<Dictionary<string, string>> _EntityPropertyValues;
        public ICollection<Dictionary<string, string>> EntityPropertyValues { get { return _EntityPropertyValues; } }

        public bool TryLoad()
        {
            LoadEntityName();
            LoadPropertyValues();

            return true;
        }

        private void LoadPropertyValues()
        {
            // Dictionary with the key being the storage mapping the value being the entity name.
            Dictionary<string, string> storageMappingEntityNames = new Dictionary<string, string>();
            foreach (ScalarProperty s in _EntityMapping.MappingFragment.ScalarProperty)
            {
                storageMappingEntityNames[s.ColumnName] = s.Name;
            }

            for (int i = 1; i < _DataFileContents.Length; i++)
            {
                Dictionary<string, string> entityNameDataValues = new Dictionary<string, string>();
                for(int k = 0; k < storageMappingEntityNames.Count; k++)
                {
                    var storageMap = storageMappingEntityNames.ElementAt( k );
                    string dataFileContentElement =
                        _DataFileContents[ 0 ].FirstOrDefault( d => d.Equals( storageMap.Key ) );
                    if(string.IsNullOrEmpty(dataFileContentElement))
                        entityNameDataValues[ storageMap.Value ] = null;
                    else
                    {
                        for(int l = 0; l < _DataFileContents[i].Length; l++)
                        {
                            if(!_DataFileContents[0][l].Equals( dataFileContentElement ))
                            {
                                continue;
                            }

                            entityNameDataValues[storageMap.Value] = _DataFileContents[ i ][ l ];
                        }
                    }
                }
                _EntityPropertyValues.Add(entityNameDataValues);
            }
        }

        private string GetMappingName(string dataFileName)
        {
            FileInfo fileInfo = new FileInfo(dataFileName);
            return fileInfo.Name.ToLower().Split('.').First();
        }

        private void LoadEntityName()
        {
            LoadDataFileName(_EntityMapping.TypeName);
        }

        private EntityTypeMapping GetEntityTypeMapping(EdmxReader<EntityTypeMapping> edmxReader)
        {
            EntityTypeMapping entityTypeMapping = edmxReader.Nodes.SingleOrDefault(
                e => e.MappingFragment.StoreEntitySet.ToLower().Equals(_MappingName)
            );
            if (entityTypeMapping == null)
                throw new IndexOutOfRangeException("The file specified is not apart of the mapping. File: " + _DataFileName);

            return entityTypeMapping;
        }

        private void LoadDataFileName(string typeName)
        {
            string[] typeNameSplit = typeName.Split(new char[] { '.' }, StringSplitOptions.RemoveEmptyEntries);
            _EntityName = typeNameSplit.Last();
        }
    }
}
