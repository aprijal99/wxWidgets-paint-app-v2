#include <wx/wx.h>
#include <wx/graphics.h>
#include "SelectablePane.hpp"
#include "PenSizePane.hpp"

PenSizePane::PenSizePane(wxWindow* parent, int penW)
  : SelectablePane(parent), penWidth(penW)
{
}

void PenSizePane::DrawContent(wxGraphicsContext* gc, const wxRect& rect, int roundness) const
{
  gc->SetPen(*wxWHITE_PEN);
  gc->SetBrush(*wxWHITE_BRUSH);
  gc->DrawRoundedRectangle(rect.GetX(), rect.GetY(), rect.GetWidth(), rect.GetHeight(), roundness);

  gc->SetPen(*wxBLACK_PEN);
  gc->SetBrush(*wxBLACK_BRUSH);
  gc->DrawEllipse(
    rect.GetX() + rect.GetWidth() / 2 - penWidth / 2,
    rect.GetY() + rect.GetHeight() / 2 - penWidth / 2,
    penWidth,
    penWidth
  );
}