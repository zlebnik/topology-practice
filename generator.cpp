#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>

using namespace std;
double random_double(int limit) {
	return (1 + rand()) % limit;	
}

int main() {
	int n;
	cin >> n;
	vector<double> x(n), y(n), w(n), h(n);

	cout << n << endl;

	int a = trunc(sqrt(n));
	int b = n / a;
	cerr << a << "x" << b << endl;

	


	double maxY = 0;
	for (int i = 0; i < b; i++) {
		
		w[i * a] = random_double(10);
		h[i * a] = random_double(10);

		x[i * a] = 0;
		y[i * a] = maxY + random_double(2);

		cout << x[i * a] + w[i * a] << " " << y[i * a] + h[i * a] << " " << w[i * a] << " " << h[i * a] << endl;

		double currentMaxY = y[i * a] + 2 * h[i * a];
		double prevX = x[i * a] + 2 * w[i * a];

		for (int j = 1; j < a && i * a + j < n; j++) {
			w[i * a + j] = random_double(10);
			h[i * a + j] = random_double(10);

			x[i * a + j] = prevX + w[i * a + j] + random_double(2);
			prevX = x[i * a + j] + 2 * w[i * a + j];

			y[i * a + j] = maxY + h[i * a + j] + random_double(2);
			
			if (y[i * a + j] + 2 * h[i * a + j] > currentMaxY) {
				currentMaxY = y[i * a + j] + 2 * h[i * a + j];
			}

			cout << x[i * a + j] + w[i * a + j] << " " << y[i * a + j] + h[i * a + j] << " " << w[i * a + j] << " " << h[i * a + j] << endl;
		}
		maxY = currentMaxY;
	}

	return 0;
}