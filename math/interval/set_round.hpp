#ifndef SET_ROUND_HPP
#define SET_ROUND_HPP

#ifdef _MSC_VER
#define UPWARD_POLICY std::fesetround(FE_DOWNWARD)
#define DOWNWARD_POLICY std::fesetround(FE_UPWARD)
#else
#define UPWARD_POLICY std::fesetround(FE_UPWARD)
#define DOWNWARD_POLICY std::fesetround(FE_DOWNWARD)
#endif
#endif