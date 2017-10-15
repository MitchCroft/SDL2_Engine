#pragma once

//! Include the Node Object
#include "Objx_node.hpp"

namespace Objx {
    /*
     *      Name: Document
     *      Author: Mitchell Croft
     *      Created: 31/07/2017
     *      Modified: 05/10/2017
     *      
     *      Purpose:
     *      Track a group of Objx::Node objects that can be added to and removed
     *      from. This object is the virtual representation of an Objx external file
    **/
    class OBJX_LIB_INC Document {
    public:
        ///////////////////////////////////////////////////////////////////////////////////
        ////////----------------------Constructors/Destructor----------------------////////
        ///////////////////////////////////////////////////////////////////////////////////
        
        /*
            Document : Default Constructor - Initialise with default values
            Created: 01/08/2017
            Modified: 01/08/2017

            param[in] pStrict - Flags if the Document object should avoid allowing multiple Objx
                                Nodes with the same name (Default false)
        */
        Document(const bool& pStrict = false);

        /*
            Document : Copy Constructor - Initialise the Document by copying the values of another Document object
            Created: 01/08/2017
            Modified: 01/08/2017

            param[in] pCopy - The Document object to copy the values of
        */
        Document(const Document& pCopy);

        /*
            Document : Destructor - Unload memory used by the Document object
            Created: 01/08/2017
            Modified: 01/08/2017
        */
        ~Document();

        ///////////////////////////////////////////////////////////////////////////////////
        ////////-------------------------Polling Functions-------------------------////////
        ///////////////////////////////////////////////////////////////////////////////////
        
        /*
            Document : size - Get the number of Node elements in the Document
            Created: 01/08/2017
            Modified: 01/08/2017

            return const size_t& - Returns a constant reference to the internal size value
        */
        inline const size_t& size() const noexcept;

        /*
            Document : hasKey - Determine if the Document contains a Node object with a specified name
            Created: 01/08/2017
            Modified: 21/08/2017

            param[in] pName - The name of the Node to find in the Document

            return bool - Returns true if there is a Node with the specified name
        */
        bool hasKey(const xstring& pName) const noexcept;

        /*
            Document : allowsConflicts - Check's to see if the current Document object allows multiple Nodes with the same name
            Created: 01/08/2017
            Modified: 01/07/2017

            return const bool& - Returns a constant reference to the internal bool flag
        */
        inline const bool& allowsConflicts() const noexcept;

        ///////////////////////////////////////////////////////////////////////////////////
        ////////---------------------------Objx Functions--------------------------////////
        ///////////////////////////////////////////////////////////////////////////////////

        /*
            Document : get - Return a reference to an Objx Node with the defined name that can be read and written to
            Created: 01/08/2017
            Modified: 21/08/2017

            param[in] pName - The name of the Node to retrieve

            return Node& - Returns a reference to the requested Node

            NOTE:
            This function will create an object with the given name if it could not be found
        */
        Node& get(const xstring& pName);

        /*
            Document : read - Read the Objx Node stored at the requested name
            Created: 01/08/2017
            Modified: 21/08/2017

            param[in] pName - The name of the Node to retrieve
            param[in] pDefault - A default Node object to be used if there is no Objx with
                                 the specified name (Default empty)

            return const Node - Returns a constant Node copy with the values of the requested name

            NOTE:
            This function will return a copy of the FIRST Objx Node found with the specified name
        */
        const Node read(const xstring& pName, const Node& pDefault = Node()) const noexcept;

        /*
            Document : write - Write an Objx Node to the Document with a specified name
            Created: 01/08/2017
            Modified: 22/08/2017

            param[in] pName - The name to store the Node under
            param[in] pNode - The Node object to be stored under the name

            return EStatusCode - Returns a status code representing the success state of the operation.
                                 EStatusCode::Empty_Data_Object - The supplied Node is null

            NOTE:
            If the Document allows conflicting names a new Objx will be created to hold the data. Otherwise
            the previous Objx will be overwritten.
        */
        EStatusCode write(const xstring& pName, const Node& pNode);

        /*
            Document : erase - Erase all contained Nodes with a specified name
            Created: 01/08/2017
            Modified: 01/08/2017

            param[in] pName - The name of the Node(s) to remove

            return size_t - Returns the number of Node objects removed
        */
        size_t erase(const xstring& pName) noexcept;

        /*
            Document : erase - Erase all contained Nodes
            Created: 01/08/2017
            Modified: 21/08/2017

            return size_t - Returns the number of Node objects removed
        */
        size_t erase() noexcept;

        /*
            Document : forEach - Iterate over all Objx stored within the Document object
            Created: 01/08/2017
            Modified: 05/10/2017

            param[in] pCB - A callback to be used to allow for the processing of all contained Nodes

            return size_t - Returns the number of Objx Nodes processed
        */
        size_t forEach(const ForEachCB& pCB);

        /*
            Document : forEach - Iterate over all Objx stored within the Document object
            Created: 01/08/2017
            Modified: 05/10/2017

            param[in] pCB - A callback to be used to allow for the processing of all contained Nodes

            return size_t - Returns the number of Objx Nodes processed
        */
        size_t forEach(const cForEachCB& pCB) const noexcept;

        ///////////////////////////////////////////////////////////////////////////////////
        ////////------------------------Iterator Functions-------------------------////////
        ///////////////////////////////////////////////////////////////////////////////////
        
        //! Prototype the Document Iterator objects
        class Iterator;
        class cIterator;

        /*
            Document : begin - Get the Iterator for the beginning of the Document's linked list
            Created: 01/08/2017
            Modified: 01/08/2017

            return Iterator - Returns a modifiable Iterator pointing to the start of the internal list
        */
        Iterator begin() noexcept;

        /*
            Document : end - Get the Iterator for the end of the Document's linked list
            Created: 01/08/2017
            Modified: 01/08/2017

            return Iterator - Returns a modifiable Iterator pointing past the end of the internal list
        */
        Iterator end() noexcept;

        /*
            Document : cbegin - Get the cIterator for the beginning of the Document's linked list
            Created: 01/08/2017
            Modified: 01/08/2017

            return cIterator - Returns a constant Iterator pointing at the start of the internal list
        */
        cIterator cbegin() const noexcept;

        /*
            Document : cend - Get the cIterator for the end of the Document's linked list
            Created: 01/08/2017
            Modified: 01/08/2017

            return cIterator - Returns a constant Iterator pointing past the end of the internal list
        */
        cIterator cend() const noexcept;

        ///////////////////////////////////////////////////////////////////////////////////
        ////////----------------------------Operators------------------------------////////
        ///////////////////////////////////////////////////////////////////////////////////
        
        /*
            Document : Assignment Operator - Assign the current Document the values of another
            Created: 01/08/2017
            Modified: 01/08/2017

            param[in] pCopy - The Document object to copy the values of

            return Document& - Returns a reference to itself
        */
        Document& operator=(const Document& pCopy) noexcept;

        /*
            Document : Subscript Operator (Index) - Access the Node element at the specified index
            Created: 01/08/2017
            Modified: 01/08/2017

            param[in] pIndex - The desired index to retrieve

            return Node& - Returns a reference to the Node at the index

            NOTE:
            This function creates additional Objxs if the index is larger than what is currently contained
        */
        Node& operator[](const size_t& pIndex) noexcept;

        /*
            Document : Const Subscript Operator (Index) - Access the Node element at the specified index
            Created: 01/08/2017
            Modified: 01/08/2017

            param[in] pIndex - The desired index to retrieve

            return const Node - Returns a constant copy of the specified Node
        */
        const Node operator[](const size_t& pIndex) const noexcept;

        /*
            Document : Subscript Operator (Key) - Access the Node element with the specified name
            Created: 01/08/2017
            Modified: 01/08/2017

            param[in] pName - The name of the Node element to retrieve

            return Node& - Returns a reference to the Node with the key
        */
        inline Node& operator[](const xstring& pName) noexcept;

        /*
            Document : Const Subscript Operator (Key) - Access the Node element with the specified name
            Created: 01/08/2017
            Modified: 01/08/2017

            param[in] pName - The name of the Node element to retrieve

            return const Node - Returns a constant copy of the specified Node
        */
        const Node operator[](const xstring& pName) const noexcept;

    private:
        //! Prototype the Node container object
        struct LinkedNode;

        //! Store ends of the Linked List
        LinkedNode* mHead;
        LinkedNode* mTail;

        //! Store the number of elements in the Document
        size_t mSize;

        //! Flag if the Document object is concerned about containing Objxs with matching names
        bool mAllowsConflict;

        ///////////////////////////////////////////////////////////////////////////////////
        ////////-------------------------Private Functions-------------------------////////
        ///////////////////////////////////////////////////////////////////////////////////
        
        //! Manage the internal list of elements
        LinkedNode* createNewNode(const xstring& pName) noexcept;
        LinkedNode* findNode(const xstring& pName) const noexcept;
        LinkedNode* getIndexedNode(const size_t& pIndex) const noexcept;
        void removeNode(LinkedNode* pNode) noexcept;
    };

    /*
     *      Name: Iterator (Document)
     *      Author: Mitchell Croft
     *      Created: 01/08/2017
     *      Modified: 01/08/2017
     *      
     *      Purpose:
     *      Provide iterative access to internal linked list elements with read
     *      and write access
    **/
    class OBJX_LIB_INC Document::Iterator {
    public:
        ///////////////////////////////////////////////////////////////////////////////////
        ////////------------------------Default Constructor------------------------////////
        ///////////////////////////////////////////////////////////////////////////////////
        
        /*
            Iterator : Default Constructor - Initialise with default values
            Created: 01/08/2017
            Modified: 01/08/2017
        */
        inline Iterator();

        ///////////////////////////////////////////////////////////////////////////////////
        ////////--------------------------Access Operators-------------------------////////
        ///////////////////////////////////////////////////////////////////////////////////
        
        /*
            Iterator : Arrow Operator - Provide direct access to the internal Node
            Created: 01/08/2017
            Modified: 01/08/2017

            return Node* - Returns a reference to the internal Node pointer
        */
        Node* operator->();

        /*
            Iterator : Dereference Operator - Provide a direct reference to the internal Node
            Created: 01/08/2017
            Modified: 01/08/2017

            return Node& - Returns a modifiable reference to the current Node object
        */
        Node& operator*();

        ///////////////////////////////////////////////////////////////////////////////////
        ////////-------------------------Logical Operators-------------------------////////
        ///////////////////////////////////////////////////////////////////////////////////
        
        /*
            Iterator : Equality Operator - Determines if two Iterators are equal
            Created: 01/08/2017
            Modified: 01/08/2017

            param[in] pRight - The Iterator to be compared against

            return bool - Returns true if both Iterators are pointing at the same memory location
        */
        inline bool operator==(const Iterator& pRight) const noexcept;

        /*
            Iterator : Inequality Operator - Determines if two Iterators are unequal
            Created: 01/08/2017
            Modified: 01/08/2017

            param[in] pRight - The Iterator to be compared against

            return bool - Returns true if both iterators are pointing at different locations in memory
        */
        inline bool operator!=(const Iterator& pRight) const noexcept;

        ///////////////////////////////////////////////////////////////////////////////////
        ////////----------------------Progressional Operators----------------------////////
        ///////////////////////////////////////////////////////////////////////////////////
        
        /*
            Iterator : Pre-Increment Operator - Increment the location that the iterator is pointing to
            Created: 01/08/2017
            Modified: 01/08/2017

            return Iterator& - Returns a reference to itself
        */
        Iterator& operator++() noexcept;

        /*
            Iterator : Post-Increment Operator - Increment the location that the iterator is pointing to
            Created: 01/08/2017
            Modified: 01/08/2017

            return Iterator - Returns an Iterator object holding the values prior to modification
        */
        Iterator operator++(int) noexcept;

        /*
            Iterator : Pre-Decrement Operator - Decrement the location that the Iterator is pointing to
            Created: 01/08/2017
            Modified: 01/08/2017

            return Iterator& - Returns a reference to itself
        */
        Iterator& operator--() noexcept;

        /*
            Iterator : Post-Decrement Operator - Decrement the location that the Iterator is pointing to
            Created: 01/08/2017
            Modified: 01/08/2017

            return Iterator - Returns an Iterator object holding the values prior to modification
        */
        Iterator operator--(int) noexcept;

    private:
        //! Assign as a friend of the Document to allow for construction
        friend class Document;

        //! Store a pointer to the next linked element
        LinkedNode* mNode;

        ///////////////////////////////////////////////////////////////////////////////////
        ////////-------------------------Custom Constructor------------------------////////
        ///////////////////////////////////////////////////////////////////////////////////
        
        /*
            Iterator : Custom Constructor - Initialise the iterator object pointing at a Node
            Created: 01/08/2017
            Modified: 01/08/2017

            param[in] pNode - A pointer to a Linked Node in a Document
        */
        inline Iterator(LinkedNode* pNode);
    };

    /*
     *      Name: cIterator (Document)
     *      Author: Mitchell Croft
     *      Created: 01/08/2017
     *      Modified: 01/08/2017
     *      
     *      Purpose:
     *      Provide iterative access to internal linked list elements with read
     *      access
    **/
    class OBJX_LIB_INC Document::cIterator {
    public:
        ///////////////////////////////////////////////////////////////////////////////////
        ////////------------------------Default Constructor------------------------////////
        ///////////////////////////////////////////////////////////////////////////////////

        /*
            cIterator : Default Constructor - Initialise with default values
            Created: 01/08/2017
            Modified: 01/08/2017
        */
        inline cIterator();

        ///////////////////////////////////////////////////////////////////////////////////
        ////////--------------------------Access Operators-------------------------////////
        ///////////////////////////////////////////////////////////////////////////////////

        /*
            cIterator : Arrow Operator - Provide direct access to the internal Node
            Created: 01/08/2017
            Modified: 01/08/2017

            return const Node* - Returns a constant reference to the internal Node pointer
        */
        const Node* operator->() const noexcept;

        /*
            cIterator : Dereference Operator - Provide a direct reference to the internal Node
            Created: 01/08/2017
            Modified: 01/08/2017

            return const Node& - Returns a constant reference to the current Node object
        */
        const Node& operator*() const noexcept;

        ///////////////////////////////////////////////////////////////////////////////////
        ////////-------------------------Logical Operators-------------------------////////
        ///////////////////////////////////////////////////////////////////////////////////

        /*
            cIterator : Equality Operator - Determines if two cIterators are equal
            Created: 01/08/2017
            Modified: 01/08/2017

            param[in] pRight - The cIterator to be compared against

            return bool - Returns true if both cIterators are pointing at the same memory location
        */
        inline bool operator==(const cIterator& pRight) const noexcept;

        /*
            cIterator : Inequality Operator - Determines if two cIterators are unequal
            Created: 01/08/2017
            Modified: 01/08/2017

            param[in] pRight - The cIterator to be compared against

            return bool - Returns true if both cIterators are pointing at different locations in memory
        */
        inline bool operator!=(const cIterator& pRight) const noexcept;

        ///////////////////////////////////////////////////////////////////////////////////
        ////////----------------------Progressional Operators----------------------////////
        ///////////////////////////////////////////////////////////////////////////////////

        /*
            cIterator : Pre-Increment Operator - Increment the location that the cIterator is pointing to
            Created: 01/08/2017
            Modified: 01/08/2017

            return cIterator& - Returns a reference to itself
        */
        cIterator& operator++() noexcept;

        /*
            cIterator : Post-Increment Operator - Increment the location that the cIterator is pointing to
            Created: 01/08/2017
            Modified: 01/08/2017

            return cIterator - Returns an cIterator object holding the values prior to modification
        */
        cIterator operator++(int) noexcept;

        /*
            cIterator : Pre-Decrement Operator - Decrement the location that the cIterator is pointing to
            Created: 01/08/2017
            Modified: 01/08/2017

            return cIterator& - Returns a reference to itself
        */
        cIterator& operator--() noexcept;

        /*
            cIterator : Post-Decrement Operator - Decrement the location that the cIterator is pointing to
            Created: 01/08/2017
            Modified: 01/08/2017

            return cIterator - Returns an cIterator object holding the values prior to modification
        */
        cIterator operator--(int) noexcept;

    private:
        //! Assign as a friend of the Document to allow for construction
        friend class Document;

        //! Store a pointer to the next linked element
        const LinkedNode* mNode;

        ///////////////////////////////////////////////////////////////////////////////////
        ////////-------------------------Custom Constructor------------------------////////
        ///////////////////////////////////////////////////////////////////////////////////

        /*
            cIterator : Custom Constructor - Initialise the cIterator object pointing at a Node
            Created: 01/08/2017
            Modified: 01/08/2017

            param[in] pNode - A pointer to a Linked Node in a Document
        */
        inline cIterator(const LinkedNode* pNode);
    };

    #pragma region Function Definitions
    #pragma region Document Definitions
    #pragma region Polling Functions
    /*
        Document : size - Get the number of Node elements in the Document
        Created: 01/08/2017
        Modified: 01/08/2017

        return const size_t& - Returns a constant reference to the internal size value
    */
    inline const size_t& Document::size() const noexcept { return mSize; }

    /*
        Document : allowsConflicts - Check's to see if the current Document object allows multiple Objxs with the same name
        Created: 01/08/2017
        Modified: 01/07/2017

        return const bool& - Returns a constant reference to the internal bool flag
    */
    inline const bool& Document::allowsConflicts() const noexcept { return mAllowsConflict; }
    #pragma endregion
    #pragma region Operator Function
    /*
        Document : Subscript Operator (Key) - Access the Node element with the specified name
        Created: 01/08/2017
        Modified: 01/08/2017

        param[in] pName - The name of the Node element to retrieve

        return Node& - Returns a reference to the Node with the key
    */
    inline Node& Document::operator[](const xstring& pName) noexcept { return get(pName); }
    #pragma endregion
    #pragma endregion
    
    #pragma region Iterator Definitions
    #pragma region Constructors
    /*
        Iterator : Default Constructor - Initialise with default values
        Created: 01/08/2017
        Modified: 01/08/2017
    */
    inline Document::Iterator::Iterator() : mNode(nullptr) {}

    /*
        Iterator : Custom Constructor - Initialise the iterator object pointing at a Node
        Created: 01/08/2017
        Modified: 01/08/2017

        param[in] pNode - A pointer to a Linked Node in a Document
    */
    inline Document::Iterator::Iterator(LinkedNode* pNode) : mNode(pNode) {}
    #pragma endregion
    #pragma region Logical Functions
    /*
        Iterator : Equality Operator - Determines if two Iterators are equal
        Created: 01/08/2017
        Modified: 01/08/2017

        param[in] pRight - The Iterator to be compared against

        return bool - Returns true if both Iterators are pointing at the same memory location
    */
    inline bool Document::Iterator::operator==(const Iterator& pRight) const noexcept { return (mNode == pRight.mNode); }

    /*
        Iterator : Inequality Operator - Determines if two Iterators are unequal
        Created: 01/08/2017
        Modified: 01/08/2017

        param[in] pRight - The Iterator to be compared against

        return bool - Returns true if both iterators are pointing at different locations in memory
    */
    inline bool Document::Iterator::operator!=(const Iterator& pRight) const noexcept { return (mNode != pRight.mNode); }
    #pragma endregion
    #pragma endregion
    
    #pragma region cIterator Definitions
    #pragma region Constructors 
    /*
        cIterator : Default Constructor - Initialise with default values
        Created: 01/08/2017
        Modified: 01/08/2017
    */
    inline Document::cIterator::cIterator() : mNode(nullptr) {}

    /*
        cIterator : Custom Constructor - Initialise the cIterator object pointing at a Node
        Created: 01/08/2017
        Modified: 01/08/2017

        param[in] pNode - A pointer to a Linked Node in a Document
    */
    inline Document::cIterator::cIterator(const LinkedNode* pNode) : mNode(pNode) {}
    #pragma endregion
    #pragma region Logical Functions
    /*
        cIterator : Equality Operator - Determines if two cIterators are equal
        Created: 01/08/2017
        Modified: 01/08/2017

        param[in] pRight - The cIterator to be compared against

        return bool - Returns true if both cIterators are pointing at the same memory location
    */
    inline bool Document::cIterator::operator==(const cIterator& pRight) const noexcept { return (mNode == pRight.mNode); }

    /*
        cIterator : Inequality Operator - Determines if two cIterators are unequal
        Created: 01/08/2017
        Modified: 01/08/2017

        param[in] pRight - The cIterator to be compared against

        return bool - Returns true if both cIterators are pointing at different locations in memory
    */
    inline bool Document::cIterator::operator!=(const cIterator& pRight) const noexcept { return (mNode != pRight.mNode); }
    #pragma endregion
    #pragma endregion
    #pragma endregion
}