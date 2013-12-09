#include <NTL/ZZ.h>

using namespace std;
using namespace NTL;

long witness(long n, long x)
{
	long m, y, z;
	long j, k;

	if (x == 0) return 0;

	// compute m, k such that n-1 = 2^k * m, m odd:

	k = 1;
	m = n / 2;
	while (m % 2 == 0) {
		k++;
		m /= 2;
	}

	z = PowerMod(x, m, n); // z = x^m % n
	if (z == 1) return 0;

	j = 0;
	do {
		y = z;
		z = (y*y) % n;
		j++;
	} while (j < k && z != 1);

	return z != 1 || y != n - 1;
}

long PrimeTest(long n, long t)
{
	if (n <= 1) return 0;

	// first, perform trial division by primes up to 2000

	PrimeSeq s;  // a class for quickly generating primes in sequence
	long p;

	/*p = s.next();  // first prime is always 2
	while (p && p < 2000) {
		if ((n % p) == 0) return (n == p);
		p = s.next();
	}*/

	// second, perform t Miller-Rabin tests

	long x;
	long i;

	for (i = 0; i < 5; i++) {
		x = rand()%n; // random number between 0 and n-1

		if (witness(n, x))
			return 0;
	}

	return 1;
}

bool RabinMiller(ZZ p, long t){
	long k = 1;
	ZZ m = p / 2;
	while (m % 2 == 0){
		k++;
		m = m / 2;
	}

	ZZ x;
	ZZ b;
	for (int i = 0; i < t; i++){
		x = RandomBnd(p);
		b = PowerMod(x, m, p);
		if (b%p == 1)
			return 1;
		else{
			for (int j = 0; j <= k - 1; j++){
				if (b%p == -1)
					return 1;
				else
					b = (b ^ 2)%p;
			}
		}
	}
		return 0;
}

int main()
{
	ZZ n,s;
	long i=8;
	bool x;
	double t;

	cout << "s: ";
	cin >> s;

	//GenPrime(n, 2048);

	SetSeed(s);

	/*while (i <= 8196){
		RandomBits(n,i);

		t = GetTime();
		if (PrimeTest(n, 10)){
			t = GetTime() - t;
			cout << n << " is probably prime, " << t << " time "<< i<<" bitov\n";
		}
		else
			cout << n << " is composite, " << t << " time " << i << " bitov\n";

		t = GetTime();
		x = RabinMiller(n, 10);
		t = GetTime() - t;

		if (x == 1)
			cout << n << " je prvocislo, trvalo to " << t << " s " << i << " bitov\n";
		else cout << n << " nie je prvocislo, trvalo to " << t << " s " << i << " bitov \n";
		i = i * 2;
	}*/

	for (int i = 2; i < 100; i++){

		x = PrimeTest(i, 10);


		if (x == 1)
			cout << i << " je prvocislo" << endl;
		else cout << i<<" nie je prvocislo"<<endl;
	}

	cin.get();
	cin.get();
}