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
#include <openssl/sha.h>
#include <sstream>
#include <vector>

using namespace std;

namespace myapps {
namespace utility {

// https://stackoverflow.com/questions/2262386/generate-sha256-with-openssl-and-c
std::string sha256hex(const std::string& str)
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

std::string sha256hex(std::istream& _ris)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX    sha256;
    SHA256_Init(&sha256);
    // SHA256_Update(&sha256, str.c_str(), str.size());
    constexpr size_t bufsz = 1024 * 64;
    char             buf[bufsz];
    while (!_ris.eof()) {
        _ris.read(buf, bufsz);
        SHA256_Update(&sha256, buf, _ris.gcount());
    }
    SHA256_Final(hash, &sha256);
    stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return ss.str();
}

std::string sha256(const std::string& str)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX    sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);

    return string(reinterpret_cast<char*>(hash), SHA256_DIGEST_LENGTH);
}

std::string sha256(std::istream& _ris)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX    sha256;
    SHA256_Init(&sha256);
    // SHA256_Update(&sha256, str.c_str(), str.size());
    constexpr size_t bufsz = 1024 * 64;
    char             buf[bufsz];
    while (!_ris.eof()) {
        _ris.read(buf, bufsz);
        SHA256_Update(&sha256, buf, _ris.gcount());
    }
    SHA256_Final(hash, &sha256);
    return string(reinterpret_cast<char*>(hash), SHA256_DIGEST_LENGTH);
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
