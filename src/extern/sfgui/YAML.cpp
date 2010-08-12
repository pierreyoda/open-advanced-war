#include <SFGUI/YAML.hpp>
#include <yaml.h>
#include <sstream>

const YAML::Node &operator>>( const YAML::Node &node, sfg::YAML::WidgetNode &base ) {
	const YAML::Node  *id( node.FindValue( "Id" ) );

	node["Type"] >> base.Type;
	node["Rect"] >> base.Rect;

	if( id ) {
		node["Id"] >> base.Id;
	}
	else {
		base.Id = "";
	}

	return node;
}

const YAML::Node &operator>>( const YAML::Node &node, sf::FloatRect &rect ) {
	std::string  coords;
	node >> coords;

	std::stringstream  sstr( coords );
	sstr >> rect.Left;
	sstr >> rect.Top;
	sstr >> rect.Width;
	sstr >> rect.Height;

	return node;
}
