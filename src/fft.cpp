#include "fft.hpp"
using namespace std;

#include <cassert>

void fft(vector<cd> &a, bool invert) {
	assert(a.size() > 0 && (a.size() & (a.size() - 1)) == 0); // Check power of 2
	int n = a.size();
	if (n == 1)
		return;

	vector<cd> a0(n / 2), a1(n / 2);
	for (int i = 0; 2 * i < n; i++) {
		a0[i] = a[2*i];
		a1[i] = a[2*i+1];
	}
	fft(a0, invert);
	fft(a1, invert);

	double ang = 2 * PI / n * (invert ? -1 : 1);
	cd w(1), wn(cos(ang), sin(ang));
	for (int i = 0; 2 * i < n; i++) {
		a[i] = a0[i] + w * a1[i];
		a[i + n/2] = a0[i] - w * a1[i];
		if (invert) {
			a[i] /= 2;
			a[i + n/2] /= 2;
		}
		w *= wn;
	}
}