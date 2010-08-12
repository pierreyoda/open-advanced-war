#include <SFGUI/Widget.hpp>
#include <cmath>

namespace sfg {

Widget::Widget( const sf::FloatRect &rect, const std::string &id ) :
	m_id( id ),
	m_visible( true ),
	m_needsredraw( false ),
	m_focused( false ),
	m_rect( rect )
{
}

Widget::~Widget() {
}

Widget::Ptr Widget::FindWidget( const std::string &id ) {
	// At first try to find Id in own widget.
	ChildrenMap::iterator  iter( m_childrenmap.find( id ) );

	if( iter != m_childrenmap.end() ) {
		return iter->second;
	}

	// If not found, go deeper.
	ChildrenMap::iterator  iterend( m_childrenmap.end() );
	Ptr                    ptr;

	for( iter = m_childrenmap.begin(); iter != iterend; ++iter ) {
		ptr = iter->second->FindWidget( id );
		if( ptr ) {
			return ptr;
		}
	}

	return Ptr();
}

bool Widget::HasId() const {
	return !m_id.empty();
}

bool Widget::AddWidget( Ptr widget ) {
	if( widget->HasId() && FindWidget( widget->GetId() ) != 0 ) {
		return false;
	}

	m_childrenmap[widget->GetId()] = widget;
	m_children.push_back( widget );

	widget->SetParent( shared_from_this() );
	NotifyRedraw();

	return true;
}

void Widget::Render( sf::RenderTarget &target ) const {
	m_needsredraw = false;

	if( !m_visible || !RenderImpl( target ) ) {
		return;
	}

	if( m_children.size() ) {
		WidgetList::const_iterator  iter( m_children.begin() );
		WidgetList::const_iterator  iterend( m_children.end() );

		for( ; iter != iterend; ++iter ) {
			(*iter)->Render( target );
		}
	}
}

bool Widget::HandleEvent( const sf::Event &event ) {
	// Fire default events.
	if( event.Type == sf::Event::MouseButtonPressed ) {
		if( MousePressed ) {
			MousePressed( shared_from_this(), event.MouseButton.X, event.MouseButton.Y, event.MouseButton.Button );
		}
	}
	else if( event.Type == sf::Event::MouseButtonReleased ) {
		if( MouseReleased ) {
			MouseReleased( shared_from_this(), event.MouseButton.X, event.MouseButton.Y, event.MouseButton.Button );
		}
	}
	else if( event.Type == sf::Event::MouseMoved ) {
		if( MouseMoved ) {
			MouseMoved( shared_from_this(), event.MouseButton.X, event.MouseButton.Y );
		}
	}
	else if( IsFocused() ) {
		if( event.Type == sf::Event::KeyPressed ) {
			if( KeyPressed ) {
				KeyPressed( shared_from_this(), event.Key.Code );
			}
		}
		else if( event.Type == sf::Event::KeyReleased ) {
			if( KeyReleased ) {
				KeyReleased( shared_from_this(), event.Key.Code );
			}
		}
	}

	if( OnEvent( event ) ) {
		return true;
	}
	else if( m_children.size() == 0 ) { // If widget doesn't have children, stop.
		return false;
	}

	// At first check if there're hooks for the event.
	if( m_hooks[event.Type].size() > 0 ) {
		WidgetList::iterator  witer( m_hooks[event.Type].begin() );
		WidgetList::iterator  witerend( m_hooks[event.Type].end() );
		WidgetList::iterator  witernext;

		while( witer != witerend ) {
			// Next iterator is cached because child widget could remove itself from
			// the hook list, thus making the current iterator invalid.
			witernext = witer;
			++witernext;

			if( (*witer)->HandleEvent( event ) ) {
				return true;
			}

			witer = witernext;
		}
	}

	// Check if we have a widget focused and if the event would fit for
	// delegating the event to the focused widget.
	if( m_focus && (event.Type == sf::Event::KeyPressed || event.Type == sf::Event::KeyReleased || event.Type == sf::Event::TextEntered) ) {
		if( m_focus->HandleEvent( event ) ) {
			return true;
		}
	}

	// Now go through all children and let them handle the event.
	WidgetList::iterator  iter( m_children.begin() );
	WidgetList::iterator  iterend( m_children.end() );
	bool                  fits( false );

	for( ; iter != iterend; ++iter ) {
		if( (*iter)->IsVisible() == false ) {
			continue;
		}

		// Let the widget only handle the event when it "fits".
		switch( event.Type ) {
			case sf::Event::MouseMoved:
				if( (*iter)->GetRect().Contains( sf::Vector2f( static_cast<float>( event.MouseMove.X ), static_cast<float>( event.MouseMove.Y ) ) ) ) {
					fits = true;
				}

				break;

			case sf::Event::MouseButtonPressed:
			case sf::Event::MouseButtonReleased:
				if( (*iter)->GetRect().Contains( sf::Vector2f( static_cast<float>( event.MouseButton.X ), static_cast<float>( event.MouseButton.Y ) ) ) ) {
					fits = true;
				}

				break;

			case sf::Event::MouseWheelMoved:
				if( (*iter)->GetRect().Contains( sf::Vector2f( static_cast<float>( event.MouseWheel.X ), static_cast<float>( event.MouseWheel.Y ) ) ) ) {
					fits = true;
				}

				break;

			default:
				break;
		}

		if( fits ) {
			if( event.Type == sf::Event::MouseButtonPressed ) {
				(*iter)->GrabFocus();
			}

			if( (*iter)->HandleEvent( event ) ) {
				return true;
			}

			fits = false;
		}
	}

	// Release focus if clicked nowhere.
	if( event.Type == sf::Event::MouseButtonPressed ) {
		ReleaseFocus();
	}

	return false;
}

void Widget::OnChildDetached( Ptr widget ) {
	// Remove id -> widget mapping if widget has id.
	if( widget->HasId() ) {
		ChildrenMap::iterator  mapiter( m_childrenmap.find( widget->GetId() ) );

		if( mapiter != m_childrenmap.end() ) {
			m_childrenmap.erase( mapiter );
		}
	}

	RemoveHooks( widget );

	if( m_focus == widget ) {
		m_focus.reset();
	}

	WidgetList::iterator  iter( std::find( m_children.begin(), m_children.end(), widget ) );
	if( iter != m_children.end() ) {
		m_children.erase( iter );
	}
}

const std::string &Widget::GetId() const {
	return m_id;
}

void Widget::SetPosition( const sf::Vector2f &position ) {
	sf::Vector2f  delta( position.x - m_rect.Left, position.y - m_rect.Top );
	SetRect( sf::FloatRect( m_rect.Left + delta.x, m_rect.Top + delta.y, m_rect.Width, m_rect.Height ) );
}

void Widget::Detach() {
	if( !m_parent ) {
		return;
	}

	m_parent->OnChildDetached( shared_from_this() );
	m_parent.reset();
}

void Widget::SetParent( Ptr parent ) {
	Detach(); // Make sure the widget gets detached first.

	m_parent = parent;

	// Give focus to parent.
	if( m_focus ) {
		m_parent->GrabFocus( m_focus );
		m_focus.reset();
	}
}

void Widget::RemoveHooks( Ptr widget ) {
	HookMap::iterator  hookiter( m_hooks.begin() );
	HookMap::iterator  hookiterend( m_hooks.end() );

	for( ; hookiter != hookiterend; ++hookiter ) {
		WidgetList::iterator  iter( std::find( hookiter->second.begin(), hookiter->second.end(), widget ) );

		if( iter != hookiter->second.end() ) {
			hookiter->second.erase( iter );
		}
	}
}

void Widget::RemoveHook( sf::Event::EventType eventtype, Ptr widget ) {
	if( m_parent ) {
		m_parent->RemoveHook( eventtype, widget );
	}
	else {
		WidgetList::iterator  iter( std::find( m_hooks[eventtype].begin(), m_hooks[eventtype].end(), widget ) );

		if( iter != m_hooks[eventtype].end() ) {
			m_hooks[eventtype].erase( iter );
		}
	}
}

void Widget::AddHook( sf::Event::EventType eventtype, Ptr widget ) {
	// If this widget has a parent, delegate adding the hook to it.
	if( m_parent ) {
		m_parent->AddHook( eventtype, widget );
	}
	else {
		// At first check if widget isn't already hooked.
		WidgetList::iterator  iter( std::find( m_hooks[eventtype].begin(), m_hooks[eventtype].end(), widget ) );

		if( iter == m_hooks[eventtype].end() ) {
			m_hooks[eventtype].push_back( widget );
		}
	}
}

const sf::FloatRect& Widget::GetRect() const {
	return m_rect;
}

void Widget::RemoveHooks( sf::Event::EventType &eventtype ) {
	m_hooks[eventtype].clear();
}

void Widget::SetRect( const sf::FloatRect &rect ) {
	sf::FloatRect  oldrect( m_rect );

	// Round values to get a pixel-perfect display.
	m_rect.Left   = std::floor( rect.Left + 0.5f );
	m_rect.Top    = std::floor( rect.Top + 0.5f );
	m_rect.Width  = std::floor( rect.Width + 0.5f );
	m_rect.Height = std::floor( rect.Height + 0.5f );

	ResizeResult  result( OnResize( oldrect ) );
	if( result == Error ) {
		m_rect = oldrect;
		return;
	}
	else if( result == Cancel ) {
		return;
	}

	// Get deltas and move children.
	if( m_children.size() ) {
		sf::Vector2f  delta( m_rect.Left - oldrect.Left, m_rect.Top - oldrect.Top );

		WidgetList::iterator  witer( m_children.begin() );
		WidgetList::iterator  witerend( m_children.end() );

		for( ; witer != witerend; ++witer ) {
			(*witer)->SetPosition( (*witer)->GetPosition() + delta );
		}
	}

	// Changing the rect means that we have to invalidate the view for sure.
	NotifyRedraw();
}

sf::Vector2f Widget::GetPosition() const {
	return sf::Vector2f( m_rect.Left, m_rect.Top );
}

void Widget::Show( bool show ) {
	m_visible = show;
	NotifyRedraw();
}

bool Widget::IsVisible() const {
	return m_visible;
}

void Widget::GrabFocus( Ptr widget ) {
	if( m_parent ) {
		m_parent->GrabFocus( widget );
		return;
	}

	if( m_focus ) {
		m_focus->SetFocus( false );
	}

	m_focus = widget;
	widget->SetFocus( true );
}

void Widget::SetFocus( bool state ) {
	m_focused = state;
	OnFocusChange( state );
}

bool Widget::IsFocused() const {
	return m_focused;
}

void Widget::ReleaseFocus() {
	if( m_parent ) {
		m_parent->ReleaseFocus();
		return;
	}

	if( m_focus ) {
		m_focus->SetFocus( false );
		m_focus.reset();
	}
}

void Widget::GetChildren( WidgetList &list ) {
	list.push_back( shared_from_this() );

	if( m_children.size() ) {
		WidgetList::const_iterator  witer( m_children.begin() );
		WidgetList::const_iterator  witerend( m_children.end() );

		for( ; witer != witerend; ++witer ) {
			(*witer)->GetChildren( list );
		}
	}
}

Widget::Ptr Widget::GetParent() {
	return m_parent;
}

void Widget::GrabFocus() {
	GrabFocus( shared_from_this() );
}

void Widget::RemoveChildren() {
	m_children.clear();
	m_childrenmap.clear();
	m_hooks.clear();
}

void Widget::NotifyRedraw() const {
	if( m_parent ) {
		m_parent->NotifyRedraw();
	}
	else {
		m_needsredraw = true;
	}
}

bool Widget::IsInvalidated() const {
	return m_needsredraw;
}

void Widget::Invalidate() const {
	m_needsredraw = true;
}

}
