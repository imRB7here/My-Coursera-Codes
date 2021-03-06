#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#define deb(x) cout << #x << " : " << x << '\n'
#define logn(x) cout << x << '\n'
#define logs(x) cout << x << ' '
#define log(x) cout << x
#define MOD 1000000007
#define uInt unsigned long long int
#define Int long long int

using namespace std;

template <typename T, typename U>
void debArr(T arr[], U n)
{
	for (U i = 0; i <= n - 1; i++)
	{
		logs(arr[i]);
	}
	log(endl);
}

template <typename T>
void debVect(vector<T> arr)
{
	for (auto i : arr)
	{
		logs(i);
	}
	log(endl);
}

vector<Int> primeFactors(Int n)
{
	vector<Int> minPrimes(n + 1, 0);
	for (Int i = 2; i * i <= n; i++)
	{
		if (minPrimes[i] == 0)
		{
			for (Int j = i * i; j <= n; j += i)
			{
				if (minPrimes[j] == 0)
					minPrimes[j] = i;
			}
		}
	}
	for (Int i = 2; i <= n; i++)
	{
		if (minPrimes[i] == 0)
			minPrimes[i] = i;
	}
	vector<Int> factors;
	while (minPrimes[n] >= 1)
	{
		factors.push_back(minPrimes[n]);
		n /= minPrimes[n];
	}
	return factors;
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	debVect(primeFactors(17));

	return 0;
}
