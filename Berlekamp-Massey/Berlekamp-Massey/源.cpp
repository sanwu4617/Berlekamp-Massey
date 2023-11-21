#include <iostream>
#include <fstream>
#include <chrono>
using namespace std::chrono;
using namespace std;
#define LFSR_length 2000
struct Function {
	bool data[LFSR_length];
	int leader;
	int zeros;
	Function()
	{
		clear();
	}
	Function operator * (int mul_degree)
	{
		Function ret;
		for (int i = 0; i <= leader; i++)
		{
			ret.data[i + mul_degree] = data[i];
		}
		ret.setleader();
		return ret;
	}
	Function operator + (Function f)
	{
		Function ret;
		for (int i = 0; i <= max(leader, f.leader); i++)
		{
			ret.data[i] = data[i] ^ f.data[i];
		}
		ret.setleader();
		return ret;
	}
	bool& operator [](int n)
	{
		return data[n];
	}
	void setleader()
	{
		for (int i = LFSR_length - 1; i >= 0; i--)
		{
			if (data[i] == 1)
			{
				leader = i;
				break;
			}
		}
	}
	void print()
	{
		bool plus = false;
		if (data[0] == 1)
		{
			cout << 1;
			plus = true;
		}
		for (int i = 1; i <= leader; i++)
		{
			if (plus)
			{
				cout << '+';
				plus = false;
			}
			if (data[i] == 1)
			{
				cout << "x";
				if (i != 1)
					cout << "^" << i;
				plus = true;
			}
		}
		cout << endl;
	}
	void clear()
	{
		memset(data, 0, sizeof(data));
		leader = -1;
		zeros = -1;
	}
};
struct Functions {
	Function data[LFSR_length];
	int degree_index[LFSR_length];
	Functions() {
		memset(degree_index, 0, sizeof(degree_index));
	}
	inline Function& operator [](int n)
	{
		return data[n + 1];
	}
	void clear()
	{
		for (int i = 0; i < LFSR_length; i++)
		{
			data[i].clear();
		}
		memset(degree_index, 0, sizeof(degree_index));
	}
};

Functions f;
int main()
{
	ifstream fin("11.txt");
	char s[2 * LFSR_length];
	auto start = high_resolution_clock::now();
	int last_index = -1;
	for (int z = 0; z < 10000; z++)
	{
		last_index = -1;
		fin >> s;
		f.clear();
		
		f[-1][0] = 1;
		f[-1].leader = 0;
		f[-1].zeros = 0;
		if (s[0] == '1')
		{
			f[0][0] = 1;
			f[0][1] = 1;
			f[0].leader = 1;
			f[0].zeros = 0;
			last_index = 0;
		}
		else
			return -1;
		for (int i = 1; i < strlen(s); i++)
		{
			int base = i - f[last_index].leader;
			int temp = 0;
			for (int j = 0; j <= f[last_index].leader - 1; j++)
			{
				temp ^= (s[base + j] - '0') & f[last_index][j];
			}
			if (s[i] - '0' != temp)
			{
				f[last_index].zeros = i - f[last_index].leader;
				//确定做加法的两个多项式
				int choose = -1;
				for (int j = last_index - 1; j >= -1; j--)
				{
					if (f[j].leader < f[last_index].leader && f[j].zeros >= 0)
					{
						choose = j;
						break;
					}
				}
				if (f[last_index].zeros > f[choose].zeros)
				{
					f[i] = f[last_index] * (f[last_index].zeros - f[choose].zeros);
					f[i] = f[i] + f[choose];
				}
				else
				{
					f[i] = f[choose] * (f[choose].zeros - f[last_index].zeros);
					f[i] = f[i] + f[last_index];
				}
				f[i].setleader();
				last_index = i;
			}
			else
				f[last_index].zeros++;

		}
		//f[last_index].print();
	}
	auto end = high_resolution_clock::now();
	f[last_index].print();
	cout << duration_cast<milliseconds>(end - start).count() / 1000.0 << endl;
}