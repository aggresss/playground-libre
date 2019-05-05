# ------------------------------------------------------------------------------
# CMakeLists.txt for aggresss/playground-libre/third_party
# ------------------------------------------------------------------------------

include(ExternalProject)

if(WITH_OPENSSL)
  ExternalProject_Add(external-openssl
    SOURCE_DIR ${OPENSSL_PREFIX}
    BUILD_ALWAYS TRUE
    BUILD_IN_SOURCE TRUE
    URL https://github.com/openssl/openssl/archive/OpenSSL_1_1_1b.tar.gz
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND
      ./Configure
      -fPIC
      no-asm
      linux-generic32
      --prefix=${OPENSSL_PREFIX}
      --cross-compile-prefix=${C_COMPILER_PREFIX}
      no-shared
    BUILD_COMMAND make
    INSTALL_COMMAND ""
    TEST_COMMAND ""
    )
endif(WITH_OPENSSL)

