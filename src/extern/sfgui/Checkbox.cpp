#include <SFGUI/Checkbox.hpp>
#include <SFGUI/Align.hpp>
#include <SFGUI/DefaultImage.hpp>
#include <SFGUI/ImageData.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

namespace sfg {

sf::Image    Checkbox::DefaultImage( CreateDefaultImage( 2 * 15, 3 * 15, sf::Color( 200, 200, 200 ) ) );
static bool  DefaultInitialized( false );

Checkbox::Checkbox(
	const sf::FloatRect &rect,
	const sf::String &caption,
	const std::string &id
) :
	Widget( rect, id ),
	m_caption( Label::Create( sf::Vector2f( 0, 0 ), caption ) ),
	m_spacing( 5 ),
	m_box( 6 ),
	m_activebox( 0 ),
	m_hover( false ),
	m_pushed( false ),
	m_checked( false )
{
	SetImage( DefaultImage );
	OnResize( GetRect() );
}

Checkbox::Ptr Checkbox::Create( const sf::FloatRect &rect, const sf::String &caption, const std::string &id ) {
	// Create default image if not done, yet.
	if( !DefaultInitialized ) {
		DefaultImage.LoadFromPixels( 2 * 15, 3 * 15, CheckboxImage );
		DefaultInitialized = true;
	}

	return Ptr( new Checkbox( rect, caption, id ) );
}

void Checkbox::SetImage( const sf::Image &image ) {
	sf::Vector2i  step( image.GetWidth() / 2, image.GetHeight() / 3 );

	for( int x = 0; x < 2; ++x ) {
		for( int y = 0; y < 3; ++y ) {
			m_box[x * 3 + y].SetImage( image, sf::IntRect( x * step.x, y * step.y, step.x, step.y ) );
		}
	}
}

void Checkbox::SetCaption( const sf::String &caption ) {
	m_caption->SetString( caption );
	UpdateRect();
}

bool Checkbox::RenderImpl( sf::RenderTarget &target ) const {
	m_box[m_activebox].Render( target );
	m_caption->Render( target );

	return false;
}

bool Checkbox::OnEvent( const sf::Event &event ) {
	switch( event.Type ) {
		case sf::Event::MouseMoved:
			if( GetRect().Contains( sf::Vector2f( static_cast<float>( event.MouseMove.X ), static_cast<float>( event.MouseMove.Y ) ) ) ) {
				m_hover = true;
				AddHook( sf::Event::MouseMoved, shared_from_this() );

				if( !m_pushed ) {
					m_activebox = (m_checked ? 4 : 1);
				}
				else {
					m_activebox = (m_checked ? 5 : 2);
				}

				NotifyRedraw();
				return true;
			}
			else {
				m_hover = false;
				RemoveHook( sf::Event::MouseMoved, shared_from_this() );
				m_activebox = (m_checked ? 3 : 0);
				NotifyRedraw();
			}

			break;

		case sf::Event::MouseButtonPressed:
			if( event.MouseButton.Button == sf::Mouse::Left ) {
				m_pushed = true;
				m_activebox = (m_checked ? 5 : 2);
				AddHook( sf::Event::MouseButtonReleased, shared_from_this() );
				PlaySound( MousePressedSound );
				NotifyRedraw();
				return true;
			}

			break;

		case sf::Event::MouseButtonReleased:
			if( m_pushed && event.MouseButton.Button == sf::Mouse::Left ) {
				m_pushed = false;
				RemoveHook( sf::Event::MouseButtonReleased, shared_from_this() );

				if( m_hover ) {
					SetChecked( !m_checked );

					if( Checked ) {
						Checked( shared_from_this() );
					}

					NotifyRedraw();
					PlaySound( MouseReleasedSound );
					return true;
				}
			}

			break;

		default:
			break;
	}

	return false;
}

Checkbox::ResizeResult Checkbox::OnResize( const sf::FloatRect &/*oldrect*/ ) {
	float  wantedwidth( m_box[m_activebox].GetRect().Width + m_spacing + m_caption->GetRect().Width );

	if( GetRect().Width != wantedwidth ) {
		UpdateRect();
		return Cancel;
	}

	for( std::size_t x = 0; x < 2; ++x ) {
		for( std::size_t y = 0; y < 3; ++y ) {
			m_box[x * 3 + y].SetRect(
				sf::FloatRect(
					GetRect().Left,
					GetRect().Top,
					GetRect().Height,
					GetRect().Height
				)
			);
		}
	}

	AlignWidgetInRect(
		*m_caption,
		sf::FloatRect(
			GetRect().Left + m_box[m_activebox].GetRect().Width + m_spacing,
			GetRect().Top,
			GetRect().Width,
			GetRect().Height
		),
		AlignLeft | AlignMiddle
	);

	return Cancel;
}

void Checkbox::OnFocusChange( bool /*state*/ ) {
}

void Checkbox::UpdateRect() {
	float  wantedwidth( m_box[m_activebox].GetRect().Width + m_spacing + m_caption->GetRect().Width );
	SetRect( sf::FloatRect( GetRect().Left, GetRect().Top, wantedwidth, GetRect().Height ) );
}

void Checkbox::SetSpacing( float spacing ) {
	m_spacing = spacing;
	OnResize( GetRect() );
}

void Checkbox::SetChecked( bool state ) {
	m_checked = state;

	if( m_hover ) {
		m_activebox = (m_checked ? 4 : 1) + (m_pushed ? 1 : 0);
	}
	else {
		m_activebox = (m_checked ? 3 : 0) + (m_pushed ? 2 : 0);
	}
}

bool Checkbox::IsChecked() const {
	return m_checked;
}

const sf::Image &Checkbox::GetImage() const {
	return m_box[0].GetImage();
}

const sf::String &Checkbox::GetCaption() const {
	return m_caption->GetString();
}

void Checkbox::SetCaptionPadding( const sf::Vector2f &padding ) {
	m_caption->SetPadding( padding );
}

void Checkbox::SetCaptionStyle( const TextStyle& style ) {
	m_caption->SetTextStyle( style );
}

}
