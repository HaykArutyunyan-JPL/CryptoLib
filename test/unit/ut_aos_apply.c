// /* Copyright (C) 2009 - 2022 National Aeronautics and Space Administration.
//    All Foreign Rights are Reserved to the U.S. Government.

//    This software is provided "as is" without any warranty of any kind, either expressed, implied, or statutory,
//    including, but not limited to, any warranty that the software will conform to specifications, any implied warranties
//    of merchantability, fitness for a particular purpose, and freedom from infringement, and any warranty that the
//    documentation will conform to the program, or any warranty that the software will be error free.

//    In no event shall NASA be liable for any damages, including, but not limited to direct, indirect, special or
//    consequential damages, arising out of, resulting from, or in any way connected with the software or its
//    documentation, whether or not based upon warranty, contract, tort or otherwise, and whether or not loss was sustained
//    from, or arose out of the results of, or use of, the software, documentation or services provided hereunder.

//    ITC Team
//    NASA IV&V
//    jstar-development-team@mail.nasa.gov
// */

// /**
//  *  Unit Tests that macke use of CRYPTO_AOS functionality on the data.
//  **/
#include "ut_crypto_aos.h"
#include "crypto.h"
#include "crypto_error.h"
#include "sa_interface.h"
#include "utest.h"

/**
 * @brief Unit Test: AOS Apply: Null Buffer. CrpyotLib should immediately recognize the null buffer and return
 **/
UTEST(AOS_APPLY, NULL_BUFFER)
{
    int32_t status = CRYPTO_LIB_ERROR;
    uint8_t *ingest = NULL;

    Crypto_Config_CryptoLib(KEY_TYPE_INTERNAL, MC_TYPE_INTERNAL, SA_TYPE_INMEMORY, CRYPTOGRAPHY_TYPE_LIBGCRYPT, 
                            IV_INTERNAL, CRYPTO_AOS_CREATE_FECF_TRUE, TC_PROCESS_SDLS_PDUS_TRUE, TC_HAS_PUS_HDR,
                            TC_IGNORE_SA_STATE_FALSE, TC_IGNORE_ANTI_REPLAY_FALSE, TC_UNIQUE_SA_PER_MAP_ID_FALSE,
                            AOS_CHECK_FECF_FALSE, 0x3F, SA_INCREMENT_NONTRANSMITTED_IV_TRUE);
    Crypto_Config_Add_Gvcid_Managed_Parameter(0, 0x0003, 0, AOS_HAS_FECF, AOS_SEGMENT_HDRS_NA, 1786);

    status = Crypto_AOS_ApplySecurity(&ingest[0]);

    ASSERT_EQ(CRYPTO_LIB_ERR_NULL_BUFFER, status);

    char* error_enum = Crypto_Get_Error_Code_Enum_String(status);
    ASSERT_STREQ("CRYPTO_LIB_ERR_NULL_BUFFER",error_enum);
    Crypto_Shutdown();
}

/**
 * @brief Unit Test: AOS Apply: No Crypto Config or Init, detect no config
 **/
UTEST(AOS_APPLY, NO_CONFIG)
{
    int32_t status = CRYPTO_LIB_ERROR;
    uint8_t ingest[1786] = {0};

    status = Crypto_AOS_ApplySecurity(&ingest[0]);

    ASSERT_EQ(CRYPTO_LIB_ERR_NO_CONFIG, status);

    char* error_enum = Crypto_Get_Error_Code_Enum_String(status);
    ASSERT_STREQ("CRYPTO_LIB_ERR_NO_CONFIG",error_enum);
    Crypto_Shutdown();
}

/**
 * @brief Unit Test: CryptoLib is configured, but not initialized - return no config
 * Note: Consider more specific errors
 **/
UTEST(AOS_APPLY, NO_INIT)
{
    // Local variables
    int32_t status = CRYPTO_LIB_ERROR;

    // No Crypto_Init(), but we still Configure It:
    Crypto_Config_CryptoLib(KEY_TYPE_INTERNAL, MC_TYPE_INTERNAL, SA_TYPE_INMEMORY, CRYPTOGRAPHY_TYPE_LIBGCRYPT, 
                            IV_INTERNAL, CRYPTO_AOS_CREATE_FECF_TRUE, TC_PROCESS_SDLS_PDUS_TRUE, TC_HAS_PUS_HDR,
                            TC_IGNORE_SA_STATE_FALSE, TC_IGNORE_ANTI_REPLAY_FALSE, TC_UNIQUE_SA_PER_MAP_ID_FALSE,
                            AOS_CHECK_FECF_FALSE, 0x3F, SA_INCREMENT_NONTRANSMITTED_IV_TRUE);

    // Test frame setup
    // 6 byte header, 2 byte blank SPI, data, FECF
    char* test_aos_h = "40C0000000000000112233445566778899AABBCCDDEEFFA107FF000006D2ABBABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAA0000";
    char* test_aos_b = NULL;
    int test_aos_len = 0;
    hex_conversion(test_aos_h, &test_aos_b, &test_aos_len);

    // Bit math to give concise access to values already set in the static transfer frame
    //TFVN Mask (2 bits) = 1100 0000 = 0xCO
    aos_frame_pri_hdr.tfvn = ((uint8_t)test_aos_b[0] & 0xC0) >> 6;
    //SCID Mask (8 bits) = 0011 1111 1100 0000 = 0x3F 0xC0
    aos_frame_pri_hdr.scid = (((uint16_t)test_aos_b[0] & 0x3F) << 2) | (((uint16_t)test_aos_b[1] & 0xC0) >> 6);
    //VCID Mask (6 bits) =  0011 1111 = 0x3F
    aos_frame_pri_hdr.vcid = ((uint8_t)test_aos_b[1] & 0x3F);

    // Sanity checks on the above bit math
    // Note: This is only checked in this UT, as the bit math is duplicated in other tests
    ASSERT_EQ(aos_frame_pri_hdr.tfvn, 0x01); //TFVN - AOS Version 2 is seto to '0b01' per protocol
    ASSERT_EQ(aos_frame_pri_hdr.scid, 0x03); //SCID 3
    ASSERT_EQ(aos_frame_pri_hdr.vcid, 0x00); //VCID 0

    status = Crypto_AOS_ApplySecurity((uint8_t*)test_aos_b);
    ASSERT_EQ(CRYPTO_LIB_ERR_NO_CONFIG, status);

    char* error_enum = Crypto_Get_Error_Code_Enum_String(status);
    ASSERT_STREQ("CRYPTO_LIB_ERR_NO_CONFIG",error_enum);

    free(test_aos_b);
    Crypto_Shutdown();
}

/**
 * @brief Unit Test: Nominal Case
 * This should call apply_security on the referenced AOS frame
 * and continue down the "happy path", finally returning CRYPTOLIB_SUCCESS
 * The Provided buffer should remain unchanged, except for filling in the SPI and FECF
 **/
UTEST(AOS_APPLY, HAPPY_PATH_CLEAR_FECF)
{
    // Local variables
    int32_t status = CRYPTO_LIB_ERROR;

    // Configure, Add Managed Params, and Init
    Crypto_Init_AOS_Unit_Test();

    // Test Frame Setup
    // 6 byte header, 2 byte blank SPI, data, FECF
    char* test_aos_h = "40C0000000000000112233445566778899AABBCCDDEEFFA107FF000006D2ABBABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAABBAA0000";
    char* test_aos_b = NULL;
    int test_aos_len = 0;
    hex_conversion(test_aos_h, &test_aos_b, &test_aos_len);

    // Truth frame setup

    // Bit math to give concise access to values already set in the static transfer frame
    aos_frame_pri_hdr.tfvn = ((uint8_t)test_aos_b[0] & 0xC0) >> 6;
    aos_frame_pri_hdr.scid = (((uint16_t)test_aos_b[0] & 0x3F) << 2) | (((uint16_t)test_aos_b[1] & 0xC0) >> 6);
    aos_frame_pri_hdr.vcid = ((uint8_t)test_aos_b[1] & 0x3F);

    status = Crypto_AOS_ApplySecurity((uint8_t*)test_aos_b);
    ASSERT_EQ(CRYPTO_LIB_SUCCESS, status);

    char* error_enum = Crypto_Get_Error_Code_Enum_String(status);
    ASSERT_STREQ("CRYPTO_LIB_SUCCESS",error_enum);

    free(test_aos_b);
    Crypto_Shutdown();
}

UTEST_MAIN();