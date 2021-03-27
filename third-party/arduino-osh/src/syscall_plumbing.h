/**
 * This file is privately included in service.c
 *
 * This software is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * This is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this source code. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef OS_SYSCALL_PLUMBING_H
#define OS_SYSCALL_PLUMBING_H

#if defined(__SAMD21__) || defined(__SAMD51__)

#define SVC_ArgN(n)                                                            \
  register int __r##n __asm("r"#n);

#define SVC_ArgR(n,t,a)                                                        \
  register t   __r##n __asm("r"#n) = a;

#define SVC_Arg0()                                                             \
  SVC_ArgN(0)                                                                  \
  SVC_ArgN(1)                                                                  \
  SVC_ArgN(2)                                                                  \
  SVC_ArgN(3)

#define SVC_Arg1(t1)                                                           \
  SVC_ArgR(0,t1,a1)                                                            \
  SVC_ArgN(1)                                                                  \
  SVC_ArgN(2)                                                                  \
  SVC_ArgN(3)

#define SVC_Arg2(t1,t2)                                                        \
  SVC_ArgR(0,t1,a1)                                                            \
  SVC_ArgR(1,t2,a2)                                                            \
  SVC_ArgN(2)                                                                  \
  SVC_ArgN(3)

#define SVC_Arg3(t1,t2,t3)                                                     \
  SVC_ArgR(0,t1,a1)                                                            \
  SVC_ArgR(1,t2,a2)                                                            \
  SVC_ArgR(2,t3,a3)                                                            \
  SVC_ArgN(3)

#define SVC_Arg4(t1,t2,t3,t4)                                                  \
  SVC_ArgR(0,t1,a1)                                                            \
  SVC_ArgR(1,t2,a2)                                                            \
  SVC_ArgR(2,t3,a3)                                                            \
  SVC_ArgR(3,t4,a4)

#if defined(__SAMD21__)
#define SVC_Call(f)                                                            \
  __asm volatile                                                               \
  (                                                                            \
    "ldr r7,="#f"\n\t"                                                         \
    "mov r12,r7\n\t"                                                           \
    "svc 0"                                                                    \
    :               "=r" (__r0), "=r" (__r1), "=r" (__r2), "=r" (__r3)         \
    :                "r" (__r0),  "r" (__r1),  "r" (__r2),  "r" (__r3)         \
    : "r7", "r12", "lr", "cc"                                                  \
  );
#else
#define SVC_Call(f)                                                            \
  __asm volatile                                                               \
  (                                                                            \
    "ldr r12,="#f"\n\t"                                                        \
    "svc 0"                                                                    \
    :               "=r" (__r0), "=r" (__r1), "=r" (__r2), "=r" (__r3)         \
    :                "r" (__r0),  "r" (__r1),  "r" (__r2),  "r" (__r3)         \
    : "r12", "lr", "cc"                                                        \
  );
#endif

#define SVC_0_1(f,t,rv)                                                        \
__attribute__((always_inline))                                                 \
static inline  t __##f (void) {                                                \
  SVC_Arg0();                                                                  \
  SVC_Call(f);                                                                 \
  return (t) rv;                                                               \
}

#define SVC_1_0(f,t,t1)                                                        \
__attribute__((always_inline))                                                 \
static inline  t __##f (t1 a1) {                                               \
  SVC_Arg1(t1);                                                                \
  SVC_Call(f);                                                                 \
}

#define SVC_1_1(f,t,t1,rv)                                                     \
__attribute__((always_inline))                                                 \
static inline  t __##f (t1 a1) {                                               \
  SVC_Arg1(t1);                                                                \
  SVC_Call(f);                                                                 \
  return (t) rv;                                                               \
}

#define SVC_2_1(f,t,t1,t2,rv)                                                  \
__attribute__((always_inline))                                                 \
static inline  t __##f (t1 a1, t2 a2) {                                        \
  SVC_Arg2(t1,t2);                                                             \
  SVC_Call(f);                                                                 \
  return (t) rv;                                                               \
}

#define SVC_3_1(f,t,t1,t2,t3,rv)                                               \
__attribute__((always_inline))                                                 \
static inline  t __##f (t1 a1, t2 a2, t3 a3) {                                 \
  SVC_Arg3(t1,t2,t3);                                                          \
  SVC_Call(f);                                                                 \
  return (t) rv;                                                               \
}

#define SVC_4_1(f,t,t1,t2,t3,t4,rv)                                            \
__attribute__((always_inline))                                                 \
static inline  t __##f (t1 a1, t2 a2, t3 a3, t4 a4) {                          \
  SVC_Arg4(t1,t2,t3,t4);                                                       \
  SVC_Call(f);                                                                 \
  return (t) rv;                                                               \
}

typedef uint32_t __attribute__((vector_size(8)))  ret64;
typedef uint32_t __attribute__((vector_size(16))) ret128;

#define RET_uint32_t                   __r0
#define RET_int32_t                    __r0
#define RET_os_status_t                __r0
#define RET_os_tuple_t                 {(os_status_t)__r0, {(uint32_t)__r1}}

#define os_tuple_return_type_t         __attribute__((pcs("aapcs"))) ret64
#define os_tuple_return_value(r)       (ret64){ r.status, r.value.u32 }

#else

#define SVC_0_1(f,t,rv)                                                        \
static inline  t __##f(void) {                                                 \
    return f();                                                                \
}

#define SVC_1_0(f,t,t1)                                                        \
static inline  t __##f (t1 a1) {                                               \
    f(a1);                                                                     \
}

#define SVC_1_1(f,t,t1,rv)                                                     \
static inline  t __##f (t1 a1) {                                               \
    return f(a1);                                                              \
}

#define SVC_2_1(f,t,t1,t2,rv)                                                  \
static inline  t __##f (t1 a1, t2 a2) {                                        \
    return f(a1, a2);                                                          \
}

#define SVC_3_1(f,t,t1,t2,t3,rv)                                               \
static inline  t __##f (t1 a1, t2 a2, t3 a3) {                                 \
    return f(a1, a2, a3);                                                      \
}

#define os_tuple_return_type_t         os_tuple_t

#define os_tuple_return_value(r)       r

#endif

#if !(defined(__SAMD21__) || defined(__SAMD51__))
inline static int32_t __get_CONTROL() {
    return 0;
}

#define __get_IPSR()     0
#endif

/**
 * Returns true if the current execution context is privileged.
 */
inline static bool osi_is_privileged() {
    return (__get_CONTROL() & 0x1) == 0x0;
}

/**
 * Returns true if the current execution context is using PSP.
 */
inline static bool osi_in_task() {
    return (__get_CONTROL() & 0x2) == 0x2;
}

#endif // OS_SYSCALL_PLUMBING_H
