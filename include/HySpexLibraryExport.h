
#ifndef HYSPEXLIBRARY_API_H
#define HYSPEXLIBRARY_API_H

#ifdef HYSPEXLIBRARY_STATIC_DEFINE
#  define HYSPEXLIBRARY_API
#  define HYSPEXLIBRARY_NO_EXPORT
#else
#  ifndef HYSPEXLIBRARY_API
#    ifdef HySpexLibrary_EXPORTS
        /* We are building this library */
#      define HYSPEXLIBRARY_API __declspec(dllexport)
#    else
        /* We are using this library */
#      define HYSPEXLIBRARY_API __declspec(dllimport)
#    endif
#  endif

#  ifndef HYSPEXLIBRARY_NO_EXPORT
#    define HYSPEXLIBRARY_NO_EXPORT 
#  endif
#endif

#ifndef HYSPEXLIBRARY_DEPRECATED
#  define HYSPEXLIBRARY_DEPRECATED __declspec(deprecated)
#endif

#ifndef HYSPEXLIBRARY_DEPRECATED_EXPORT
#  define HYSPEXLIBRARY_DEPRECATED_EXPORT HYSPEXLIBRARY_API HYSPEXLIBRARY_DEPRECATED
#endif

#ifndef HYSPEXLIBRARY_DEPRECATED_NO_EXPORT
#  define HYSPEXLIBRARY_DEPRECATED_NO_EXPORT HYSPEXLIBRARY_NO_EXPORT HYSPEXLIBRARY_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef HYSPEXLIBRARY_NO_DEPRECATED
#    define HYSPEXLIBRARY_NO_DEPRECATED
#  endif
#endif

#endif /* HYSPEXLIBRARY_API_H */
