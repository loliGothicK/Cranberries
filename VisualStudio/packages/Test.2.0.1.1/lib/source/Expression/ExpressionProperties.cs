using System;
using System.Diagnostics.Contracts;
using System.Linq.Expressions;
using System.Reflection;

namespace Dnp.Expression
{
    public class ExpressionProperties
    {
        /// <summary>
        ///   Gets the name of the property in the property expression.
        /// </summary>
        /// <typeparam name = "TProperty">The type of the property.</typeparam>
        /// <param name = "propertyExpression">The property expression from which to parse the propery name.</param>
        /// <returns></returns>
        public static string GetPropertyName<TProperty>( Expression<Func<TProperty>> propertyExpression, Type type )
        {
            Contract.Assert( propertyExpression != null );
            var memberExpression = propertyExpression.Body as MemberExpression;
            Contract.Assert( memberExpression != null );
            var property = memberExpression.Member as PropertyInfo;
            Contract.Assert( property != null );

            if ( !property.DeclaringType.IsAssignableFrom( type ) )
            {
                throw new ArgumentException( "The referenced property does not belong to this type.",
                                             "propertyExpression" );
            }

            MethodInfo getMethod = property.GetGetMethod( true );

            if ( getMethod.IsStatic )
            {
                throw new ArgumentException( "The referenced property is a static.", "propertyExpression" );
            }

            return memberExpression.Member.Name;
        }
    }
}
