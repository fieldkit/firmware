#ifndef ALOGGING_ASSERT_H_INCLUDED
#define ALOGGING_ASSERT_H_INCLUDED

#define assert(EXPRESSION)       cfy_assert(EXPRESSION)

#define cfy_assert(EXPRESSION)   (void)((EXPRESSION) || (__cfy_assert(#EXPRESSION, __FILE__, __LINE__), 0))

#ifdef __cplusplus
extern "C" {
#endif

void __cfy_assert(const char *assertion, const char *file, int line);

#ifdef __cplusplus
}
#endif

#endif
