#include "ola/common/utility/crypto.hpp"
#include "solid/system/exception.hpp"
#include <iostream>
#include <string>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

using namespace std;

int test_crypto_basic(int argc, char* argv[])
{

    string key = to_string(boost::uuids::random_generator()());

    cout << "key = " << key << endl;

    ola::utility::Enigma e;
    e.configure(key);

    string plain_text = "The quick brown fox jumps over the lazy dog";

    string enc_text = e.encode(plain_text);

    cout << "encoded text: " << enc_text << endl;

    string dec_text = e.decode(enc_text);

    cout << "decoded text: " << dec_text << endl;

    solid_check(dec_text == plain_text);

    return 0;
}
