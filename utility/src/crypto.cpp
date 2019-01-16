#include "ola/common/utility/crypto.hpp"

#include "solid/system/exception.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <iomanip>
#include <openssl/conf.h>
#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <sstream>
#include <vector>

using namespace std;

namespace ola {
namespace utility {

//https://stackoverflow.com/questions/2262386/generate-sha256-with-openssl-and-c
std::string sha256(const std::string& str)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX    sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return ss.str();
}

//-----------------------------------------------------------------------------
//https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption

namespace {
const string iv = "fda66004-197a-11e9-a947-ff513e7f94ff";
} //namespace

struct Enigma::Data {
    EVP_CIPHER_CTX* enc_ctx_;
    EVP_CIPHER_CTX* dec_ctx_;
    string          iv_;
    string          key_;

    Data()
    {
        enc_ctx_ = EVP_CIPHER_CTX_new();
        dec_ctx_ = EVP_CIPHER_CTX_new();
        iv_      = iv; //to_string(boost::uuids::random_generator()());
    }

    ~Data()
    {
        if (enc_ctx_) {
            EVP_CIPHER_CTX_free(enc_ctx_);
        }
        if (dec_ctx_) {
            EVP_CIPHER_CTX_free(dec_ctx_);
        }
    }
};

Enigma::Enigma()
    : pimpl_(solid::make_pimpl<Data>())
{
}

Enigma::~Enigma() {}

void Enigma::configure(const std::string& _s)
{
    pimpl_->key_ = _s;

    const uint8_t* pkey = reinterpret_cast<const uint8_t*>(_s.c_str());
    const uint8_t* piv  = reinterpret_cast<const uint8_t*>(pimpl_->iv_.c_str());

    /* Initialise the encryption operation. IMPORTANT - ensure you use a key
    * and IV size appropriate for your cipher
    * In this example we are using 256 bit AES (i.e. a 256 bit key). The
    * IV size for *most* modes is the same as the block size. For AES this
    * is 128 bits */
    EVP_EncryptInit_ex(pimpl_->enc_ctx_, EVP_aes_256_cbc(), NULL, pkey, piv);

    /* Initialise the decryption operation. IMPORTANT - ensure you use a key
    * and IV size appropriate for your cipher
    * In this example we are using 256 bit AES (i.e. a 256 bit key). The
    * IV size for *most* modes is the same as the block size. For AES this
    * is 128 bits */
    EVP_DecryptInit_ex(pimpl_->dec_ctx_, EVP_aes_256_cbc(), NULL, pkey, piv);
}

std::string Enigma::encode(const std::string& _plain_text)
{

    {
        EVP_CIPHER_CTX_reset(pimpl_->enc_ctx_);
        const uint8_t* pkey = reinterpret_cast<const uint8_t*>(pimpl_->key_.c_str());
        const uint8_t* piv  = reinterpret_cast<const uint8_t*>(pimpl_->iv_.c_str());

        /* Initialise the encryption operation. IMPORTANT - ensure you use a key
        * and IV size appropriate for your cipher
        * In this example we are using 256 bit AES (i.e. a 256 bit key). The
        * IV size for *most* modes is the same as the block size. For AES this
        * is 128 bits */
        EVP_EncryptInit_ex(pimpl_->enc_ctx_, EVP_aes_256_cbc(), NULL, pkey, piv);
    }

    int len;

    int ciphertext_len = 0;

    string res;
    res.resize(_plain_text.size() * 2);

    uint8_t*       pres_d   = reinterpret_cast<uint8_t*>(const_cast<char*>(res.data()));
    const uint8_t* pplain_d = reinterpret_cast<const uint8_t*>(_plain_text.data());

    /* Provide the message to be encrypted, and obtain the encrypted output.
    * EVP_EncryptUpdate can be called multiple times if necessary
    */
    if (1 == EVP_EncryptUpdate(pimpl_->enc_ctx_, pres_d, &len, pplain_d, _plain_text.size())) {
        ciphertext_len = len;

        /* Finalise the encryption. Further ciphertext bytes may be written at
        * this stage.
        */
        if (1 == EVP_EncryptFinal_ex(pimpl_->enc_ctx_, pres_d + len, &len)) {
            ciphertext_len += len;
        }
    }

    solid_check(ciphertext_len < res.size());

    res.resize(ciphertext_len);

    return res;
}

std::string Enigma::decode(const std::string& _cipher_text)
{

    {
        EVP_CIPHER_CTX_reset(pimpl_->dec_ctx_);
        const uint8_t* pkey = reinterpret_cast<const uint8_t*>(pimpl_->key_.c_str());
        const uint8_t* piv  = reinterpret_cast<const uint8_t*>(pimpl_->iv_.c_str());

        /* Initialise the decryption operation. IMPORTANT - ensure you use a key
        * and IV size appropriate for your cipher
        * In this example we are using 256 bit AES (i.e. a 256 bit key). The
        * IV size for *most* modes is the same as the block size. For AES this
        * is 128 bits */
        EVP_DecryptInit_ex(pimpl_->dec_ctx_, EVP_aes_256_cbc(), NULL, pkey, piv);
    }
    int len;

    int plaintext_len = 0;

    string plain_text;
    plain_text.resize(_cipher_text.size() + 64);

    uint8_t*       pplain_d  = reinterpret_cast<uint8_t*>(const_cast<char*>(plain_text.data()));
    const uint8_t* pcipher_d = reinterpret_cast<const uint8_t*>(_cipher_text.data());

    /* Provide the message to be decrypted, and obtain the plaintext output.
    * EVP_DecryptUpdate can be called multiple times if necessary
    */
    if (1 == EVP_DecryptUpdate(pimpl_->dec_ctx_, pplain_d, &len, pcipher_d, _cipher_text.size())) {
        plaintext_len = len;

        /* Finalise the decryption. Further plaintext bytes may be written at
        * this stage.
        */
        if (1 == EVP_DecryptFinal_ex(pimpl_->dec_ctx_, pplain_d + len, &len)) {
            plaintext_len += len;
        }
    }
    solid_check(plaintext_len < plain_text.size());

    plain_text.resize(plaintext_len);

    return plain_text;
}
//-----------------------------------------------------------------------------
//https://stackoverflow.com/questions/7053538/how-do-i-encode-a-string-to-base64-using-only-boost
namespace {
const std::string base64_padding[] = {"", "==", "="};
} //namespace
std::string base64_encode(const std::string& _txt)
{
    namespace bai = boost::archive::iterators;

    std::stringstream os;

    // convert binary values to base64 characters
    typedef bai::base64_from_binary
        // retrieve 6 bit integers from a sequence of 8 bit bytes
        <bai::transform_width<const char*, 6, 8>>
            base64_enc; // compose all the above operations in to a new iterator

    std::copy(base64_enc(_txt.c_str()), base64_enc(_txt.c_str() + _txt.size()),
        std::ostream_iterator<char>(os));

    os << base64_padding[_txt.size() % 3];
    return os.str();
}
std::string base64_decode(const std::string& _txt)
{
    namespace bai = boost::archive::iterators;

    std::stringstream os;

    typedef bai::transform_width<bai::binary_from_base64<const char*>, 8, 6> base64_dec;

    unsigned int size = _txt.size();

    // Remove the padding characters, cf. https://svn.boost.org/trac/boost/ticket/5629
    if (size && _txt[size - 1] == '=') {
        --size;
        if (size && _txt[size - 1] == '=')
            --size;
    }
    if (size == 0)
        return std::string();

    std::copy(base64_dec(_txt.data()), base64_dec(_txt.data() + size),
        std::ostream_iterator<char>(os));

    return os.str();
}
//-----------------------------------------------------------------------------
} //namespace utility
} //namespace ola
