#include <wx/graphics.h>
#include <memory>
#include "DrawingView.hpp"
#include "App.hpp"
// #include "DrawingCanvas.hpp"

wxIMPLEMENT_DYNAMIC_CLASS(DrawingView, wxView);

DrawingDocument* DrawingView::GetDocument() const
{
  return wxStaticCast(wxView::GetDocument(), DrawingDocument);
}

bool DrawingView::OnCreate(wxDocument* doc, long flags)
{
  if (!wxView::OnCreate(doc, flags))
  {
    return false;
  }

  App::SetupCanvasForView(this);

  return true;
}

void DrawingView::OnDraw(wxDC* dc)
{
  dc->SetBackground(*wxWHITE_BRUSH);
  dc->Clear();

  std::unique_ptr<wxGraphicsContext> gc(wxGraphicsContext::CreateFromUnknownDC(*dc));
  if (gc)
  {
    std::vector<Path> squiggles = GetDocument()->squiggles;
    for (const Path& s: squiggles)
    {
      std::vector<wxPoint2DDouble> pointsVector = s.points;
      if (pointsVector.size() > 1)
      {
        gc->SetPen(wxPen(s.color, s.width));
        gc->StrokeLines(pointsVector.size(), pointsVector.data());
      }
    }
  }
}

void DrawingView::OnChangeFilename()
{
  wxString appName = wxTheApp->GetAppDisplayName();
  wxString title;

  wxString docName = GetDocument()->GetUserReadableName();
  title = docName + (GetDocument()->IsModified() ? " - Edited" : "") + wxString(_(" - ")) + appName;

  GetFrame()->SetLabel(title);
}

void DrawingView::OnClear()
{
  GetDocument()->squiggles.clear();
  GetDocument()->Modify(true);
}

bool DrawingView::OnClose(bool deleteWindow)
{
  if (deleteWindow)
  {
    App::SetupCanvasForView(nullptr);
  }

  return wxView::OnClose(deleteWindow);
}

void DrawingView::OnMouseDown(wxPoint pt)
{
  wxColour currentColor = App::GetToolSettings().currentColor;
  int currentWidth = App::GetToolSettings().currentWidth;

  GetDocument()->squiggles.push_back({{}, currentColor, currentWidth});

  AddPointToCurrentSquiggle(pt);
}

void DrawingView::OnMouseDrag(wxPoint pt)
{
  AddPointToCurrentSquiggle(pt);
}

void DrawingView::OnMouseDragEnd()
{
}

void DrawingView::AddPointToCurrentSquiggle(wxPoint pt)
{
  Path& currentSquiggle = GetDocument()->squiggles.back();
  currentSquiggle.points.push_back(pt);

  GetDocument()->Modify(true);
}