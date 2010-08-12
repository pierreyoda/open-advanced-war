#include <SFGUI/GUI.hpp>
#include <SFGUI/Image.hpp>
#include <SFGUI/YAML.hpp>
#include <SFGUI/WidgetFactory.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <fstream>

namespace sfg {

GUI::GUI( const sf::FloatRect &size ) :
	m_top( Container::Create( size ) ),
	m_skin( Skin() ),
	m_usesystempointer( true ),
	m_userendertoimage( false ),
	m_skinloaded( false )
{
	// "Brute-force" enabling of render-to-image. IsAvailable() doesn't work on
	// all systems.
	if( !m_renderimage.Create( static_cast<unsigned int>( size.Width ), static_cast<unsigned int>( size.Height ) ) ) {
		m_userendertoimage = false;
	}
	else {
		m_userendertoimage = true;
	}

	m_rendersprite.SetImage( m_renderimage.GetImage(), true );
	m_rendersprite.SetBlendMode( sf::Blend::None );

	RegisterFactory<WidgetFactory>();
}

GUI::~GUI() {
	Clear();

	// Delete factories.
	FactoryList::iterator  iter( m_factories.begin() );
	FactoryList::iterator  iterend( m_factories.end() );

	for( ; iter != iterend; ++iter ) {
		delete *iter;
	}

	m_factories.clear();
}

const Skin &GUI::GetSkin() const {
	return m_skin;
}

void GUI::AddWidget( Widget::Ptr widget ) {
	// Apply skin settings to widget and its children.
	Widget::WidgetList  list;

	widget->GetChildren( list );

	Widget::WidgetList::iterator  witer( list.begin() );
	Widget::WidgetList::iterator  witerend( list.end() );

	for( ; witer != witerend; ++witer ) {
		ApplySkinSettings( *witer );
	}

	m_top->AddWidget( widget );
	m_top->Invalidate();
}

void GUI::Render( sf::RenderTarget &target ) const {
	if( !m_userendertoimage ) {
		m_top->Render( target );
	}
	else {
		if( m_top->IsInvalidated() ) {
			m_renderimage.Clear( sf::Color( 0, 0, 0, 0 ) );
			m_top->Render( m_renderimage );
			m_renderimage.Display();
		}

		target.Draw( m_rendersprite );
	}


	if( !m_usesystempointer ) {
		target.Draw( m_mousepointer );
	}
}

bool GUI::HandleEvent( const sf::Event &event ) {
	if( !m_usesystempointer && event.Type == sf::Event::MouseMoved ) {
		m_mousepointer.SetPosition( static_cast<float>( event.MouseMove.X ), static_cast<float>( event.MouseMove.Y ) );
	}

	return m_top->HandleEvent( event );
}

const sf::FloatRect &GUI::GetRect() const {
	return m_top->GetRect();
}

bool GUI::ApplySkinSettings( Widget::Ptr widget, const std::string& cls ) {
	if( !m_skinloaded ) {
		return false;
	}

	FactoryList::iterator  faciter( m_factories.begin() );
	FactoryList::iterator  faciterend( m_factories.end() );

	// Go through all factories and try to apply settings.
	for( ; faciter != faciterend; ++faciter ) {
		if( (*faciter)->ApplySkinSettings( widget, cls, m_skin ) ) {
			return true;
		}
	}

	return false;
}

bool GUI::LoadSkinFromFile( const std::string &filename ) {
	if( !m_skin.LoadFromFile( filename ) ) {
		return false;
	}

	m_skinloaded = true;

	// Re-initialize widgets.
	Widget::WidgetList  list;

	m_top->GetChildren( list );

	Widget::WidgetList::iterator  witer( list.begin() );
	Widget::WidgetList::iterator  witerend( list.end() );

	for( ; witer != witerend; ++witer ) {
		ApplySkinSettings( *witer );
	}

	// Check if a custom mouse pointer is about to be used.
	if( m_skin.GetInteger( "Mouse.System", 1 ) == 1 ) {
		UseSystemPointer();
	}
	else {
		UseCustomPointer( m_skin.GetImage( "Mouse" ), m_skin.GetVector2f( "Mouse.Origin" ) );
	}

	return true;
}

void GUI::UseCustomPointer( const sf::Image &image, const sf::Vector2f &origin ) {
	m_usesystempointer = false;
	m_mousepointer.SetImage( image, true );
	m_mousepointer.SetOrigin( origin );
}

void GUI::UseSystemPointer() {
	m_usesystempointer = true;
	m_mousepointer.SetImage( m_skin.GetImage( "" ) );
}

void GUI::Clear() {
	m_top->RemoveChildren();
}

bool GUI::LoadYAML( const std::string &filename, Widget::Ptr parentwidget ) {
	// Check if caller wants an own parent.
	Widget::Ptr  parent( parentwidget ? parentwidget : m_top );

	std::ifstream    in( filename.c_str() );
	::YAML::Parser   parser( in );
	::YAML::Node     doc;

	if( !in.is_open() ) {
		return false;
	}

	parser.GetNextDocument( doc );

	const ::YAML::Node*  widgetsnode( doc.FindValue( "Widgets" ) );
	if( !widgetsnode ) {
		return false;
	}

	// Go through all widgets.
	sfg::YAML::WidgetNode  base;
	Widget::Ptr            widget;
	FactoryList::iterator  faciter( m_factories.begin() );
	FactoryList::iterator  faciterend( m_factories.end() );

	for( std::size_t widgetid = 0; widgetid < widgetsnode->size(); ++widgetid ) {
		// Extract base infos.
		(*widgetsnode)[widgetid] >> base;

		// Go through all factories and try to build the widget.
		for( faciter = m_factories.begin(); faciter != faciterend; ++faciter ) {
			widget = (*faciter)->BuildWidget( base, (*widgetsnode)[widgetid], parent );

			if( widget ) {
				ApplySkinSettings( widget );
				parent->AddWidget( widget );
				widget.reset();
				break;
			}
		}
	}

	// Get skin filename and load it.
	try {
		std::string  skinfilename;
		doc["Skin"] >> skinfilename;

		if( !LoadSkinFromFile( skinfilename ) ) {
			std::cerr << "SFGUI YAML error: Failed to load skin from \"" << skinfilename << "\"." << std::endl;
		}
	}
	catch( const ::YAML::Exception &e ) {
	}

	in.close();
	return true;
}

Widget::Ptr GUI::FindWidget( const std::string &id ) {
	return m_top->FindWidget( id );
}

void GUI::EnableRenderToImage( bool enable ) {
	m_userendertoimage = enable;
}

}
