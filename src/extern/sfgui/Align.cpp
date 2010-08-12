#include <SFGUI/Align.hpp>
#include <SFGUI/Widget.hpp>

namespace sfg {

void AlignWidgetInRect( Widget &w, const sf::FloatRect &rect, unsigned int alignment, const sf::Vector2f &padding ) {
	sf::Vector2f   pos( w.GetRect().Left, w.GetRect().Top );

	if( alignment & AlignLeft ) {
		pos.x = rect.Left + padding.x;
	}
	else if( alignment & AlignCenter ) {
		pos.x = (rect.Left + (rect.Width / 2)) - (w.GetRect().Width / 2);
	}
	else if( alignment & AlignRight ) {
		pos.x = (rect.Left + rect.Width) - w.GetRect().Width - padding.x;
	}

	if( alignment & AlignTop ) {
		pos.y = rect.Top + padding.y;
	}
	else if( alignment & AlignMiddle ) {
		pos.y = (rect.Top + (rect.Height / 2)) - (w.GetRect().Height / 2);
	}
	else if( alignment & AlignBottom ) {
		pos.y = (rect.Top + rect.Height) - w.GetRect().Height - padding.y;
	}

	//w.SetRect( sf::FloatRect( pos.x, pos.y, w.GetRect().Width, w.GetRect().Height ) );
	w.SetPosition( pos );
}

}
