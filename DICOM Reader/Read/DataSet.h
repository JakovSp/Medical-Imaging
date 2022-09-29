/*
DataSet.h

	Defines the format and runtime structure of DICOM data set
*/
#pragma once
#include "pch.h"

// #include "DataElement.h"
#include "Registry/DataDictionary.h"

namespace vxe::med {
	class DataElement;

	using TransferSyntax = std::byte; // Packed flags, bits are set for non default TS :
	constexpr std::byte BEND			= (std::byte)1<<0;
	constexpr std::byte IMPLICIT_VR		= (std::byte)1<<1;
	constexpr std::byte ENCAPSULATED	= (std::byte)1<<2;

	class DataSet {
	public:
		DataSet();
		const DataElement& operator[](tag_t TagID) const;

		TransferSyntax TS{0};
		std::vector<DataElement> elements;
	};

}