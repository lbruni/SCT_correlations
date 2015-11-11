#ifndef platform_h__
#define platform_h__

#ifdef WIN32 
#ifndef __CINT__
#define DllExport   __declspec( dllexport )
#else
#define DllExport   
#endif // __CINT__
#else 
#define DllExport   
#endif // WIN32

#ifdef WIN32
#define SCT_FUNC __FUNCSIG__
#else
#define  SCT_FUNC __PRETTY_FUNCTION__
#endif // WIN32



#endif // platform_h__
