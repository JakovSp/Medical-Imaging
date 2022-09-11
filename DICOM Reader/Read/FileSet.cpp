#include "FileSet.h"

using namespace vxe::med;

void FileSet::InsertNewInstance(DICOMInstance& newDI) {
	const std::string& patient	= newDI.MainDataSet[PatientID].FetchValue<std::string>();
	const std::string& study	= newDI.MainDataSet[StudyInstanceUID].FetchValue<std::string>();
	const std::string& series	= newDI.MainDataSet[SeriesInstanceUID].FetchValue<std::string>();
	newDI.UID					= newDI.MainDataSet[SOPInstanceUID].FetchValue<std::string>();
	Root[patient][study][series].AddInstance(newDI);
}


const DICOMPatient FileSetRoot::operator[](const std::string& PatientID) const {
	for (size_t i = 0; i < patients.size(); i++) {
		if (patients[i].ID == PatientID) {
			return patients[i];
		}
	}
}
DICOMPatient& FileSetRoot::operator[](const std::string& PatientID) {
	for (size_t i = 0; i < patients.size(); i++) {
		if (patients[i].ID == PatientID) {
			return patients[i];
		}
	}
	patients.push_back({PatientID});
	return patients.back();
}

DICOMStudy DICOMPatient::operator[](const std::string& StudyUID) const {
	for (size_t i = 0; i < studies.size(); i++) {
		if (studies[i].UID == StudyUID) {
			return studies[i];
		}
	}
}
DICOMStudy& DICOMPatient::operator[](const std::string& StudyUID) {
	for (size_t i = 0; i < studies.size(); i++) {
		if (studies[i].UID == StudyUID) {
			return studies[i];
		}
	}
	studies.push_back({StudyUID});
	return studies.back();
}

DICOMSeries DICOMStudy::operator[](const std::string& SeriesUID) const {
	for (size_t i = 0; i < series.size(); i++) {
		if (series[i].UID == SeriesUID) {
			return series[i];
		}
	}
}
DICOMSeries& DICOMStudy::operator[](const std::string& SeriesUID) {
	for (size_t i = 0; i < series.size(); i++) {
		if (series[i].UID == SeriesUID) {
			return series[i];
		}
	}
	series.push_back({SeriesUID});
	return series.back();
}

void DICOMSeries::AddInstance(DICOMInstance& newDI){
	instances.push_back(newDI);
}

bool DICOMSeries::IsHomogeneous(std::string FORUID) {
	if (instances.size()) {
		if (FORUID == "") {
			FORUID = instances[0].MainDataSet[FrameOfReferenceUID].FetchValue<std::string>();
		}
		for (size_t i = 0; i < instances.size(); i++) {
			DICOMInstance& instance = instances[i];
			if (FORUID != instances[i].MainDataSet[FrameOfReferenceUID].FetchValue<std::string>()) {
				return false;
			}
		}
	}
	return true;
}

DICOMInstance DICOMSeries::operator[](const std::string& InstanceUID) const {
	for (size_t i = 0; i < instances.size(); i++) {
		if (instances[i].UID == InstanceUID) {
			return instances[i];
		}
	}
}
DICOMInstance& DICOMSeries::operator[](const std::string& InstanceUID) {
	for (size_t i = 0; i < instances.size(); i++) {
		if (instances[i].UID == InstanceUID) {
			return instances[i];
		}
	}
	instances.push_back({ InstanceUID });
	return instances.back();
}

const DataElement& DICOMInstance::operator[](tag_t TagID) const {
	static const DataElement empty;

	for (int i = 0; i < MetaInfo.elements.size(); i++) {
		if (TagID == MetaInfo.elements[i].tag) {
			return MetaInfo.elements[i];
		}
	}

	for (int i = 0; i < MainDataSet.elements.size(); i++) {
		if (TagID == MainDataSet.elements[i].tag) {
			return MainDataSet.elements[i];
		}
	}

	return empty;
};
