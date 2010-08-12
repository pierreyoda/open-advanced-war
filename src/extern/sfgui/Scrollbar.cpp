#include <SFGUI/Scrollbar.hpp>
#include <SFGUI/Image.hpp>
#include <SFGUI/DefaultImage.hpp>
#include <SFGUI/ImageData.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <iostream> // XXX

namespace sfg {

sf::Image    Scrollbar::DefaultImage( CreateDefaultImage( 5 * 15, 3 * 15, sf::Color( 200, 200, 200 ) ) );
static bool  DefaultInitialized( false );

Scrollbar::Scrollbar( const sf::FloatRect &rect, bool horizontal, const std::string &id ) :
	Widget( rect, id ),
	m_horizontal( horizontal ),
	m_btnleft( Button::Create( sf::FloatRect( 0, 0, 0, 0 ), L"" ) ),
	m_btnright( Button::Create( sf::FloatRect( 0, 0, 0, 0 ), L"" ) ),
	m_btnslider( Button::Create( sf::FloatRect( 0, 0, 0, 0 ), L"" ) ),
	m_btnbackground( Button::Create( sf::FloatRect( 0, 0, 0, 0 ), L"" ) ),
	m_minslidersize( 15 ),
	m_min( 0 ),
	m_max( 0 ),
	m_current( 0 ),
	m_smallstep( 1 ),
	m_bigstep( 10 ),
	m_scrolling( false ),
	m_mousedelta( 0 ),
	m_lastmousepress( 0, 0 )
{
	SetImage( DefaultImage );
	SetRect( rect );

	m_btnleft->Clicked = Slot<Button::ClickSlot>( &Scrollbar::OnLeftClicked, this );
	m_btnleft->MousePressed = Slot<Button::MousePressSlot>( &Scrollbar::OnMousePressed, this );
	m_btnright->Clicked = Slot<Button::ClickSlot>( &Scrollbar::OnRightClicked, this );
	m_btnright->MousePressed = Slot<Button::MousePressSlot>( &Scrollbar::OnMousePressed, this );
	m_btnslider->MousePressed = Slot<Button::MousePressSlot>( &Scrollbar::OnSliderMousePressed, this );
	m_btnslider->MouseReleased = Slot<Button::MousePressSlot>( &Scrollbar::OnSliderMouseReleased, this );
	m_btnbackground->Clicked = Slot<Button::ClickSlot>( &Scrollbar::OnBackgroundClicked, this );
	m_btnbackground->MousePressed = Slot<Button::MousePressSlot>( &Scrollbar::OnMousePressed, this );
}

Scrollbar::Ptr Scrollbar::Create( const sf::FloatRect &rect, bool horizontal, const std::string &id ) {
	// Create default image if not done, yet.
	if( !DefaultInitialized ) {
		DefaultImage.LoadFromPixels( 5 * 15, 3 * 15, ScrollbarImage );
		DefaultInitialized = true;
	}

	// Need to workaround this way, since shared_from_this() is not allowed to be
	// called from the constructor.
	Ptr  scrollbar( new Scrollbar( rect, horizontal, id ) );

	scrollbar->Init();

	return scrollbar;
}

void Scrollbar::Init() {
	AddWidget( m_btnslider );
	AddWidget( m_btnleft );
	AddWidget( m_btnright );
	AddWidget( m_btnbackground );
}

void Scrollbar::SetImage( const sfg::Image &image ) {
	sf::Vector2i  step( image.GetSubRect().Width / 5, image.GetSubRect().Height / 3 );

	m_minslidersize = step.x;

	// Buttons.
	m_btnleft->SetImage(
		sfg::Image(
			image,
			sf::IntRect( 
				image.GetSubRect().Left,
				image.GetSubRect().Top,
				step.x,
				image.GetSubRect().Height
			)
		)
	);

	m_btnright->SetImage(
		sfg::Image(
			image,
			sf::IntRect( 
				image.GetSubRect().Left,
				image.GetSubRect().Top,
				step.x,
				image.GetSubRect().Height
			)
		)
	);

	// Background.
	m_btnbackground->SetImage(
		sfg::Image(
			image,
			sf::IntRect( 
				image.GetSubRect().Left + 2 * step.x,
				image.GetSubRect().Top,
				step.x,
				image.GetSubRect().Height
			)
		)
	);

	// Slider.
	m_btnslider->SetImage(
		sfg::Image(
			image,
			sf::IntRect( 
				image.GetSubRect().Left + 1 * step.x,
				image.GetSubRect().Top,
				step.x,
				image.GetSubRect().Height
			)
		)
	);

	// Arrows.
	m_firstarrow.SetImage( image );
	m_firstarrow.SetSubRect(
		sf::IntRect(
			image.GetSubRect().Left + (m_horizontal ? 3 : 4) * step.x,
			image.GetSubRect().Top  + 0,
			step.x,
			step.y
		)
	);

	m_secondarrow.SetImage( image );
	m_secondarrow.SetSubRect(
		sf::IntRect(
			image.GetSubRect().Left + (m_horizontal ? 3 : 4) * step.x,
			image.GetSubRect().Top  + step.y,
			step.x,
			step.y
		)
	);

	// Update positions.
	SetRect( GetRect() );
}

bool Scrollbar::RenderImpl( sf::RenderTarget &target ) const {
	m_btnleft->Render( target );
	m_btnright->Render( target );
	m_btnbackground->Render( target );
	m_btnslider->Render( target );
	target.Draw( m_firstarrow );
	target.Draw( m_secondarrow );

	return false;
}

bool Scrollbar::OnEvent( const sf::Event &event ) {
	if( event.Type == sf::Event::MouseButtonReleased && m_scrolling ) {
		m_scrolling = false;
		RemoveHook( sf::Event::MouseMoved, shared_from_this() );
		RemoveHook( sf::Event::MouseButtonReleased, shared_from_this() );
		RepositionSlider();
	}
	else if( event.Type == sf::Event::MouseMoved && m_scrolling ) {
		if( m_horizontal ) {
			SetValue(
				static_cast<int>(
					static_cast<float>( m_max - m_min ) *
					(
						(static_cast<float>( event.MouseMove.X - m_mousedelta ) - m_btnbackground->GetRect().Left) *
						100.f /
						(m_btnbackground->GetRect().Width - m_btnslider->GetRect().Width)
					) /
					100.f
				) + m_min
			);

			// "Smooth" scrolling.
			float  pos( static_cast<float>( event.MouseMove.X - m_mousedelta ) );

			if( pos < m_btnbackground->GetRect().Left ) {
				pos = m_btnbackground->GetRect().Left;
			}
			else if( pos >= m_btnbackground->GetRect().Left + m_btnbackground->GetRect().Width - m_btnslider->GetRect().Width ) {
				pos = m_btnbackground->GetRect().Left + m_btnbackground->GetRect().Width - m_btnslider->GetRect().Width;
			}

			m_btnslider->SetPosition( sf::Vector2f( pos, m_btnslider->GetRect().Top ) );
		}
		else {
			SetValue(
				static_cast<int>(
					static_cast<float>( m_max - m_min ) *
					(
						(static_cast<float>( event.MouseMove.Y - m_mousedelta ) - m_btnbackground->GetRect().Top) *
						100.f /
						(m_btnbackground->GetRect().Height - m_btnslider->GetRect().Height)
					) /
					100.f
				) + m_min
			);

			// "Smooth" scrolling.
			float  pos( static_cast<float>( event.MouseMove.Y - m_mousedelta ) );

			if( pos < m_btnbackground->GetRect().Top ) {
				pos = m_btnbackground->GetRect().Top;
			}
			else if( pos >= m_btnbackground->GetRect().Top + m_btnbackground->GetRect().Height - m_btnslider->GetRect().Height ) {
				pos = m_btnbackground->GetRect().Top + m_btnbackground->GetRect().Height - m_btnslider->GetRect().Height;
			}

			m_btnslider->SetPosition( sf::Vector2f( m_btnslider->GetRect().Left, pos ) );
		}

		FireChangedSignal();
		NotifyRedraw();
	}
	else if( event.Type == sf::Event::MouseWheelMoved ) {
		SetValue( m_current + (event.MouseWheel.Delta < 0 ? m_smallstep : -m_smallstep) );

		FireChangedSignal();
		NotifyRedraw();

		return true;
	}

	return false;
}

Scrollbar::ResizeResult Scrollbar::OnResize( const sf::FloatRect &/*oldrect*/ ) {
	// Set positions for buttons.
	sf::FloatRect  buttonrect(
		GetRect().Left,
		GetRect().Top,
		m_horizontal ? GetRect().Height : GetRect().Width,
		m_horizontal ? GetRect().Height : GetRect().Width
	);

	m_btnleft->SetRect( buttonrect );

	buttonrect.Left += (m_horizontal ? GetRect().Width - buttonrect.Width : 0);
	buttonrect.Top  += (!m_horizontal ? GetRect().Height - buttonrect.Height : 0);

	m_btnright->SetRect( buttonrect );

	// Background.
	sf::FloatRect  bgrect(
		m_horizontal  ? GetRect().Left       + buttonrect.Width  : GetRect().Left,
		!m_horizontal ? GetRect().Top        + buttonrect.Height : GetRect().Top,
		m_horizontal  ? GetRect().Width - 2  * buttonrect.Width  : GetRect().Width,
		!m_horizontal ? GetRect().Height - 2 * buttonrect.Height : GetRect().Height
	);

	m_btnbackground->SetRect( bgrect );

	// Arrows.
	m_firstarrow.SetPosition(
		m_btnleft->GetRect().Left + m_btnleft->GetRect().Width / 2 - static_cast<float>( m_firstarrow.GetSubRect().Width ) / 2,
		m_btnleft->GetRect().Top  + m_btnleft->GetRect().Height / 2 - static_cast<float>( m_firstarrow.GetSubRect().Height ) / 2
	);

	m_secondarrow.SetPosition(
		m_btnright->GetRect().Left + m_btnright->GetRect().Width / 2 - static_cast<float>( m_secondarrow.GetSubRect().Width ) / 2,
		m_btnright->GetRect().Top  + m_btnright->GetRect().Height / 2 - static_cast<float>( m_secondarrow.GetSubRect().Height ) / 2
	);

	// Slider.
	ResizeSlider();
	RepositionSlider();

	return Cancel;
}

void Scrollbar::OnFocusChange( bool /*state*/ ) {
}

void Scrollbar::ResizeSlider() {
	float  size( m_horizontal ? m_btnbackground->GetRect().Width : m_btnbackground->GetRect().Height );

	// Enforce a minimum size.
	size = std::max( static_cast<float>( m_minslidersize ), size / static_cast<float>( m_max - m_min + 1 ) );

	sf::FloatRect  newrect(
		m_btnbackground->GetRect().Left,
		m_btnbackground->GetRect().Top,
		(m_horizontal  ? size : m_btnbackground->GetRect().Width),
		(!m_horizontal ? size : m_btnbackground->GetRect().Height)
	);

	m_btnslider->SetRect( newrect );
	NotifyRedraw();
}

void Scrollbar::RepositionSlider() {
	float  size( 0 );

	if( m_horizontal ) {
		size = m_btnbackground->GetRect().Width - m_btnslider->GetRect().Width;
	}
	else {
		size = m_btnbackground->GetRect().Height - m_btnslider->GetRect().Height;
	}

	float  pos( size * static_cast<float>( m_current - m_min ) / static_cast<float>( std::max( 1, m_max - m_min ) ) );

	sf::FloatRect  newrect(
		m_btnbackground->GetRect().Left + (m_horizontal ? pos : 0  ),
		m_btnbackground->GetRect().Top  + (m_horizontal ? 0   : pos),
		m_btnslider->GetRect().Width,
		m_btnslider->GetRect().Height
	);

	m_btnslider->SetRect( newrect );
	NotifyRedraw();
}

void Scrollbar::SetMinimum( int min ) {
	if( m_min == min ) {
		return;
	}

	m_min = min;

	if( m_min > m_max ) {
		m_max = m_min;
	}

	if( m_current < m_min && m_current > m_max ) {
		m_current = m_min;
	}

	ResizeSlider();
	RepositionSlider();
}

void Scrollbar::SetMaximum( int max ) {
	if( m_max == max ) {
		return;
	}

	m_max = max;

	if( m_max < m_min ) {
		m_min = m_max;
	}

	if( m_current < m_min && m_current > m_max ) {
		m_current = m_min;
	}

	ResizeSlider();
	RepositionSlider();
}

void Scrollbar::SetValue( int value ) {
	if( m_current == value ) {
		return;
	}

	if( value < m_min ) {
		value = m_min;
	}
	else if( value > m_max ) {
		value = m_max;
	}

	m_current = value;
	RepositionSlider();
}

void Scrollbar::OnSliderMousePressed( Widget::Ptr /*widget*/, int x, int y, sf::Mouse::Button /*button*/ ) {
	m_scrolling = true;

	if( m_horizontal ) {
		m_mousedelta = x - static_cast<int>( m_btnslider->GetRect().Left );
	}
	else {
		m_mousedelta = y - static_cast<int>( m_btnslider->GetRect().Top );
	}

	AddHook( sf::Event::MouseButtonReleased, shared_from_this() );
	AddHook( sf::Event::MouseMoved, shared_from_this() );

	PlaySound( MousePressedSound );
}

void Scrollbar::OnLeftClicked( Widget::Ptr /*widget*/ ) {
	SetValue( m_current - m_smallstep );
	FireChangedSignal();
}

void Scrollbar::OnRightClicked( Widget::Ptr /*widget*/ ) {
	SetValue( m_current + m_smallstep );
	FireChangedSignal();
}

void Scrollbar::OnBackgroundClicked( Widget::Ptr /*widget*/ ) {
	if( m_horizontal ) {
		if( m_lastmousepress.x < m_btnslider->GetRect().Left ) {
			SetValue( m_current - m_bigstep );
		}
		else if( m_lastmousepress.x >= m_btnslider->GetRect().Left + m_btnslider->GetRect().Width ) {
			SetValue( m_current + m_bigstep );
		}
	}
	else {
		if( m_lastmousepress.y < m_btnslider->GetRect().Top ) {
			SetValue( m_current - m_bigstep );
		}
		else if( m_lastmousepress.y >= m_btnslider->GetRect().Top + m_btnslider->GetRect().Height ) {
			SetValue( m_current + m_bigstep );
		}
	}

	FireChangedSignal();
}

void Scrollbar::SetSmallStep( unsigned int step ) {
	m_smallstep = step;
}

void Scrollbar::SetBigStep( unsigned int step ) {
	m_bigstep = step;
}

void Scrollbar::OnMousePressed( Widget::Ptr /*widget*/, int x, int y, sf::Mouse::Button /*button*/ ) {
	PlaySound( MousePressedSound );

	// Remember mouse coordinates for later use in OnBackgroundClicked().
	m_lastmousepress = sf::Vector2i( x, y );
}

unsigned int Scrollbar::GetSmallStep() const {
	return m_smallstep;
}

unsigned int Scrollbar::GetBigStep() const {
	return m_bigstep;
}

void Scrollbar::OnSliderMouseReleased( Widget::Ptr /*widget*/, int /*x*/, int /*y*/, sf::Mouse::Button /*button*/ ) {
	if( m_scrolling ) {
		m_scrolling = false;
		RemoveHook( sf::Event::MouseMoved, shared_from_this() );
		RemoveHook( sf::Event::MouseButtonReleased, shared_from_this() );
		RepositionSlider();
	}
}

int Scrollbar::GetValue() const {
	return m_current;
}

void Scrollbar::FireChangedSignal() {
	if( Changed ) {
		Changed( shared_from_this() );
	}
}

}
