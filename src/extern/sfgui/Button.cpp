#include <SFGUI/Button.hpp>
#include <SFGUI/Align.hpp>
#include <SFGUI/Image.hpp>
#include <SFGUI/DefaultImage.hpp>
#include <SFGUI/ImageData.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <iostream>

namespace sfg {

sf::Image    Button::DefaultImage( CreateDefaultImage( 15, 3 * 15, sf::Color( 0, 0, 0 ) ) );
static bool  DefaultInitialized( false );

Button::Button(
	const sf::FloatRect& rect,
	const sf::String& text,
	const std::string& id
) :
	Widget( rect, id ),
	m_box( 3 ),
	m_activebox( 0 ),
	m_caption( Label::Create( sf::Vector2f( rect.Left, rect.Top ), text ) ),
	m_pushedbutton( sf::Mouse::ButtonCount )
{
	SetImage( DefaultImage );
	OnResize( rect );
}

Button::Ptr Button::Create( const sf::FloatRect &rect, const sf::String &text, const std::string &id ) {
	// Create default image if not done, yet.
	if( !DefaultInitialized ) {
		DefaultImage.LoadFromPixels( 15, 3 * 15, ButtonImage );
		DefaultInitialized = true;
	}

	return Ptr( new Button( rect, text, id ) );
}

void Button::SetImage( const sfg::Image &image ) {
	int  ystep( image.GetSubRect().Height / 3 );

	for( int box = 0; box < 3; ++box ) {
		sf::IntRect  newrect(
			image.GetSubRect().Left,
			image.GetSubRect().Top + box * ystep,
			image.GetSubRect().Width,
			ystep
		);

		m_box[box].SetImage( image, newrect );
	}

	NotifyRedraw();
}

bool Button::RenderImpl( sf::RenderTarget &target ) const {
	m_box[m_activebox].Render( target );
	m_caption->Render( target );

	return false;
}

bool Button::OnEvent( const sf::Event &event ) {
	if( event.Type == sf::Event::MouseMoved ) {
		if( GetRect().Contains( static_cast<float>( event.MouseMove.X ), static_cast<float>( event.MouseMove.Y ) ) ) {
			AddHook( sf::Event::MouseMoved, shared_from_this() );

			if( m_pushedbutton == sf::Mouse::ButtonCount ) {
				m_activebox = 1;
			}
			else {
				m_activebox = 2;
			}

			NotifyRedraw();
		}
		else {
			RemoveHook( sf::Event::MouseMoved, shared_from_this() );
			m_activebox = 0;

			NotifyRedraw();
		}
	}
	else if( event.Type == sf::Event::MouseButtonPressed ) {
		if( m_pushedbutton == sf::Mouse::ButtonCount ) {
			m_pushedbutton = event.MouseButton.Button;
			AddHook( sf::Event::MouseButtonReleased, shared_from_this() );

			m_activebox = 2;

			PlaySound( MousePressedSound );
			NotifyRedraw();
		}

		return true;
	}
	else if( event.Type == sf::Event::MouseButtonReleased ) {
		if( m_pushedbutton == event.MouseButton.Button ) {
			bool  result( false );

			if( GetRect().Contains( static_cast<float>( event.MouseButton.X ), static_cast<float>( event.MouseButton.Y ) ) ) {
				if( Clicked ) {
					Clicked( shared_from_this() );
				}

				m_activebox = 1;

				PlaySound( MouseReleasedSound );
				result = true;
			}
			else {
				m_activebox = 0;
			}

			m_pushedbutton = sf::Mouse::ButtonCount;
			RemoveHook( sf::Event::MouseButtonReleased, shared_from_this() );

			NotifyRedraw();
			return result;
		}
	}

	return false;
}

Button::ResizeResult Button::OnResize( const sf::FloatRect &/*oldrect*/ ) {
	m_box[0].SetRect( GetRect() );
	m_box[1].SetRect( GetRect() );
	m_box[2].SetRect( GetRect() );

	AlignWidgetInRect( *m_caption, GetRect(), AlignCenter | AlignMiddle );

	return Cancel;
}

void Button::SetCaption( const sf::String& caption ) {
	m_caption->SetString( caption );

	// Call OnResize to realign the caption if necessary.
	OnResize( GetRect() );
}

void Button::OnFocusChange( bool /*state*/ ) {
}

void Button::SetCaptionPadding( const sf::Vector2f &padding ) {
	m_caption->SetPadding( padding );
}

const sf::String& Button::GetCaption() const {
	return m_caption->GetString();
}

void Button::SetCaptionStyle( const TextStyle& style ) {
	m_caption->SetTextStyle( style );
	OnResize( GetRect() );
}

}
