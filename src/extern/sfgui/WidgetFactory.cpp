#include <SFGUI/WidgetFactory.hpp>
#include <SFGUI/Skin.hpp>
#include <SFGUI/YAML.hpp>
#include <SFGUI/Label.hpp>
#include <SFGUI/Editbox.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/Checkbox.hpp>
#include <SFGUI/Radiobutton.hpp>
#include <SFGUI/Listbox.hpp>
#include <SFGUI/Scrollbar.hpp>
#include <SFML/System/String.hpp>

namespace sfg {

Widget::Ptr WidgetFactory::BuildWidget( const YAML::WidgetNode& base, const ::YAML::Node& node, const sfg::Widget::Ptr parent ) {
	if( base.Type == "Label" ) {
			sf::String  text( sfg::YAML::GetValue<std::string>( node, "Text", "" ) );
			return Label::Create( sf::Vector2f( base.Rect.Left, base.Rect.Top ), text, base.Id );
	}
	else if( base.Type == "Editbox" ) {
		sf::String   text( sfg::YAML::GetValue<std::string>( node, "Text", "" ) );
		sf::String   passwdchars( sfg::YAML::GetValue<std::string>( node, "PasswordChars", "" ) );
		sf::String   allowedchars( sfg::YAML::GetValue<std::string>( node, "AllowedChars", "" ) );
		std::size_t  maxlength( sfg::YAML::GetValue<std::size_t>( node, "MaxLength", 0 ) );

		Editbox::Ptr  widget( Editbox::Create( base.Rect, base.Id ) );

		widget->SetText( text );
		widget->SetAllowedCharacters( allowedchars );

		if( passwdchars.GetSize() ) {
			widget->HideText( passwdchars );
		}

		if( maxlength ) {
			widget->SetMaxLength( maxlength );
		}

		return widget;
	}
	else if( base.Type == "Button" ) {
		sf::String  caption( sfg::YAML::GetValue<std::string>( node, "Caption", "" ) );
		return Button::Create( base.Rect, caption, base.Id );
	}
	else if( base.Type == "Checkbox" ) {
		sf::String  caption( sfg::YAML::GetValue<std::string>( node, "Caption", "" ) );
		bool        checked( sfg::YAML::GetValue<bool>( node, "Checked", false ) );

		Checkbox::Ptr  widget( Checkbox::Create( base.Rect, caption, base.Id ) );
		widget->SetChecked( checked );

		return widget;
	}
	else if( base.Type == "Radiobutton" ) {
		sf::String   caption( sfg::YAML::GetValue<std::string>( node, "Caption", "" ) );
		bool         checked( sfg::YAML::GetValue<bool>( node, "Checked", false ) );
		std::string  linkid( sfg::YAML::GetValue<std::string>( node, "Link", "" ) );

		Radiobutton::Ptr  widget( Radiobutton::Create( base.Rect, caption, base.Id ) );

		if( !linkid.empty() ) {
			Radiobutton::Ptr  radlink( sfg::CastWidget<Radiobutton>( parent->FindWidget( linkid ) ) );

			if( radlink ) {
				widget->Link( radlink );
			}
			else {
				std::cerr << "SFGUI YAML error: Could not link " << base.Id << " to " << linkid << "." << std::endl;
			}
		}

		widget->SetChecked( checked );

		return widget;
	}
	else if( base.Type == "StringListbox" ) {
		return StringListbox::Create( base.Rect, base.Id );
	}
	else if( base.Type == "SpriteListbox" ) {
		return SpriteListbox::Create( base.Rect, base.Id );
	}
	else if( base.Type == "Scrollbar" ) {
		bool  horizontal( sfg::YAML::GetValue<bool>( node, "Horizontal", true ) );
		int   min( sfg::YAML::GetValue<int>( node, "Min", 0 ) );
		int   max( sfg::YAML::GetValue<int>( node, "Max", 0 ) );
		int   smallstep( sfg::YAML::GetValue<int>( node, "SmallStep", 1 ) );
		int   bigstep( sfg::YAML::GetValue<int>( node, "BigStep", 5 ) );
		int   value( sfg::YAML::GetValue<int>( node, "Value", 0 ) );

		Scrollbar::Ptr  widget( Scrollbar::Create( base.Rect, horizontal, base.Id ) );

		widget->SetMinimum( min );
		widget->SetMaximum( max );
		widget->SetSmallStep( smallstep );
		widget->SetBigStep( bigstep );
		widget->SetValue( value );

		return widget;
	}

	return Widget::Ptr();
}

bool WidgetFactory::ApplySkinSettings( sfg::Widget::Ptr widget, const std::string& cls, const Skin& skin ) {
	if( typeid( *widget ) == typeid( Label ) ) {
		Label::Ptr  label( sfg::CastWidget<Label>( widget ) );

		std::string  rcls( cls.empty() ? "Label" : cls );

		label->SetTextStyle( skin.GetTextStyle( rcls ) );
		label->SetPadding( skin.GetVector2f( rcls + ".Padding" ) );
	}
	else if( typeid( *widget ) == typeid( Button ) ) {
		Button::Ptr  button( sfg::CastWidget<Button>( widget ) );

		std::string  rcls( cls.empty() ? "Button" : cls );

		button->SetCaptionStyle( skin.GetTextStyle( rcls + ".Caption" ) );
		button->SetCaptionPadding( skin.GetVector2f( rcls + ".Caption.Padding" ) );

		// Buttons in a scrollbar don't have sounds themselves. Their images are
		// also managed by Scrollbar.
		if( !button->GetParent() || typeid( *button->GetParent() ) != typeid( Scrollbar ) ) {
			button->SetImage( skin.GetImage( rcls, &Button::DefaultImage ) );
			button->SetSound( MousePressedSound, skin.GetSoundBuffer( rcls + ".MousePressed" ) );
			button->SetSound( MouseReleasedSound, skin.GetSoundBuffer( rcls + ".MouseReleased" ) );
		}
	}
	else if( typeid( *widget ) == typeid( Editbox ) ) {
		Editbox::Ptr  editbox( sfg::CastWidget<Editbox>( widget ) );

		std::string  rcls( cls.empty() ? "Editbox" : cls );

		editbox->SetTextStyle( skin.GetTextStyle( rcls ) );
		editbox->SetPadding( skin.GetVector2f( rcls + ".Padding" ) );
		editbox->SetImage( skin.GetImage( rcls, &Editbox::DefaultImage ) );
		editbox->SetSound( MousePressedSound, skin.GetSoundBuffer( rcls + ".MousePressed" ) );
		editbox->SetSound( KeyPressedSound, skin.GetSoundBuffer( rcls + ".KeyPressed" ) );
	}
	else if( typeid( *widget ) == typeid( Checkbox ) ) {
		Checkbox::Ptr  checkbox( sfg::CastWidget<Checkbox>( widget ) );

		std::string  rcls( cls.empty() ? "Checkbox" : cls );

		checkbox->SetCaptionStyle( skin.GetTextStyle( rcls + ".Caption" ) );
		checkbox->SetCaptionPadding( skin.GetVector2f( rcls + ".Caption.Padding" ) );
		checkbox->SetImage( skin.GetImage( rcls, &Checkbox::DefaultImage ) );
		checkbox->SetSpacing( static_cast<float>( skin.GetInteger( rcls + ".Spacing" ) ) );
		checkbox->SetSound( MousePressedSound, skin.GetSoundBuffer( rcls + ".MousePressed" ) );
		checkbox->SetSound( MouseReleasedSound, skin.GetSoundBuffer( rcls + ".MouseReleased" ) );
	}
	else if( typeid( *widget ) == typeid( Radiobutton ) ) {
		Radiobutton::Ptr  radiobutton( sfg::CastWidget<Radiobutton>( widget ) );

		std::string  rcls( cls.empty() ? "Radiobutton" : cls );

		radiobutton->SetCaptionStyle( skin.GetTextStyle( rcls + ".Caption" ) );
		radiobutton->SetCaptionPadding( skin.GetVector2f( rcls + ".Caption.Padding" ) );
		radiobutton->SetImage( skin.GetImage( rcls, &Radiobutton::DefaultImage ) );
		radiobutton->SetSpacing( static_cast<float>( skin.GetInteger( rcls + ".Spacing" ) ) );
		radiobutton->SetSound( MousePressedSound, skin.GetSoundBuffer( rcls + ".MousePressed" ) );
		radiobutton->SetSound( MouseReleasedSound, skin.GetSoundBuffer( rcls + ".MouseReleased" ) );
	}
	else if( typeid( *widget ) == typeid( Scrollbar ) ) {
		Scrollbar::Ptr  scrollbar( sfg::CastWidget<Scrollbar>( widget ) );

		std::string  rcls( cls.empty() ? "Scrollbar" : cls );

		// Don't change stuff for scrollbars inside listboxes.
		if( !scrollbar->GetParent() || typeid( *scrollbar->GetParent() ) != typeid( StringListbox ) ) {
			scrollbar->SetImage( skin.GetImage( rcls, &Scrollbar::DefaultImage ) );
			scrollbar->SetSound( MousePressedSound, skin.GetSoundBuffer( rcls + ".MousePressed" ) );
			scrollbar->SetSound( MouseReleasedSound, skin.GetSoundBuffer( rcls + ".MouseReleased" ) );
		}
	}
	else if( typeid( *widget ) == typeid( StringListbox ) ) {
		StringListbox::Ptr  listbox( sfg::CastWidget<StringListbox>( widget ) );

		std::string  rcls( cls.empty() ? "Listbox" : cls );

		// Apply prototypes.
		Label::Ptr  labelpt( Label::Create( sf::Vector2f( 0, 0 ), "" ) );
		ApplySkinSettings( labelpt, "Listbox.Item.Label", skin );
		listbox->SetPrototype( labelpt );

		listbox->SetImage( skin.GetImage( rcls, &StringListbox::DefaultImage ) );
		listbox->SetScrollbarImage( skin.GetImage( rcls + ".Scrollbar", &Scrollbar::DefaultImage ) );
		listbox->SetScrollbarSize( static_cast<float>( skin.GetInteger( rcls + ".Scrollbar.Size", 15 ) ) );
		listbox->SetItemPadding( skin.GetVector2f( rcls + ".Item.Padding" ) );
		listbox->SetSound( MousePressedSound, skin.GetSoundBuffer( rcls + ".MousePressed" ) );
		listbox->SetSound( MouseReleasedSound, skin.GetSoundBuffer( rcls + ".MouseReleased" ) );
	}
	else if( typeid( *widget ) == typeid( SpriteListbox ) ) {
		SpriteListbox::Ptr  listbox( sfg::CastWidget<SpriteListbox>( widget ) );

		std::string  rcls( cls.empty() ? "Listbox" : cls );

		listbox->SetImage( skin.GetImage( rcls, &SpriteListbox::DefaultImage ) );
		listbox->SetScrollbarImage( skin.GetImage( rcls + ".Scrollbar", &Scrollbar::DefaultImage ) );
		listbox->SetScrollbarSize( static_cast<float>( skin.GetInteger( rcls + ".Scrollbar.Size", 15 ) ) );
		listbox->SetItemPadding( skin.GetVector2f( rcls + ".Item.Padding" ) );
		listbox->SetSound( MousePressedSound, skin.GetSoundBuffer( rcls + ".MousePressed" ) );
		listbox->SetSound( MouseReleasedSound, skin.GetSoundBuffer( rcls + ".MouseReleased" ) );
	}
	else {
		return false;
	}

	return true;
}

}
