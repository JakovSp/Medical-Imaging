#pragma once
#include "pch.h"
#include "DataSet.h"
#include "DataElement.h"

namespace vxe::med {
	struct DICOMInstance {
		std::string UID;
		DataSet MetaInfo;
		DataSet MainDataSet;
		const DataElement& operator[](tag_t TagID) const;
	};

	struct DICOMSeries {
		std::string UID;
		std::vector<DICOMInstance> instances;
		DICOMSeries() : UID("0") { }
		DICOMSeries(const std::string& newUID) : UID(newUID) { }

		void AddInstance(DICOMInstance& newDI);
		bool IsHomogeneous(std::string FORUID = "");

		DICOMInstance operator[](const std::string& InstanceUID) const;
		DICOMInstance& operator[](const std::string& InstanceUID);
		DICOMInstance operator[](const size_t i) const{
			return instances[i];
		};
	};

	struct DICOMStudy {
		std::string UID;
		std::vector<DICOMSeries> series;
		DICOMStudy() : UID("0") { }
		DICOMStudy(const std::string& newUID) : UID(newUID) { }

		DICOMSeries operator[](const std::string& SeriesUID) const;
		DICOMSeries& operator[](const std::string& SeriesUID);
		DICOMSeries operator[](const size_t i) const{
			return series[i];
		};
	};

	struct DICOMPatient {
		std::string ID;
		std::vector<DICOMStudy> studies;
		DICOMPatient() : ID("") { }
		DICOMPatient(const std::string& newID) : ID(newID) { }

		DICOMStudy operator[](const std::string& StudyUID) const;
		DICOMStudy& operator[](const std::string& StudyUID);
		DICOMStudy operator[](const size_t i) const{
			return studies[i];
		};
	};

	struct FileSetRoot{
		std::vector<DICOMPatient> patients;
		FileSetRoot() { }
		const DICOMPatient operator[](const std::string& PatientID) const;
		DICOMPatient& operator[](const std::string& PatientID);
		const DICOMPatient operator[](const size_t i) const {
			return patients[i];
		}

	};

	class FileSet{
	public:
		FileSet() { }
		FileSet(DataSet DirectoryStorage);
		void InsertNewInstance(DICOMInstance& newDI);
		FileSetRoot Root;
	};

}
