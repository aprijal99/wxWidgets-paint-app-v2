#pragma once

#include <wx/wx.h>
#include <wx/graphics.h>
#include <wx/dcbuffer.h>

class SelectablePane: public wxWindow
{
  wxDECLARE_EVENT_TABLE();
public:
  SelectablePane(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);
  void OnPaint(wxPaintEvent& event); // Event paint handler
  bool selected = false;

  // Override window size
  wxSize DoGetBestSize() const override { return FromDIP(wxSize(45, 45)); }

protected:
  virtual void DrawContent(wxGraphicsContext* gc, const wxRect& rect, int roundness) const = 0;
};