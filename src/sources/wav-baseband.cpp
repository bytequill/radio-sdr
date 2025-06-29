#include "wav-baseband.hpp"

#include <cassert>
#include <iostream>
#include <cstring>

bool RFSourceWAV::Load(std::string filename) {
	file.open(filename.c_str(), std::ios::in | std::ios::binary);
	if (!file || !file.is_open()) {return false;}
	
	std::cout << header.samplerate << std::endl;
	
	char buff[sizeof(header)];
	file.read(buff, sizeof(header));
	
	memcpy(&header, buff, sizeof(header));
	
	// Assumptions about the format which if not true require refactoring of the code
	if (std::strncmp(header.Magic, "RIFF", 4) != 0 ||
		std::strncmp(header.RIFFType, "WAV", 4) != 0 ||
		std::strncmp(header.FMTMARK, "fmt ", 4) != 0 ||
		std::strncmp(header.DATAMARK, "data", 4) != 0 ||
		header.fmtType != 1 ||
		header.chnNum != 2)
	{
		// TODO: figure out a DEBUG/ERROR system for more precise data than just the failure
		return false;
	}
	
	this->samplerate = header.samplerate;
	
	return false;
}

// converts the `char`(1byte) raw sampleData into correct bitsPerSample based values
int64_t convertSample(const char *sampleData, uint_fast16_t bitsPerSample) {
	assert(bitsPerSample > 0 && (bitsPerSample & (bitsPerSample - 1)) == 0); // Check power of 2
	assert(bitsPerSample <= 64);
	
	if (bitsPerSample == 8) {
		// 8-bit WAV audio is typically stored as unsigned values.
		// Convert to signed: 0-255 (unsigned) -> -128 to 127.
		uint8_t raw = *(reinterpret_cast<const uint8_t*>(sampleData));
		return static_cast<int64_t>(static_cast<int16_t>(raw) - 128);
	} else if (bitsPerSample == 16) {
		int16_t sample = static_cast<int16_t>(
			(static_cast<uint16_t>(static_cast<uint8_t>(sampleData[1])) << 8) |
			 static_cast<uint8_t>(sampleData[0])
		);
		return static_cast<int64_t>(sample);
	} else if (bitsPerSample == 32) {
		int32_t sample = static_cast<int32_t>(
			(static_cast<uint32_t>(static_cast<uint8_t>(sampleData[3])) << 24) |
			(static_cast<uint32_t>(static_cast<uint8_t>(sampleData[2])) << 16) |
			(static_cast<uint32_t>(static_cast<uint8_t>(sampleData[1])) <<  8) |
			 static_cast<uint8_t>(sampleData[0])
		);
		return static_cast<int64_t>(sample);
	} else if (bitsPerSample == 64 && sizeof(size_t) >= 8) {
		int64_t sample = static_cast<int64_t>(
			(static_cast<uint64_t>(static_cast<uint8_t>(sampleData[7])) << 56) |
			(static_cast<uint64_t>(static_cast<uint8_t>(sampleData[6])) << 48) |
			(static_cast<uint64_t>(static_cast<uint8_t>(sampleData[5])) << 40) |
			(static_cast<uint64_t>(static_cast<uint8_t>(sampleData[4])) << 32) |
			(static_cast<uint64_t>(static_cast<uint8_t>(sampleData[3])) << 24) |
			(static_cast<uint64_t>(static_cast<uint8_t>(sampleData[2])) << 16) |
			(static_cast<uint64_t>(static_cast<uint8_t>(sampleData[1])) <<  8) |
			 static_cast<uint8_t>(sampleData[0])
		);
		return static_cast<int64_t>(sample);
	} else {
		return 0;
	}
}

std::vector<cd> RFSourceWAV::GetSamples(uintmax_t indx, int n) {
	// an I/Q PCM stream MUST have 2 channels but setting value here for readability
	int8_t chnNum = 2; 
	assert (chnNum == header.chnNum);
	
	std::vector<cd> samples;
	if (!file || !file.is_open()) {return samples;}
	
	std::vector<char> buf((header.bitsPerSample / 8) * chnNum * n);
	uintmax_t offset = sizeof(header) + indx * header.blockAlign;
	if (offset + buf.size() > header.dataLen + sizeof(header)) {return samples;}
	
	file.seekg(offset);
	if (!file) {return samples;}
	file.read(buf.data(), buf.size());
	
	for (size_t i = 0; i < buf.size(); i += header.blockAlign) {
		int sampleI = convertSample(&buf.data()[i], header.bitsPerSample);
		int sampleQ = convertSample(&buf.data()[i + (header.bitsPerSample / 8)],
									header.bitsPerSample);
		samples.push_back(cd(sampleI, sampleQ));
	}
	
	return samples;
}

RFSourceWAV::~RFSourceWAV() {
	if(file){
		if(file.is_open()) {file.close();}
	}
}