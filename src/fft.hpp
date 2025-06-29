#include <cmath>
#include <complex>
#include <vector>

using cd = std::complex<double>;
const double PI = acos(-1);

void fft(std::vector<cd> &a, bool invert);