#include <assert.h>
#include <cybozu/sha2.hpp>
#include <iomanip>
#include<omp.h>
#include"function.h"
#include <jsoncpp/json/json.h>
#include"threadpool.h"
NTL_CLIENT
using namespace std;
using namespace mcl::bn;

SHA256_CTX ctx;
uint8_t buf[65] = { 0 };



void verify()
{
	initPairing(mcl::BN_SNARK1);
    G1 g1;
	G2 g2;
    Fp2 g2x, g2y;
    Fp g2xa, g2xb, g2ya, g2yb, pair1x, pair1y, pair2x, pair2y;
    g2xa.setStr("10857046999023057135944570762232829481370756359578518086990519993285655852781", 10);
    
    g2xb.setStr("11559732032986387107991004021392285783925812861821192530917403151452391805634", 10);
    g2ya.setStr("8495653923123431417604973247489272438418190587263600148770280649306958101930", 10);
    g2yb.setStr("4082367875863433681332203403145435568316851327593401208105741076214120093531", 10);
    g2x.set(g2xa, g2xb);
    g2y.set(g2ya, g2yb);
    g1.set(1, 2);
    g2.set(g2x, g2y);

    ZZ p = conv<ZZ>("21888242871839275222246405745257275088548364400416034343698204186575808495617");
    ZZ_p::init(p);

	ThreadPool pool(4);
	int n=128;
	Fr beta_temp;
	ZZ_p beta, gamma, alpha, zeta, nu, u, Z_H_zeta, L1_zeta, PI_zeta, r0, a_zeta, b_zeta, c_zeta, S_sigma1_zeta, S_sigma2_zeta, z_omega_zeta, k1, k2, omega;
	G1 D_commit, F_commit, E_commit;
	G1 a_commit, b_commit, c_commit, z_commit, t_lo_commit, t_mid_commit, t_hi_commit, W_zeta_poly_commit, W_zeta_omega_poly_commit
		, q_C_commit, q_L_commit, q_M_commit, q_O_commit, q_R_commit, S_sigma1_commit, S_sigma2_commit, S_sigma3_commit;
	G2 g2_commit; 
	ZZ_pX PI;
	Vec<ZZ_p> domain,public_input;
	char beta_zz[78] ,a_zeta_zz[78],b_zeta_zz[78],S_sigma1_zeta_zz[78],S_sigma2_zeta_zz[78],c_zeta_zz[78],z_omega_zeta_zz[78];
	domain.SetLength(n);
	public_input.SetLength(n);

	
	ifstream ifs("snark.json");
	Json::Reader rd;
	Json::Value snark;
	rd.parse(ifs, snark);
	string a_commit_str=snark["a_commit"].asString();
	string b_commit_str=snark["b_commit"].asString();
	string c_commit_str=snark["c_commit"].asString();
	string z_commit_str=snark["z_commit"].asString();
	string t_lo_commit_str=snark["t_lo_commit"].asString();
	string t_mid_commit_str=snark["t_mid_commit"].asString();
	string t_hi_commit_str=snark["t_hi_commit"].asString();
	string W_zeta_poly_commit_str=snark["W_zeta_poly_commit"].asString();
	string W_zeta_omega_poly_commit_str=snark["W_zeta_omega_poly_commit"].asString();
	string a_zeta_str=snark["a_zeta"].asString();
	string b_zeta_str=snark["b_zeta"].asString();
	string c_zeta_str=snark["c_zeta"].asString();
	string S_sigma1_zeta_str=snark["S_sigma1_zeta"].asString();
	string S_sigma2_zeta_str=snark["S_sigma2_zeta"].asString();
	string z_omega_zeta_str=snark["z_omega_zeta"].asString();

	string maxstr=snark["PI"][0].asString();
	string minstr=snark["PI"][1].asString();
	
	ifstream ifs1("setup.json");
	Json::Reader key_rd;
	Json::Value setup;
	key_rd.parse(ifs1, setup);
	
	
	string q_C_commit_str=setup["q_C_commit"].asString();
	string q_O_commit_str=setup["q_O_commit"].asString();
	string q_R_commit_str=setup["q_R_commit"].asString();
	string q_M_commit_str=setup["q_M_commit"].asString();
	string q_L_commit_str=setup["q_L_commit"].asString();
	string S_sigma1_commit_str=setup["S_sigma1_commit"].asString();
	string S_sigma2_commit_str=setup["S_sigma2_commit"].asString();
	string S_sigma3_commit_str=setup["S_sigma3_commit"].asString();
	string g2_commit_str=setup["g2_commit"].asString();
	string k1_str=setup["k1"].asString();
	string k2_str=setup["k2"].asString();
	string omega_str=setup["omega"].asString();

	
	



	a_commit.deserializeHexStr(a_commit_str);
	b_commit.deserializeHexStr(b_commit_str);
	c_commit.deserializeHexStr(c_commit_str);
	z_commit.deserializeHexStr(z_commit_str);
	t_lo_commit.deserializeHexStr(t_lo_commit_str);
	t_mid_commit.deserializeHexStr(t_mid_commit_str);
	t_hi_commit.deserializeHexStr(t_hi_commit_str);
	W_zeta_poly_commit.deserializeHexStr(W_zeta_poly_commit_str);
	W_zeta_omega_poly_commit.deserializeHexStr(W_zeta_omega_poly_commit_str);
	a_zeta=conv<ZZ_p>(a_zeta_str.c_str());
	b_zeta=conv<ZZ_p>(b_zeta_str.c_str());
	c_zeta=conv<ZZ_p>(c_zeta_str.c_str());
	S_sigma1_zeta=conv<ZZ_p>(S_sigma1_zeta_str.c_str());
	S_sigma2_zeta=conv<ZZ_p>(S_sigma2_zeta_str.c_str());
	z_omega_zeta=conv<ZZ_p>(z_omega_zeta_str.c_str());
	public_input[0]=conv<ZZ_p>(minstr.c_str());
	public_input[1]=conv<ZZ_p>(maxstr.c_str());
	
	
	

	q_C_commit.deserializeHexStr(q_C_commit_str);
	q_O_commit.deserializeHexStr(q_O_commit_str);
	q_R_commit.deserializeHexStr(q_R_commit_str);
	q_M_commit.deserializeHexStr(q_M_commit_str);
	q_L_commit.deserializeHexStr(q_L_commit_str);
	S_sigma1_commit.deserializeHexStr(S_sigma1_commit_str);
	S_sigma2_commit.deserializeHexStr(S_sigma2_commit_str);
	S_sigma3_commit.deserializeHexStr(S_sigma3_commit_str);
	g2_commit.deserializeHexStr(g2_commit_str);
	k1=conv<ZZ_p>(k1_str.c_str());
	k2=conv<ZZ_p>(k2_str.c_str());
	omega=conv<ZZ_p>(omega_str.c_str());
	







	//********************** Check 3 *****************************
	
	
		SHA256_Init(&ctx);
		SHA256_Update(&ctx, a_commit_str.c_str(), 64);
		SHA256_Update(&ctx, b_commit_str.c_str(), 64);
		SHA256_Update(&ctx, c_commit_str.c_str(), 64);
		SHA256_Final(buf, &ctx);
		beta = hashtoZZ(buf);
		
		
		SHA256_Init(&ctx);
		SHA256_Update(&ctx, a_commit_str.c_str(), 64);
		SHA256_Update(&ctx, b_commit_str.c_str(), 64);
		SHA256_Update(&ctx, c_commit_str.c_str(), 64);
		ZZtostr(beta_zz,beta);
		SHA256_Update(&ctx, beta_zz, 64);
		SHA256_Final(buf, &ctx);
		
		gamma = hashtoZZ(buf);
		
		
		SHA256_Init(&ctx);
		SHA256_Update(&ctx, a_commit_str.c_str(), 64);
		SHA256_Update(&ctx, b_commit_str.c_str(), 64);
		SHA256_Update(&ctx, c_commit_str.c_str(), 64);
		SHA256_Update(&ctx, z_commit_str.c_str(), 64);
		SHA256_Final(buf, &ctx);
		alpha = hashtoZZ(buf);
		

		SHA256_Init(&ctx);
		SHA256_Update(&ctx, a_commit_str.c_str(), 64);
		SHA256_Update(&ctx, b_commit_str.c_str(), 64);
		SHA256_Update(&ctx, c_commit_str.c_str(), 64);
		SHA256_Update(&ctx, z_commit_str.c_str(), 64);
		SHA256_Update(&ctx, t_lo_commit_str.c_str(), 64);
		SHA256_Update(&ctx, t_mid_commit_str.c_str(), 64);
		SHA256_Update(&ctx, t_hi_commit_str.c_str(), 64);
		SHA256_Final(buf, &ctx);
		zeta = hashtoZZ(buf);
		
		SHA256_Init(&ctx);
		SHA256_Update(&ctx, a_commit_str.c_str(), 64);
		SHA256_Update(&ctx, b_commit_str.c_str(), 64);
		SHA256_Update(&ctx, c_commit_str.c_str(), 64);
		SHA256_Update(&ctx, z_commit_str.c_str(), 64);
		SHA256_Update(&ctx, t_lo_commit_str.c_str(), 64);
		SHA256_Update(&ctx, t_mid_commit_str.c_str(), 64);
		SHA256_Update(&ctx, t_hi_commit_str.c_str(), 64);
		
		
		SHA256_Update(&ctx,a_zeta_str.c_str(), 64);
		SHA256_Update(&ctx, b_zeta_str.c_str(), 64);
		SHA256_Update(&ctx,c_zeta_str.c_str(), 64);
		SHA256_Update(&ctx, S_sigma1_zeta_str.c_str(), 64);
		SHA256_Update(&ctx,S_sigma2_zeta_str.c_str(), 64);
		SHA256_Update(&ctx, z_omega_zeta_str.c_str(), 64);
		SHA256_Final(buf, &ctx);
		nu = hashtoZZ(buf);
		
		
		SHA256_Init(&ctx);
		SHA256_Update(&ctx, a_commit_str.c_str(), 64);
		SHA256_Update(&ctx, b_commit_str.c_str(), 64);
		SHA256_Update(&ctx, c_commit_str.c_str(), 64);
		SHA256_Update(&ctx, z_commit_str.c_str(), 64);
		SHA256_Update(&ctx, t_lo_commit_str.c_str(), 64);
		SHA256_Update(&ctx, t_mid_commit_str.c_str(), 64);
		SHA256_Update(&ctx, t_hi_commit_str.c_str(), 64);
		SHA256_Update(&ctx, W_zeta_poly_commit_str.c_str(), 64);
		SHA256_Update(&ctx, W_zeta_omega_poly_commit_str.c_str(), 64);
		
		
		SHA256_Update(&ctx,a_zeta_str.c_str(), 64);
		SHA256_Update(&ctx, b_zeta_str.c_str(), 64);
		SHA256_Update(&ctx,c_zeta_str.c_str(), 64);
		SHA256_Update(&ctx, S_sigma1_zeta_str.c_str(), 64);
		SHA256_Update(&ctx,S_sigma2_zeta_str.c_str(), 64);
		SHA256_Update(&ctx, z_omega_zeta_str.c_str(), 64);
		SHA256_Final(buf, &ctx);
		u = hashtoZZ(buf);
	
	//********************** Step 5 *****************************

	Z_H_zeta = power(zeta, n) - 1;
	


	//********************** Step 6 *****************************
	L1_zeta = Z_H_zeta / (n * (zeta - 1));
	//L1_zeta = omega * Z_H_zeta / (n * (zeta - omega));

	//********************** Step 7 *****************************
	
	for (int i = 0; i < n; i++)
	{
		domain[i] = power(omega, i);
	}
	
	
	interpolate(PI, domain, -1*public_input);
	

	eval(PI_zeta, PI, zeta);


	//********************** Step 8 *****************************

	r0 = PI_zeta - L1_zeta * power(alpha, 2) - alpha * (a_zeta + beta * S_sigma1_zeta + gamma) * (b_zeta + beta * S_sigma2_zeta + gamma) * (c_zeta + gamma) * z_omega_zeta;



	//********************** Step 9 *****************************
	
	
	
	D_commit = q_M_commit *ZZtoFr(a_zeta * b_zeta) + q_L_commit* ZZtoFr(a_zeta) + q_R_commit* ZZtoFr(b_zeta) +
		q_O_commit* ZZtoFr(c_zeta) + q_C_commit +
		z_commit* ZZtoFr((a_zeta + beta * zeta + gamma) * (b_zeta + beta * k1 * zeta + gamma) * (c_zeta + beta * k2 * zeta + gamma) * alpha + L1_zeta * power(alpha, 2) + u) +
		-(S_sigma3_commit* ZZtoFr((a_zeta + beta * S_sigma1_zeta + gamma) * (b_zeta + beta * S_sigma2_zeta + gamma) * alpha * beta * z_omega_zeta)) +
		-((t_lo_commit + t_mid_commit* ZZtoFr(power(zeta, n)) + t_hi_commit* ZZtoFr(power(zeta, 2 * n)))* ZZtoFr(Z_H_zeta));


	//********************** Step 10 *****************************


	
		F_commit = D_commit + a_commit* ZZtoFr(nu) + b_commit* ZZtoFr(power(nu, 2)) + c_commit* ZZtoFr(power(nu, 3)) + S_sigma1_commit* ZZtoFr(power(nu, 4)) + S_sigma2_commit* ZZtoFr(power(nu, 5));
	
	//********************** Step 11 *****************************
	
	
	E_commit = g1* ZZtoFr(nu * a_zeta + power(nu, 2) * b_zeta + power(nu, 3) * c_zeta + power(nu, 4) * S_sigma1_zeta + power(nu, 5) * S_sigma2_zeta + u * z_omega_zeta - r0);
	
	//********************** Step 12 *****************************


	G1 pair1, pair2;
	Fp12 pair1_res, pair2_res;

	pair1 = W_zeta_poly_commit + W_zeta_omega_poly_commit* ZZtoFr(u);
	pair2 = W_zeta_poly_commit* ZZtoFr(zeta) + W_zeta_omega_poly_commit*ZZtoFr(u * zeta * omega) + F_commit + (-E_commit);

	
	
	
	pool.enqueue([pair1,g2_commit,&pair1_res]
	{
	pairing(pair1_res, pair1, g2_commit);
	});
	
	pool.enqueue([pair2,g2,&pair2_res]
	{
	pairing(pair2_res, pair2, g2);
	
	});
	
	
	pool.Stop();

	
	if (pair1_res== pair2_res)
	{
		cout << "pair_success!" << endl;
	}
	else
	{
		cout << "pair_fail" << endl;
	}
	
}


int main()
{
	
    verify();

}