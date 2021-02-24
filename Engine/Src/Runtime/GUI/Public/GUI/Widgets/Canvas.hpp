#pragma once

#include <Core/Definitions.hpp>
#include <GUI/Widgets/Widget.hpp>

namespace Fade { inline namespace GUI {

/**
 * GUI Canvas class
 */
class FADE_GUI_API CCanvas : public CWidget
{
public:
	virtual ~CCanvas() override = default;
};

} }
