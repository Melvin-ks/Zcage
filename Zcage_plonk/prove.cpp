#include <assert.h>
#include <cybozu/sha2.hpp>
#include <iomanip>
#include <chrono>
#include"circom.h"
#include<omp.h>
#include"function.h"
#include <jsoncpp/json/json.h>
#include"para.h"
#include "sm2_create_key_pair.h"
#include "sm2_sign_and_verify.h"

NTL_CLIENT
using namespace std;
using namespace mcl::bn;

SHA256_CTX ctx;
uint8_t buf[65] = { 0 };




	
void proof(
	int n,
	Vec<ZZ_p> witness,
	Vec<ZZ_p> domain,
	Vec<G1> CRS,
	Vec<ZZ_p> sigma,
	ZZ_p k1,
	ZZ_p k2,
	ZZ_pX q_M,
	ZZ_pX q_L,
	ZZ_pX q_R,
	ZZ_pX q_O,
	ZZ_pX q_C,
	//ZZ_pX PI,
	Vec<ZZ_p> public_input,
	ZZ_pX S_sigma1,
	ZZ_pX S_sigma2,
	ZZ_pX S_sigma3)
{
	auto prove_start = std::chrono::steady_clock::now();

    
	
	int i;
	Fr beta_temp;
	ZZ_pX a_poly, b_poly, c_poly, Z_H, z_poly, z_1, ax, bx, cx, t_poly, z_w_poly, t_lo, t_mid, t_hi, r_poly, W_zeta_poly, W_zeta_omega_poly, L1,PI;
	ZZ_p alpha, beta, gamma, nume, deno, acc, zeta, nu, a_zeta, b_zeta, c_zeta, S_sigma1_zeta, S_sigma2_zeta, z_omega_zeta, L1_zeta, PI_zeta, Z_H_zeta;
	Vec<ZZ_p> blind, z, witness_a, witness_b, witness_c, L_1_coeff;
	G1 a_commit, b_commit, c_commit, z_commit, t_lo_commit, t_mid_commit, t_hi_commit, W_zeta_poly_commit, W_zeta_omega_poly_commit;
	char beta_zz[78],a_zeta_zz[78],b_zeta_zz[78],S_sigma1_zeta_zz[78],S_sigma2_zeta_zz[78],c_zeta_zz[78],z_omega_zeta_zz[65];

	witness_a.SetLength(n);
	witness_b.SetLength(n);
	witness_c.SetLength(n);
	z.SetLength(n);
	L_1_coeff.SetLength(n);
	public_input.SetLength(n);
	vec_zz_copy(witness_a, witness, 0, n);
	vec_zz_copy(witness_b, witness, n, n);
	vec_zz_copy(witness_c, witness, 2 * n, n);
	//cout << witness_b.length() << endl;
	interpolate(ax, domain, witness_a);
	interpolate(bx, domain, witness_b);
	interpolate(cx, domain, witness_c);
	interpolate(PI, domain, -1*public_input);
	
	SetCoeff(Z_H, 0, -1);
	SetCoeff(Z_H, n, 1);
	
	//**************************** Round1 ***********************************
	
	blind.SetLength(9);
	for (int i = 0; i < 9; i++)
	{
		blind[i] = random();
	}
	ZZ_pX mm(blind[0]);
	
	a_poly = (ZZ_pX(INIT_MONO, 1, blind[0]) + ZZ_pX(blind[1])) * Z_H + ax;
	b_poly = (ZZ_pX(INIT_MONO, 1, blind[2]) + ZZ_pX(blind[3])) * Z_H + bx;
	c_poly = (ZZ_pX(INIT_MONO, 1, blind[4]) + ZZ_pX(blind[5])) * Z_H + cx;
	
	poly_commitment(a_commit, CRS, a_poly);
	
	poly_commitment(b_commit, CRS, b_poly);
	poly_commitment(c_commit,  CRS, c_poly);
	
	//**************************** Round2 ***********************************
	
	SHA256_Init(&ctx);
	SHA256_Update(&ctx, a_commit.serializeToHexStr().c_str(), 64);
	SHA256_Update(&ctx, b_commit.serializeToHexStr().c_str(), 64);
	SHA256_Update(&ctx, c_commit.serializeToHexStr().c_str(), 64);
	SHA256_Final(buf, &ctx);
	
	beta=hashtoZZ(buf);
	//beta = conv<ZZ_p>(" 7");
	
	SHA256_Init(&ctx);
	SHA256_Update(&ctx, a_commit.serializeToHexStr().c_str(), 64);
	SHA256_Update(&ctx, b_commit.serializeToHexStr().c_str(), 64);
	SHA256_Update(&ctx, c_commit.serializeToHexStr().c_str(), 64);
	
	
	
	ZZtostr(beta_zz,beta);
	
	SHA256_Update(&ctx, beta_zz, 64);
	
	SHA256_Final(buf, &ctx);
	gamma = hashtoZZ(buf);
	
	SetCoeff(z_1, 2, blind[6]);
	SetCoeff(z_1, 1, blind[7]);
	SetCoeff(z_1, 0, blind[8]);
	z[0] = 1;
	acc = 1;
	for (i = 1; i < n; i++)
	{
		nume = (witness[i - 1] + beta * domain[i - 1] + gamma) * (witness[n + i - 1] + beta * k1 * domain[i - 1] + gamma) * (witness[2 * n + i - 1] + beta * k2 * domain[i - 1] + gamma);
		deno = (witness[i - 1] + beta * sigma[i - 1] + gamma) * (witness[n + i - 1] + beta * sigma[n + i - 1] + gamma) * (witness[2 * n + i - 1] + beta * sigma[2 * n + i - 1] + gamma);
		acc *= nume / deno;
		z[i] = acc;

	}
	
	
	interpolate(z_poly, domain, z);
	
	z_poly += z_1 * Z_H;
	
	
	poly_commitment(z_commit,  CRS, z_poly);
	//**************************** Round3 ***********************************
	long long round3 = clock();
	SHA256_Init(&ctx);
	SHA256_Update(&ctx, a_commit.serializeToHexStr().c_str(), 64);
	SHA256_Update(&ctx, b_commit.serializeToHexStr().c_str(), 64);
	SHA256_Update(&ctx, c_commit.serializeToHexStr().c_str(), 64);
	SHA256_Update(&ctx, z_commit.serializeToHexStr().c_str(), 64);
	SHA256_Final(buf, &ctx);
	alpha = hashtoZZ(buf);
	poly_mut_vec(z_w_poly, z_poly, domain, domain[1], n);
	L_1_coeff[0] = 1;
	
	interpolate(L1, domain, L_1_coeff);
	
	
	


	t_poly = a_poly * b_poly * q_M + a_poly * q_L + b_poly * q_R + c_poly * q_O + PI + q_C;
	t_poly += (a_poly + ZZ_pX(INIT_MONO, 1, beta) + ZZ_pX(gamma)) * (b_poly + ZZ_pX(INIT_MONO, 1, beta * k1) + ZZ_pX(gamma)) * (c_poly + ZZ_pX(INIT_MONO, 1, beta * k2) + ZZ_pX(gamma)) * z_poly * alpha;
	t_poly -= (a_poly + beta * S_sigma1 + ZZ_pX(gamma)) * (b_poly + beta * S_sigma2 + ZZ_pX(gamma)) * (c_poly + beta * S_sigma3 + ZZ_pX(gamma)) * z_w_poly * alpha;
	t_poly += (z_poly - 1) * L1 * power(alpha, 2);
	t_poly /= Z_H;

	
	divide_three_part(t_lo, t_mid, t_hi, t_poly, n);
	
	
	
	poly_commitment(t_lo_commit, CRS, t_lo);
	poly_commitment(t_mid_commit, CRS, t_mid);
	poly_commitment(t_hi_commit,  CRS, t_hi);
	



	//**************************** Round4 ***********************************
	
	SHA256_Init(&ctx);
	SHA256_Update(&ctx, a_commit.serializeToHexStr().c_str(), 64);
	SHA256_Update(&ctx, b_commit.serializeToHexStr().c_str(), 64);
	SHA256_Update(&ctx, c_commit.serializeToHexStr().c_str(), 64);
	SHA256_Update(&ctx, z_commit.serializeToHexStr().c_str(), 64);
	SHA256_Update(&ctx, t_lo_commit.serializeToHexStr().c_str(), 64);
	SHA256_Update(&ctx, t_mid_commit.serializeToHexStr().c_str(), 64);
	SHA256_Update(&ctx, t_hi_commit.serializeToHexStr().c_str(), 64);
	SHA256_Final(buf, &ctx);
	zeta = hashtoZZ(buf);
	
	
	eval(a_zeta, a_poly, zeta);
	eval(b_zeta, b_poly, zeta);
	eval(c_zeta, c_poly, zeta);
	eval(S_sigma1_zeta, S_sigma1, zeta);
	eval(S_sigma2_zeta, S_sigma2, zeta);
	eval(z_omega_zeta, z_poly, zeta * domain[1]);
	


	//**************************** Round5 ***********************************
	
	SHA256_Init(&ctx);
	SHA256_Update(&ctx, a_commit.serializeToHexStr().c_str(), 64);
	SHA256_Update(&ctx, b_commit.serializeToHexStr().c_str(), 64);
	SHA256_Update(&ctx, c_commit.serializeToHexStr().c_str(), 64);
	SHA256_Update(&ctx, z_commit.serializeToHexStr().c_str(), 64);
	SHA256_Update(&ctx, t_lo_commit.serializeToHexStr().c_str(), 64);
	SHA256_Update(&ctx, t_mid_commit.serializeToHexStr().c_str(), 64);
	SHA256_Update(&ctx, t_hi_commit.serializeToHexStr().c_str(), 64);
	ZZtostr(a_zeta_zz,a_zeta);
	ZZtostr(b_zeta_zz,b_zeta);
	ZZtostr(c_zeta_zz,c_zeta);
	ZZtostr(S_sigma1_zeta_zz,S_sigma1_zeta);
	ZZtostr(S_sigma2_zeta_zz,S_sigma2_zeta);
	ZZtostr(z_omega_zeta_zz,z_omega_zeta);
	
	SHA256_Update(&ctx,a_zeta_zz, 64);
	SHA256_Update(&ctx, b_zeta_zz, 64);
	SHA256_Update(&ctx,c_zeta_zz, 64);
	SHA256_Update(&ctx, S_sigma1_zeta_zz, 64);
	SHA256_Update(&ctx,S_sigma2_zeta_zz, 64);
	SHA256_Update(&ctx, z_omega_zeta_zz, 64);
	SHA256_Final(buf, &ctx);
	nu = hashtoZZ(buf);
	

	eval(PI_zeta, PI, zeta);
	eval(Z_H_zeta, Z_H, zeta);
	L1_zeta = Z_H_zeta / (n * (zeta - 1));


	r_poly = a_zeta * b_zeta * q_M + a_zeta * q_L + b_zeta * q_R + c_zeta * q_O + PI_zeta + q_C;
	r_poly += (a_zeta + beta * zeta + gamma) * (b_zeta + beta * k1 * zeta + gamma) * (c_zeta + beta * k2 * zeta + gamma) * z_poly * alpha;
	r_poly -= (a_zeta + beta * S_sigma1_zeta + gamma) * (b_zeta + beta * S_sigma2_zeta + gamma) * (c_zeta + beta * S_sigma3 + gamma) * z_omega_zeta * alpha;
	r_poly += (z_poly - 1) * L1_zeta * power(alpha, 2);
	r_poly -= Z_H_zeta * (t_lo + power(zeta, n) * t_mid + power(zeta, 2 * n) * t_hi);
	
	W_zeta_poly = r_poly + nu * (a_poly - a_zeta) + power(nu, 2) * (b_poly - b_zeta) + power(nu, 3) * (c_poly - c_zeta) + power(nu, 4) * (S_sigma1 - S_sigma1_zeta) + power(nu, 5) * (S_sigma2 - S_sigma2_zeta);
	W_zeta_poly /= (ZZ_pX(INIT_MONO, 1, 1) - zeta);

	W_zeta_omega_poly = (z_poly - z_omega_zeta) / (ZZ_pX(INIT_MONO, 1, 1) - zeta * domain[1]);

	poly_commitment(W_zeta_poly_commit, CRS, W_zeta_poly);
	
	poly_commitment(W_zeta_omega_poly_commit,  CRS, W_zeta_omega_poly);
	
	




	string min_str,max_str;
	ZZtostr(max_str,public_input[0]);
	ZZtostr(min_str,public_input[1]);
	Json::Value snark;
	Json::FastWriter fast_writer;

	snark["a_commit"]=a_commit.serializeToHexStr();
	snark["b_commit"]=b_commit.serializeToHexStr();
	snark["c_commit"]=c_commit.serializeToHexStr();
	snark["z_commit"]=z_commit.serializeToHexStr();
	snark["t_lo_commit"]=t_lo_commit.serializeToHexStr();
	snark["t_mid_commit"]=t_mid_commit.serializeToHexStr();
	snark["t_hi_commit"]=t_hi_commit.serializeToHexStr();
	snark["W_zeta_poly_commit"]=W_zeta_poly_commit.serializeToHexStr();
	snark["W_zeta_omega_poly_commit"]=W_zeta_omega_poly_commit.serializeToHexStr();
	snark["a_zeta"]=a_zeta_zz;
	snark["b_zeta"]=b_zeta_zz;
	snark["c_zeta"]=c_zeta_zz;
	snark["S_sigma1_zeta"]=S_sigma1_zeta_zz;
	snark["S_sigma2_zeta"]=S_sigma2_zeta_zz;
	snark["z_omega_zeta"]=z_omega_zeta_zz;
	snark["PI"].append(min_str);
	snark["PI"].append(max_str);
	string str = fast_writer.write(snark);
	
	ofstream ofs("snark.json");
	if(!ofs.is_open())
  		cout<<"open file error!"<<endl;
    
	ofs << str;
	ofs.close();
	
	unsigned char mdStr[33] = {0};
	char buf_512[258]={0};
	char buf[66] = {0};
	char tmp[3] = {0};
	SHA256((const unsigned char *)str.c_str(), str.length(), mdStr);
	 
	for (int i = 0; i < 32; i++)
	{
		sprintf(tmp, "%02x", mdStr[i]);
		strcat(buf, tmp);
	}
	
	string encodedHexStr = std::string(buf);
	
	
	
	
	int error_code;
	unsigned char msg[] = {"message digest"};
	unsigned int msg_len = (unsigned int)(strlen((char *)msg));
	unsigned char user_id[] = {"1234567812345678"};
	unsigned int user_id_len = (unsigned int)(strlen((char *)user_id));
	SM2_KEY_PAIR key_pair;
	SM2_SIGNATURE_STRUCT sm2_sig;


	if ( error_code = sm2_create_key_pair(&key_pair) )
	{
	   printf("Create SM2 key pair failed!\n");
	   
	}
	
	

	
	for (int i = 0; i < 64; i++)  
	{
		sprintf(tmp, "%02x", key_pair.pub_key[i]);
		strcat(buf_512, tmp);
	}
	

	string publickey=buf_512;

	
	error_code = sm2_sign_data((unsigned char*)encodedHexStr.c_str(),
		                        msg_len,
					user_id,
					user_id_len,
					key_pair.pub_key,
					key_pair.pri_key,
					&sm2_sig) ;
	
	
	char sig_r_buf[129]={0};
	for (i = 0; i < 32; i++)
	{
	  	sprintf(tmp, "%02x", sm2_sig.r_coordinate[i]);
		strcat(sig_r_buf, tmp);
	}
	
	string sig_r=sig_r_buf;
	char sig_s_buf[129]={0};
	for (i = 0; i < 32; i++)
	{
	   sprintf(tmp, "%02x", sm2_sig.s_coordinate[i]);
		strcat(sig_s_buf, tmp);
	}
	//验签
	// if ( error_code = sm2_verify_sig((unsigned char*)buf,
	// 	                         msg_len,
	// 				 user_id,
	// 				 user_id_len,
	// 				 key_pair.pub_key,
	// 				 &sm2_sig) )
	// {
	//    printf("Verify SM2 signature failed!\n");
	  
	// }
	// printf("Verify SM2 signature succeeded!\n");

	string sig_s=sig_s_buf;
	Json::Value sig;
	Json::FastWriter sig_writer;
	

	sig["Publickey"]=publickey;
	sig["r"]=sig_r;
	sig["s"]=sig_s;
	sig["digest"]=encodedHexStr;

	string sig_str = fast_writer.write(sig);
	ofstream ofs1("sig.json");
	if(!ofs1.is_open())
  		cout<<"open file error!"<<endl;
    
	ofs1 << sig_str;
	ofs1.close();
}


int main(int argc, char* argv[])
{
  
    initPairing(mcl::BN_SNARK1);
    G1 g1;
    G2 g2;
    Fp2 g2x, g2y;
    Fp g2xa, g2xb, g2ya, g2yb;
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

	int n = 128;
    long long  MIN,MAX,X;
	MIN=atoll(argv[1]);
	MAX=atoll(argv[2]);
	X=atoll(argv[3]);
	
	
	Cricom range;
	range.Setup();
	positive_no_gadget(range, MAX, MIN);
	Get_wit(range, X, MAX, MIN);
	
	
	Vec<G1> CRS;
	ZZ_p k1, k2, secret;
	ZZ_pX q_M, q_L, q_R, q_C, q_O, PI, S_sigma1, S_sigma2, S_sigma3, L1;
	Vec<ZZ_p> witness, qmm, qll, qrr, qcc, qoo, public_input, domain, sigma;
	G1 q_C_commit, q_M_commit, q_O_commit, q_R_commit, q_L_commit, S_sigma1_commit, S_sigma2_commit, S_sigma3_commit, x_commit;
	G2 g2_commit;
	witness.SetLength(3 * n);
	qmm.SetLength(n);
	qll.SetLength(n);
	qrr.SetLength(n);
	qcc.SetLength(n);
	qoo.SetLength(n);
	public_input.SetLength(n);
	for (int i = 0; i < 3 * n; i++)
		witness[i] = conv<ZZ_p>(Witness[i]);
    
	domain.SetLength(n);
	CRS.SetLength(n + 6);
	sigma.SetLength(3 * n);
	eval_domain(domain, n);
	
	//generate_CRS(CRS, g1, secret, n);
	for(int i=0;i<CRS.length();i++)
	{
		CRS[i].deserializeHexStr(srs[i]);
	}
	g2_commit.deserializeHexStr(g2_commit_str);
	
	k1 = rand();
	k2 = power(k1,2);
	for (int i = 0; i < n; i++)
	{
		qmm[i] = range.qM[i];
		qll[i] = range.qL[i];
		qrr[i] = range.qR[i];
		qoo[i] = range.qO[i];
		qcc[i] = range.qC[i];
		
	}


	
	perm_domain(sigma, S_sigma1, S_sigma2, S_sigma3, domain, range.permutation, k1, k2);
	
	
	interpolate(q_M, domain, qmm);
	interpolate(q_O, domain, qoo);
	interpolate(q_L, domain, qll);
	interpolate(q_R, domain, qrr);
	interpolate(q_C, domain, qcc);
	public_input[0]=range.p_i[0];
	public_input[1]=range.p_i[1];
	



	poly_commitment(q_M_commit, CRS, q_M);
	poly_commitment(q_L_commit, CRS, q_L);
	poly_commitment(q_O_commit, CRS, q_O);
	poly_commitment(q_R_commit, CRS, q_R);
	poly_commitment(q_C_commit, CRS, q_C);
	poly_commitment(S_sigma1_commit, CRS, S_sigma1);
	poly_commitment(S_sigma2_commit, CRS, S_sigma2);
	poly_commitment(S_sigma3_commit, CRS, S_sigma3);
	SNARK prove;
	
	
	proof(n, witness, domain, CRS, sigma, k1, k2, q_M, q_L, q_R, q_O, q_C, public_input, S_sigma1, S_sigma2, S_sigma3);
	
	
	
	
	Json::Value setup;
	Json::FastWriter fast_writer;
	string domain_str,k1_str,k2_str;

	setup["q_C_commit"]=q_C_commit.serializeToHexStr();
	setup["q_O_commit"]=q_O_commit.serializeToHexStr();
	setup["q_R_commit"]=q_R_commit.serializeToHexStr();
	setup["q_M_commit"]=q_M_commit.serializeToHexStr();
	setup["q_L_commit"]=q_L_commit.serializeToHexStr();
	setup["S_sigma1_commit"]=S_sigma1_commit.serializeToHexStr();
	setup["S_sigma2_commit"]=S_sigma2_commit.serializeToHexStr();
	setup["S_sigma3_commit"]=S_sigma3_commit.serializeToHexStr();
	setup["g2_commit"]=g2_commit.serializeToHexStr();

	
	ZZtostr(domain_str,domain[1]);
	ZZtostr(k1_str,k1);
	ZZtostr(k2_str,k2);
	setup["omega"]=domain_str;
	setup["k1"]=k1_str;
	setup["k2"]=k2_str;

	string str = fast_writer.write(setup);
	
	ofstream ofs("setup.json");
	if(!ofs.is_open())
  		cout<<"open file error!"<<endl;
    
	ofs << str;
	ofs.close();

	cout<<"generate success"<<endl;

	
	return 1;
}