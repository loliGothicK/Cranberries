using System;
using System.Collections.Generic;
using System.Linq;
using System.Xml;
using System.Reflection;
using Dnp.Collections;

namespace Dnp.Data.Metadata
{
    public class EdmxReader<TNode> : IDisposable
        where TNode : new()
    {
        #region Constructors

        public EdmxReader(string edmxFile)
        {
            _Nodes = new List<TNode>();
            Reader = new XmlTextReader(edmxFile);
        }

        #endregion Constructors

        #region Public Properties

        bool _HasLoaded;
        public bool HasLoaded { get { return _HasLoaded; } }

    	readonly ICollection<TNode> _Nodes;
        public ICollection<TNode> Nodes { get { return _Nodes; } }

        #endregion Public Properties

        #region Private Properties

        private XmlTextReader Reader { get; set; }

        #endregion Private Properties

        #region Public Methods

        public bool TryLoad()
        {
            try
            {
                Load();
            }
            catch (Exception ex)
            {
                throw new Exception("Failed loading edmx with the appropriate format.", ex);
            }

            _HasLoaded = true;
            return true;
        }

        #region IDisposable

        public void Dispose()
        {
            Reader.Close();
        }

        #endregion IDisposable

        #endregion Public Methods

        #region Private Methods

        private void Load()
        {
            Type parentNodeType = typeof(TNode);
            string parentNodeName = parentNodeType.Name;

            while (Reader.Read())
            {
                switch (Reader.NodeType)
                {
                    case XmlNodeType.Element:
                        if (Reader.Name.Equals(parentNodeName))
                        {
                            TNode parentNode = new TNode();
                            LoadNode(parentNodeType, parentNodeName, parentNode);
                            _Nodes.Add(parentNode);
                        }
                        break;
                }
            }
        }

        private void LoadNode(Type nodeType, string nodeName, object node)
        {
            LoadAttributes(nodeType, node);

            for (
                bool hasMore = Reader.Read(); 
                hasMore && 
                !(Reader.NodeType == XmlNodeType.EndElement && 
                Reader.Name.Equals(nodeName));
                hasMore = Reader.Read()
            )
            {
                switch (Reader.NodeType)
                {
                    case XmlNodeType.Element:
                        var elementPropertyInfos = GetPropertyWhere(nodeType, p => p.Name.Equals(Reader.Name));
                        if (elementPropertyInfos.Count != 1)
                            throw new InvalidOperationException("Node type is prevelant less than or more than once within the class");
                        PropertyInfo elementPropertyInfo = elementPropertyInfos.First();

                        Type elementType = elementPropertyInfo.PropertyType;
                        object element = elementPropertyInfo.GetValue(node, null);

                        if (element is IPropertyCollection)
                        {
                            Type propertyType = elementType.GetGenericArguments()[0];
                            object property = Activator.CreateInstance(propertyType, false);
                            ((IPropertyCollection)element).Add(property);
                            if (Reader.IsEmptyElement)
                            {
                                LoadAttributes(propertyType, property);
                            }
                            else
                            {
                                LoadNode(propertyType, Reader.Name, property);
                            }
                        }
                        else
                        {
                            LoadNode(elementType, Reader.Name, element);
                        }
                        break;
                }
            }
        }

        private void LoadAttributes(Type nodeType, object node)
        {
            PropertyInfo[] propertyInfos = nodeType.GetProperties();
            while (Reader.MoveToNextAttribute())
            {
                PropertyInfo attributeProperty = propertyInfos.FirstOrDefault(p => p.Name.Equals(Reader.Name));
                if (attributeProperty == null)
                    throw new InvalidOperationException("The attribute within the edmx does not match up with the specified class property");
                attributeProperty.SetValue(node, Reader.Value, null);
            }
        }

        private static ICollection<PropertyInfo> GetPropertyWhere(Type nodeType, Func<PropertyInfo, bool> predicate)
        {
            return nodeType.GetProperties().Where(predicate).ToList();
        }

        #endregion Private Methods
    }
}
