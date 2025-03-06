#include <iostream>
#include<cmath>
using namespace std;
int main()
{
	int i, m,n,k;
	int t;
	cin >> m >> n;

	if (m > n)
	{
		t = m;
		m = n;
		n = t;
	}

	for (i = 1;;i++)
	{
		if (i > m)break;
		if (n % i != 0)
			continue;
		if (m % i != 0)
			continue;
		k = i;
	}
	cout << k << endl;
	return 0;
}





	


