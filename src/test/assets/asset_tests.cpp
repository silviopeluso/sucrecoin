// Copyright (c) 2017 The Sucrecoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <assets/assets.h>

#include <test/test_sucrecoin.h>

#include <boost/test/unit_test.hpp>

#include <amount.h>
#include <base58.h>
#include <chainparams.h>

BOOST_FIXTURE_TEST_SUITE(asset_tests, BasicTestingSetup)

    BOOST_AUTO_TEST_CASE(unit_validation_tests) {
        BOOST_CHECK(IsAssetUnitsValid(COIN));
        BOOST_CHECK(IsAssetUnitsValid(CENT));
    }

    BOOST_AUTO_TEST_CASE(name_validation_tests) {
        AssetType type;
    
        // regular
        BOOST_CHECK(IsAssetNameValid("MIN", type));
        BOOST_CHECK(type == AssetType::ROOT);
        BOOST_CHECK(IsAssetNameValid("MAX_ASSET_IS_30_CHARACTERS_LNG", type));
        BOOST_CHECK(!IsAssetNameValid("MAX_ASSET_IS_31_CHARACTERS_LONG", type));
        BOOST_CHECK(type == AssetType::INVALID);
        BOOST_CHECK(IsAssetNameValid("A_BCDEFGHIJKLMNOPQRSTUVWXY.Z", type));
        BOOST_CHECK(IsAssetNameValid("0_12345678.9", type));

        BOOST_CHECK(!IsAssetNameValid("NO", type));
        BOOST_CHECK(!IsAssetNameValid("nolower", type));
        BOOST_CHECK(!IsAssetNameValid("NO SPACE", type));
        BOOST_CHECK(!IsAssetNameValid("(#&$(&*^%$))", type));

        BOOST_CHECK(!IsAssetNameValid("_ABC", type));
        BOOST_CHECK(!IsAssetNameValid("_ABC", type));
        BOOST_CHECK(!IsAssetNameValid("ABC_", type));
        BOOST_CHECK(!IsAssetNameValid(".ABC", type));
        BOOST_CHECK(!IsAssetNameValid("ABC.", type));
        BOOST_CHECK(!IsAssetNameValid("AB..C", type));
        BOOST_CHECK(!IsAssetNameValid("A__BC", type));
        BOOST_CHECK(!IsAssetNameValid("A._BC", type));
        BOOST_CHECK(!IsAssetNameValid("AB_.C", type));

        //- Versions of SUCRECOIN NOT allowed
        BOOST_CHECK(!IsAssetNameValid("XSR", type));
        BOOST_CHECK(!IsAssetNameValid("SUCRECOIN", type));
        BOOST_CHECK(!IsAssetNameValid("SUCRECOIN", type));
        BOOST_CHECK(!IsAssetNameValid("SUCRECOINC0IN", type));
        BOOST_CHECK(!IsAssetNameValid("SUCRECOINCO1N", type));
        BOOST_CHECK(!IsAssetNameValid("SUCRECOINC01N", type));

        //- Versions of SUCRECOIN ALLOWED
        BOOST_CHECK(IsAssetNameValid("SUCRECOIN.COIN", type));
        BOOST_CHECK(IsAssetNameValid("SUCRECOIN_COIN", type));
        BOOST_CHECK(IsAssetNameValid("XSRSPYDER", type));
        BOOST_CHECK(IsAssetNameValid("SPYDERXSR", type));
        BOOST_CHECK(IsAssetNameValid("SUCRECOINSPYDER", type));
        BOOST_CHECK(IsAssetNameValid("SPYDESUCRECOIN", type));
        BOOST_CHECK(IsAssetNameValid("BLACK_SUCRECOINS", type));
        BOOST_CHECK(IsAssetNameValid("SEXSROT", type));

        // subs
        BOOST_CHECK(IsAssetNameValid("ABC/A", type));
        BOOST_CHECK(type == AssetType::SUB);
        BOOST_CHECK(IsAssetNameValid("ABC/A/1", type));
        BOOST_CHECK(IsAssetNameValid("ABC/A_1/1.A", type));
        BOOST_CHECK(IsAssetNameValid("ABC/AB/XYZ/STILL/MAX/30/123456", type));

        BOOST_CHECK(!IsAssetNameValid("ABC//MIN_1", type));
        BOOST_CHECK(!IsAssetNameValid("ABC/", type));
        BOOST_CHECK(!IsAssetNameValid("ABC/NOTRAIL/", type));
        BOOST_CHECK(!IsAssetNameValid("ABC/_X", type));
        BOOST_CHECK(!IsAssetNameValid("ABC/X_", type));
        BOOST_CHECK(!IsAssetNameValid("ABC/.X", type));
        BOOST_CHECK(!IsAssetNameValid("ABC/X.", type));
        BOOST_CHECK(!IsAssetNameValid("ABC/X__X", type));
        BOOST_CHECK(!IsAssetNameValid("ABC/X..X", type));
        BOOST_CHECK(!IsAssetNameValid("ABC/X_.X", type));
        BOOST_CHECK(!IsAssetNameValid("ABC/X._X", type));
        BOOST_CHECK(!IsAssetNameValid("ABC/nolower", type));
        BOOST_CHECK(!IsAssetNameValid("ABC/NO SPACE", type));
        BOOST_CHECK(!IsAssetNameValid("ABC/(*#^&$%)", type));
        BOOST_CHECK(!IsAssetNameValid("ABC/AB/XYZ/STILL/MAX/30/OVERALL/1234", type));

        // unique
        BOOST_CHECK(IsAssetNameValid("ABC#AZaz09", type));
        BOOST_CHECK(type == AssetType::UNIQUE);
        BOOST_CHECK(IsAssetNameValid("ABC#@$%&*()[]{}<>-_.;?\\:", type));
        BOOST_CHECK(!IsAssetNameValid("ABC#no!bangs", type));
        BOOST_CHECK(IsAssetNameValid("ABC/THING#_STILL_30_MAX------_", type));

        BOOST_CHECK(!IsAssetNameValid("MIN#", type));
        BOOST_CHECK(!IsAssetNameValid("ABC#NO#HASH", type));
        BOOST_CHECK(!IsAssetNameValid("ABC#NO SPACE", type));
        BOOST_CHECK(!IsAssetNameValid("ABC#RESERVED/", type));
        BOOST_CHECK(!IsAssetNameValid("ABC#RESERVED~", type));
        BOOST_CHECK(!IsAssetNameValid("ABC#RESERVED^", type));

        // channel
        BOOST_CHECK(IsAssetNameValid("ABC~1", type));
        BOOST_CHECK(type == AssetType::CHANNEL);
        BOOST_CHECK(IsAssetNameValid("ABC~STILL_MAX_OF_30.CHARS_1234", type));

        BOOST_CHECK(!IsAssetNameValid("MIN~", type));
        BOOST_CHECK(!IsAssetNameValid("ABC~NO~TILDE", type));
        BOOST_CHECK(!IsAssetNameValid("ABC~_ANN", type));
        BOOST_CHECK(!IsAssetNameValid("ABC~ANN_", type));
        BOOST_CHECK(!IsAssetNameValid("ABC~.ANN", type));
        BOOST_CHECK(!IsAssetNameValid("ABC~ANN.", type));
        BOOST_CHECK(!IsAssetNameValid("ABC~X__X", type));
        BOOST_CHECK(!IsAssetNameValid("ABC~X._X", type));
        BOOST_CHECK(!IsAssetNameValid("ABC~X_.X", type));
        BOOST_CHECK(!IsAssetNameValid("ABC~X..X", type));
        BOOST_CHECK(!IsAssetNameValid("ABC~nolower", type));

        // owner
        BOOST_CHECK(IsAssetNameAnOwner("ABC!"));
        BOOST_CHECK(!IsAssetNameAnOwner("ABC"));
        BOOST_CHECK(!IsAssetNameAnOwner("ABC!COIN"));
        BOOST_CHECK(IsAssetNameAnOwner("MAX_ASSET_IS_30_CHARACTERS_LNG!"));
        BOOST_CHECK(!IsAssetNameAnOwner("MAX_ASSET_IS_31_CHARACTERS_LONG!"));
        BOOST_CHECK(IsAssetNameAnOwner("ABC/A!"));
        BOOST_CHECK(IsAssetNameAnOwner("ABC/A/1!"));
        BOOST_CHECK(IsAssetNameValid("ABC!", type));
        BOOST_CHECK(type == AssetType::OWNER);


    }

    BOOST_AUTO_TEST_CASE(transfer_asset_coin_check) {

        SelectParams(CBaseChainParams::MAIN);

        // Create the asset scriptPubKey
        CAssetTransfer asset("SUCRECOIN", 1000);
        CScript scriptPubKey = GetScriptForDestination(DecodeDestination(Params().GlobalBurnAddress()));
        asset.ConstructTransaction(scriptPubKey);

        CTxOut txOut;
        txOut.nValue = 0;
        txOut.scriptPubKey = scriptPubKey;


        Coin coin(txOut, 0, 0);

        BOOST_CHECK_MESSAGE(coin.IsAsset(), "Transfer Asset Coin isn't as asset");
    }

    BOOST_AUTO_TEST_CASE(new_asset_coin_check) {

        SelectParams(CBaseChainParams::MAIN);

        // Create the asset scriptPubKey
        CNewAsset asset("SUCRECOIN", 1000, 8, 1, 0, "");
        CScript scriptPubKey = GetScriptForDestination(DecodeDestination(Params().GlobalBurnAddress()));
        asset.ConstructTransaction(scriptPubKey);

        CTxOut txOut;
        txOut.nValue = 0;
        txOut.scriptPubKey = scriptPubKey;

        Coin coin(txOut, 0, 0);

        BOOST_CHECK_MESSAGE(coin.IsAsset(), "New Asset Coin isn't as asset");
    }

BOOST_AUTO_TEST_SUITE_END()
