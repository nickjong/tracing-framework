#ifndef TRACING_FRAMEWORK_BINDINGS_CPP_INCLUDE_WTF_MACROS_H_
#define TRACING_FRAMEWORK_BINDINGS_CPP_INCLUDE_WTF_MACROS_H_

#include "wtf/runtime.h"

#define __INTERNAL_WTF_NAMESPACE ::wtf

// Enables and disables WTF tracing macros for the current namespace.
// There can be at most one of these in a namespace and its definition must
// be visible at the point where macros are invoked. It is recommended to
// have a header file that includes something like (typically with some
// domain specific preprocessor #ifdef/#ifndef to make it conditional):
//
// #include ".../wtf/macros.h"
//
// namespace foo {
// WTF_NAMESPACE_DISABLE();
// namesapce should_enable {
// WTF_NAMESPACE_ENABLE();
// }
// }
#define WTF_NAMESPACE_ENABLE()                    \
  static constexpr bool kWtfEnabledForNamespace = \
      (__INTERNAL_WTF_NAMESPACE::kMasterEnable && true)

#define WTF_NAMESPACE_DISABLE() \
  static constexpr bool kWtfEnabledForNamespace = false

// Enables WTF tracing for a thread based on a condition.
// Allowed Scopes: Within a function.
#define WTF_THREAD_ENABLE_IF(condition, name)                              \
  if (condition) {                                                         \
    __INTERNAL_WTF_NAMESPACE::Runtime::GetInstance()->EnableCurrentThread( \
        name, nullptr, __FILE__);                                          \
  }

// Enables WTF tracing for a thread based on whether the current namespace
// is enabled.
#define WTF_THREAD_ENABLE(name) \
  WTF_THREAD_ENABLE_IF(kWtfEnabledForNamespace, name)

// Shortcut to trace a no-arg event.
// Allowed Scopes: Within a function.
// This creates an anonymous event anchored to the specific location in the
// code where it is referenced and invokes it.
//
// Example:
//   WTF_EVENT0("MyClass#something_important");
#define WTF_EVENT0(name_spec)                                       \
  static __INTERNAL_WTF_NAMESPACE::EventIf<kWtfEnabledForNamespace> \
      __wtf_event0__##__LINE__{name_spec};                          \
  __wtf_event0__##__LINE__.Invoke()

// Shortcut to trace an event with arbitrary arguments.
// Allowed Scopes: Within a function.
// This creates an anonymous event anchored to the specific location in the
// code where it is referenced and invokes it.
//
// Example:
//   WTF_EVENT("MyClass#stuff", int, uint32_t)(1, 2);
#define WTF_EVENT(name_spec, ...)                                     \
  static __INTERNAL_WTF_NAMESPACE::EventIf<                           \
      kWtfEnabledForNamespace, __VA_ARGS__> __wtf_eventn__##__LINE__{ \
      name_spec};                                                     \
  __wtf_eventn__##__LINE__.Invoke

// Shortcut to trace a no-arg scope.
// Allowed Scopes: Within a function.
// This creates an anonymous scope anchored to the specific location in the
// code where it is referenced. The scope will begin at the point this macro
// is defined and will exit when it goes out of scope.
//
// Example:
//   WTF_SCOPE0("MyClass#MyMethod");
#define WTF_SCOPE0(name_spec)                                             \
  static __INTERNAL_WTF_NAMESPACE::ScopedEventIf<kWtfEnabledForNamespace> \
      __wtf_scope_event0_##__LINE__{name_spec};                           \
  __INTERNAL_WTF_NAMESPACE::AutoScopeIf<kWtfEnabledForNamespace>          \
      __wtf_scope0_##__LINE__{__wtf_scope_event0_##__LINE__};             \
  __wtf_scope0_##__LINE__.Enter()

// Shortcut to trace a scope with arbitrary arguments.
// Allowed Scopes: Within a function.
// This creates an anonymous scope anchored to the specific location in the
// code where it is referenced. The scope will begin at the point this macro
// is defined and will exit when it goes out of scope.
//
// Example:
//   WTF_SCOPE0("MyClass#MyMethod", int, uint32_t)(1, 2);
#define WTF_SCOPE(name_spec, ...)                                             \
  static __INTERNAL_WTF_NAMESPACE::ScopedEventIf<                             \
      kWtfEnabledForNamespace, __VA_ARGS__> __wtf_scope_eventn_##__LINE__{    \
      name_spec};                                                             \
  __INTERNAL_WTF_NAMESPACE::AutoScopeIf<kWtfEnabledForNamespace, __VA_ARGS__> \
      __wtf_scopen_##__LINE__{__wtf_scope_eventn_##__LINE__};                 \
  __wtf_scopen_##__LINE__.Enter

#endif  // TRACING_FRAMEWORK_BINDINGS_CPP_INCLUDE_WTF_MACROS_H_
