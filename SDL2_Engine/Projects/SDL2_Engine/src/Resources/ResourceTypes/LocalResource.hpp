#pragma once

//! Include the Library Management functionality
#include "../../__LibraryManagement.hpp"

//! Include the Resource Base type
#include "LocalResourceBase.hpp"

namespace SDL2_Engine {
	namespace ResourceTypes {
		/*
		 *		Name: LocalResource
		 *		Author: Mitchell Croft
		 *		Created: 04/10/2017
		 *		Modified: 04/10/2017
		 *
		 *		Purpose:
		 *		Provide a generalised resource point for specialisations to derive from
		**/
		template<typename T>
		class SDL2_LIB_INC __LocalResource;
	}
}