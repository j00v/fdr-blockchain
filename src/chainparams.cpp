// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2017 The PIVX developers
// Copyright (c) 2018-2019 The fdreserve Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


#include "chainparams.h"
#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>
#include <limits>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
        (0, uint256("000004cd47fe736ef70d94b35fe7a7dae8338138efe5cda2a2869532a69f17cb"))
        (21500, uint256("b58f3bb544cf851485ab35bb5f6edfe6b1bdb831d8bd48f697d3502436aede07"))
        (22930, uint256("0a12e373028cd75ef5948211591a0889c8bbebac7a740cbb3ea666ddd3a781d0"))
        (25800, uint256("1f41b7307c79f85e460f63559d99d5fc639993302a03b36756172f185a270bdb"))
        (285843, uint256("78c023e90a7d613c61a43181d9cb63593756990d9c62d062a3a3ba32f474dbc8"))
        (285844, uint256("fa94ac718726a0876bbee2c921ba8c40bacdd937ecbacb008d821cebcecb3603"))
        (285845, uint256("6f4701a0963a3e3d11eff329d4d2f44e867f178b0d12140b1ed701a956869d3f"))
        (285846, uint256("761b098399ecc8f3ce5efbf973ef6f5d1a39fa516446adf9182b02628a730f54"));

static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1596091771, // * UNIX timestamp of last checkpoint block
    613542,     // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    3000        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet = boost::assign::map_list_of(0, uint256("0"));
static const Checkpoints::CCheckpointData dataTestnet = {&mapCheckpointsTestnet, 1541462411, 0, 250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest = boost::assign::map_list_of(0, uint256("0"));
static const Checkpoints::CCheckpointData dataRegtest = {&mapCheckpointsRegtest, 0, 0, 0};

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0xa5;
        pchMessageStart[1] = 0xc2;
        pchMessageStart[2] = 0xd1;
        pchMessageStart[3] = 0x74;

        nDefaultPort = 12474;
        bnProofOfWorkLimit = ~uint256(0) >> 20;
        bnStartWork = ~uint256(0) >> 24;

        nMaxReorganizationDepth = 100;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 0;
        nTargetSpacing = 1 * 60;  // 1 minute
        nTargetSpacingSlowLaunch = 1 * 60; // before block 100
        nPoSTargetSpacing = 60;  // 1 minute
        nMaturity = 40;
        nMasternodeCountDrift = 3;
        nMaxMoneyOut = 24000000  * COIN;
        nStartMasternodePaymentsBlock = 100;

        /** Height or Time Based Activations **/
        nLastPOWBlock = 200;
        nModifierUpdateBlock = std::numeric_limits<decltype(nModifierUpdateBlock)>::max();

        const char* pszTimestamp = "New FDReserve Coin chain starts on 12 January 2020";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 50 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("045e57e77fef3a1de6c7c9ff6338476aedc5c48478fbfa1819cf4940f61d250a98ccd8cc1b6542a2da21e9855a31e55b3350064eb5863c8dab1f0b5486caaec3be") << OP_CHECKSIG;
        txNew.blob = "Genesis Tx";
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime = 1578839400; 
        genesis.nBits = 504365040; 
        genesis.nNonce = 2689909;

        hashGenesisBlock = genesis.GetHash();

        assert(genesis.hashMerkleRoot == uint256("a8f120c10e385278ea588957d3f04ff35f8864118342b380c9ccc61c9bb57b18"));
        assert(hashGenesisBlock == uint256("000004cd47fe736ef70d94b35fe7a7dae8338138efe5cda2a2869532a69f17cb"));

        //vFixedSeeds.clear();
        //vSeeds.clear();
        vSeeds.push_back(CDNSSeedData("94.237.99.107", "94.237.99.107"));
        vSeeds.push_back(CDNSSeedData("94.237.92.91",  "94.237.92.91"));
        vSeeds.push_back(CDNSSeedData("94.237.98.129", "94.237.98.129"));
        vSeeds.push_back(CDNSSeedData("94.237.98.104", "94.237.98.104"));
           
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 95); // f
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 92); // e
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 97); // g
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x02)(0x2D)(0x25)(0x23).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x02)(0x21)(0x31)(0x1B).convert_to_container<std::vector<unsigned char> >();
        // BIP44 coin type is from https://github.com/satoshilabs/slips/blob/master/slip-0044.md 9984
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0xa4)(0x31).convert_to_container<std::vector<unsigned char> >(); 

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = true;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 3;

        nStakeInputMin = 10 * COIN;
        strDevFeeAddress = "fGiWUrgbRuokWsdWjRZmmBon6MgAuqW2gY";

        vAlertPubKey = ParseHex("049f554869e59fd728e649d1def5b29e34f924efcc63fb0f0b6761d2a6c6ae0964733a0d49701fe8113747495d2b73cb978e2e375489b177fbfc26b10bcabd5d16");
        vGMPubKey = ParseHex("04e15b75d4b3b5e33bbec45d58836a36e8c2d213350e6d6cf11696e82c8abe4bfedd56bb52dbb7602a8905d376cbda4b51d13086447d72e63d9b97f63fe0cb9b41");
        strSporkKey = "045f9ead1a95758ce55b69879adfae6065bbbc4677db7b43e4975d44e67be49faf6a0940dd1aede569f9139c81e11c084a3a232623253840de8a09400f15beb6c4";
        strObfuscationPoolDummyAddress = "fHP7weAZMjVcqU2Rb8QJDJTmMmYnWQNce1";

    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0x17;
        pchMessageStart[1] = 0x27;
        pchMessageStart[2] = 0xa6;
        pchMessageStart[3] = 0xbc;

        bnProofOfWorkLimit = ~uint256(0) >> 1;
        bnStartWork = bnProofOfWorkLimit;

        nDefaultPort = 42322;
        nEnforceBlockUpgradeMajority = 51;
        nRejectBlockOutdatedMajority = 75;
        nToCheckBlockUpgradeMajority = 100;
        nMinerThreads = 0;
        nTargetSpacing = 1 * 60;  // 1 minute
//        nLastPOWBlock = std::numeric_limits<decltype(nLastPOWBlock)>::max();
        nMaturity = 15;
        nMasternodeCountDrift = 4;
        nModifierUpdateBlock = std::numeric_limits<decltype(nModifierUpdateBlock)>::max();
        nMaxMoneyOut = 1000000000 * COIN;

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1578839400;
        genesis.nNonce = 2689909;

        hashGenesisBlock = genesis.GetHash();

        assert(hashGenesisBlock == uint256("000004cd47fe736ef70d94b35fe7a7dae8338138efe5cda2a2869532a69f17cb"));

        vFixedSeeds.clear();
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 137); // Testnet fdreserve addresses start with 'x'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 19);  // Testnet fdreserve script addresses start with '8' or '9'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);     // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x3a)(0x80)(0x61)(0xa0).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x3a)(0x80)(0x58)(0x37).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();

        //convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        nStakeInputMin = 1 * COIN;
        strDevFeeAddress = "xJETLzAQWJj18aQ74cHqAtdStrZves2U4A";

        vAlertPubKey = ParseHex("04d88c9d69f5918853c6da2790b54abf42912ce7c4e6231aaa5a594a34766ab597a8630aa9f37e510c2176ff263c144ab98456526a3341c0420dc664cc25a9bbba");
        vGMPubKey = ParseHex("040e54f437a4c3dec7046ee917e32f50fa3693e0d75ce82cc64f16ab1004fd8d2482af9df2f45c379eab0dabf2ea20f159c436c87e48ac0bedab0aa49f87b3d7f9");
        strSporkKey = "0430eee22a904c95f022eaab0ab575848877fb58a221fb720f1f367c3088e2ec9cfd8ca1cbb6928f0502278dfad68abb98e1c355a44d8d89ca4f252a94b3151684";
        strObfuscationPoolDummyAddress = "xJR9MjNhPLKLLCowMWNznC9gkEQHQPjcJr";

    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        strNetworkID = "regtest";
        pchMessageStart[0] = 0xa1;
        pchMessageStart[1] = 0xcf;
        pchMessageStart[2] = 0x7e;
        pchMessageStart[3] = 0xac;

        bnStartWork = ~uint256(0) >> 20;

        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetSpacing = 1 * 60;
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        genesis.nTime = 1578839400;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 1;

        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 52322;

        //assert(hashGenesisBlock == uint256("300552a9db8b2921c3c07e5bbf8694df5099db579742e243daeaf5008b1e74de"));

        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 51478;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fMineBlocksOnDemand = true;


    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams* pCurrentParams = 0;

CModifiableParams* ModifiableParams()
{
    assert(pCurrentParams);
    assert(pCurrentParams == &unitTestParams);
    return (CModifiableParams*)&unitTestParams;
}

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    case CBaseChainParams::UNITTEST:
        return unitTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
