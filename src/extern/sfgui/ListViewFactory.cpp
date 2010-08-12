#include <SFGUI/ListViewFactory.hpp>
#include <SFML/Graphics/Sprite.hpp>

namespace sfg {

ListViewFactory::ListViewFactory() :
	m_labelprototype( Label::Create( sf::Vector2f( 0, 0 ) ) )
{
}

ListViewFactory::~ListViewFactory() {
}

Label::Ptr ListViewFactory::CreateItem( const sf::String &text ) {
	Label::Ptr  label( Label::Create( sf::Vector2f( 0, 0 ), text ) );
	label->SetTextStyle( m_labelprototype->GetTextStyle() );
	return label;
}

SpriteItemView::Ptr ListViewFactory::CreateItem( const sf::Sprite &sprite ) {
	return SpriteItemView::Ptr( new SpriteItemView( sprite ) );
}

void ListViewFactory::SetPrototype( Label::Ptr label ) {
	m_labelprototype = label;
}

}
