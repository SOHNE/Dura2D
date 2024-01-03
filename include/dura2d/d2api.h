#ifndef DURA2D_D2API_H
#define DURA2D_D2API_H

#ifdef D2_SHARED
#if defined _WIN32 || defined __CYGWIN__
    #ifdef dura2d_EXPORTS
      #ifdef __GNUC__
        #define D2_API __attribute__ ((dllexport))
      #else
        #define D2_API __declspec(dllexport)
      #endif
    #else
      #ifdef __GNUC__
        #define D2_API __attribute__ ((dllimport))
      #else
        #define D2_API __declspec(dllimport)
      #endif
    #endif
  #else
    #if __GNUC__ >= 4
      #define D2_API __attribute__ ((visibility ("default")))
    #else
      #define D2_API
    #endif
  #endif
#else
#define D2_API
#endif

#endif //DURA2D_D2API_H
