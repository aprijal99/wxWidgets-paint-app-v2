#pragma once

#include <wx/wx.h>
#include <wx/graphics.h>
#include "SelectablePane.hpp"

class PenSizePane: public SelectablePane
{
public:
  PenSizePane(wxWindow* parent, int penW);
  int penWidth;

private:
  virtual void DrawContent(wxGraphicsContext* gc, const wxRect& rect, int roundness) const override;
};