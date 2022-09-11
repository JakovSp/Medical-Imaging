#include "DataSet.h"
#include "DataElement.h"

using namespace vxe::med;

DataSet::DataSet(): elements({0}) {}

const DataElement& DataSet::operator[](tag_t TagID) const {
	static const DataElement empty;

	for (int i = 0; i < elements.size(); i++) {
		if (TagID == elements[i].tag) {
			return elements[i];
		}
	}

	return empty;
}
