#pragma once

#ifdef _OBJX_USE_DLL_
#ifdef _BUILD_OBJX_LIB_
#define OBJX_LIB_INC __declspec(dllexport)
#else
#define OBJX_LIB_INC __declspec(dllimport)
#endif
#else
#define OBJX_LIB_INC
#endif

//! Use standard strings to hold characters
#include <string>

//! Include the Standard Function objects to allow for for-each iteration of properties
#include <functional>

namespace Objx {
    //! Define the type of the Objx values
    enum ETypeMask : unsigned char {
        //! Define the standard types
        TYPE_NULL       = 0 << 0,
        TYPE_INT        = 1 << 0,
        TYPE_DEC        = 1 << 1,
        TYPE_BOOL       = 1 << 2,
        TYPE_STRING     = 1 << 3,

        //! Define container type
        TYPE_OBJX       = 1 << 4,

        //! Define the array type
        TYPE_ARRAY      = 1 << 5,
    };

	/*
		typeMaskToString - Convert an ETypeMask value to a string value
		Author: Mitchell Croft
		Created: 29/08/2017
		Modified: 29/08/2017

		param[in] pMask - The ETypeMask value to convert

		return const char* - Returns a constant pointer to a c-string array
	*/
	extern OBJX_LIB_INC const char* typeMaskToString(const ETypeMask& pMask);

    //! Define status codes used by Objx Components
    enum class EStatusCode : unsigned char {
        //! Operation occurred without any errors
        Success,                        

        //! Operation was supplied with a nullptr to read from
        Null_Data_Pointer,

        //! Operation was given an array with a count of 0 to read from
        Empty_Array_Bounds,

        //! Operation was given an empty, null data Node to read from
        Empty_Data_Object,

        //! Operation was attempted on a Node object that was assumed to be an Objx type
        Non_Objx_Node,

		//! Error codes for the various potential errors that can occur during the parsing process
		Unidentified_Syntax_Error,
		Comment_Block_Reached_EOS,
		String_Block_Reached_EOS,
		Unexpected_EOS,
		Value_Setup_Failed,
		Objx_Depth_Limit_Reached,
		Unexpected_Token_During_Parsing,
		Invalid_Value_During_Parsing,
		Invalid_Parsed_Array_Type,
    };

	/*
		statusCodeToString - Convert an EStatusCode value to a string value
		Author: Mitchell Croft
		Created: 29/08/2017
		Modified: 29/08/2017

		param[in] pCode - The EStatusCode value to convert

		return const char* - Returns a constant pointer to a c-string array
	*/
	extern OBJX_LIB_INC const char* statusCodeToString(const EStatusCode& pCode);

    //! Template the Objx namespace objects
    class Node;
    class Document;
    class Reader;
    class Writer;

    //! Define generic data value
    typedef void* xdata;

    //! Define the types that can be used with the Objx system
    typedef int             xint;
    typedef double          xdec;
    typedef bool            xbool;
    typedef std::string     xstring;
    typedef Node            xobjx;

    //! Create a type define for the callback objects, bool return used to indicate if the loop should continue executing
    typedef std::function<bool(const xstring& pName, Node& pProp)>          ForEachCB;
    typedef std::function<bool(const xstring& pName, const Node& pProp)>   cForEachCB;

	//! Create a type define for the function signature used for Read operation error callback
	typedef std::function<void(const EStatusCode& pCode, const size_t& pLine, const size_t& pColumn, const char* pMsg)> ErrorCB;

    /*
     *      Name: Implementation 
     *      Author: Mitchell Croft
     *      Created: 11/04/2017
     *      Modified: 11/04/2017
     *      
     *      Purpose:
     *      Abstract away the objects used to implement partially specialised
     *      templated objects
    **/
    namespace Implementation {
        /*
         *      Name: PointerDepth_impl (Base Type)
         *      Author: Mitchell Croft
         *      Created: 11/04/2017
         *      Modified: 11/04/2017
         *      
         *      template T - A non-pointer type
         *      
         *      Purpose:
         *      Provide a starting depth value for the Pointer Analysis 
         *      object
        **/
        template<typename T> struct PointerDepth_impl { 
            //! Provide a starting point for the pointer depth counter
            enum { depth = 0 }; 
        };

        /*
         *      Name: PointerDepth_impl (Pointer)
         *      Author: Mitchell Croft
         *      Created: 11/04/2017
         *      Modified: 11/04/2017
         *      
         *      template T - A type with at least one pointer level
         *      
         *      Purpose:
         *      Recursively determine the depth of the specified type
         *      through partial specialisation of the object for pointer 
         *      types
        **/
        template<typename T> struct PointerDepth_impl<T*> {
            //! Increment the pointer depth counter per recursion into the specialisation
            enum { depth = PointerDepth_impl<T>::depth + 1 };
        };

        /*
         *      Name: PointerBase_impl (Base Type)
         *      Author: Mitchell Croft
         *      Created: 11/04/2017
         *      Modified: 11/04/2017
         *      
         *      template T - A non-pointer type
         *      
         *      Purpose:
         *      Store the base type for any pointer template type
        **/
        template<typename T> struct PointerBase_impl {
            //! Store the base type of the template
            typedef T type;
        };

        /*
         *      Name: PointerBase_impl (Pointer)
         *      Author: Mitchell Croft
         *      Created: 11/04/2017
         *      Modified: 11/04/2017
         *      
         *      template T - A type with at least one pointer level
         *      
         *      Purpose:
         *      Recursively determine the type of the template
         *      through partial specialisation of the object for
         *      pointer types
        **/
        template<typename T> struct PointerBase_impl<T*> {
            //! Recurse down the pointer chain
            typedef typename PointerBase_impl<T>::type type;
        };

        /*
         *      Name: templateToTypeMask_impl
         *      Author: Mitchell Croft
         *      Created: 03/05/2017
         *      Modified: 03/05/2017
         *      
         *      template T - The type to convert to an ETypeMask value
         *      
         *      Purpose:
         *      Store the ETypeMask values for various Objx types as well
         *      as flag any invalid types that are passed as template types
        **/
        template<typename T> struct templateToTypeMask_impl { enum { invalid = 1, mask = ETypeMask::TYPE_NULL }; };
        template<> struct templateToTypeMask_impl<xint> { enum { invalid = 0, mask = ETypeMask::TYPE_INT }; };
        template<> struct templateToTypeMask_impl<xdec> { enum { invalid = 0, mask = ETypeMask::TYPE_DEC }; };
        template<> struct templateToTypeMask_impl<xbool> { enum { invalid = 0, mask = ETypeMask::TYPE_BOOL }; };
        template<> struct templateToTypeMask_impl<xstring> { enum { invalid = 0, mask = ETypeMask::TYPE_STRING }; };
        template<> struct templateToTypeMask_impl<xobjx> { enum { invalid = 0, mask = ETypeMask::TYPE_OBJX }; };
        template<typename T> struct templateToTypeMask_impl<T*> { enum { invalid = templateToTypeMask_impl<T>::invalid + PointerDepth_impl<T>::depth, mask = (ETypeMask::TYPE_ARRAY | templateToTypeMask_impl<T>::mask) }; };
        template<typename T, size_t N> struct templateToTypeMask_impl<T[N]> { enum { invalid = templateToTypeMask_impl<T>::invalid + PointerDepth_impl<T>::depth, mask = (ETypeMask::TYPE_ARRAY | templateToTypeMask_impl<T>::mask) }; };
    }

    /*
     *      Name: PointerAnalysis
     *      Author: Mitchell Croft
     *      Created: 11/04/2017
     *      Modified: 11/04/2017
     *      
     *      template T - Any type defined can be analyzed through this object
     *      
     *      Purpose:
     *      Store the final pointer depth level of the templated type and
     *      the base type of the template
     *      
     *      Implementation used is from James McNellis
     *      http://stackoverflow.com/questions/5520429/how-can-i-define-a-template-class-which-gives-the-pointer-depth-level-of-a-type
    **/
    template<typename T>
    struct PointerAnalysis { 
        //! The final depth level for the type passed into the template
        enum { depth = Implementation::PointerDepth_impl<T>::depth };

        //! The base type of the template type
        typedef typename Implementation::PointerBase_impl<T>::type type;
    };

    /*
        templateToTypeMask - Convert a template type into an ETypeMask value while providing compile time type validation
        Author: Mitchell Croft
        Created: 03/05/2017
        Modified: 03/05/2017

        template T - The type to convert to an ETypeMask value

        return ETypeMask - Returns an ETypeMask value corresponding to the type of the template
    */
    template<typename T>
    inline ETypeMask templateToTypeMask() {
        static_assert(!Implementation::templateToTypeMask_impl<T>::invalid, "Supplied type is not an Objx compatible type. Cannot use non-Objx types with this library implementation. Please use Objx::xint, Objx::xdec, Objx::xbool, Objx::xstring, Objx::xobjx or array (E.g. type*) variations of them");
        return (ETypeMask)Implementation::templateToTypeMask_impl<T>::mask;
    }
}