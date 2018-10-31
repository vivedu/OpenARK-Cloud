#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include "Util.h"
using namespace std;
namespace NetworkLib {
	namespace Util {
		//Encode string to base64 string
		bool Util::Base64Encode(const string& input, string* output) {
			typedef boost::archive::iterators::base64_from_binary<boost::archive::iterators::transform_width<string::const_iterator, 6, 8> > Base64EncodeIterator;
			stringstream result;
			copy(Base64EncodeIterator(input.begin()), Base64EncodeIterator(input.end()), ostream_iterator<char>(result));
			size_t equal_count = (3 - input.length() % 3) % 3;
			for (size_t i = 0; i < equal_count; i++) {
				result.put('=');
			}
			*output = result.str();
			return output->empty() == false;
		}
		//Decode base64 string to string
		bool Util::Base64Decode(const string& input, string* output) {
			typedef boost::archive::iterators::transform_width<boost::archive::iterators::binary_from_base64<string::const_iterator>, 8, 6> Base64DecodeIterator;
			stringstream result;
			try {
				copy(Base64DecodeIterator(input.begin()), Base64DecodeIterator(input.end()), ostream_iterator<char>(result));
			}
			catch (...) {
				return false;
			}
			*output = result.str();
			return output->empty() == false;
		}
	}
}
