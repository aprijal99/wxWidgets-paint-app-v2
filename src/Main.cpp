/*
 * Implementation for App.hpp and Frame.hpp
 */

#include "App.hpp"
#include "Frame.hpp"

/* App.hpp */
wxIMPLEMENT_APP(App);

bool App::OnInit()
{
  wxInitAllImageHandlers();
  SetAppDisplayName("Paint App");

  docManager.reset(new wxDocManager);
  docManager->SetMaxDocsOpen(1);

  new wxDocTemplate(docManager.get(), "Drawing", "*.pxz", "", "pxz", "DrawingDocument", "DrawingView", CLASSINFO(DrawingDocument), CLASSINFO(DrawingView));

  frame = new Frame(docManager.get(), nullptr, wxID_ANY, wxGetApp().GetAppDisplayName());
  frame->Show(true);

  return true;
}

void App::SetupCanvasForView(DrawingView* view)
{
  wxGetApp().frame->SetupCanvasForView(view);
}

ToolSettings& App::GetToolSettings()
{
  return wxGetApp().toolSettings;
}

/* Frame.hpp */
Frame::Frame(wxDocManager* manager, wxFrame* frame, wxWindowID id, const wxString& title)
  : wxDocParentFrame(manager, frame, id, title)
{
  wxSplitterWindow* splitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_BORDER | wxSP_LIVE_UPDATE);
  splitter->SetMinimumPaneSize(FromDIP(150));

  wxPanel* controlsPanel = BuildControlsPanel(splitter);
  docPanel = new wxPanel(splitter, wxID_ANY);
  docPanel->SetSizer(new wxBoxSizer(wxVERTICAL));

  splitter->SplitVertically(controlsPanel, docPanel);
  splitter->SetSashPosition(FromDIP(220));

  SelectColorPane(colorPanes[0]);
  SelectPenPane(penPanes[0]);

  // Set frame window size
  this->SetSize(FromDIP(1000), FromDIP(700));
  this->SetMinSize(wxSize(FromDIP(400), FromDIP(200)));

  // Set frame title bar icon
  wxIcon icon("logo.png", wxBITMAP_TYPE_PNG);
  SetIcon(icon);

  BuildMenuBar();
}

void Frame::SetupCanvasForView(DrawingView* view)
{
  if (docPanel->GetChildren().size() > 0)
  {
    docPanel->GetSizer()->Clear(true);
  }
  
  if (view != nullptr)
  {
    DrawingCanvas* canvas = new DrawingCanvas(docPanel, view, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    docPanel->GetSizer()->Add(canvas, 1, wxEXPAND);
    view->SetFrame(this);
  }
  else
  {
    this->SetTitle(wxGetApp().GetAppDisplayName());
  }

  docPanel->Layout();
}

wxPanel* Frame::BuildControlsPanel(wxWindow* parent)
{
  wxScrolled<wxPanel>* controlsPanel = new wxScrolled<wxPanel>(parent, wxID_ANY);
  controlsPanel->SetScrollRate(0, FromDIP(10));
  controlsPanel->SetBackgroundColour(wxColour(wxSystemSettings::GetAppearance().IsDark() ? darkBackground : lightBackground));

  wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

  // Colors pane
  wxStaticText* text = new wxStaticText(controlsPanel, wxID_ANY, "Colors");
  text->SetForegroundColour(wxColour(*wxWHITE));
  mainSizer->Add(text, 0, wxLEFT | wxTOP, FromDIP(10));

  wxWrapSizer* colorPaneSizer = new wxWrapSizer(wxHORIZONTAL);
  SetupColorPanes(controlsPanel, colorPaneSizer);
  mainSizer->Add(colorPaneSizer, 0, wxALL, FromDIP(5));

  // Pens pane
  text = new wxStaticText(controlsPanel, wxID_ANY, "Pens");
  text->SetForegroundColour(wxColour(*wxWHITE));
  mainSizer->Add(text, 0, wxLEFT | wxTOP, FromDIP(10));

  wxWrapSizer* penPaneSizer = new wxWrapSizer(wxHORIZONTAL);
  SetupPenPanes(controlsPanel, penPaneSizer);
  mainSizer->Add(penPaneSizer, 0, wxALL, FromDIP(5));

  controlsPanel->SetSizer(mainSizer);

  return controlsPanel;
}

void Frame::SetupColorPanes(wxWindow* parent, wxSizer* sizer)
{
  for (const std::string& color: niceColors)
  {
    ColorPane* colorPane = new ColorPane(parent, wxColour(color));
    colorPane->Bind(wxEVT_LEFT_DOWN, [this, colorPane](wxMouseEvent& event) { SelectColorPane(colorPane); });
    
    colorPanes.push_back(colorPane);
    sizer->Add(colorPane, 0, wxRIGHT | wxBOTTOM, FromDIP(5));
  }
}

void Frame::SelectColorPane(ColorPane* pane)
{
  for (ColorPane* colorPane: colorPanes)
  {
    colorPane->selected = (colorPane == pane);
    colorPane->Refresh();
  }

  App::GetToolSettings().currentColor = pane->color;
}

void Frame::SetupPenPanes(wxWindow* parent, wxSizer* sizer)
{
  for (int i = 0; i < penCount; i++)
  {
    PenSizePane* penPane = new PenSizePane(parent, i * FromDIP(4) + FromDIP(1));
    penPane->Bind(wxEVT_LEFT_DOWN, [this, penPane](wxMouseEvent& event) { SelectPenPane(penPane); });

    penPanes.push_back(penPane);
    sizer->Add(penPane, 0, wxRIGHT | wxBOTTOM, FromDIP(5));
  }
}

void Frame::SelectPenPane(PenSizePane *pane)
{
  for (PenSizePane* penPane: penPanes)
  {
    penPane->selected = (penPane == pane);
    penPane->Refresh();
  }

  App::GetToolSettings().currentWidth = pane->penWidth;
}

void Frame::BuildMenuBar()
{
  auto menuBar = new wxMenuBar;

  auto fileMenu = new wxMenu;
  fileMenu->Append(wxID_NEW);
  fileMenu->Append(wxID_OPEN);
  fileMenu->Append(wxID_SAVE);
  fileMenu->Append(wxID_SAVEAS);
  fileMenu->Append(wxID_CLOSE);
  fileMenu->Append(wxID_EXIT);

  menuBar->Append(fileMenu, "&File");

  auto editMenu = new wxMenu;
  editMenu->Append(wxID_UNDO);
  editMenu->Append(wxID_REDO);
  editMenu->AppendSeparator();
  editMenu->Append(wxID_CUT);
  editMenu->Append(wxID_COPY);
  editMenu->Append(wxID_PASTE);
  editMenu->Append(wxID_DELETE);
  editMenu->AppendSeparator();
  editMenu->Append(wxID_SELECTALL);

  menuBar->Append(editMenu, "&Edit");

  SetMenuBar(menuBar);
}