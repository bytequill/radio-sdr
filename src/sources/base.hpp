#include <cstdint>
#include <string>
#include <complex>
#include <vector>

using cd = std::complex<double>;

class RFSourceBase {
	public:
	bool live; // TRUE - Live RF stream FALSE - Recorded file
	
	// Fundamental metadata. In Hz
	long bandwidth;
	long samplerate;
	long centerFreq;
	
	bool Load(std::string sourceID);
	std::vector<cd> GetSamples(uintmax_t indx, int n);
};