#pragma once
#include <NTL/ZZ_pX.h>
#include <NTL/ZZX.h>
#include<NTL/ZZ.h>
NTL_CLIENT


#define Size 128
#define Size_n 62
#define Size_pub 2

long long Witness[3 * Size];

string AAA[Size];
int fff = 0;

class Cricom
{
public:
	string a[Size], b[Size], c[Size], sym[Size];
	int permutation[Size * 3] = { 1 };
	int L[Size_pub];
	long long p_i[Size_pub];
	long long qL[Size] = { 0 }, qR[Size] = { 0 }, qM[Size] = { 0 }, qO[Size] = { 0 };
	long long qC[Size] = { 0 };
	int number, I, L_i, sym_i;

	void empty_gate()
	{

	}

	void bool_gate(int n, int num)
	{
		for (int i = 0; i < num; i++)
		{
			a[I] = "b" + to_string(n) + to_string(i);
			b[I] = "b" + to_string(n) + to_string(i);
			c[I] = "empty";
			qL[I] = -1;
			qR[I] = 0;
			qM[I] = 1;
			qO[I] = 0;
			qC[I] = 0;
			I++;
			AAA[fff] = "bool_gate";
			fff++;
		}
	}

	void const_gate(long long number_pub, string str)
	{
		a[I] = str;
		b[I] = "empty";
		c[I] = "empty";
		qL[I] = 1;
		qR[I] = 0;
		qM[I] = 0;
		qO[I] = 0;
		qC[I] = -number_pub;
		I++;
		AAA[fff] = "const_gate";
		fff++;
	}

	void public_input(long long number_pub, string str)
	{
		L[L_i] = I;
		p_i[L_i] = number_pub;
		L_i++;
		a[I] = str;
		b[I] = "empty";
		c[I] = "empty";
		qL[I] = 1;
		qR[I] = 0;
		qM[I] = 0;
		qO[I] = 0;
		qC[I] = 0;
		I++;
		AAA[fff] = "public_input";
		fff++;
	}

	//public:
	void Setup()
	{
		I = 0;
		number = Size; L_i = 0;
		/*sym = new string[num];
		a = new string[num];
		b = new string[num];
		c = new string[num];
		qL = new int[num];
		qR = new int[num];
		qO = new int[num];
		qC = new int[num];
		qM = new int[num];*/
		sym_i = 0;
	}

	void End()
	{
		for (int i = I; i < Size; i++)
		{
			a[i] = "empty";
			b[i] = "empty";
			c[i] = "empty";
			AAA[fff] = "empty";
			fff++;
		}
		I = Size;
		string wire[Size * 3];
		for (int i = 0; i < Size; i++)
		{
			wire[i] = a[i];
			wire[i + Size] = b[i];
			wire[i + Size * 2] = c[i];
		}
		for (int i = 1; i <= Size * 3; i++)
		{
			permutation[i - 1] = i;
		}
		for (int i = 0; i < Size * 3; i++)
		{
			for (int j = i + 1; j < Size * 3; j++)
			{
				if (wire[i] == wire[j])
				{
					int aaa = permutation[i];
					permutation[i] = permutation[j];
					permutation[j] = aaa;
				}
			}
		}
	}

	void add(string ai, string bi, string ci, long long num_a, long long num_b)
	{
		a[I] = ai;
		b[I] = bi;
		c[I] = ci;
		qL[I] = num_a;
		qR[I] = num_b;
		qM[I] = 0;
		qO[I] = -1;
		qC[I] = 0;
		I++;
		AAA[fff] = "add";
		fff++;
	}

	void mul(string ai, string bi, string ci)
	{
		a[I] = ai;
		b[I] = bi;
		c[I] = ci;
		qL[I] = 0;
		qR[I] = 0;
		qM[I] = 1;
		qO[I] = -1;
		qC[I] = 0;
		I++;
		AAA[fff] = "mul";
		fff++;
	}
};
long long pow_2(int num)
{
	long long A;
	A = 1;
	for (int i = 0; i < num; i++)
	{
		A = A * 2;
	}
	return A;
}

void positive_no_gadget(Cricom& cs, long long max, long long min)
{
	cs.public_input(max, "max");
	cs.public_input(min, "min");
	cs.add("x", "a", "max", 1, 1);
	cs.add("min", "b", "x", 1, 1);
	cs.mul("a", "b", "c");
	cs.bool_gate(0, Size_n);
	int n = 1;
	cs.add("b00", "b01", "var" + to_string(0), 1, 2);
	for (int i = 2; i < Size_n-1; i++)
	{
		cs.add("var" + to_string(n-1), "b0" + to_string(i),"var"+ to_string(n), 1, pow_2(i));
		n++;
	}
	cs.add("var" + to_string(n - 1), "b061", "c", 1, pow_2(61));
	cs.End();
}

void Get_wit(Cricom& cs,long long x,long long MAX,long long MIN)
{
	int n = 0;
	int m = 1;
	long long a = MAX - x;
	long long var[500];
	long long b = x - MIN;
	long long c = a * b;
	long long bit[Size_n];
	bit[0] = c & 1;
	long long cc;
	cc = c;
	for (int i = 1; i < Size_n; i++)
	{
		cc = cc >> 1;
		bit[i] = cc & 1;
	}

	var[0] = bit[1] * pow_2(1) + bit[0];

	for (int i = 2; i < Size_n - 1; i++)
	{
		var[m] = var[m - 1] + bit[i] * pow_2(i);
		m++;
	}
	//a
	Witness[n] = MAX;
	n++;
	Witness[n] = MIN;
	n++;
	Witness[n] = x;
	n++;
	Witness[n] = MIN;
	n++;
	Witness[n] = a;
	n++;
	for (int i = 0; i < Size_n; i++)
	{
		Witness[n] = bit[i];
		n++;
	}
	Witness[n] = bit[0];
	n++;
	int p;
	for (int i = 0; i <= Size_n-3; i++)
	{
		if (i == Size_n - 3)
		{
			Witness[n] = var[i];
			n++;
			p = i + 1;//61
			break;
		}
		Witness[n] = var[i];
		n++;
	}
	//b
	Witness[n] = 0;
	n++;
	Witness[n] = 0;
	n++;
	Witness[n] = a;
	n++;
	Witness[n] = b;
	n++;
	Witness[n] = b;
	n++;
	for (int i = 0; i < Size_n; i++)
	{
		Witness[n] = bit[i];
		n++;
	}
	Witness[n] = bit[1];
	n++;
	for (int i = 2; i <= Size_n - 1; i++)
	{
		Witness[n] = bit[i];
		n++;
	}
	//c
	Witness[n] = 0;
	n++;
	Witness[n] = 0;
	n++;
	Witness[n] = MAX;
	n++;
	Witness[n] = x;
	n++;
	Witness[n] = c;
	n++;
	for (int j = 0; j < Size_n ; j++)
	{
		Witness[n] = 0;
		n++;
	}///////////////////////////
	for (int i = 0; i < 60; i++)
	{
		Witness[n] = var[i];
		n++;
	}
	Witness[n] = c;
	n++;
}


// #define Size1 16 


// ZZ Sets[Size1] = { ZZ(1),ZZ(2),ZZ(3),ZZ(4) , ZZ(5), ZZ(6), ZZ(7), ZZ(8), ZZ(9), ZZ(10), ZZ(11), ZZ(12), ZZ(13), ZZ(14), ZZ(15), ZZ(16), };


// void Circuit_get(Cricom& cs, ZZ* Set)
// {
// 	cout << sizeof(Sets) - sizeof(Sets[0]) << endl;
// 	for (int i = 0; i < (sizeof(Sets) - sizeof(Sets[0])) / 8 + 1; i++)
// 	{
// 		cs.public_input(Sets[i], "a" + to_string(i));
// 	}
// 	cs.bool_gate(0, (sizeof(Sets) - sizeof(Sets[0])) / 8 + 1);
// 	int n = 0;
// 	for (int i = 0; i < (sizeof(Sets) - sizeof(Sets[0])) / 8 + 1; i++)
// 	{
// 		cs.mul("a" + to_string(i), "b0" + to_string(i), "var" + to_string(n));
// 		n++;
// 	}
// 	int p = n;
// 	cs.add("var" + to_string(0), "var" + to_string(1), "var" + to_string(n));
// 	n++;
// 	for (int i = 2; i < (sizeof(Sets) - sizeof(Sets[0])) / 8; i++)
// 	{
// 		cs.add("var" + to_string(n - 1), "var" + to_string(n - p + 1), "var" + to_string(n));
// 		n++;
// 	}
// 	cs.add("var" + to_string(n - 1), "var" + to_string(n - p + 1), "x");
// 	cs.End();
// }

// void get_witness(Cricom& cs, ZZ X, ZZ* Set)
// {
// 	int* bit_set = new int((sizeof(Sets) - sizeof(Sets[0])) / 8 + 1);
// 	for (int i = 0; i < (sizeof(Sets) - sizeof(Sets[0])) / 8 + 1; i++)
// 	{
// 		if (X != Sets[i])
// 			bit_set[i] = 0;
// 		else bit_set[i] = 1;
// 	}
// 	ZZ var[500];
// 	for (int i = 0; i < (sizeof(Sets) - sizeof(Sets[0])) / 8 + 1; i++)
// 	{
// 		var[i] = bit_set[i] * Sets[i];
// 	}
// 	var[(sizeof(Sets) - sizeof(Sets[0])) / 8 + 1] = var[0] + var[1];
// 	for (int i = (sizeof(Sets) - sizeof(Sets[0])) / 8 + 1 + 1; i < 2 * ((sizeof(Sets) - sizeof(Sets[0])) / 8 + 1) - 2; i++)
// 	{
// 		var[i] = var[i - ((sizeof(Sets) - sizeof(Sets[0])) / 8)] + var[i - 1];
// 		cout << var[i] << endl;
// 	}
// 	//a
// 	int n = 0;
// 	for (int i = 0; i < (sizeof(Sets) - sizeof(Sets[0])) / 8 + 1; i++)
// 	{
// 		Witness[n] = Sets[i];
// 		n++;
// 	}
// 	for (int i = 0; i < (sizeof(Sets) - sizeof(Sets[0])) / 8 + 1; i++)
// 	{
// 		Witness[n] = bit_set[i];
// 		n++;
// 	}
// 	for (int i = 0; i < (sizeof(Sets) - sizeof(Sets[0])) / 8 + 1; i++)
// 	{
// 		Witness[n] = Sets[i];
// 		n++;
// 	}
// 	Witness[n] = var[0];
// 	n++;
// 	for (int i = (sizeof(Sets) - sizeof(Sets[0])) / 8 + 1; i < 2 * ((sizeof(Sets) - sizeof(Sets[0])) / 8 + 1) - 2; i++)
// 	{
// 		Witness[n] = var[i];
// 		n++;
// 	}
// 	int m = n;
// 	for (int i = m; i < Gate_Size; i++)
// 	{
// 		Witness[n] = ZZ(0);
// 		n++;
// 	}
// 	//b
// 	for (int i = 0; i < (sizeof(Sets) - sizeof(Sets[0])) / 8 + 1; i++)
// 	{
// 		Witness[n] = ZZ(0);
// 		n++;
// 	}
// 	for (int i = 0; i < (sizeof(Sets) - sizeof(Sets[0])) / 8 + 1; i++)
// 	{
// 		Witness[n] = bit_set[i];
// 		n++;
// 	}
// 	for (int i = 0; i < (sizeof(Sets) - sizeof(Sets[0])) / 8 + 1; i++)
// 	{
// 		Witness[n] = bit_set[i];
// 		n++;
// 	}
// 	Witness[n] = var[1];
// 	n++;
// 	for (int i = 2; i < (sizeof(Sets) - sizeof(Sets[0])) / 8 + 1; i++)
// 	{
// 		Witness[n] = var[i];
// 		n++;
// 	}
// 	for (int i = m; i < Gate_Size; i++)
// 	{
// 		Witness[n] = ZZ(0);
// 		n++;
// 	}
// 	//c
// 	for (int i = 0; i < (sizeof(Sets) - sizeof(Sets[0])) / 8 + 1; i++)
// 	{
// 		Witness[n] = ZZ(0);
// 		n++;
// 	}
// 	for (int i = 0; i < (sizeof(Sets) - sizeof(Sets[0])) / 8 + 1; i++)
// 	{
// 		Witness[n] = ZZ(0);
// 		n++;
// 	}
// 	for (int i = 0; i < 2 * ((sizeof(Sets) - sizeof(Sets[0])) / 8 + 1) - 2; i++)
// 	{
// 		Witness[n] = var[i];
// 		n++;
// 	}
// 	Witness[n] = X;
// 	cout << n << Witness[n] << endl;
// 	n++;
// 	for (int i = m; i < Gate_Size; i++)
// 	{
// 		Witness[n] = ZZ(0);
// 		n++;
// 	}
// }



// void positive_no_gadget_compare(Cricom& cs)
// {
// 	for (int i = 1; i < Size_n; i++)
// 	{
// 		cs.public_input(pow_2(i), "2^" + to_string(i));
// 	}
// 	cs.add("y", "a", "x");
// 	cs.bool_gate(0, Size_n);
// 	int n = 1;
// 	cs.mul("b0" + to_string(1), "2^" + to_string(1), "var0");
// 	for (int i = 2; i < Size_n; i++)
// 	{
// 		cs.mul("b0" + to_string(i), "2^" + to_string(i), "var" + to_string(n));
// 		n++;
// 		cs.add("var" + to_string(n - 2), "var" + to_string(n - 1), "var" + to_string(n));
// 		n++;
// 	}
// 	cs.add("var" + to_string(n - 1), "b00", "a");
// 	cs.End();
// }

// void Get_compare_wit(Cricom& cs, ZZ x, ZZ y)
// {
// 	int n = 0;
// 	int m = 1;
// 	ZZ var[500];
// 	ZZ a = x - y;
// 	ZZ bit[Size_n];
// 	bit[0] = ZZ(a & 1);
// 	ZZ aa;
// 	aa = a;
// 	for (int i = 1; i < Size_n; i++)
// 	{
// 		aa = aa >> 1;
// 		bit[i] = ZZ(aa & 1);
// 	}

// 	var[0] = ZZ(bit[1]) * pow_2(1);//cs.mul("b0" + to_string(1), "2^" + to_string(1), "var0");

// 	for (int i = 2; i < Size_n; i++)
// 	{
// 		var[m] = ZZ(bit[i]) * pow_2(i);//cs.mul("b0" + to_string(i), "2^" + to_string(i), "var" + to_string(n));
// 		m++;
// 		var[m] = var[m - 2] + var[m - 1];//cs.add("var" + to_string(n - 2), "var" + to_string(n - 1), "var" + to_string(n));
// 		m++;
// 	}
// 	//a
// 	for (int i = 1; i < Size_n; i++)
// 	{
// 		Witness[n] = pow_2(i);
// 		n++;
// 	}
// 	Witness[n] = y;
// 	n++;
// 	for (int i = 0; i < Size_n; i++)
// 	{
// 		Witness[n] = ZZ(bit[i]);
// 		n++;
// 	}
// 	Witness[n] = ZZ(bit[1]);
// 	n++;
// 	int p;
// 	for (int i = 0; i <= Size_n - 2; i++)
// 	{
// 		if (i == Size_n - 2)
// 		{
// 			Witness[n] = ZZ(var[2 * i]);
// 			n++;
// 			p = 2 * i + 1;
// 			break;
// 		}
// 		Witness[n] = ZZ(bit[i + 2]);
// 		n++;
// 		Witness[n] = ZZ(var[2 * i]);
// 		n++;
// 	}
// 	int v = n;
// 	for (int i = 0; i < Gate_Size - v; i++)
// 	{
// 		Witness[n] = ZZ(0);
// 		n++;
// 	}
// 	//b
// 	for (int i = 1; i < Size_n; i++)
// 	{
// 		Witness[n] = ZZ(0);
// 		n++;
// 	}
// 	Witness[n] = a;
// 	n++;
// 	for (int i = 0; i < Size_n; i++)
// 	{
// 		Witness[n] = ZZ(bit[i]);
// 		n++;
// 	}
// 	Witness[n] = ZZ(pow_2(1));
// 	n++;
// 	for (int i = 0; i <= Size_n - 3; i++)
// 	{
// 		Witness[n] = ZZ(pow_2(i + 2));
// 		n++;
// 		Witness[n] = ZZ(var[2 * i + 1]);
// 		n++;
// 	}
// 	Witness[n] = ZZ(bit[0]);
// 	n++;
// 	for (int i = 0; i < Gate_Size - v; i++)
// 	{
// 		Witness[n] = ZZ(0);
// 		n++;
// 	}
// 	//c
// 	for (int i = 1; i < Size_n; i++)
// 	{
// 		Witness[n] = ZZ(0);
// 		n++;
// 	}
// 	Witness[n] = ZZ(x);
// 	n++;
// 	for (int j = 0; j < Size_n; j++)
// 	{
// 		Witness[n] = ZZ(0);
// 		n++;
// 	}
// 	for (int i = 0; i <= 124; i++)
// 	{
// 		Witness[n] = ZZ(var[i]);
// 		n++;
// 	}
// 	Witness[n] = ZZ(a);
// 	n++;
// 	for (int i = 0; i < Gate_Size - v; i++)
// 	{
// 		Witness[n] = ZZ(0);
// 		n++;
// 	}
// }
