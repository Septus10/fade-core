
#include <GUI/Gwen.h>
#include <GUI/Controls/SplitterBar.h>

using namespace Gwen;
using namespace Controls;

GWEN_CONTROL_CONSTRUCTOR( SplitterBar )
{
	SetTarget( this );
	RestrictToParent( true );
}

void SplitterBar::Layout( Skin::Base* /*skin*/ )
{
	MoveTo( X(), Y() );
}