/*
DataDictionary.h
	
	Read registry (static data) before parse-time
	Provide access to the registry
*/
#pragma once
#include <limits.h>
#include <map>
#include <string>
#include <vector>
#include <filesystem>

#include "DataElementTags.h"

namespace vxe::med {
	struct DEInfo {
		tag_t Tag{0};
		std::string name{0};
		uint16_t VR{0};
		size_t VM{0};
	};

	struct DEInfoVerbose {
		uint32_t Tag{0};
		std::string name{0};
		std::string VR{0};
		std::string VM{0};
		// std::string::npos == POSition Not found
		size_t GetVM() {
			if (VM.find("-")  != std::string::npos ||
				VM.find("n") != std::string::npos ||
				VM.find("or") != std::string::npos) {
				return 0;
			}
			return (size_t)std::stoull(VM);
		}
		uint16_t GetVR() {
			// Variable case:
			uint16_t value;
			value = 0;
			if (VR.length() >= 2 && VR.find("or", 3) == std::string::npos) {
				value += (uint16_t)(VR[1]);
				value = value << 8;
				value += (uint16_t)(VR[2]);
			}
			return value;
		}
	};
	
	class DataDictionary {
	public:
		DataDictionary(): _DERegistryPath(L"DataElementRegistry.csv") {}
		DEInfoVerbose GetDEInfoVerbose(tag_t Tag) {
			return _DEVerboseRegistry[Tag];
		}
		DEInfo GetDEInfo(tag_t Tag) {
			return _DERegistry[Tag];
		}
		void LoadDERegistry();
		void LoadDEVerboseRegistry();
	private:
		std::filesystem::path _DERegistryPath;
		std::map<tag_t, DEInfoVerbose> _DEVerboseRegistry;
		std::map<tag_t, DEInfo> _DERegistry;
	};
}
