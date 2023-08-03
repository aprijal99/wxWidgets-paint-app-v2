#pragma once

#include <wx/wx.h>
#include <wx/docview.h>
#include "DrawingDocument.hpp"

class DrawingView: public wxView
{
public:
  DrawingDocument* GetDocument() const;
  bool OnCreate(wxDocument* doc, long flags) override;
  void OnDraw(wxDC* dc) override;
  void OnChangeFilename() override;
  void OnClear();
  bool OnClose(bool deleteWindow = true) override;

  void OnMouseDown(wxPoint);
  void OnMouseDrag(wxPoint);
  void OnMouseDragEnd();

  wxDECLARE_DYNAMIC_CLASS(DrawingView);

private:
  void AddPointToCurrentSquiggle(wxPoint);
};