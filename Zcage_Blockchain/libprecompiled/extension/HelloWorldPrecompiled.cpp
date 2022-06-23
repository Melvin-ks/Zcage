/*
 * @CopyRight:
 * FISCO-BCOS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FISCO-BCOS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with FISCO-BCOS.  If not, see <http://www.gnu.org/licenses/>
 * (c) 2016-2018 fisco-dev contributors.
 */
/** @file HelloWorldPrecompiled.cpp
 *  @author octopuswang
 *  @date 20190111
 */
#include "HelloWorldPrecompiled.h"
#include <libblockverifier/ExecutiveContext.h>
#include <libethcore/ABI.h>
#include <libprecompiled/TableFactoryPrecompiled.h>
#include <assert.h>
#include <cybozu/sha2.hpp>
#include <iomanip>
#include <chrono>
#include<omp.h>
#include"function.h"
#include"threadpool.h"
#include <jsoncpp/json/json.h>
#include "CryptoPrecompiled.h"
#include "ffi_vrf.h"
#include "libdevcrypto/Hash.h"
#include "libdevcrypto/SM2Signature.h"
#include "libdevcrypto/SM3Hash.h"
#include "libethcore/ABI.h"
using namespace dev::eth;
using namespace dev::crypto;
using namespace std;
using namespace mcl::bn;
using namespace dev;
using namespace dev::blockverifier;
using namespace dev::storage;
using namespace dev::precompiled;
NTL_CLIENT

//declarations--start---------------------------------------------------------------------------------------------------------------
SHA256_CTX ctx;
uint8_t buf[65] = { 0 };
string a_commit_str,
b_commit_str,
c_commit_str,
z_commit_str,
t_lo_commit_str,
t_mid_commit_str,
t_hi_commit_str,
W_zeta_poly_commit_str,
W_zeta_omega_poly_commit_str,
a_zeta_str,
b_zeta_str,
c_zeta_str,
S_sigma1_zeta_str,
S_sigma2_zeta_str,
z_omega_zeta_str,
pi_max,
pi_min,
json_publickey,
json_r,
json_s,
json_message;
std::string InputCompanyName;
std::string SecondKey;
std::string retValue;
std::string retValue_S_sigma1_zeta;
std::string retValue_S_sigma2_zeta;
std::string retValue_W_zeta_omega_poly_commit;
std::string retValue_W_zeta_poly_commit;
std::string retValue_a_commit;
std::string retValue_a_zeta;
std::string retValue_b_commit;
std::string retValue_b_zeta;
std::string retValue_c_commit;
std::string retValue_c_zeta;
std::string retValue_t_hi_commit;
std::string retValue_t_lo_commit;
std::string retValue_t_mid_commit;
std::string retValue_z_commit;
std::string retValue_z_omega_zeta;
std::string InputProofName;
std::string InputProofNameForVerify;
std::string retValmax;
std::string retValmin;
std::string retPublickey;
std::string retR;
std::string retS;
std::string retMessage;
const std::string HELLO_WORLD_TABLE_NAME = "hello_world";
const std::string HELLOWORLD_KEY_FIELD = "key";
const std::string HELLOWORLD_KEY_FIELD_NAME = "hello_key";
const std::string HELLOWORLD_VALUE_FIELD = "value";
const char* const CRYPTO_METHOD_SM2_VERIFY_STR = "sm2()";
const char* const HELLO_WORLD_METHOD_GET = "get()";
const char* const HELLO_WORLD_METHOD_SET = "set(string)";
const char* const PROVER_TABLE_METHOD_GET = "get_value(string,string)";
const char* const METHOD_SEND_SIGN_ON_CHAIN = "send_sign(string)";
const char* const METHOD_SEND_PROVE_ON_CHAIN = "send_prove_onchain(string)";
const char* const METHOD_VERIFY_PROOF = "VerifyProof(string)";
const std::string PROVER_DATA_TABLE = "Prover_table";
const std::string PROVER_S_sigma1_zeta = "S_sigma1_zeta_str";
const std::string PROVER_S_sigma2_zeta = "S_sigma2_zeta_str";
const std::string PROVER_W_zeta_omega_poly_commit = "W_zeta_omega_poly_commit_str";
const std::string PROVER_W_zeta_poly_commit = "W_zeta_poly_commit_str";
const std::string PROVER_a_commit = "a_commit_str";
const std::string PROVER_a_zeta = "a_zeta_str";
const std::string PROVER_b_commit = "b_commit_str";
const std::string PROVER_b_zeta = "b_zeta_str";
const std::string PROVER_c_commit = "c_commit_str";
const std::string PROVER_c_zeta = "c_zeta_str";
const std::string PROVER_t_hi_commit = "t_hi_commit_str";
const std::string PROVER_t_lo_commit = "t_lo_commit_str";
const std::string PROVER_t_mid_commit = "t_mid_commit_str";
const std::string PROVER_z_commit = "z_commit_str";
const std::string PROVER_z_omega_zeta = "z_omega_zeta_str";
const std::string PROVER_DATA_TABLE_PRIMARY_KEY = "Company_index";
const std::string PROVE_PI_MAX = "Pi_max";
const std::string PROVE_PI_MIN = "Pi_min";
const std::string PROVE_publickey = "publickey";
const std::string PROVE_r = "r";
const std::string PROVE_s = "s";
const std::string PROVE_message = "message";
const std::string PROVER_KEY_FIELD = PROVER_S_sigma1_zeta + ',' + PROVER_S_sigma2_zeta + ',' + PROVER_W_zeta_omega_poly_commit + ','
									+ PROVER_W_zeta_poly_commit + ',' + PROVER_a_commit + ',' +
									PROVER_a_zeta + ',' + PROVER_b_commit + ',' + PROVER_b_zeta + ',' + PROVER_c_commit + ',' + PROVER_c_zeta + ',' + PROVER_t_hi_commit + ','
									+ PROVER_t_lo_commit + ',' + PROVER_t_mid_commit + ',' +
									PROVER_z_commit + ',' + PROVER_z_omega_zeta + ',' + PROVE_PI_MAX + ',' + PROVE_PI_MIN + ',' + PROVE_publickey + ',' + PROVE_r + ','
									+ PROVE_s + ',' + PROVE_message;



//declarations--end---------------------------------------------------------------------------------------------------------------

//function declaration start---------------------------------------------------------------------------------------------------------------
string32 StringToString32(std::string input)
{
	string32 result;
	for (int i = 0; i < input.length(); i = i + 2)
	{
		string temp = input.substr(i, 2);
		unsigned int hex_num = stoi(temp, 0, 16);
		result[i / 2] = (char)hex_num;
	}
	return result;
}
bytes StringToBytes(std::string input)
{
	bytes result;
	result.resize(64);
	for (int i = 0; i < input.length(); i = i + 2)
	{
		string temp = input.substr(i, 2);
		unsigned int hex_num = stoi(temp, 0, 16);
		result[i / 2] = (unsigned char)hex_num;
	}
	return result;
}
void get_proof(const char *proof_name)//proof_name是 证明文件名字
{
	string proof_prefix = "/home/ks/snark_proof/";   /*FIXME:该目录用于本小组开发环境，您在使用时请修改为本地工作目录*/
	string full_path = proof_prefix + proof_name;
	string afterfix = ".json";
	full_path += afterfix;
	const char* new_path = full_path.c_str();
	ifstream ifs(new_path);
	Json::Reader rd;
	Json::Value snark;
	rd.parse(ifs, snark);
	a_commit_str=snark["a_commit"].asString();
	b_commit_str=snark["b_commit"].asString();
	c_commit_str=snark["c_commit"].asString();
	z_commit_str=snark["z_commit"].asString();
	t_lo_commit_str=snark["t_lo_commit"].asString();
	t_mid_commit_str=snark["t_mid_commit"].asString();
	t_hi_commit_str=snark["t_hi_commit"].asString();
	W_zeta_poly_commit_str=snark["W_zeta_poly_commit"].asString();
	W_zeta_omega_poly_commit_str=snark["W_zeta_omega_poly_commit"].asString();
	a_zeta_str=snark["a_zeta"].asString();
	b_zeta_str=snark["b_zeta"].asString();
	c_zeta_str=snark["c_zeta"].asString();
	S_sigma1_zeta_str=snark["S_sigma1_zeta"].asString();
	S_sigma2_zeta_str=snark["S_sigma2_zeta"].asString();
	z_omega_zeta_str=snark["z_omega_zeta"].asString();
	pi_max= snark["PI"][0].asString();
	pi_min= snark["PI"][1].asString();

}
void get_sign(const char* proof_name)//proof_name是 证明文件名字
{
	string proof_prefix = "/home/ks/snark_proof/";
	string full_path = proof_prefix + proof_name;
	string afterfix = ".json";
	full_path += afterfix;
	const char* new_path = full_path.c_str();
	ifstream ifs(new_path);
	Json::Reader rd;
	Json::Value snark;
	rd.parse(ifs, snark);
	json_publickey = snark["publickey"].asString();
	json_r = snark["r"].asString();
	json_s = snark["s"].asString();
	json_message = snark["message"].asString();
}



string verify(string a_commit_str,
			  string b_commit_str,
			  string c_commit_str,
			  string z_commit_str,
			  string t_lo_commit_str,
			  string t_mid_commit_str,
			  string t_hi_commit_str,
			  string W_zeta_poly_commit_str,
			  string W_zeta_omega_poly_commit_str,
			  string a_zeta_str,
			  string b_zeta_str,
			  string c_zeta_str,
			  string S_sigma1_zeta_str,
			  string S_sigma2_zeta_str,
			  string z_omega_zeta_str,
			  string maxstr,
			  string minstr)
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

	
	ifstream ifs1("/home/ks/setup_json/setup.json");
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
	


	


	

	//********************** Check 1 *****************************

	//********************** Check 2 *****************************

	




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
	
	string retval;
	if (pair1_res== pair2_res)
	{
		retval="verify success!";
		
		
	}
	else
	{
		retval="verify fail";
	
	}
	
	
	return retval;
}
//function declaration end---------------------------------------------------------------------------------------------------------------


//function registion start---------------------------------------------------------------------------------------------------------------
HelloWorldPrecompiled::HelloWorldPrecompiled()
{
    name2Selector[PROVER_TABLE_METHOD_GET] = getFuncSelector(PROVER_TABLE_METHOD_GET);
	name2Selector[METHOD_SEND_PROVE_ON_CHAIN] = getFuncSelector(METHOD_SEND_PROVE_ON_CHAIN);
	name2Selector[METHOD_VERIFY_PROOF] = getFuncSelector(METHOD_VERIFY_PROOF);
	name2Selector[METHOD_SEND_SIGN_ON_CHAIN] = getFuncSelector(METHOD_SEND_SIGN_ON_CHAIN);
}

std::string HelloWorldPrecompiled::toString()
{
    return "HelloWorld";
}
//function registion end---------------------------------------------------------------------------------------------------------------

//call function start---------------------------------------------------------------------------------------------------------------
PrecompiledExecResult::Ptr HelloWorldPrecompiled::call(
    dev::blockverifier::ExecutiveContext::Ptr _context, bytesConstRef _param,
    Address const& _origin, Address const&)
{

    PRECOMPILED_LOG(TRACE) << LOG_BADGE("HelloWorldPrecompiled") << LOG_DESC("call")
        << LOG_KV("param", toHex(_param));
    uint32_t func = getParamFunc(_param);
    bytesConstRef data = getParamData(_param);
    auto callResult = m_precompiledExecResultFactory->createPrecompiledResult();
    callResult->gasPricer()->setMemUsed(_param.size());
    dev::eth::ContractABI abi;
    Table::Ptr table = openTable(_context, precompiled::getTableName(PROVER_DATA_TABLE));
    callResult->gasPricer()->appendOperation(InterfaceOpcode::OpenTable);
    if (!table)
    {
        table = createTable(_context, precompiled::getTableName(PROVER_DATA_TABLE),
            PROVER_DATA_TABLE_PRIMARY_KEY, PROVER_KEY_FIELD, _origin);//-----------------------------------------------建表，主键+a-commit

        callResult->gasPricer()->appendOperation(InterfaceOpcode::CreateTable);
        if (!table)
        {
            PRECOMPILED_LOG(ERROR) << LOG_BADGE("SNARK") << LOG_DESC("set")
                << LOG_DESC("open table failed.");
            getErrorCodeOut(callResult->mutableExecResult(), storage::CODE_NO_AUTHORIZED);
            return callResult;
        }
    }

    else if (func == name2Selector[PROVER_TABLE_METHOD_GET])
	{
		abi.abiOut(data, InputCompanyName, SecondKey);


		auto entries = table->select(InputCompanyName, table->newCondition());
		if (0u != entries->size())
		{
			callResult->gasPricer()->updateMemUsed(getEntriesCapacity(entries));
			callResult->gasPricer()->appendOperation(InterfaceOpcode::Select, entries->size());
			auto entry = entries->get(0);
			retValue = entry->getField(SecondKey);
			unsigned int length = entries->size();
			string str_length = to_string(length);
		}
		else
		{
			retValue = "No Record";
		}
		callResult->setExecResult(abi.abiIn("", retValue));
	}
	else if (func == name2Selector[METHOD_VERIFY_PROOF])
	{
		string res="";
		string ins="";
		string temp="";
		abi.abiOut(data, InputProofNameForVerify);
		int l1=InputProofNameForVerify.find("_");
		int l2=InputProofNameForVerify.find("_",l1+1);
		for(int i=l1+1;i<l2;i++)
		{
			ins=ins+InputProofNameForVerify[i];
		}
		auto entries = table->select(InputProofNameForVerify, table->newCondition());
		if (0u != entries->size())
		{
			string temp="";
			callResult->gasPricer()->updateMemUsed(getEntriesCapacity(entries));
			callResult->gasPricer()->appendOperation(InterfaceOpcode::Select, entries->size());
			auto entry = entries->get(0);
			 retValue_S_sigma1_zeta = entry->getField(PROVER_S_sigma1_zeta);
			 retValue_S_sigma2_zeta = entry->getField(PROVER_S_sigma2_zeta);
			 retValue_W_zeta_omega_poly_commit = entry->getField(PROVER_W_zeta_omega_poly_commit);
			 retValue_W_zeta_poly_commit = entry->getField(PROVER_W_zeta_poly_commit);
			 retValue_a_commit = entry->getField(PROVER_a_commit);
			 retValue_a_zeta = entry->getField(PROVER_a_zeta);
			 retValue_b_commit = entry->getField(PROVER_b_commit);
			 retValue_b_zeta = entry->getField(PROVER_b_zeta);
			 retValue_c_commit = entry->getField(PROVER_c_commit);
			 retValue_c_zeta = entry->getField(PROVER_c_zeta);
			 retValue_t_hi_commit = entry->getField(PROVER_t_hi_commit);
			 retValue_t_lo_commit = entry->getField(PROVER_t_lo_commit);
			 retValue_t_mid_commit = entry->getField(PROVER_t_mid_commit);
			 retValue_z_commit = entry->getField(PROVER_z_commit);
			 retValue_z_omega_zeta = entry->getField(PROVER_z_omega_zeta);
			 retValmax = entry->getField(PROVE_PI_MAX);
			 retValmin = entry->getField(PROVE_PI_MIN);
			 
			 try
			 {

				 string32 message;
				 string32 r;
				 string32 s; 
				 bytes v; 
					 retPublickey = entry->getField(PROVE_publickey);
					 retR = entry->getField(PROVE_r);
					 retS = entry->getField(PROVE_s);
					 retMessage = entry->getField(PROVE_message);
					  v = StringToBytes(retPublickey);
					  r = StringToString32(retR);




				 std::shared_ptr<SM2Signature> sm2Signature =
					 std::make_shared<SM2Signature>(fromString32(r), fromString32(s), h512(v));
				 Address account;
				 bool verifySucc = true;
				 if (!sm2Signature->isValid())
				 {
					 PRECOMPILED_LOG(DEBUG)
						 << LOG_DESC("CryptoPrecompiled: sm2Verify failed for invalid signature");
					 retValue = "CryptoPrecompiled: sm2Verify failed for invalid signature";
					 callResult->setExecResult(abi.abiIn("", retValue));
					 return callResult;
				 }
				 auto publicKey = sm2Recover(sm2Signature, fromString32(message));
				 if (publicKey == dev::h512())
				 {
					 PRECOMPILED_LOG(DEBUG) << LOG_DESC(
						 "CryptoPrecompiled: sm2Verify failed for recover public key failed");
					 retValue = "CryptoPrecompiled: sm2Verify failed for recover public key failed";
						 callResult->setExecResult(abi.abiIn("", retValue));
					 return callResult;
				 }
				 account = right160(sm3(publicKey));
				 PRECOMPILED_LOG(TRACE)
					 << LOG_DESC("CryptoPrecompiled: sm2Verify") << LOG_KV("verifySucc", verifySucc)
					 << LOG_KV("publicKey", toHex(publicKey)) << LOG_KV("account", account);
				 callResult->setExecResult(abi.abiIn("", verifySucc, account));
			 }
			 catch (std::exception const& e)
			 {
				 PRECOMPILED_LOG(WARNING) << LOG_DESC("CryptoPrecompiled: sm2Verify exception")
					 << LOG_KV("e", boost::diagnostic_information(e));
				 Address emptyAccount;
			 }



			 res=verify(retValue_a_commit,
			 				   retValue_b_commit,
							   retValue_c_commit,
							   retValue_z_commit,
			 				   retValue_t_lo_commit,
							   retValue_t_mid_commit,
							   retValue_t_hi_commit,
							   retValue_W_zeta_poly_commit,
			  				   retValue_W_zeta_omega_poly_commit,
			  				   retValue_a_zeta,
			  				   retValue_b_zeta,
			  				   retValue_c_zeta,
			  	 				retValue_S_sigma1_zeta,
			  	 				retValue_S_sigma2_zeta,
			  	 				retValue_z_omega_zeta,
			  	 				retValmax,
			  	 				retValmin);
			temp=temp+ins;
			temp=temp+" ";
			temp=temp+retValmax;
			temp=temp+"-";
			temp=temp+retValmin;
			temp=temp+" ";
			temp=temp+res;
			res=temp;
			 
		}
		retValue=res;
		callResult->setExecResult(abi.abiIn("", retValue));
	}
	else if (func == name2Selector[METHOD_SEND_PROVE_ON_CHAIN])
	{
		abi.abiOut(data, InputProofName);
		const char* input_name = InputProofName.c_str();
		get_proof(input_name);
		auto entries = table->select(InputProofName, table->newCondition());
		callResult->gasPricer()->updateMemUsed(getEntriesCapacity(entries));
		callResult->gasPricer()->appendOperation(InterfaceOpcode::Select, entries->size());
		auto entry = table->newEntry();
		entry->setField(PROVER_DATA_TABLE_PRIMARY_KEY, InputCompanyName);
		entry->setField(PROVER_S_sigma1_zeta, S_sigma1_zeta_str);
		entry->setField(PROVER_S_sigma2_zeta, S_sigma2_zeta_str);
		entry->setField(PROVER_W_zeta_omega_poly_commit, W_zeta_omega_poly_commit_str);
		entry->setField(PROVER_W_zeta_poly_commit, W_zeta_poly_commit_str);
		entry->setField(PROVER_a_commit, a_commit_str);
		entry->setField(PROVER_a_zeta, a_zeta_str);
		entry->setField(PROVER_b_commit, b_commit_str);
		entry->setField(PROVER_b_zeta, b_zeta_str);
		entry->setField(PROVER_c_commit, c_commit_str);
		entry->setField(PROVER_c_zeta, c_zeta_str);
		entry->setField(PROVER_t_hi_commit, t_hi_commit_str);
		entry->setField(PROVER_t_lo_commit, t_lo_commit_str);
		entry->setField(PROVER_t_mid_commit, t_mid_commit_str);
		entry->setField(PROVER_z_commit, z_commit_str);
		entry->setField(PROVER_z_omega_zeta, z_omega_zeta_str);
		entry->setField(PROVE_PI_MAX, pi_max);
		entry->setField(PROVE_PI_MIN, pi_min);
		int count = 0;
		if (0u != entries->size())
		{ 
			count = table->update(InputProofName, entry, table->newCondition(),
				std::make_shared<AccessOptions>(_origin));
			if (count > 0)
			{
				callResult->gasPricer()->appendOperation(InterfaceOpcode::Update, count);
				callResult->gasPricer()->updateMemUsed(entry->capacity() * count);
			}
		}
		else
		{  

			count =
				table->insert(InputProofName, entry, std::make_shared<AccessOptions>(_origin));
			if (count > 0)
			{
				callResult->gasPricer()->updateMemUsed(entry->capacity() * count);
				callResult->gasPricer()->appendOperation(InterfaceOpcode::Insert, count);
			}
		}

		if (count == storage::CODE_NO_AUTHORIZED)
		{ 
			PRECOMPILED_LOG(ERROR) << LOG_BADGE("HelloWorldPrecompiled") << LOG_DESC("set")
				<< LOG_DESC("permission denied");
		}
		getErrorCodeOut(callResult->mutableExecResult(), count);
	}
	else if (func == name2Selector[METHOD_SEND_SIGN_ON_CHAIN])
	{
	abi.abiOut(data, InputProofName);
	const char* input_name = InputProofName.c_str();
	get_sign(input_name);
	auto entries = table->select(InputProofName, table->newCondition());
	callResult->gasPricer()->updateMemUsed(getEntriesCapacity(entries));
	callResult->gasPricer()->appendOperation(InterfaceOpcode::Select, entries->size());

	auto entry = table->newEntry();
	entry->setField(PROVER_DATA_TABLE_PRIMARY_KEY, InputCompanyName);
	entry->setField(PROVE_publickey, json_publickey);
	entry->setField(PROVE_r, json_r);
	entry->setField(PROVE_s, json_s);
	entry->setField(PROVE_message, json_message);
	
	int count = 0;
	if (0u != entries->size())
	{  
		count = table->update(InputProofName, entry, table->newCondition(),
			std::make_shared<AccessOptions>(_origin));
		if (count > 0)
		{
			callResult->gasPricer()->appendOperation(InterfaceOpcode::Update, count);
			callResult->gasPricer()->updateMemUsed(entry->capacity() * count);
		}
	}
	else
	{  
		count =
			table->insert(InputProofName, entry, std::make_shared<AccessOptions>(_origin));
		if (count > 0)
		{
			callResult->gasPricer()->updateMemUsed(entry->capacity() * count);
			callResult->gasPricer()->appendOperation(InterfaceOpcode::Insert, count);
		}
	}

	if (count == storage::CODE_NO_AUTHORIZED)
	{  
		PRECOMPILED_LOG(ERROR) << LOG_BADGE("HelloWorldPrecompiled") << LOG_DESC("set")
			<< LOG_DESC("permission denied");
	}
	getErrorCodeOut(callResult->mutableExecResult(), count);
	}
    else
    { 
        PRECOMPILED_LOG(ERROR) << LOG_BADGE("HelloWorldPrecompiled") << LOG_DESC(" unknown func ")
            << LOG_KV("func", func);
        callResult->setExecResult(abi.abiIn("", u256(int32_t(CODE_UNKNOW_FUNCTION_CALL))));
    }
    return callResult;
}
