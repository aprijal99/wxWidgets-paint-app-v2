#include <wx/graphics.h>
#include <wx/dcbuffer.h>
#include "DrawingCanvas.hpp"

DrawingCanvas::DrawingCanvas(wxWindow* parent, DrawingView* view, wxWindowID id, const wxPoint& pos, const wxSize& size)
  : wxWindow(parent, id, pos, size), view(view)
{
  this->SetBackgroundStyle(wxBG_STYLE_PAINT);

  this->Bind(wxEVT_PAINT, &DrawingCanvas::OnPaint, this);
  this->Bind(wxEVT_LEFT_DOWN, &DrawingCanvas::OnMouseDown, this);
  this->Bind(wxEVT_MOTION, &DrawingCanvas::OnMouseMove, this);
  this->Bind(wxEVT_LEFT_UP, &DrawingCanvas::OnMouseUp, this);
  this->Bind(wxEVT_LEAVE_WINDOW, &DrawingCanvas::OnMouseLeave, this);

  BuildContextMenu();

  this->Bind(wxEVT_CONTEXT_MENU, &DrawingCanvas::OnContextMenuEvent, this);
}

void DrawingCanvas::ShowExportDialog()
{
  if (view)
  {
    wxFileDialog exportFileDialog(this, _("Export drawing"), "", "", "PNG files (*.png)|*.png", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (exportFileDialog.ShowModal() == wxID_CANCEL) return;

    wxBitmap bitmap(this->GetSize() * this->GetContentScaleFactor());

    wxMemoryDC memDC;
    memDC.SetUserScale(this->GetContentScaleFactor(), this->GetContentScaleFactor());
    memDC.SelectObject(bitmap);

    view->OnDraw(&memDC);

    bitmap.SaveFile(exportFileDialog.GetPath(), wxBITMAP_TYPE_PNG);
  }
}

// void DrawingCanvas::SetView(DrawingView* view)
// {
//   this->view = view;
//   Refresh();
// }

void DrawingCanvas::OnPaint(wxPaintEvent&)
{
  wxAutoBufferedPaintDC dc(this);

  if (view)
  {
    view->OnDraw(&dc);
  }
}

void DrawingCanvas::OnMouseDown(wxMouseEvent& event)
{
  view->OnMouseDown(event.GetPosition());
  isDragging = true;
  Refresh();
}

void DrawingCanvas::OnMouseMove(wxMouseEvent& event)
{
  if (isDragging)
  {
    view->OnMouseDrag(event.GetPosition());
    Refresh();
  }
}

void DrawingCanvas::OnMouseUp(wxMouseEvent &)
{
  if (isDragging)
  {
    isDragging = false;
    view->OnMouseDragEnd();
  }
}

void DrawingCanvas::OnMouseLeave(wxMouseEvent &)
{
  if (isDragging)
  {
    isDragging = false;
    view->OnMouseDragEnd();
  }
}

void DrawingCanvas::BuildContextMenu()
{
  wxMenuItem* clear = contextMenu.Append(wxID_ANY, "&Clear");
  wxMenuItem* save = contextMenu.Append(wxID_ANY, "&Export");

  this->Bind(wxEVT_MENU, [this](wxCommandEvent&) {
    this->view->OnClear();
    this->Refresh();
  }, clear->GetId());

  this->Bind(wxEVT_MENU, [this](wxCommandEvent&) {
    this->ShowExportDialog();
  }, save->GetId());
}

void DrawingCanvas::OnContextMenuEvent(wxContextMenuEvent& event)
{
  auto clientPos = event.GetPosition() == wxDefaultPosition
    ? wxPoint(this->GetSize().GetWidth() / 2, this->GetSize().GetHeight() / 2)
    : this->ScreenToClient(event.GetPosition());
  PopupMenu(&this->contextMenu, clientPos);
}