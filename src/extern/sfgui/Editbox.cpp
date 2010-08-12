#include <SFGUI/Editbox.hpp>
#include <SFGUI/Align.hpp>
#include <SFGUI/Globals.hpp>
#include <SFGUI/DefaultImage.hpp>
#include <SFGUI/ImageData.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace sfg {

sf::Image    Editbox::DefaultImage( CreateDefaultImage( 15, 3 * 15, sf::Color( 200, 200, 200 ) ) );
static bool  DefaultInitialized( false );

Editbox::Editbox(
	const sf::FloatRect &rect,
	const std::string &id
) :
	Widget( rect, id ),
	m_string( L"" ),
	m_caret( sf::Shape::Line( rect.Left, rect.Top, rect.Left, rect.Top + rect.Height, 2.f, sf::Color( 255, 255, 255 ) ) ),
	m_label( Label::Create( sf::Vector2f( rect.Left, rect.Top ) ) ),
	m_blinkdelay( 0.5f ),
	m_showcaret( true ),
	m_caretpos( 0 ),
	m_maxlength( 0 ),
	m_rangebegin( 0 ),
	m_rangeend( 0 ),
	m_passwordch( L"" ),
	m_allowedchars( L"" ),
	m_box( 3 ),
	m_activebox( 0 ),
	m_nextblink( 0.f ),
	m_clock( sf::Clock() )
{
	SetImage( DefaultImage );
	OnResize( rect );
}

Editbox::Ptr Editbox::Create( const sf::FloatRect &rect, const std::string& id ) {
	// Create default image if not done, yet.
	if( !DefaultInitialized ) {
		DefaultImage.LoadFromPixels( 15, 3 * 15, EditboxImage );
		DefaultInitialized = true;
	}

	return Ptr( new Editbox( rect, id ) );
}

void Editbox::SetText( const sf::String &string ) {
	m_string = string;
	m_caretpos = m_string.GetSize();
	InvalidateVisibleText();
	UpdateCaret();
}

const sf::String &Editbox::GetText() const {
	return m_string;
}

void Editbox::SetMaxLength( std::size_t maxlength ) {
	m_maxlength = maxlength;

	if( m_maxlength > 0 && m_string.GetSize() > m_maxlength ) {
		m_string.Erase( m_maxlength, m_string.GetSize() - m_maxlength );

		if( m_caretpos > m_maxlength ) {
			m_caretpos = m_maxlength;
			UpdateCaret();
		}

		InvalidateVisibleText();
	}

	NotifyRedraw();
}

void Editbox::HideText( const sf::String &characters ) {
	m_passwordch = characters;
	InvalidateVisibleText();
	UpdateCaret();
}

void Editbox::ShowText() {
	m_passwordch = L"";
	InvalidateVisibleText();
	UpdateCaret();
}

bool Editbox::RenderImpl( sf::RenderTarget &target ) const {
	m_box[m_activebox].Render( target );
	m_label->Render( target );

	if( IsFocused() && m_showcaret ) {
		target.Draw( m_caret );
	}

	return true;
}

bool Editbox::OnEvent( const sf::Event &event ) {
	if( event.Type == sf::Event::MouseMoved ) {
		if( !IsFocused() ) {
			if( GetRect().Contains( static_cast<float>( event.MouseMove.X ), static_cast<float>( event.MouseMove.Y ) ) ) {
				AddHook( sf::Event::MouseMoved, shared_from_this() );
				m_activebox = 1;
			}
			else {
				RemoveHook( sf::Event::MouseMoved, shared_from_this() );
				m_activebox = 0;
			}

			NotifyRedraw();
		}
	}
	else if( event.Type == sf::Event::MouseButtonPressed ) {
		GrabFocus( shared_from_this() );
		PlaySound( MousePressedSound );
		NotifyRedraw();
		return true;
	}
	else if( event.Type == sf::Event::KeyPressed ) {
		bool  valid( false );

		if( event.Key.Code == sf::Key::Back && m_caretpos > 0 ) {
			m_string.Erase( m_caretpos - 1 );
			InvalidateVisibleText();
			MoveCaret( -1 );
			valid = true;
		}
		else if( event.Key.Code == sf::Key::Delete && m_caretpos < m_string.GetSize() ) {
			m_string.Erase( m_caretpos );
			InvalidateVisibleText();
			valid = true;
		}
		else if( event.Key.Code == sf::Key::Left && m_caretpos > 0 ) {
			MoveCaret( -1 );
			valid = true;
		}
		else if( event.Key.Code == sf::Key::Right && m_caretpos < m_string.GetSize() ) {
			MoveCaret( 1 );
			valid = true;
		}
		else if( event.Key.Code == sf::Key::Home ) {
			MoveCaret( static_cast<int>( m_caretpos ) * -1 );
			valid = true;
		}
		else if( event.Key.Code == sf::Key::End ) {
			MoveCaret( static_cast<int>( m_string.GetSize() - m_caretpos ) + 1 );
			valid = true;
		}
		else if( event.Key.Code == sf::Key::Return ) {
			ReleaseFocus();

			if( Accepted ) {
				Accepted( shared_from_this() );
			}

			NotifyRedraw();
			valid = true;
		}

		if( valid ) {
			PlaySound( KeyPressedSound );
			return true;
		}
	}
	else if( event.Type == sf::Event::TextEntered ) {
		// Skip non-printable characters.
		if( event.Text.Unicode < 32 || event.Text.Unicode == 127 ) {
			return true;
		}

		if( m_allowedchars.GetSize() == 0 || m_allowedchars.Find( event.Text.Unicode ) != sf::String::InvalidPos ) {
			if( m_maxlength == 0 || m_string.GetSize() < m_maxlength ) {
				m_string.Insert( m_caretpos, event.Text.Unicode );
				++m_rangeend;
				InvalidateVisibleText();
				MoveCaret( 1 );
				PlaySound( KeyPressedSound );

				if( Changed ) {
					Changed( shared_from_this() );
				}
			}
		}

		return true;
	}

	return false;
}

Editbox::ResizeResult Editbox::OnResize( const sf::FloatRect &/*oldrect*/ ) {
	m_box[0].SetRect( GetRect() );
	m_box[1].SetRect( GetRect() );
	m_box[2].SetRect( GetRect() );

	sfg::AlignWidgetInRect( *m_label, GetRect(), sfg::AlignLeft | sfg::AlignMiddle, sf::Vector2f( 5, 0 ) );

	UpdateCaret();

	return Cancel;
}

bool Editbox::IsTextHidden() const {
	return m_passwordch.GetSize() > 0;
}

void Editbox::SetImage( const sf::Image &image ) {
	unsigned int  ystep( image.GetHeight() / 3 );

	m_box[0].SetImage( image, sf::IntRect( 0, 0 * ystep, image.GetWidth(), ystep ) );
	m_box[1].SetImage( image, sf::IntRect( 0, 1 * ystep, image.GetWidth(), ystep ) );
	m_box[2].SetImage( image, sf::IntRect( 0, 2 * ystep, image.GetWidth(), ystep ) );
}

void Editbox::OnFocusChange( bool state ) {
	if( state ) {
		m_activebox = 2;
	}
	else {
		m_activebox = 0;
	}

	NotifyRedraw();
}

void Editbox::UpdateCaret() {
	float  xpos( 0.f );

	xpos = m_label->GetRect().Left + (m_label->GetCharacterPos( m_caretpos - m_rangebegin ).x);

	m_caret = sf::Shape::Line(
		xpos,
		(GetRect().Top + GetRect().Height / 2) - (m_label->GetRect().Height / 2),
		xpos,
		(GetRect().Top + GetRect().Height / 2) + (m_label->GetRect().Height / 2),
		2.f,
		m_label->GetTextStyle().GetColor()
	);

	NotifyRedraw();
}

void Editbox::MoveCaret( int delta ) {
	if( delta < 0 && static_cast<std::size_t>( delta * -1 ) > m_caretpos ) {
		m_caretpos = 0;
	}
	else if( m_caretpos + delta > m_string.GetSize() ) {
		m_caretpos = m_string.GetSize();
	}
	else {
		m_caretpos += delta;
	}

	if( m_caretpos < m_rangebegin ) {
		m_rangebegin = m_caretpos;
		InvalidateVisibleText();
	}
	else if( m_caretpos > m_rangeend ) {
		m_rangebegin += m_caretpos - m_rangeend;
		InvalidateVisibleText();

		while( m_rangeend < m_caretpos ) {
			++m_rangebegin;
			InvalidateVisibleText();
		}

	}

	m_showcaret = true;
	UpdateCaret();
}

void Editbox::SetPadding( const sf::Vector2f &padding ) {
	m_label->SetPadding( padding );
	NotifyRedraw();
}

void Editbox::InvalidateVisibleText() {
	// Check boundaries, at first.
	if( m_rangebegin >= m_string.GetSize() ) {
		m_rangebegin = m_string.GetSize() - (m_string.GetSize() > 0 ? 1 : 0);
	}

	// Get last visible character.
	int  x( static_cast<int>( m_label->GetRect().Left ) );
	int  passwdadvance( 0 );
	m_rangeend = m_rangebegin;

	// Calculate how far we need to move per password "character".
	if( IsTextHidden() ) {
		for( std::size_t chpos = 0; chpos < m_passwordch.GetSize(); ++chpos ) {
			passwdadvance += m_label->GetTextStyle().GetFont().GetGlyph( m_passwordch[chpos], m_label->GetTextStyle().GetCharacterSize(), false ).Advance;
		}
	}

	// Move range end as far as possible.
	while( m_rangeend < m_string.GetSize() ) {
		if( !IsTextHidden() ) {
			x += m_label->GetTextStyle().GetFont().GetGlyph( m_string[m_rangeend], m_label->GetTextStyle().GetCharacterSize(), false ).Advance;
		}
		else {
			x += passwdadvance;
		}

		if( x >= GetRect().Left + GetRect().Width ) {
			break;
		}

		++m_rangeend;
	}

	if( !IsTextHidden() ) {
		// TODO: Improve this. sf::String needs SubString() method.
		sf::String  tmp;

		for( std::size_t pos = m_rangebegin; pos < m_rangeend; ++pos ) {
			tmp += m_string[pos];
		}

		m_label->SetString( tmp );
		//m_label->SetString( m_string.substr( m_rangebegin, m_rangeend - m_rangebegin ) );
	}
	else {
		sf::String    passwdstring;
		std::size_t   pos;

		for( pos = 0; pos < m_rangeend - m_rangebegin; ++pos ) {
			passwdstring += m_passwordch;
		}

		m_label->SetString( passwdstring );
	}

	NotifyRedraw();
}

void Editbox::SetAllowedCharacters( const sf::String &charset ) {
	m_allowedchars = charset;
}

void Editbox::SetTextStyle( const TextStyle& style ) {
	m_label->SetTextStyle( style );
}

}
