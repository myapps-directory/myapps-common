#include "myapps/common/utility/encode.hpp"

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
#include <sstream>
#include <vector>

using namespace std;

namespace myapps {
namespace utility {

namespace {
using DigestContextPtrT = std::unique_ptr<EVP_MD_CTX, decltype(&EVP_MD_CTX_free)>;
} // namespace
#if 0
std::string sha256hex(const std::string& str)
{
    unsigned char md_value[EVP_MAX_MD_SIZE];
    unsigned int md_len;
    const EVP_MD *md = EVP_sha3_256();
    DigestContextPtrT digest_ctx_ptr{EVP_MD_CTX_new(), EVP_MD_CTX_free};
    
    EVP_DigestInit_ex(digest_ctx_ptr.get(), md, NULL);
    EVP_DigestUpdate(digest_ctx_ptr.get(), str.c_str(), str.size());
    EVP_DigestFinal_ex(digest_ctx_ptr.get(), md_value, &md_len);

    stringstream ss;
    for (size_t i = 0; i < md_len; ++i) {
        ss << hex << setw(2) << setfill('0') << static_cast<int>(md_value[i]);
    }
    return ss.str();
}

std::string sha256hex(std::istream& _ris)
{
    unsigned char md_value[EVP_MAX_MD_SIZE];
    unsigned int md_len;
    DigestContextPtrT digest_ctx_ptr{EVP_MD_CTX_new(), EVP_MD_CTX_free};
    constexpr size_t bufsz = 1024 * 64;
    char             buf[bufsz];
    const EVP_MD *md = EVP_sha3_256();

    EVP_DigestInit_ex(digest_ctx_ptr.get(), md, NULL);
    
    while (!_ris.eof()) {
        _ris.read(buf, bufsz);
        EVP_DigestUpdate(digest_ctx_ptr.get(), buf, _ris.gcount());
    }
    EVP_DigestFinal_ex(digest_ctx_ptr.get(), md_value, &md_len);

    stringstream ss;
    for (size_t i = 0; i < md_len; ++i) {
        ss << hex << setw(2) << setfill('0') << static_cast<int>(md_value[i]);
    }
    return ss.str();
}
#endif

std::string sha256(const std::string& str)
{
    unsigned char     md_value[EVP_MAX_MD_SIZE];
    unsigned int      md_len;
    const EVP_MD*     md = EVP_sha3_256();
    DigestContextPtrT digest_ctx_ptr{EVP_MD_CTX_new(), EVP_MD_CTX_free};

    EVP_DigestInit_ex(digest_ctx_ptr.get(), md, NULL);
    EVP_DigestUpdate(digest_ctx_ptr.get(), str.c_str(), str.size());
    EVP_DigestFinal_ex(digest_ctx_ptr.get(), md_value, &md_len);

    return string(reinterpret_cast<char*>(md_value), md_len);
}

std::string sha256(std::istream& _ris)
{
    unsigned char     md_value[EVP_MAX_MD_SIZE];
    unsigned int      md_len;
    DigestContextPtrT digest_ctx_ptr{EVP_MD_CTX_new(), EVP_MD_CTX_free};
    constexpr size_t  bufsz = 1024 * 64;
    char              buf[bufsz];
    const EVP_MD*     md = EVP_sha3_256();

    EVP_DigestInit_ex(digest_ctx_ptr.get(), md, NULL);

    while (!_ris.eof()) {
        _ris.read(buf, bufsz);
        EVP_DigestUpdate(digest_ctx_ptr.get(), buf, _ris.gcount());
    }
    EVP_DigestFinal_ex(digest_ctx_ptr.get(), md_value, &md_len);
    return string(reinterpret_cast<char*>(md_value), md_len);
}

//-----------------------------------------------------------------------------
// https://stackoverflow.com/questions/7053538/how-do-i-encode-a-string-to-base64-using-only-boost
namespace {
const std::string base64_padding[] = {"", "==", "="};
} // namespace

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

    typedef bai::transform_width<bai::binary_from_base64<const char*>, 8, 6>
        base64_dec;

    unsigned int size = _txt.size();

    // Remove the padding characters, cf.
    // https://svn.boost.org/trac/boost/ticket/5629
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
std::string hex_encode(const std::string& _txt)
{
    std::ostringstream oss;
    for (const auto& c : _txt) {
        oss << hex << setw(2) << setfill('0')
            << static_cast<int>(static_cast<uint8_t>(c));
    }
    return oss.str();
}
//-----------------------------------------------------------------------------

namespace {

inline constexpr int hex_char_decode(const char c)
{

    if (c >= '0' && c <= '9') {
        return (c - '0');
    } else {
        return ((c - 'a') + 10);
    }
}

inline char hex_char_decode(const char* _pc)
{
    const int rv = (hex_char_decode(*_pc) << 4) | hex_char_decode(*(_pc + 1));
    return static_cast<char>(static_cast<uint8_t>(rv));
}

} // namespace

std::string hex_decode(const std::string& _txt)
{
    string out;
    out.resize(_txt.size() / 2);
    const char* p = _txt.c_str();
    for (size_t i = 0; i < out.size(); ++i) {
        out[i] = hex_char_decode(p);
        p += 2;
    }
    return out;
}
//-----------------------------------------------------------------------------
} // namespace utility
} // namespace myapps
