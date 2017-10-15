#pragma once

//! Include the Objx standard values
#include "Objx_values.hpp"

namespace Objx {
    /*
    *       Name: Node
    *       Author: Mitchell Croft
    *       Created: 04/04/2017
    *       Modified: 05/10/2017
    *
    *       Purpose:
    *       Store single values or arrays of data values or a collection
    *       of related information which can be accessed and modified through
    *       a simple templated interface
    **/
    class OBJX_LIB_INC Node {
    public:
        ///////////////////////////////////////////////////////////////////////////////////
        ////////----------------------Constructors/Destructor----------------------////////
        ///////////////////////////////////////////////////////////////////////////////////

        /*
            Node : Constructor - Initialise with default values
            Created: 26/07/2017
            Modified: 26/07/2017
        */
        inline Node();

        /*
            Node : Custom Constructor - Initialise the Node with starting data
            Created: 26/07/2017
            Modified: 26/07/2017

            Template T - Template type that matches one of the Objx standard types
                         or an array variation of them (E.g. xint, xint*)

            param[in] pVal - The single value or pointer to the data to copy the values of
            param[in] pCount - The number of elements in pVal if it is an array (Default 0)
        */
        template<typename T>
        inline Node(const T& pVal, const size_t& pCount = 0);

        /*
            Node : Copy Constructor - Initialise the Node by copying the values of another Node object
            Created: 26/07/2017
            Modified: 26/07/2017

            param[in] pCopy - The Node Object to copy the values of
        */
        inline Node(const Node& pCopy);

        /*
            Node : Destructor - Unload memory used by the Node object
            Created: 26/07/2017
            Modified: 26/07/2017
        */
        inline ~Node();

        ///////////////////////////////////////////////////////////////////////////////////
        ////////--------------------------Type Functions---------------------------////////
        ///////////////////////////////////////////////////////////////////////////////////

        /*
            Node : getTypeMask - Get the Type Mask of the current Node object
            Created: 26/07/2017
            Modified: 26/07/2017

            return const ETypeMask& - Returns a constant reference to the internal ETypeMask value
        */
        inline const ETypeMask& getTypeMask() const noexcept;

        /*
            Node : isType - Checks to see if the Node object is of a type through templates
            Created: 26/07/2017
            Modified: 26/07/2017

            Template T - Template type that matches one of the Objx standard types
                         or an array variation of them (E.g. xint, xint*)

            return bool - Returns true if Node's type mask matches the one generated for the supplied
                          template type
        */
        template<typename T>
        inline bool isType() const noexcept;

        /*
            Node : isType - Checks to see if the Node object is of a type through integral bitmask
            Created: 26/07/2017
            Modified: 26/07/2017

            param[in] pMask - An integral bitmask value defining the types to check against

            return bool - Returns true if Node's type mask matches the supplied bitmask
        */
        inline bool isType(const int& pMask) const noexcept;

        /*
            Node : isType - Checks to see if the Node object is of a type through an ETypeMask value
            Created: 26/07/2017
            Modified: 26/07/2017

            param[in] pMask - An ETypeMask value defining the types to check against

            return bool - Returns true if the Node's type mask matches the supplied ETypeMask
        */
        inline bool isType(const ETypeMask& pMask) const noexcept;

        /*
            Node : partType - Checks to see if the Node is at least partially of a type through templates
            Created: 26/07/2017
            Modified: 26/07/2017

            Template T - Template type that matches one of the Objx standard types
                         or an array variation of them (E.g. xint, xint*)

            return bool - Returns true if the Node's type mask partially matches the one generated for the supplied
                          template type
        */
        template<typename T>
        inline bool partType() const noexcept;

        /*
            Node : partType - Checks to see if the Node object is at least partially of a type through integral bitmask
            Created: 26/07/2017
            Modified: 26/07/2017

            param[in] pMask - An integral bitmask value defining the types to check against

            return bool - Returns true if Node's type mask partially matches the supplied bitmask
        */
        inline bool partType(const int& pMask) const noexcept;

        /*
            Node : partType - Checks to see if the Node object is at least partially of a type through an ETypeMask value
            Created: 26/07/2017
            Modified: 26/07/2017

            param[in] pMask - An ETypeMask value defining the types to check against

            return bool - Returns true if the Node's type mask partially matches the supplied ETypeMask
        */
        inline bool partType(const ETypeMask& pMask) const noexcept;

        ///////////////////////////////////////////////////////////////////////////////////
        ////////-------------------------Value Functions---------------------------////////
        ///////////////////////////////////////////////////////////////////////////////////

        /*
            Node : getVal - Return a reference to a value of a defined type that can be read and written to
            Created: 26/07/2017
            Modified: 26/07/2017

            Template T - A template that matches one of the Objx standard types, excluding arrays and containers

            return T& - Returns a reference to the data object for use

            NOTE:
            This function will erase previous information stored within the Node if required to ensure that the 
            reference retrieved is of the type requested
        */
        template<typename T>
        inline T& getVal();

        /*
            Node : readVal - Read the value stored in the Node object as a requested type
            Created: 26/07/2017
            Modified: 26/07/2017

            Template T - A template that matches one of the Objx standard types, excluding arrays and containers

            param[in] pDefault - An optional value of the requested type to be used if the Node doesn't match the requested type

            return const T - Returns a constant value of the requested type for reading
        */
        template<typename T>
        inline const T readVal(const T& pDefault = T()) const noexcept;

        /*
            Node : writeVal - Write a value to be stored in the Node object
            Created: 26/07/2017
            Modified: 07/08/2017

            Template T - A template that matches one of the Objx standard types, excluding arrays and containers

            param[in] pVal - The singular value to be written to the Node object

            return EStatusCode - Returns a status code representing the success state of the operation

            NOTE:
            This function will erase previous information stored within the Node 
        */
        template<typename T>
        inline EStatusCode writeVal(const T& pVal);

        ///////////////////////////////////////////////////////////////////////////////////
        ////////-------------------------Array Functions---------------------------////////
        ///////////////////////////////////////////////////////////////////////////////////

        /*
            Node : getArray - Return a reference to a single value within an array of a defined type that can be read and written to
            Created: 26/07/2017
            Modified: 07/08/2017

            Template T - A template that matches one of the Objx types, excluding arrays (Assumed by the function)

            param[in] pIndex - An index value defining the element in the array to retrieve

            return T& - Returns a reference to requested element in the specified type

            NOTE:
            This function will erase previous information stored within the Node if required to ensure that the
            reference retrieved is of the type requested at a valid array index
        */
        template<typename T>
        inline T& getArray(const size_t& pIndex);

        /*
            Node : readArray - Read the value stored in the Node object array as a requested type
            Created: 26/07/2017
            Modified: 26/07/2017

            Template T - A template that matches one of the Objx types, excluding arrays (Assumed by the function)

            param[in] pIndex - An index value defining the element in the array to retrieve
            param[in] pDefault - An optional value of the requested type to be used if the Node doesn't match the requested type
                                 or there are not enough elements in the array

            return const T - Returns a constant value of the requested type for reading
        */
        template<typename T>
        inline const T readArray(const size_t& pIndex, const T& pDefault = T()) const noexcept;

        /*
            Node : writeArray - Write an array of values to be stored in the Node object
            Created: 26/07/2017
            Modified: 07/08/2017

            Template T - A template that matches one of the Objx types, excluding arrays (Assumed by the function)

            param[in] pArray - An array of the Objx types to be read for input
            param[in] pCount - The number of elements that are in the supplied array

            return EStatusCode - Returns a status code representing the success state of the operation
                                 EStatusCode::Null_Data_Pointer - Supplied pointer is nullptr
                                 EStatusCode::Empty_Array_Bounds - Supplied size of array is 0

            NOTE:
            This function will erase previous information stored within the Node
        */
        template<typename T>
        inline EStatusCode writeArray(const T* pArray, const size_t& pCount);

        ///////////////////////////////////////////////////////////////////////////////////
        ////////------------------------Iterator Functions-------------------------////////
        ///////////////////////////////////////////////////////////////////////////////////

        //! Prototype the Node Iterator objects
        template<typename T, bool Strict = false> class Iterator;
        template<typename T, bool Strict = false> class cIterator;

        /*
            Node : begin - Get the Iterator for the beginning of the Node's internal array
            Created: 28/07/2017
            Modified: 28/07/2017

            Template T - A template that matches one of the Objx types, excluding arrays (Assumed by the function)
            Template Strict - A boolean flag indicating how strict the Iterator should be.
                              Strict iterators will only operate on arrays of data, non - strict
                              iterators will process single values as an array of one element

            return Iterator<T, Strict> - Returns a modifiable Iterator pointing at the start of the internal array
        */
        template<typename T, bool Strict = false>
        inline Iterator<T, Strict> begin() noexcept;

        /*
            Node : end - Get the Iterator for the end of the Node's internal array
            Created: 28/07/2017
            Modified: 28/07/2017

            Template T - A template that matches one of the Objx types, excluding arrays (Assumed by the function)
            Template Strict - A boolean flag indicating how strict the Iterator should be.
                              Strict iterators will only operate on arrays of data, non - strict
                              iterators will process single values as an array of one element

            return Iterator<T, Strict> - Returns a modifiable Iterator pointing past the end of the internal array
        */
        template<typename T, bool Strict = false>
        inline Iterator<T, Strict> end() noexcept;

        /*
            Node : cbegin - Get the cIterator for the beginning of the Node's internal array
            Created: 28/07/2017
            Modified: 28/07/2017

            Template T - A template that matches one of the Objx types, excluding arrays (Assumed by the function)
            Template Strict - A boolean flag indicating how strict the cIterator should be.
                              Strict iterators will only operate on arrays of data, non - strict
                              iterators will process single values as an array of one element

            return cIterator<T, Strict> - Returns a constant Iterator pointing at the start of the internal array
        */
        template<typename T, bool Strict = false>
        inline cIterator<T, Strict> cbegin() const noexcept;

        /*
            Node : cend - Get the cIterator for the end of the Node's internal array
            Created: 28/07/2017
            Modified: 28/07/2017

            Template T - A template that matches one of the Objx types, excluding arrays (Assumed by the function)
            Template Strict - A boolean flag indicating how strict the cIterator should be.
                              Strict iterators will only operate on arrays of data, non - strict
                              iterators will process single values as an array of one element

            return cIterator<T, Strict> - Returns a constant Iterator pointing past the end of the internal array
        */
        template<typename T, bool Strict = false>
        inline cIterator<T, Strict> cend() const noexcept;

        ///////////////////////////////////////////////////////////////////////////////////
        ////////------------------------Property Functions-------------------------////////
        ///////////////////////////////////////////////////////////////////////////////////
        
        /*
            Node : hasProperty - Checks to see if the Node object has a specific property
            Created: 26/07/2017
            Modified: 26/07/2017

            param[in] pKey - The name of the property value to retrieve

            return bool - Returns true if the specified property exists
        */
        bool hasProperty(const xstring& pKey) const noexcept;

        /*
            Node : getProp - Return a reference to a property of the current Node object that can read from and written to
            Created: 26/07/2017
            Modified: 26/07/2017

            param[in] pKey - The name of the property value to retrieve

            return Node& - Returns a reference to the requested property

            NOTE:
            This function will erase previous information stored within the Node if required to ensure that the
            reference retrieved is of a Node property
        */
        Node& getProp(const xstring& pKey);

        /*
            Node : readProp - Read the value of a Node stored within the current Node object
            Created: 26/07/2017
            Modified: 26/07/2017

            param[in] pKey - The name of the property value to retrieve
            param[in] pDefault - An optional default Node object to use if the key value does not exist

            return const Node - Returns a constant Node object that can be read from
        */
        const Node readProp(const xstring& pKey, const Node& pDefault = Node()) const noexcept;

        /*
            Node : writeProp - Write a value(s) to a named property inside the current Node object
            Created: 26/07/2017
            Modified: 09/08/2017

            Template T - Template type that matches one of the Objx standard types
                         or an array variation of them (E.g. xint, xint*)

            param[in] pKey - The name of the property value to retrieve
            param[in] pVal - The single value or pointer to the data to copy the values of
            param[in] pCount - The number of elements in pVal if it is an array (Default 0)

            return EStatusCode - Returns a status code representing the success state of the operation
                                 EStatusCode::Null_Data_Pointer - Supplied pointer is nullptr
                                 EStatusCode::Empty_Array_Bounds - Supplied size of array is 0
                                 EStatusCode::Empty_Data_Object - Created Node object was empty and/or unable to be read

            NOTE:
            This function will erase previous information if required to store the new property
        */
        template<typename T>
        inline EStatusCode writeProp(const xstring& pKey, const T& pVal, const size_t& pCount = 0);

        /*
            Node : eraseProp - Remove a named property from within the current Node object
            Created: 31/07/2017
            Modified: 07/08/2017

            param[in] pKey - The name of the property to remove

            return EStatusCode - Returns a status code representing the success state of the operation
                                 EStatusCode::Non_Objx_Node - Node object is not an Objx type, it has no properties
        */
        EStatusCode eraseProp(const xstring& pKey) noexcept;

        /*
            Node : Subscript Operator (Non-const) - Get a reference to a modifiable Node property object
            Created: 26/07/2017
            Modified: 26/07/2017

            param[in] pKey - The name of the property value to retrieve

            return Node& - Returns a reference to the requested property

            NOTE:
            This function will erase previous information stored within the Node if required to ensure that the
            reference retrieved is of a Node property
        */
        inline Node& operator[](const xstring& pKey);

        /*
            Node : Subscript Operator (Const) - Read the value of a Node stored within the current Node object
            Created: 26/07/2017
            Modified: 26/07/2017

            param[in] pKey - The name of the property value to retrieve

            return const Node - Returns a constant Node object that can be read from
        */
        inline const Node operator[](const xstring& pKey) const noexcept;

        /*
            Node : forEachProp - Iterate over all properties stored within the Node object
            Created: 28/07/2017
            Modified: 05/10/2017

            param[in] pCB - A callback to be used to allow for the processing of all properties of the Node

            return size_t - Returns the number of properties processed
        */
        size_t forEachProp(const ForEachCB& pCB);

        /*
            Node : forEachProp - Iterate over all properties stored within the Node object to allow reading of data
            Created: 28/07/2017
            Modified: 05/10/2017

            param[in] pCB - A callback to be used to allow for the processing of all properties of the Node

            return size_t - Returns the number of properties processed
        */
        size_t forEachProp(const cForEachCB& pCB) const noexcept;

        ///////////////////////////////////////////////////////////////////////////////////
        ////////-----------------------Assignment Functions------------------------////////
        ///////////////////////////////////////////////////////////////////////////////////

        /*
            Node : Assignment Operator - Assign the current Node the values of another
            Created: 03/05/2017
            Modified: 07/08/2017

            param[in] pCopy - The Node Object to copy the values of

            return EStatusCode - Returns a status code representing the success state of the operation

            NOTE:
            This function will erase previous information stored within the Node
        */
        EStatusCode operator=(const Node& pCopy);

        /*
            Node : set - Assign the current Node object specific data 
            Created: 26/07/2017
            Modified: 07/08/2017

            Template T - Template type that matches one of the Objx standard types
                         or an array variation of them (E.g. xint, xint*)

            param[in] pVal - The single value or pointer to the data to copy the values of
            param[in] pCount - The number of elements in pVal if it is an array (Default 0)

            return EStatusCode - Returns a status code representing the success state of the operation
                                 EStatusCode::Null_Data_Pointer - Supplied pointer is nullptr
                                 EStatusCode::Empty_Array_Bounds - Supplied size of array is 0

            NOTE:
            This function will erase previous information stored within the Node
        */
        template<typename T>
        inline EStatusCode set(const T& pVal, const size_t& pCount = 0);

        ///////////////////////////////////////////////////////////////////////////////////
        ////////--------------------------Data Functions---------------------------////////
        ///////////////////////////////////////////////////////////////////////////////////
        
        /*
            Node : Equality Operator - Compare two Node objects to determine if they are equal
            Created: 07/08/2017
            Modified: 07/08/2017

            param[in] pRight - The Node object to compare against

            return bool - Returns true if the Node objects contain the same values
        */
        bool operator==(const Node& pRight) const noexcept;

        /*
            Node : Inequality Operator - Compare two Node objects to determine if they are unequal
            Created: 07/08/2017
            Modified: 07/08/2017

            param[in] pRight - The Node object to compare against

            return bool - Returns true if the two Node objects are different
        */
        inline bool operator!=(const Node& pRight) const noexcept;

        ///////////////////////////////////////////////////////////////////////////////////
        ////////--------------------------Data Functions---------------------------////////
        ///////////////////////////////////////////////////////////////////////////////////
        
        /*
            Node : size - Get the number of elements contained within the Node
            Created: 26/07/2017
            Modified: 28/07/2017

            return const size_t - Returns a constant size_t value containing the size information

            NOTE:
            Depending on the type of Node this number will mean different things. Refer to the 
            following list:

            ----TYPE----|------------------------------MEANING------------------------------
            Null            This number will be 0 as there is no information stored
            Value           This number will be 1 as there is exactly one value stored
            Array           This number will represent the total number of elements in the array
            Objx            This number will represent the number of properties the Node has
        */
        const size_t size() const noexcept;

        /*
            Node : data - Retrieve a constant copy of the pure data inside the Node object of the specified type
            Created: 26/07/2017
            Modified: 27/07/2017

            Template T - Template type that matches one of the Objx standard types
                         or an array variation of them (E.g. xint, xint*), exluding containers

            param[in] pDefault - An optional value that will be returned if the Node does not match the template type

            return const T - Returns a constant value of the type requested
        */
        template<typename T>
        const T data(const T& pDefault = T()) const noexcept;

        /*
            Node : freeData - Reset the Node object to default, releasing all memory
            Created: 03/05/2017
            Modified: 26/07/2017
        */
        void freeData();

    private:
        ///////////////////////////////////////////////////////////////////////////////////
        ////////---------------------------Internal Data---------------------------////////
        ///////////////////////////////////////////////////////////////////////////////////

        //! Store the internal data used by the object
        xdata mData;

        //! Store the number of array elements within this object
        size_t mSize;

        //! Store a bitmask of the current type of the Node object
        ETypeMask mType;

        ///////////////////////////////////////////////////////////////////////////////////
        ////////-------------------------Private Functions-------------------------////////
        ///////////////////////////////////////////////////////////////////////////////////

        //! Store information within the current Node object
        EStatusCode storeValues(const ETypeMask& pMask, const xdata pPointer, const size_t& pCount = 0);
        EStatusCode storeProp(const xstring& pKey, const Node& pNode);

        //! Load specific information from within the current Node object
        xdata loadValues(const ETypeMask& pMask, const size_t& pIndex = 0);
    };

    /*
     *      Name: Iterator (Node)
     *      Author: Mitchell Croft
     *      Created: 28/07/2017
     *      Modified: 28/07/2017
     *      
     *      Template T - Template type that matches one of the Objx standard types
     *                   (E.g. xint, xdec, xbool, xstring, xobjx)
     *      Template Strict - Used to flag Iterators constructed with different parameters
     *                        to prevent the comparison of Iterators constructed with different
     *                        levels of strictness
     *                        
     *      Purpose:
     *      Provide iterative access to array elements within a Node object with read
     *      and write access
    **/
    template<typename T, bool Strict /*= false*/>
    class Node::Iterator {
    public:
        //! Assert correct value access
        static_assert(!Implementation::templateToTypeMask_impl<T>::invalid, "Supplied type is not an Objx compatible type. Cannot use non-Objx types with this library implementation. Please use an array variation of the Objx base types (E.g. Objx::xint*, Objx::xdec*, Objx::xbool*, Objx::xstring*, Objx::xobjx*)");
        static_assert(!PointerAnalysis<T>::depth, "Node:Iterator assumes array type already, it does not require the array specifier '*' in the template parameter");

        ///////////////////////////////////////////////////////////////////////////////////
        ////////------------------------Default Constructor------------------------////////
        ///////////////////////////////////////////////////////////////////////////////////
        
        /*
            Iterator : Default Constructor - Initialise with default values 
            Created: 28/07/2017
            Modified: 28/07/2017
        */
        inline Iterator();

        ///////////////////////////////////////////////////////////////////////////////////
        ////////--------------------------Access Operators-------------------------////////
        ///////////////////////////////////////////////////////////////////////////////////
        
        /*
            Iterator : Arrow Operator - Provide direct access to the internal pointer
            Created: 28/07/2017
            Modified: 28/07/2017

            return T*& - Returns a reference to the internal pointer 
        */
        inline T*& operator->();

        /*
            Iterator : Dereference Operator - Provide a direct reference to the indicated data
            Created: 28/07/2017
            Modified: 28/07/2017

            return T& - Returns a modifiable reference to the data at the internal pointers location
        */
        inline T& operator*();

        ///////////////////////////////////////////////////////////////////////////////////
        ////////-------------------------Logical Operators-------------------------////////
        ///////////////////////////////////////////////////////////////////////////////////
        
        /*
            Iterator : Equality Operator - Determines if two Iterators are equal
            Created: 28/07/2017
            Modified: 28/07/2017

            param[in] pRight - The Iterator to be compared against

            return bool - Returns true if both iterators are pointing at the same memory location
        */
        inline bool operator==(const Iterator& pRight) const noexcept;

        /*
            Iterator : Inequality Operator - Determines if two Iterators are unequal
            Created: 28/07/2017
            Modified: 28/07/2017

            param[in] pRight - The Iterator to be compared against

            return bool - Returns true if both iterators are pointing at different locations in memory
        */
        inline bool operator!=(const Iterator& pRight) const noexcept;

        ///////////////////////////////////////////////////////////////////////////////////
        ////////----------------------Progressional Operators----------------------////////
        ///////////////////////////////////////////////////////////////////////////////////
        
        /*
            Iterator : Pre-Increment Operator - Increment the location that the Iterator is pointing to
            Created: 28/07/2017
            Modified: 28/07/2017

            return Iterator& - Returns a reference to itself
        */
        inline Iterator& operator++() noexcept;

        /*
            Iterator : Post-Increment Operator - Increment the location that the Iterator is pointing to
            Created: 28/07/2017
            Modified: 28/07/2017

            return Iterator - Returns an Iterator object holding the values prior to modification
        */  
        inline Iterator operator++(int) noexcept;

        /*
            Iterator - Pre-Decrement Operator - Decrement the location that the Iterator is pointing to
            Created: 28/07/2017
            Modified: 28/07/2017

            return Iterator& - Returns a reference to itself
        */
        inline Iterator& operator--() noexcept;

        /*
            Iterator - Post-Decrement Operator - Decrement the location that the Iterator is pointing to
            Created: 28/07/2017
            Modified: 28/07/2017

            return Iterator - Returns an iterator object holding the values prior to modification
        */
        inline Iterator operator--(int) noexcept;

    private:
        //! Assign as a friend of the Node object to allow for construction 
        friend class Node;

        //! Store a pointer to the data that is being made accessible
        T* mPointer;

        ///////////////////////////////////////////////////////////////////////////////////
        ////////-------------------------Custom Constructor------------------------////////
        ///////////////////////////////////////////////////////////////////////////////////
        
        /*
            Iterator : Custom Constructor - Initialise the iterator object pointing at usable memory
            Created: 28/07/2017
            Modified: 28/07/2017

            param[in] pMemory - A pointer to memory managed by an Objx::Node object
        */
        inline Iterator(T* pMemory);
    };

    /*
    *       Name: cIterator (Node)
    *       Author: Mitchell Croft
    *       Created: 28/07/2017
    *       Modified: 28/07/2017
    *
    *       Template T - Template type that matches one of the Objx standard types
    *                    (E.g. xint, xdec, xbool, xstring, xobjx)
    *       Template Strict - Used to flag Iterators constructed with different parameters
    *                         to prevent the comparison of Iterators constructed with different
    *                         levels of strictness
    *
    *       Purpose:
    *       Provide iterative access to array elements within a Node object with read
    *       access
    **/
    template<typename T, bool Strict /*= false*/>
    class Node::cIterator {
    public:
        //! Assert correct value access
        static_assert(!Implementation::templateToTypeMask_impl<T>::invalid, "Supplied type is not an Objx compatible type. Cannot use non-Objx types with this library implementation. Please use an array variation of the Objx base types (E.g. Objx::xint*, Objx::xdec*, Objx::xbool*, Objx::xstring*, Objx::xobjx*)");
        static_assert(!PointerAnalysis<T>::depth, "Node:cIterator assumes array type already, it does not require the array specifier '*' in the template parameter");

        ///////////////////////////////////////////////////////////////////////////////////
        ////////------------------------Default Constructor------------------------////////
        ///////////////////////////////////////////////////////////////////////////////////

        /*
            cIterator : Default Constructor - Initialise with default values
            Created: 28/07/2017
            Modified: 28/07/2017
        */
        inline cIterator();

        ///////////////////////////////////////////////////////////////////////////////////
        ////////--------------------------Access Operators-------------------------////////
        ///////////////////////////////////////////////////////////////////////////////////

        /*
            cIterator : Arrow Operator - Provide direct access to the internal pointer
            Created: 28/07/2017
            Modified: 28/07/2017

            return const T*& - Returns a constant reference to the internal pointer
        */
        inline const T*& operator->() const noexcept;

        /*
            cIterator : Dereference Operator - Provide a direct reference to the indicated data
            Created: 28/07/2017
            Modified: 28/07/2017

            return const T& - Returns a constant reference to the data at the internal pointers location
        */
        inline const T& operator*() const noexcept;

        ///////////////////////////////////////////////////////////////////////////////////
        ////////-------------------------Logical Operators-------------------------////////
        ///////////////////////////////////////////////////////////////////////////////////

        /*
            cIterator : Equality Operator - Determines if two cIterators are equal
            Created: 28/07/2017
            Modified: 28/07/2017

            param[in] pRight - The cIterator to be compared against

            return bool - Returns true if both cIterators are pointing at the same memory location
        */
        inline bool operator==(const cIterator& pRight) const noexcept;

        /*
            cIterator : Inequality Operator - Determines if two cIterators are unequal
            Created: 28/07/2017
            Modified: 28/07/2017

            param[in] pRight - The cIterator to be compared against

            return bool - Returns true if both cIterators are pointing at different locations in memory
        */
        inline bool operator!=(const cIterator& pRight) const noexcept;

        ///////////////////////////////////////////////////////////////////////////////////
        ////////----------------------Progressional Operators----------------------////////
        ///////////////////////////////////////////////////////////////////////////////////

        /*
            cIterator : Pre-Increment Operator - Increment the location that the cIterator is pointing to
            Created: 28/07/2017
            Modified: 28/07/2017

            return cIterator& - Returns a reference to itself
        */
        inline cIterator& operator++() noexcept;

        /*
            cIterator : Post-Increment Operator - Increment the location that the cIterator is pointing to
            Created: 28/07/2017
            Modified: 28/07/2017

            return cIterator - Returns an cIterator object holding the values prior to modification
        */
        inline cIterator operator++(int) noexcept;

        /*
            cIterator - Pre-Decrement Operator - Decrement the location that the cIterator is pointing to
            Created: 28/07/2017
            Modified: 28/07/2017

            return cIterator& - Returns a reference to itself
        */
        inline cIterator& operator--() noexcept;

        /*
            cIterator - Post-Decrement Operator - Decrement the location that the cIterator is pointing to
            Created: 28/07/2017
            Modified: 28/07/2017

            return cIterator - Returns an cIterator object holding the values prior to modification
        */
        inline cIterator operator--(int) noexcept;

    private:
        //! Assign as a friend of the Node object to allow for construction 
        friend class Node;

        //! Store a pointer to the data that is being made accessible
        const T* mPointer;

        ///////////////////////////////////////////////////////////////////////////////////
        ////////-------------------------Custom Constructor------------------------////////
        ///////////////////////////////////////////////////////////////////////////////////

        /*
            cIterator : Custom Constructor - Initialise the cIterator object pointing at usable memory
            Created: 28/07/2017
            Modified: 28/07/2017

            param[in] pMemory - A constant pointer to memory managed by an Objx::Node object
        */
        inline cIterator(const T* pMemory);
    };

    #pragma region Function Definitions
    #pragma region Node Definitions
    #pragma region Constructors / Destructors
    /*
        Node : Constructor - Initialise with default values
        Created: 26/07/2017
        Modified: 26/07/2017
    */
    inline Node::Node() : mData(nullptr), mSize(0), mType(ETypeMask::TYPE_NULL) {}

    /*
        Node : Custom Constructor - Initialise the Node with starting data
        Created: 26/07/2017
        Modified: 26/07/2017

        Template T - Template type that matches one of the Objx standard types
                     or an array variation of them (E.g. xint, xint*)

        param[in] pVal - The single value or pointer to the data to copy the values of
        param[in] pCount - The number of elements in pVal if it is an array (Default 0)
    */
    template<typename T>
    inline Node::Node(const T& pVal, const size_t& pCount /*= 0*/) : mData(nullptr), mSize(0), mType(ETypeMask::TYPE_NULL) { set(pVal, pCount); }

    /*
        Node : Copy Constructor - Initialise the Node by copying the values of another Node object
        Created: 26/07/2017
        Modified: 26/07/2017

        param[in] pCopy - The Node Object to copy the values of
    */
    inline Node::Node(const Node& pCopy) : mData(nullptr), mSize(0), mType(ETypeMask::TYPE_NULL) { *this = pCopy; }

    /*
        Node : Destructor - Unload memory used by the Node object
        Created: 26/07/2017
        Modified: 26/07/2017
    */
    inline Node::~Node() { freeData(); }
    #pragma endregion
    #pragma region Type Functions
    /*
        Node : getTypeMask - Get the Type Mask of the current Node object
        Created: 26/07/2017
        Modified: 26/07/2017

        return const ETypeMask& - Returns a constant reference to the internal ETypeMask value
    */
    inline const ETypeMask& Node::getTypeMask() const noexcept { return mType; }

    /*
        Node : isType - Checks to see if the Node object is of a type through templates
        Created: 26/07/2017
        Modified: 26/07/2017

        Template T - Template type that matches one of the Objx standard types
                     or an array variation of them (E.g. xint, xint*)

        return bool - Returns true if Node's type mask matches the one generated for the supplied
                      template type
    */
    template<typename T>
    inline bool Node::isType() const noexcept { return isType(templateToTypeMask<T>()); }

    /*
        Node : isType - Checks to see if the Node object is of a type through integral bitmask
        Created: 26/07/2017
        Modified: 26/07/2017

        param[in] pMask - An integral bitmask value defining the types to check against

        return bool - Returns true if Node's type mask matches the supplied bitmask
    */
    inline bool Node::isType(const int& pMask) const noexcept { return isType((ETypeMask)pMask); }

    /*
        Node : isType - Checks to see if the Node object is of a type through an ETypeMask value
        Created: 26/07/2017
        Modified: 26/07/2017

        param[in] pMask - An ETypeMask value defining the types to check against

        return bool - Returns true if the Node's type mask matches the supplied ETypeMask
    */
    inline bool Node::isType(const ETypeMask& pMask) const noexcept { return (mType == pMask); }

    /*
        Node : partType - Checks to see if the Node is at least partially of a type through templates
        Created: 26/07/2017
        Modified: 26/07/2017

        Template T - Template type that matches one of the Objx standard types
                     or an array variation of them (E.g. xint, xint*)

        return bool - Returns true if the Node's type mask partially matches the one generated for the supplied
                      template type
    */
    template<typename T>
    inline bool Node::partType() const noexcept { return partType(templateToTypeMask<T>()); }

    /*
        Node : partType - Checks to see if the Node object is at least partially of a type through integral bitmask
        Created: 26/07/2017
        Modified: 26/07/2017

        param[in] pMask - An integral bitmask value defining the types to check against

        return bool - Returns true if Node's type mask partially matches the supplied bitmask
    */
    inline bool Node::partType(const int& pMask) const noexcept { return partType((ETypeMask)pMask); }

    /*
        Node : partType - Checks to see if the Node object is at least partially of a type through an ETypeMask value
        Created: 26/07/2017
        Modified: 26/07/2017

        param[in] pMask - An ETypeMask value defining the types to check against

        return bool - Returns true if the Node's type mask partially matches the supplied ETypeMask
    */
    inline bool Node::partType(const ETypeMask& pMask) const noexcept { return ((mType & pMask) == pMask); }
    #pragma endregion
    #pragma region Value Functions
    /*
        Node : getVal - Return a reference to a value of a defined type that can be read and written to
        Created: 26/07/2017
        Modified: 26/07/2017

        Template T - A template that matches one of the Objx standard types, excluding arrays and containers

        return T& - Returns a reference to the data object for use

        NOTE:
        This function will erase previous information stored within the Node if required to ensure that the
        reference retrieved is of the type requested
    */
    template<typename T>
    inline T& Node::getVal() {
        static_assert(!PointerAnalysis<T>::depth, "Can not access array values from the Node::getVal function. Use Node::getArray instead");
        static_assert(!std::is_same<Node, T>::value, "Can not access property values from the Node::getVal function. Use Node::getProp instead");
        return *(T*)loadValues(templateToTypeMask<T>());
    }

    /*
        Node : readVal - Read the value stored in the Node object as a requested type
        Created: 26/07/2017
        Modified: 26/07/2017

        Template T - A template that matches one of the Objx standard types, excluding arrays and containers

        param[in] pDefault - An optional value of the requested type to be used if the Node doesn't match the requested type

        return const T - Returns a constant value of the requested type for reading
    */
    template<typename T>
    inline const T Node::readVal(const T& pDefault /*= T()*/) const noexcept {
        static_assert(!PointerAnalysis<T>::depth, "Can not read array values from the Node::readVal function. Use Node::readArray instead");
        static_assert(!std::is_same<Node, T>::value, "Can not read property values from the Node::readVal function. Use Node::readProp instead");
        return (isType<T>() ? *(T*)mData : pDefault);
    }

    /*
        Node : writeVal - Write a value to be stored in the Node object
        Created: 26/07/2017
        Modified: 07/08/2017

        Template T - A template that matches one of the Objx standard types, excluding arrays and containers

        param[in] pVal - The singular value to be written to the Node object

        return EStatusCode - Returns a status code representing the success state of the operation

        NOTE:
        This function will erase previous information stored within the Node
    */
    template<typename T>
    inline EStatusCode Node::writeVal(const T& pVal) {
        static_assert(!PointerAnalysis<T>::depth, "Can not write array values through the Node::writeVal function. Use Node::writeArray instead");
        static_assert(!std::is_same<Node, T>::value, "Can not write property values through the Node::writeVal function. Use Node::writeProp instead");
        return storeValues(templateToTypeMask<T>(), (xdata)&pVal);
    }
    #pragma endregion
    #pragma region Array Functions
    /*
        Node : getArray - Return a reference to a single value within an array of a defined type that can be read and written to
        Created: 26/07/2017
        Modified: 07/08/2017

        Template T - A template that matches one of the Objx types, excluding arrays (Assumed by the function)

        param[in] pIndex - An index value defining the element in the array to retrieve

        return T& - Returns a reference to requested element in the specified type

        NOTE:
        This function will erase previous information stored within the Node if required to ensure that the
        reference retrieved is of the type requested at a valid array index
    */
    template<typename T>
    inline T& Node::getArray(const size_t& pIndex) {
        static_assert(!PointerAnalysis<T>::depth, "Node::getArray assumes array type already, it does not require the array specifier. E.g. getArray<xint*> would become getArray<xint>");
        return *(T*)loadValues(templateToTypeMask<T*>(), pIndex);
    }

    /*
        Node : readArray - Read the value stored in the Node object array as a requested type
        Created: 26/07/2017
        Modified: 26/07/2017

        Template T - A template that matches one of the Objx types, excluding arrays (Assumed by the function)

        param[in] pIndex - An index value defining the element in the array to retrieve
        param[in] pDefault - An optional value of the requested type to be used if the Node doesn't match the requested type
                             or there are not enough elements in the array

        return const T - Returns a constant value of the requested type for reading
    */
    template<typename T>
    inline const T Node::readArray(const size_t& pIndex, const T& pDefault /*= T()*/) const noexcept {
        static_assert(!PointerAnalysis<T>::depth, "Node::readArray assumes array type already, it does not require the array specifier. E.g. readArray<xint*> would become readArray<xint>");
        return (isType<T*>() && pIndex < mSize ? *((T*)mData + pIndex) : pDefault);
    }

    /*
        Node : writeArray - Write an array of values to be stored in the Node object
        Created: 26/07/2017
        Modified: 07/08/2017

        Template T - A template that matches one of the Objx types, excluding arrays (Assumed by the function)

        param[in] pArray - An array of the Objx types to be read for input
        param[in] pCount - The number of elements that are in the supplied array

        return EStatusCode - Returns a status code representing the success state of the operation
                             EStatusCode::Null_Data_Pointer - Supplied pointer is nullptr
                             EStatusCode::Empty_Array_Bounds - Supplied size of array is 0

        NOTE:
        This function will erase previous information stored within the Node
    */
    template<typename T>
    inline EStatusCode Node::writeArray(const T* pArray, const size_t& pCount) {
        static_assert(!PointerAnalysis<T>::depth, "Node:writeArray assumes array type already, it does not require the array specifier. Array type is determined by parameter value");
        return storeValues(templateToTypeMask<T*>(), (xdata)pArray, pCount);
    }
    #pragma endregion
    #pragma region Iterator Functions
    /*
        Node : begin - Get the Iterator for the beginning of the Node's internal array
        Created: 28/07/2017
        Modified: 28/07/2017

        Template T - A template that matches one of the Objx types, excluding arrays (Assumed by the function)
        Template Strict - A boolean flag indicating how strict the Iterator should be.
                          Strict iterators will only operate on arrays of data, non - strict
                          iterators will process single values as an array of one element

        return Iterator<T, Strict> - Returns a modifiable Iterator pointing at the start of the internal array
    */
    template<typename T, bool Strict /*= false*/>
    inline Node::Iterator<T, Strict> Node::begin() noexcept { return Iterator<T, Strict>((T*)mData); }

    /*
        Node : end - Get the Iterator for the end of the Node's internal array
        Created: 28/07/2017
        Modified: 28/07/2017

        Template T - A template that matches one of the Objx types, excluding arrays (Assumed by the function)
        Template Strict - A boolean flag indicating how strict the Iterator should be.
                          Strict iterators will only operate on arrays of data, non - strict
                          iterators will process single values as an array of one element

        return Iterator<T, Strict> - Returns a modifiable Iterator pointing past the end of the internal array
    */
    template<typename T, bool Strict /*= false*/>
    inline Node::Iterator<T, Strict> Node::end() noexcept {
        //Check if the requested type is a match with the Node's type
        if (isType<T*>() || (partType<T>() && !Strict)) 
            return Iterator<T, Strict>((T*)mData + mSize);

        //Otherwise return the start of the array
        else return Iterator<T, Strict>((T*)mData);
    }

    /*
        Node : cbegin - Get the cIterator for the beginning of the Node's internal array
        Created: 28/07/2017
        Modified: 28/07/2017

        Template T - A template that matches one of the Objx types, excluding arrays (Assumed by the function)
        Template Strict - A boolean flag indicating how strict the cIterator should be.
                          Strict iterators will only operate on arrays of data, non - strict
                          iterators will process single values as an array of one element

        return cIterator<T, Strict> - Returns a constant Iterator pointing at the start of the internal array
    */
    template<typename T, bool Strict /*= false*/>
    inline Node::cIterator<T, Strict> Node::cbegin() const noexcept { return cIterator<T, Strict>((T*)mData); }

    /*
        Node : cend - Get the cIterator for the end of the Node's internal array
        Created: 28/07/2017
        Modified: 28/07/2017

        Template T - A template that matches one of the Objx types, excluding arrays (Assumed by the function)
        Template Strict - A boolean flag indicating how strict the cIterator should be.
                          Strict iterators will only operate on arrays of data, non - strict
                          iterators will process single values as an array of one element

        return cIterator<T, Strict> - Returns a constant Iterator pointing past the end of the internal array
    */
    template<typename T, bool Strict /*= false*/>
    inline Node::cIterator<T, Strict> Node::cend() const noexcept {
        //Check if the requested type is a match with the Node's type
        if (isType<T*>() || (partType<T>() && !Strict))
            return cIterator<T, Strict>((const T*)mData + mSize);

        //Otherwise return the start of the array
        else return cIterator<T, Strict>((const T*)mData);
    }
    #pragma endregion
    #pragma region Property Functions
    /*
        Node : writeProp - Write a value(s) to a named property inside the current Node object
        Created: 26/07/2017
        Modified: 09/08/2017

        Template T - Template type that matches one of the Objx standard types
                     or an array variation of them (E.g. xint, xint*)

        param[in] pKey - The name of the property value to retrieve
        param[in] pVal - The single value or pointer to the data to copy the values of
        param[in] pCount - The number of elements in pVal if it is an array (Default 0)

        return EStatusCode - Returns a status code representing the success state of the operation
                             EStatusCode::Null_Data_Pointer - Supplied pointer is nullptr
                             EStatusCode::Empty_Array_Bounds - Supplied size of array is 0
                             EStatusCode::Empty_Data_Object - Created Node object was empty and/or unable to be read

        NOTE:
        This function will erase previous information if required to store the new property
    */
    template<typename T>
    inline EStatusCode Node::writeProp(const xstring& pKey, const T& pVal, const size_t& pCount /*= 0*/) { 
        //Create a Node to hold the data
        Node temp;

        //Assign the data to the Node
        EStatusCode res = temp.set(pVal, pCount);

        //Check the operation was success
        if (res != EStatusCode::Success) return res;

        //Store the data
        return storeProp(pKey, temp); 
    }

    /*
        Node : Subscript Operator (Non-const) - Get a reference to a modifiable Node property object
        Created: 26/07/2017
        Modified: 26/07/2017

        param[in] pKey - The name of the property value to retrieve

        return Node& - Returns a reference to the requested property

        NOTE:
        This function will erase previous information stored within the Node if required to ensure that the
        reference retrieved is of a Node property
    */
    inline Node& Node::operator[](const xstring& pKey) { return getProp(pKey); }

    /*
        Node : Subscript Operator (Const) - Read the value of a Node stored within the current Node object
        Created: 26/07/2017
        Modified: 26/07/2017

        param[in] pKey - The name of the property value to retrieve

        return const Node - Returns a constant Node object that can be read from
    */
    inline const Node Node::operator[](const xstring& pKey) const noexcept { return readProp(pKey); }
    #pragma endregion
    #pragma region Misc. Functions
    /*
        Node : set - Assign the current Node object specific data
        Created: 26/07/2017
        Modified: 07/08/2017

        Template T - Template type that matches one of the Objx standard types
                     or an array variation of them (E.g. xint, xint*)

        param[in] pVal - The single value or pointer to the data to copy the values of
        param[in] pCount - The number of elements in pVal if it is an array (Default 0)

        return EStatusCode - Returns a status code representing the success state of the operation
                             EStatusCode::Null_Data_Pointer - Supplied pointer is nullptr
                             EStatusCode::Empty_Array_Bounds - Supplied size of array is 0

        NOTE:
        This function will erase previous information stored within the Node
    */
    template<typename T>
    inline EStatusCode Node::set(const T& pVal, const size_t& pCount /*= 0*/) {
        //Get the type mask for the template type
        ETypeMask mask = templateToTypeMask<T>();

        //Delegate assignment operations
        if (mask & ETypeMask::TYPE_ARRAY) return storeValues(mask, *(xdata*)&pVal, pCount);
        else if (mask & ETypeMask::TYPE_OBJX) return (*this = *((Node*)&pVal));
        else return storeValues(mask, (xdata)&pVal);
    }

    /*
        Node : Inequality Operator - Compare two Node objects to determine if they are unequal
        Created: 07/08/2017
        Modified: 07/08/2017

        param[in] pRight - The Node object to compare against

        return bool - Returns true if the two Node objects are different
    */
    inline bool Node::operator!=(const Node& pRight) const noexcept { return !(*this == pRight); }

    /*
        Node : data - Retrieve a constant copy of the pure data inside the Node object of the specified type
        Created: 26/07/2017
        Modified: 27/07/2017

        Template T - Template type that matches one of the Objx standard types
                     or an array variation of them (E.g. xint, xint*), exluding containers

        param[in] pDefault - An optional value that will be returned if the Node does not match the template type

        return const T - Returns a constant value of the type requested
    */
    template<typename T>
    const T Node::data(const T& pDefault /*= T()*/) const noexcept {
        static_assert(!std::is_same<Node, T>::value, "Objx property data can not be accessed through the Node::data function.");
        return (!isType<T>() ? pDefault : (partType(ETypeMask::TYPE_ARRAY) ? *(T*)&mData : *(T*)mData));
    }
    #pragma endregion
    #pragma endregion

    #pragma region Iterator Definitions
    #pragma region Constructors 
    /*
        Iterator : Default Constructor - Initialise with default values
        Created: 28/07/2017
        Modified: 28/07/2017
    */
    template<typename T, bool Strict>
    inline Node::Iterator<T, Strict>::Iterator() : mPointer(nullptr) {}

    /*
        Iterator : Custom Constructor - Initialise the iterator object pointing at usable memory
        Created: 28/07/2017
        Modified: 28/07/2017

        param[in] pMemory - A pointer to memory managed by an Objx::Node object
    */
    template<typename T, bool Strict>
    inline Node::Iterator<T, Strict>::Iterator(T* pMemory) : mPointer(pMemory) {}
    #pragma endregion
    #pragma region Access Operators
    /*
        Iterator : Arrow Operator - Provide direct access to the internal pointer
        Created: 28/07/2017
        Modified: 28/07/2017

        return T*& - Returns a reference to the internal pointer
    */
    template<typename T, bool Strict>
    inline T*& Node::Iterator<T, Strict>::operator->() { return mPointer; }

    /*
        Iterator : Dereference Operator - Provide a direct reference to the indicated data
        Created: 28/07/2017
        Modified: 28/07/2017

        return T& - Returns a modifiable reference to the data at the internal pointers location
    */
    template<typename T, bool Strict>
    inline T& Node::Iterator<T, Strict>::operator*() { return *mPointer; }
    #pragma endregion
    #pragma region Logical Operators
    /*
        Iterator : Equality Operator - Determines if two Iterators are equal
        Created: 28/07/2017
        Modified: 28/07/2017

        param[in] pRight - The Iterator to be compared against

        return bool - Returns true if both iterators are pointing at the same memory location
    */
    template<typename T, bool Strict>
    inline bool Node::Iterator<T, Strict>::operator==(const Iterator& pRight) const noexcept { return (mPointer == pRight.mPointer); }

    /*
        Iterator : Inequality Operator - Determines if two Iterators are unequal
        Created: 28/07/2017
        Modified: 28/07/2017

        param[in] pRight - The Iterator to be compared against

        return bool - Returns true if both iterators are pointing at different locations in memory
    */
    template<typename T, bool Strict>
    inline bool Node::Iterator<T, Strict>::operator!=(const Iterator& pRight) const noexcept { return (mPointer != pRight.mPointer); }
    #pragma endregion
    #pragma region Progressional Operators
    /*
        Iterator : Pre-Increment Operator - Increment the location that the iterator is pointing to
        Created: 28/07/2017
        Modified: 28/07/2017

        return Iterator& - Returns a reference to itself
    */
    template<typename T, bool Strict>
    inline Node::Iterator<T, Strict>& Node::Iterator<T, Strict>::operator++() noexcept { ++mPointer; return *this; }

    /*
        Iterator : Post-Increment Operator - Increment the location that the Iterator is pointing to
        Created: 28/07/2017
        Modified: 28/07/2017

        return Iterator - Returns an iterator object holding the values prior to modification
    */
    template<typename T, bool Strict>
    inline Node::Iterator<T, Strict> Node::Iterator<T, Strict>::operator++(int) noexcept { Iterator<T, Strict> temp(*this); ++mPointer; return temp; }

    /*
        Iterator - Pre-Decrement Operator - Decrement the location that the Iterator is pointing to
        Created: 28/07/2017
        Modified: 28/07/2017

        return Iterator& - Returns a reference to itself
    */
    template<typename T, bool Strict>
    inline Node::Iterator<T, Strict>& Node::Iterator<T, Strict>::operator--() noexcept { --mPointer; return *this; }

    /*
        Iterator - Post-Decrement Operator - Decrement the location that the Iterator is pointing to
        Created: 28/07/2017
        Modified: 28/07/2017

        return Iterator - Returns an iterator object holding the values prior to modification
    */
    template<typename T, bool Strict>
    inline Node::Iterator<T, Strict> Node::Iterator<T, Strict>::operator--(int) noexcept { Iterator<T, Strict> temp(*this); --mPointer; return temp; }
    #pragma endregion
    #pragma endregion

    #pragma region cIterator Definitions
    #pragma region Constructors 
    /*
        cIterator : Default Constructor - Initialise with default values
        Created: 28/07/2017
        Modified: 28/07/2017
    */
    template<typename T, bool Strict>
    inline Node::cIterator<T, Strict>::cIterator() : mPointer(nullptr) {}

    /*
        cIterator : Custom Constructor - Initialise the cIterator object pointing at usable memory
        Created: 28/07/2017
        Modified: 28/07/2017

        param[in] pMemory - A constant pointer to memory managed by an Objx::Node object
    */
    template<typename T, bool Strict>
    inline Node::cIterator<T, Strict>::cIterator(const T* pMemory) : mPointer(pMemory) {}
#pragma endregion
    #pragma region Access Operators
    /*
        cIterator : Arrow Operator - Provide direct access to the internal pointer
        Created: 28/07/2017
        Modified: 28/07/2017

        return const T*& - Returns a constant reference to the internal pointer
    */
    template<typename T, bool Strict>
    inline const T*& Node::cIterator<T, Strict>::operator->() const noexcept { return mPointer; }

    /*
        cIterator : Dereference Operator - Provide a direct reference to the indicated data
        Created: 28/07/2017
        Modified: 28/07/2017

        return const T& - Returns a constant reference to the data at the internal pointers location
    */
    template<typename T, bool Strict>
    inline const T& Node::cIterator<T, Strict>::operator*() const noexcept { return *mPointer; }
    #pragma endregion
    #pragma region Logical Operators
    /*
        cIterator : Equality Operator - Determines if two cIterators are equal
        Created: 28/07/2017
        Modified: 28/07/2017

        param[in] pRight - The cIterator to be compared against

        return bool - Returns true if both cIterators are pointing at the same memory location
    */
    template<typename T, bool Strict>
    inline bool Node::cIterator<T, Strict>::operator==(const cIterator& pRight) const noexcept { return (mPointer == pRight.mPointer); }

    /*
        cIterator : Inequality Operator - Determines if two cIterators are unequal
        Created: 28/07/2017
        Modified: 28/07/2017

        param[in] pRight - The cIterator to be compared against

        return bool - Returns true if both cIterators are pointing at different locations in memory
    */
    template<typename T, bool Strict>
    inline bool Node::cIterator<T, Strict>::operator!=(const cIterator& pRight) const noexcept { return (mPointer != pRight.mPointer); }
    #pragma endregion
    #pragma region Progressional Operators
    /*
        cIterator : Pre-Increment Operator - Increment the location that the cIterator is pointing to
        Created: 28/07/2017
        Modified: 28/07/2017

        return cIterator& - Returns a reference to itself
    */
    template<typename T, bool Strict>
    inline Node::cIterator<T, Strict>& Node::cIterator<T, Strict>::operator++() noexcept { ++mPointer; return *this; }

    /*
        cIterator : Post-Increment Operator - Increment the location that the cIterator is pointing to
        Created: 28/07/2017
        Modified: 28/07/2017

        return cIterator - Returns an cIterator object holding the values prior to modification
    */
    template<typename T, bool Strict>
    inline Node::cIterator<T, Strict> Node::cIterator<T, Strict>::operator++(int) noexcept { cIterator<T, Strict> temp(*this); ++mPointer; return temp; }

    /*
        cIterator - Pre-Decrement Operator - Decrement the location that the cIterator is pointing to
        Created: 28/07/2017
        Modified: 28/07/2017

        return cIterator& - Returns a reference to itself
    */
    template<typename T, bool Strict>
    inline Node::cIterator<T, Strict>& Node::cIterator<T, Strict>::operator--() noexcept { --mPointer; return *this; }

    /*
        cIterator - Post-Decrement Operator - Decrement the location that the cIterator is pointing to
        Created: 28/07/2017
        Modified: 28/07/2017

        return cIterator - Returns an cIterator object holding the values prior to modification
    */
    template<typename T, bool Strict>
    inline Node::cIterator<T, Strict> Node::cIterator<T, Strict>::operator--(int) noexcept { cIterator<T, Strict> temp(*this); --mPointer; return temp; }
    #pragma endregion
    #pragma endregion
    #pragma endregion
}