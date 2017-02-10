#ifndef INTERPOLATION_HPP
#define INTERPOLATION_HPP
#include <iostream>
#include <cmath>
#include <array>
#include <vector>
#include <tuple>

namespace cranberries {
    namespace math {
        
        template < typename T >
        constexpr T PI = T(3.141592653589793238462643383279502884197L);
        
        inline namespace interpolation {


            // array<Type,Dim+1> lstsq<Dim,N,Type>(array<Type,N> x,array<Type,N> y)
            // N個の2次元の点を表す2つのarray<Type,N> x,y から
            // Dim次の多項式係数を格納したarray<Type,Dim+1>を返す
            template < size_t Dim, size_t N, typename Type = long double >
            std::array<Type,Dim+1> lstsq(std::array<Type,N> const& x, std::array<Type,N> const& y)
            {
                std::array<Type,Dim+1> c{};
                size_t i{}, j{}, k{};
                Type aik{}, pivot{}, w1{}, w2{}, w3{};

                static_assert(Dim <= N || Dim == 0,"Error : Illegal parameter in lstsq()");
                static constexpr size_t mp1 = Dim + 1;
                static constexpr size_t mp2 = Dim + 2;
                static constexpr size_t m2 = 2 * Dim;
                std::array<Type,mp1 * mp2> a{};
                std::array<Type,mp1 * 2> w{};
                for (i = 0; i < m2; ++i)
                {
                    w1 = 0.;
                    for (j = 0; j < N; ++j)
                    {
                        w2 = w3 = x[j];
                        for (k = 0; k < i; ++k)	w2 *= w3;
                        w1 += w2;
                    }
                    w[i] = w1;
                }
                a[0] = N;
                for (i = 0; i < mp1; ++i)
                    for (j = 0; j < mp1; ++j)
                      if (i || j)	a[i * mp2 + j] = w[i + j - 1];

                w1 = 0.;
                for (i = 0; i < N; ++i)	w1 += y[i];
                a[mp1] = w1;
                for (i = 0; i < Dim; ++i)
                {
                    w1 = 0.;
                    for (j = 0; j < N; ++j)
                    {
                        w2 = w3 = x[j];
                        for (k = 0; k < i; ++k)	w2 *= w3;
                        w1 += y[j] * w2;
                    }
                    a[mp2 * ( i + 1 ) + mp1] = w1;
                }

                for (k = 0; k < mp1; ++k)
                {
                    pivot = a[mp2 * k + k];
                    a[mp2 * k + k] = 1.0;
                    for (j = k + 1; j < mp2; ++j)	a[mp2 * k + j] /= pivot;
                    for (i = 0; i < mp1; ++i)
                    {
                        if (i != k)
                        {
                            aik = a[mp2 * i + k];
                            for (j = k; j < mp2; ++j)
                                a[mp2 * i + j] -= aik * a[mp2 * k + j];
                        }
                    }
                }
                for (i = 0; i < mp1; ++i)	c[i] = a[mp2 * i + mp1];
                return c;
            }
            /*
            template < size_t N >
            double lagra(std::array<double,N> xx, std::array<double,N> yy, double xi)
            {
                int flag, i, j, *k;
                double *p, *q, *r, *s, w, w1, w2, w3;
                std::array<int,N> jun{};
                std::array<double,N> x{},y{};

                for (i = 0, p = xx, flag = 0; i < N - 1; ++i, ++p)
                {
                    if (*p >= *( p + 1 ))
                    {
                        flag = 1;
                        break;
                    }
                }
                if (flag)
                {
                    sortdi1(xx, n, jun);
                    for (i = 0, p = x, q = y, k = jun; i < N; ++i)
                    {
                        *++p = *( xx + *k );
                        *++q = *( yy + *++k );
                    }
                }
                else
                {
                    for (i = 0, p = x, q = y, r = xx, s = yy; i < N; ++i)
                    {
                        *++p = *++r;
                        *++q = *++s;
                    }
                }

                if (N < 2 || xi < *x || xi > *( x + N - 1 ))
                {
                    fprintf(stderr, "Error : Illegal parameter  in lagra()\n");
                    return 0.;
                }
                for (i = 0, p = x; i < N; ++i)
                {
                    if (*++p == xi)
                    {
                        return *( y + i );
                    }
                }
                w = 0.;
                for (i = 0, p = x, q = y; i < N; ++i)
                {
                    w1 = 1.;
                    w2 = *++p;
                    for (j = 0, r = x; j < N; ++j)
                    {
                        w3 = *++r;
                        if (i != j)	w1 *= ( xi - w3 ) / ( w2 - w3 );
                    }
                    w += w1 * *++q;
                }
                return w;
            }
            
            double splint(double xx[], double yy[], int n, double xi)
            {
                int flag, i, *jun, *k;
                double dxp, dxm, *h, hi, hi2, *p, *q, *r, *s, sm, si, *sp, *x, *y, yi;

                x = (double *)malloc(n * sizeof(double));
                if (x == nullptr)
                {
                    fprintf(stderr, "Error : Out of memory  in splint()\n");
                    return 0.;
                }
                y = (double *)malloc(n * sizeof(double));
                if (y == nullptr)
                {
                    fprintf(stderr, "Error : Out of memory  in splint()\n");
                    free((char *)x);
                    return 0.;
                }
                for (i = 0, p = xx, flag = 0; i < n - 1; ++i, ++p)
                {
                    if (*p >= *( p + 1 ))
                    {
                        flag = 1;
                        break;
                    }
                }
                if (flag)
                {
                    jun = (int *)malloc(n * sizeof(int));
                    if (jun == nullptr)
                    {
                        fprintf(stderr, "Error : Out of memory  in splint()\n");
                        free((char *)x);
                        free((char *)y);
                        return 0.;
                    }
                    sortdi1(xx, n, jun);
                    for (i = 0, p = x, q = y, k = jun; i < n; ++i)
                    {
                        *++p = *( xx + *k );
                        *++q = *( yy + *++k );
                    }
                    free((char *)jun);
                }
                else
                {
                    for (i = 0, p = x, q = y, r = xx, s = yy; i < n; ++i)
                    {
                        *++p = *++r;
                        *++q = *++s;
                    }
                }

                if (n < 2 || xi < *x || xi > *( x + n - 1 ))
                {
                    fprintf(stderr, "Error : Illegal parameter  in splint()\n");
                    free((char *)x);
                    free((char *)y);
                    return 0.;
                }
                for (i = 0, p = x; i < n; ++i)
                {
                    if (*++p == xi)
                    {
                        free((char *)x);
                        free((char *)y);
                        return *( y + i );
                    }
                }
                h = (double *)malloc(n * sizeof(double));
                if (h == nullptr)
                {
                    fprintf(stderr, "Error : Out of memory  in splint()\n");
                    free((char *)x);
                    free((char *)y);
                    return 0.;
                }
                sp = (double *)malloc(n * sizeof(double));
                if (sp == nullptr)
                {
                    fprintf(stderr, "Error : Out of memory  in splint()\n");
                    free((char *)x);
                    free((char *)y);
                    free((char *)h);
                    return 0.;
                }

                subspl(x, y, n - 1, h, sp);
                for (i = 1, p = x + 1; i <= n; ++i, ++p)
                {
                    if (*( p - 1 ) <= xi && xi < *p)
                    {
                        sm = *( sp + i - 1 );
                        si = *( sp + i );
                        hi = *( h + i );
                        hi2 = hi * hi;
                        dxp = *p - xi;
                        dxm = xi - *( p - 1 );
                        yi = ( sm * dxp * dxp * dxp + si * dxm * dxm * dxm
                            + ( 6. * *( y + i - 1 ) - hi2 * sm ) * dxp
                            + ( 6. * *( y + i ) - hi2 * si ) * dxm ) / hi / 6.;
                        free((char *)x);
                        free((char *)y);
                        free((char *)h);
                        free((char *)sp);
                        return yi;
                    }
                }
                return 0;
            }
            */

            // chebyshev のヘルパー関数
            // 係数の格納されたコンテナ a 、値 x より多項式の値を計算する
            template < typename Container >
            inline auto polynomial(Container const& a, typename Container::value_type x) noexcept
            {
                auto w = *a.crbegin();
                for (auto&& iter = a.crbegin()+1; iter != a.crend();++ iter) w = w * x + *iter;
                return w;
            }


            // array<Type,N+1> chebyshev<N,Type>(func,{lower, upper})
            // [lower, upper]における、funcのN次chebyshev多項式近似の係数を格納した
            // array<Type,N+1>を返す
            // Type はデフォルト引数が設定されており、省略するとlong doubleになる
            template < int N, typename Type = long double, typename F >
            inline std::array<Type, N + 1> chebyshev(F _f, std::tuple<Type, Type> range) noexcept
            {
                std::array<Type, N + 1> result{};
                //static auto T = std::array<std::array<Type, N + 1>, N + 1>{};
                auto T = std::vector<std::vector<Type>>{};
                T.reserve(N+1);
                for(size_t i = 1; i < N+2;++ i)
                    T.emplace_back(std::vector<Type>(i));
                std::array<Type, N + 1> c{}, func{}, x{};
                Type k1{}, k2{}, n1{}, w{};
                size_t i{}, j{};
                auto lower = std::get<0>(range);
                auto upper = std::get<1>(range);

                static_assert(N > 0,"Error : illegal parameter input in chebyshev()");
                if (upper <= lower)
                {
                    std::cerr << "Error : illegal parameter input in chebyshev()" << std::endl;
                    return{};
                }

                /*	Chebyshev Polynomial	*/
                T[0][0] = 1;
                T[1][0] = 0;
                T[1][1] = 1;
                for (i = 2; i <= N; ++i)
                {
                    for (j = 1; j <= i; ++j)	T[i][j] = 2. * T[i - 1][j - 1];
                    for (j = 0; j < i - 1; ++j)	T[i][j] -= T[i - 2][j];
                }

                k1 = ( upper - lower ) / 2.;
                k2 = ( lower + upper ) / 2.;
                n1 = N + 1;

                w = 0.5 * PI<Type> / n1;
                for (i = 0; i <= N; ++i)
                {
                    x[i] = std::cos(( 2 * i + 1 ) * w);
                    func[i] = _f(k1 * x[i] + k2);
                }

                w = 0.;
                for (j = 0; j <= N; ++j)	w += func[j];
                c[0] = w / n1;

                for (i = 1; i <= N; ++i)
                {
                    w = 0.;
                    for (j = 0; j <= N; ++j)	w += func[j] * polynomial(T[i], x[j]);
                    c[i] = 2. * w / n1;
                }

                for (i = 0; i <= N; ++i)
                {
                    w = c[i];
                    for (j = 0; j <= i; ++j)	result[j] += w * T[i][j];
                }

                if (lower != -1. || upper != 1.)
                {
                    for (i = 1; i <= N; ++i)	for (j = 1; j <= i; ++j)	result[i] /= k1;

                    if (k2 != 0.) T[0][0] = 1;

                    for (i = 1; i <= N; ++i)
                    {
                        T[i][0] = 1;
                        T[i][i] = 1;
                        for (j = 1; j < i; ++j)	T[i][j] = T[i - 1][j] + T[i - 1][j - 1];
                    }

                    for (i = 0; i <= N; ++i)
                    {
                        c[i] = result[i];
                        w = -k2 * result[i];
                        for (j = i - 1; j != size_t(-1); j--)
                        {
                            c[j] += T[i][j] * w;
                            w *= -k2;
                        }
                    }
                    for (i = 0; i <= N; ++i)	result[i] = c[i];
                }
                return result;
            }
        }
    }
}
#endif // !INTERPOLATION_HPP