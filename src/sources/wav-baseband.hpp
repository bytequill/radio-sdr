#include "base.hpp"

#include <fstream>

#pragma pack(push, 1)
struct WAVHeader {
    char Magic[4];            // Always "RIFF". Magic value for all RIFF-Based formats!
    uint32_t Size;
    char RIFFType[4];         // Always "WAV"
    char FMTMARK[4];
    uint32_t fmtChunkLen;     // Always 16
    uint16_t fmtType;         // 1 - PCM
    uint16_t chnNum;
    uint32_t samplerate;
    uint32_t byteRate;        // samplerate * bitsPerSample * chnNum / 8
    uint16_t blockAlign;      // bitsPerSample * chnNum / 8
    uint16_t bitsPerSample;
    char DATAMARK[4];
    uint32_t dataLen;
};
#pragma pack(pop)

class RFSourceWAV : RFSourceBase {
	public:
		bool Load(std::string filename);
		// Get `n` samples at index `indx` for both I and Q channels from the WAV file and return an
		// FFT-friendly vector
		std::vector<cd> GetSamples(uintmax_t indx, int n);
		~RFSourceWAV();
	
	private:
		std::ifstream file;
		WAVHeader header;
};

