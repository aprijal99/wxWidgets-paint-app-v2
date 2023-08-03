#include <wx/wx.h>
#include <wx/graphics.h>
#include "SelectablePane.hpp"
#include "ColorPane.hpp"

ColorPane::ColorPane(wxWindow* parent, const wxColour& paneColor)
  : SelectablePane(parent), color(paneColor)
{
}

void ColorPane::DrawContent(wxGraphicsContext* gc, const wxRect& rect, int roundness) const
{
  gc->SetPen(wxPen(color));
  gc->SetBrush(wxBrush(color));
  gc->DrawRoundedRectangle(rect.GetX(), rect.GetY(), rect.GetWidth(), rect.GetHeight(), roundness);
}