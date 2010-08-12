#include <SFGUI/StringUtils.hpp>
#include <SFGUI/Skin.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Image.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

namespace sfg {

Skin::Skin() :
	m_settings( SkinSettings() ),
	m_basedirectory( "" ),
	m_errorimage(),
	m_fonts( FontMap() ),
	m_images( ImageMap() ),
	m_soundbuffers( SoundBufferMap() )
{
	m_errorimage.Create( 30, 30, sf::Color( 255, 0, 0 ) );
}

bool Skin::LoadFromFile( const std::string &filename ) {
	std::ifstream  in( filename.c_str() );
	if( !in.good() ) {
		return false;
	}

	Skin          copy( *this ); // Make a copy to be able to restore in case of a failure.
	std::string   buffer;
	std::size_t   slashpos( filename.rfind( "/" ) );
	unsigned int  linenum( 0 );

	m_basedirectory = (slashpos == std::string::npos) ? "./" : filename.substr( 0, slashpos + 1 );
	m_settings.clear();

	while( std::getline( in, buffer ) ) {
		++linenum;

		std::size_t  cpos( buffer.find( ":" ) );

		if( buffer.size() < 1 || buffer[0] == '#' ) {
			continue;
		}
		else if( cpos == std::string::npos ) {
			std::cerr << filename << ":" << linenum <<": Colon missing." << std::endl;
			*this = copy;
			return false;
		}

		// Get key and value.
		std::string  key( buffer.substr( 0, cpos ) );
		std::string  value( buffer.substr( cpos + 1 ) );

		// Remove spaces and tabs from key and value.
		util::TrimString( key, "\t " );
		util::TrimString( value, "\t " );

		// Check if key and value are given.
		if( key.empty() ) {
			std::cerr << filename << ":" << linenum <<": Key missing." << std::endl;
			*this = copy;
			return false;
		}
		else if( value.empty() ) {
			std::cerr << filename << ":" << linenum <<": Value missing." << std::endl;
			*this = copy;
			return false;
		}

		m_settings[key] = value;
	}

	in.close();
	return true;
}

Skin::SkinSettings::const_iterator Skin::GetAbsoluteIterator( const std::string &key ) const {
	SkinSettings::const_iterator  iter( m_settings.find( key ) );

	if( iter == m_settings.end() ) {
		return iter;
	}

	// If the value is a reference, dereference it.
	if( iter->second[0] == '@' ) {
		return GetAbsoluteIterator( iter->second.substr( 1 ) );
	}

	return iter;
}

std::string Skin::GetString( const std::string &key, const std::string &def ) const {
	SkinSettings::const_iterator  iter( GetAbsoluteIterator( key ) );
	return iter == m_settings.end() ? def : iter->second;
}

int Skin::GetInteger( const std::string &key, int def ) const {
	SkinSettings::const_iterator  iter( GetAbsoluteIterator( key ) );

	if( iter == m_settings.end() ) {
		return def;
	}

	std::stringstream  sstr( iter->second );
	int                value;

	sstr >> value;

	return value;
}

sf::Vector2f Skin::GetVector2f( const std::string &key, const sf::Vector2f &def ) const {
	SkinSettings::const_iterator  iter( GetAbsoluteIterator( key ) );

	if( iter == m_settings.end() ) {
		return def;
	}

	std::stringstream  sstr( iter->second );
	float              x( def.x );
	float              y( def.y );

	sstr >> x >> y;

	return sf::Vector2f( x, y );
}

sf::Color Skin::GetColor( const std::string &widget, const sf::Color &def ) const {
	std::string  colorstring( GetString( widget + ".Color", "" ) );

	if( colorstring == "" ) {
		return def;
	}
	else if( colorstring[0] != '#' || colorstring.size() != 7 ) {
		return def;
	}

	std::stringstream  sstr;
	int                compvalue[3] = {0, 0, 0};

	for( std::size_t compid = 0; compid < 3; ++compid ) {
		sstr.str( colorstring.substr( 1 + compid * 2, 2 ) );
		sstr >> std::hex >> compvalue[compid];
		sstr.clear();
	}

	return sf::Color( static_cast<sf::Uint8>( compvalue[0] ), static_cast<sf::Uint8>( compvalue[1] ), static_cast<sf::Uint8>( compvalue[2] ) );
}

unsigned int Skin::GetFontSize( const std::string& widget ) const {
	return static_cast<unsigned int>( GetInteger( widget + ".Font.Size", 14 ) );
}

TextStyle Skin::GetTextStyle( const std::string& widget ) const {
	unsigned int  fontsize( static_cast<unsigned int>( GetInteger( widget + ".Font.Size", 14 ) ) );
	TextStyle     style;

	style.SetFont( GetFont( widget ) );
	style.SetCharacterSize( fontsize );
	style.SetColor( GetColor( widget ) );

	return style;
}

const sf::Font &Skin::GetFont( const std::string &widget ) const {
	std::string  face( GetString( widget + ".Font.Face", "" ) );

	if( face == "" ) {
		return sf::Font::GetDefaultFont();
	}

	FontMap::iterator  iter( m_fonts.find( face ) );

	if( iter == m_fonts.end() ) {
		// If font hasn't been loaded yet, try to load it now.
		sf::Font  font;

		if( !font.LoadFromFile( m_basedirectory + face ) ) {
			std::cerr << "Failed to load font: " << m_basedirectory << face << std::endl;
			return sf::Font::GetDefaultFont();
		}

		m_fonts[face] = font;
		return m_fonts[face];
	}

	return iter->second;
}

const sf::Image &Skin::GetImage( const std::string &widget, const sf::Image* def ) const {
	std::string  filename( GetString( widget + ".Image.Filename", "" ) );

	if( filename == "" ) {
		if( !def ) {
			return m_errorimage;
		}

		return *def;
	}

	ImageMap::iterator  iter( m_images.find( filename ) );

	if( iter == m_images.end() ) {
		// Image not loaded yet, try to load it now.
		bool       smooth( GetInteger( widget + ".Image.Smooth", 0 ) != 0 ? true : false );
		sf::Image  image;

		if( !image.LoadFromFile( m_basedirectory + filename ) ) {
			if( !def ) {
				return m_errorimage;
			}
			
			return *def;
		}

		image.SetSmooth( smooth );
		m_images[filename] = image;
		return m_images[filename];
	}

	return iter->second;
}

const sf::SoundBuffer &Skin::GetSoundBuffer( const std::string &widget ) const {
	std::string  filename( GetString( widget + ".Sound.Filename", "" ) );

	if( filename == "" ) {
		return m_errorsoundbuffer;
	}

	SoundBufferMap::iterator  iter( m_soundbuffers.find( filename ) );

	if( iter == m_soundbuffers.end() ) {
		// Sound buffer not loaded yet, try to load it now.
		sf::SoundBuffer  soundbuffer;

		if( !soundbuffer.LoadFromFile( m_basedirectory + filename ) ) {
			return m_errorsoundbuffer;
		}

		m_soundbuffers[filename] = soundbuffer;
		return m_soundbuffers[filename];
	}

	return iter->second;
}

sf::Vector2i Skin::GetVector2i( const std::string &key, const sf::Vector2i &def ) const {
	SkinSettings::const_iterator  iter( GetAbsoluteIterator( key ) );

	if( iter == m_settings.end() ) {
		return def;
	}

	std::stringstream  sstr( iter->second );
	int                x( def.x );
	int                y( def.y );

	sstr >> x >> y;

	return sf::Vector2i( x, y );
}

}
