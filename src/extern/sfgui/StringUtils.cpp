#include <SFGUI/StringUtils.hpp>

namespace sfg {
namespace util {

void TrimString( std::string &str, const std::string &chars ) {
	std::size_t  first( 0 );
	std::size_t  last( str.size() - 1 );

	for( std::size_t pos = 0; pos < str.size(); ++pos ) {
		if( chars.find( str[pos] ) == std::string::npos ) {
			first = pos;
			break;
		}
	}

	for( std::size_t pos = 0; pos < str.size(); ++pos ) {
		if( chars.find( str[str.size() - pos - 1] ) == std::string::npos ) {
			last = str.size() - pos - 1;
			break;
		}
	}

	str = str.substr( first, last - first + 1 );
}

}
}
