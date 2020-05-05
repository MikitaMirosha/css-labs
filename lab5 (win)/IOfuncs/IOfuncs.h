// IOfuncs.h - Contains declaration of Function class  
#pragma once  

#ifdef IOfuncs_EXPORTS  
#define IOfuncs_API __declspec(dllexport)   
#else  
#define IOfuncs_API __declspec(dllimport)   
#endif  

namespace IOfuncs  
{  
    // This class is exported from the IOfuncs.dll  
    class Functions  
    {  
    public:
		static IOfuncs_API DWORD WINAPI runWriter(LPVOID lpParam);
		static IOfuncs_API DWORD WINAPI runReader(LPVOID lpParam);
    };  
}  
