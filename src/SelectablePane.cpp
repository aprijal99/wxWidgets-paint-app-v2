#include <wx/wx.h>
#include <wx/graphics.h>
#include <wx/dcbuffer.h>
#include <wx/settings.h>
#include "SelectablePane.hpp"

wxBEGIN_EVENT_TABLE(SelectablePane, wxWindow)
  EVT_PAINT(SelectablePane::OnPaint)
wxEND_EVENT_TABLE()

SelectablePane::SelectablePane(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size)
  : wxWindow(parent, id, pos, size, wxFULL_REPAINT_ON_RESIZE)
{
  this->SetBackgroundStyle(wxBG_STYLE_PAINT); // Mandatory for Windows OS
}

void SelectablePane::OnPaint(wxPaintEvent& event)
{
  wxAutoBufferedPaintDC dc(this);
  dc.SetBackground(this->GetParent()->GetBackgroundColour());
  dc.Clear();

  wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
  if (gc)
  {
    wxRect selectionRect(0, 0, this->GetSize().GetWidth(), this->GetSize().GetHeight());
    selectionRect.Deflate(FromDIP(1));
    
    wxRect contentRect = selectionRect;
    contentRect.Deflate(FromDIP(2));

    const int roundness = FromDIP(4);

    DrawContent(gc, contentRect, roundness);

    if (this->selected)
    {
      gc->SetPen(wxSystemSettings::GetAppearance().IsDark() ? *wxWHITE_PEN : *wxBLACK_PEN);
      gc->SetBrush(*wxTRANSPARENT_BRUSH);
      gc->DrawRoundedRectangle(selectionRect.GetX(), selectionRect.GetY(), selectionRect.GetWidth(), selectionRect.GetHeight(), roundness);
    }

    delete gc;
  }
}