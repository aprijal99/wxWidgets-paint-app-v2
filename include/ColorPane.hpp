#pragma once

#include <wx/wx.h>
#include <wx/graphics.h>
#include "SelectablePane.hpp"

class ColorPane: public SelectablePane
{
public:
  ColorPane(wxWindow* parent, const wxColour& paneColor);
  wxColour color;

private:
  virtual void DrawContent(wxGraphicsContext* gc, const wxRect& rect, int roundness) const override;
};