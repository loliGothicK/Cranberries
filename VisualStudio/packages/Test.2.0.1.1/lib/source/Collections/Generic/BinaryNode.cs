namespace Dnp.Collections.Generic
{
    public class BinaryNode<T>
    {
        public BinaryNode()
        {
        }

        public BinaryNode( BinaryNode<T> left, BinaryNode<T> right, T data )
        {
            Left = left;
            Right = right;
            Data = data;
        }

        public BinaryNode<T> Left { get; set; }
        public BinaryNode<T> Right { get; set; }
        public T Data { get; set; }
    }
}