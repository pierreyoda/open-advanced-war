#include <SFGUI/Radiobutton.hpp>

namespace sfg {

Radiobutton::Radiobutton(
	const sf::FloatRect &rect,
	const sf::String &caption,
	const std::string &id
) :
	Checkbox( rect, caption, id )
{
}

Radiobutton::Ptr Radiobutton::Create( const sf::FloatRect &rect, const sf::String &caption, const std::string &id ) {
	return Ptr( new Radiobutton( rect, caption, id ) );
}

void Radiobutton::SetChecked( bool state ) {
	Checkbox::SetChecked( state );

	if( state ) {
		if( m_prev ) {
			m_prev->SetChecked( false, false );
		}
		if( m_next ) {
			m_next->SetChecked( false, true );
		}
	}
}

void Radiobutton::SetChecked( bool state, bool forward ) {
	if( state != IsChecked() ) {
		Checkbox::SetChecked( state );

		if( Checked ) {
			Checked( shared_from_this() );
		}
	}

	if( forward && m_next ) {
		m_next->SetChecked( state, forward );
	}
	else if( !forward && m_prev ) {
		m_prev->SetChecked( state, forward );
	}
}

void Radiobutton::Link( Ptr other ) {
	Checkbox::SetChecked( false );

	m_next        = other;
	other->m_prev = boost::dynamic_pointer_cast<Radiobutton>( shared_from_this() );
	//shared_from_this();
}

bool Radiobutton::OnEvent( const sf::Event &event ) {
	// Cancel event handling when radiobutton is checked. Radiobuttons can't be unchecked!
	if( (event.Type == sf::Event::MouseButtonPressed || event.Type == sf::Event::MouseButtonReleased) && IsChecked() ) {
		return false;
	}

	return Checkbox::OnEvent( event );
}

}
