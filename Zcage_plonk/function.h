#pragma once
#include <mcl/bls12_381.hpp>
#include <NTL/ZZ_pX.h>
#include <NTL/ZZX.h>
#include <sstream>
#include <iostream>
#include <fstream>

NTL_CLIENT
using namespace mcl::bn;

char hash_buf[64] = { 0 };



struct SNARK
{
	G1 a_commit;
	G1 b_commit;
	G1 c_commit;
	G1 z_commit;
	G1 t_lo_commit;
	G1 t_mid_commit;
	G1 t_hi_commit;
	G1 W_zeta_poly_commit;
	G1 W_zeta_omega_poly_commit;
	ZZ_p a_zeta;
	ZZ_p b_zeta;
	ZZ_p c_zeta;
	ZZ_p S_sigma1_zeta;
	ZZ_p S_sigma2_zeta;
	ZZ_p z_omega_zeta;
};

struct preprocessed
{
	int n;
	G1 q_M_commit;
	G1 q_L_commit;
	G1 q_R_commit;
	G1 q_O_commit;
	G1 q_C_commit;
	G1 S_sigma1_commit;
	G1 S_sigma2_commit;
	G1 S_sigma3_commit;
	G1 g1;
	G2 g2;
	G2 g2_commit;
	ZZ_pX PI;
	ZZ_p k1;
	ZZ_p k2;
	ZZ_p omega;
};



ZZ_p get_omega(ZZ n)
{
	ZZ_p ran = random_ZZ_p();
	ZZ_p g = power(ran, (ran.modulus() - 1) / n);
	if ((g / 2 == 1) || g == 1)
	{
		return get_omega(n);
	}
	assert(power(g, n) == 1);
	assert(power(g, n / 2) != 1);
	return g;
}

void eval_domain(vec_ZZ_p& domain, int n)
{
	//ZZ_p omega_base= get_omega(conv<ZZ>("268435456"));
	ZZ_p omega_base = conv<ZZ_p>("1247783447479827439192423861283237434044953076499070802508126931013410047146");
	domain.SetLength(conv<int>(n));
	ZZ_p omega = power(omega_base, (conv<ZZ>("268435456") / n));
	for (int i = 0; i < n; i++)
	{
		domain[i] = power(omega, i);
	}

}
ZZ_p FrtoZZ(Fr zzp)
{

	stringstream stream;
	ZZ_p res;
	char result[200];
	stream << zzp;
	stream >> result;
	res = conv<ZZ_p>(result);
	stream.clear();
	return res;
}

// char* ZZtostr(ZZ_p input)
// {
// 	stringstream stream;
// 	char result[100];
// 	stream << input;
// 	stream >> result;
// 	stream.clear();
// 	return result;
// }

void ZZtostr(char * res,ZZ_p input)
{
	stringstream stream;
	stream << input;
	stream >> res;
	stream.clear();
}

void ZZtostr(string& res,ZZ_p input)
{
	stringstream stream;
	stream << input;
	stream >> res;
	stream.clear();
}

// void ZZtostr(ZZ_p input)
// {
	
// 	stringstream stream;
// 	stream << input;
// 	stream >> hash_buf;
	
// }

Fr ZZtoFr(ZZ_p zzp)
{

	stringstream stream;
	Fr res;
	char result[200];
	stream << zzp;
	stream >> result;
	res.setStr(result, 10);
	stream.clear();
	return res;
}


void vec_zz_copy(Vec<ZZ_p>& res, Vec<ZZ_p> in, int start, int len)
{
	for (int i = 0; i < len; i++)
		res[i] = in[start + i];
}


void generate_CRS(Vec<G1>& CRS, G1 g1, ZZ_p secret, int n)
{

	for (int i = 0; i < n + 6; i++)
	{
		CRS[i] = g1 * ZZtoFr((power(secret, i)));
	}

}




void get_poly_coeff(ZZ_pX in, vector<Fr> yVec)
{
	int poly_len = deg(in) + 1;

	for (int i = 0; i < poly_len; i++)
	{
		
		yVec[i] = ZZtoFr(coeff(in, i));
		//cout<<"yVec:"<<yVec[i]<<" in: "<<ZZtoFr(coeff(in, i))<<endl;
	}
	cout<<"fr_vec1: "<<yVec[1]<<endl;
	cout<<"in:"<<in[1].modulus()<<endl;
	cout<<"fr_mod: "<<yVec[1].getModulo()<<endl;
}

// void poly_commitment(G1& commit, Vec<G1> CRS, ZZ_pX poly)
// {
// 	//vector<Fr> yVec;
// 	int len = deg(poly) + 1;
// 	commit.clear();
// 	//#pragma omp parallel for
// 	for (int i = 0; i < len; i++)
// 		commit = commit + (CRS[i] * ZZtoFr(coeff(poly, i)));


// }

void poly_commitment(G1& commit, Vec<G1> CRS, ZZ_pX poly)
{
	//vector<Fr> yVec;
	int poly_len = deg(poly) + 1;
	vector<Fr> temp(poly_len);
	for (int i = 0; i < poly_len; i++)
	{
		
		temp[i] = ZZtoFr(coeff(poly, i));
		//cout<<"yVec:"<<yVec[i]<<" in: "<<ZZtoFr(coeff(in, i))<<endl;
	}
	commit.clear();
	//#pragma omp parallel for
	G1::mulVec(commit, CRS.data(), temp.data(), poly_len);

}




void poly_mut_vec(ZZ_pX& res, ZZ_pX poly, Vec<ZZ_p> vec, ZZ_p omega, int n)
{

	for (int i = 0; i < n; i++)
	{
		SetCoeff(res, i, coeff(poly, i) * vec[i]);
	}
	SetCoeff(res, n, coeff(poly, n) * power(omega, n));
	SetCoeff(res, n + 1, coeff(poly, n + 1) * power(omega, n + 1));
	SetCoeff(res, n + 2, coeff(poly, n + 2) * power(omega, n + 2));

}

void divide_three_part(ZZ_pX& t_lo, ZZ_pX& t_mid, ZZ_pX& t_hi, ZZ_pX t, int n)
{
	int i;
	for (int i = 0; i < n; i++)
	{
		SetCoeff(t_lo, i, t[i]);
		SetCoeff(t_mid, i, t[n + i]);

	}
	for (i = 2 * n; i < deg(t) + 1; i++)
		SetCoeff(t_hi, i - 2 * n, t[i]);
}

void perm_domain(Vec<ZZ_p>& res, ZZ_pX& S_sigma1, ZZ_pX& S_sigma2, ZZ_pX& S_sigma3, Vec<ZZ_p> domain, int* permation, ZZ_p k1, ZZ_p k2)
{
	int len = domain.length();
	Vec<ZZ_p> a, b, perm_domain_a, perm_domain_b, perm_domain_c;
	Vec<ZZ_p> temp;
	perm_domain_a.SetLength(len);
	perm_domain_b.SetLength(len);
	perm_domain_c.SetLength(len);

	a = k1 * domain;
	b = k2 * domain;
	temp = domain;
	temp.append(a);
	temp.append(b);
	for (int i = 0; i < res.length(); i++)
		res[i] = temp[permation[i] - 1];
	for (int i = 0; i < len; i++)
	{
		perm_domain_a[i] = res[i];
		perm_domain_b[i] = res[i + len];
		perm_domain_c[i] = res[i + 2 * len];
	}
	interpolate(S_sigma1, domain, perm_domain_a);
	interpolate(S_sigma2, domain, perm_domain_b);
	interpolate(S_sigma3, domain, perm_domain_c);
}

ZZ_p hashtoZZ(unsigned char* hash)
{
	mpz_class res;
	char buf[65] = { 0 };
	char tmp[3] = { 0 };
	for (int i = 0; i < 32; i++)
	{
		sprintf(tmp, "%02x", hash[i]);
		strcat(buf, tmp);
	}
	res.set_str(buf, 16);
	string ss;
	ss = res.get_str();
	ZZ_p r;
	r = conv<ZZ_p>(ss.c_str());

	return r;

}
