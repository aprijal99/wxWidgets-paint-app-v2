#pragma once

#include <wx/wx.h>
#include <wx/wrapsizer.h>
#include <wx/splitter.h>
#include <string>
#include <vector>
#include <chrono>
#include "App.hpp"
#include "ColorPane.hpp"
#include "PenSizePane.hpp"
#include "DrawingDocument.hpp"
#include "DrawingView.hpp"
#include "DrawingCanvas.hpp"

class Frame: public wxDocParentFrame
{
public:
  Frame(wxDocManager* manager, wxFrame* frame, wxWindowID id, const wxString& title);
  void SetupCanvasForView(DrawingView* view);

private:
  wxPanel* BuildControlsPanel(wxWindow* parent);
  void SetupColorPanes(wxWindow* parent, wxSizer* sizer);
  void SetupPenPanes(wxWindow* parent, wxSizer* sizer);
  void SelectColorPane(ColorPane* pane);
  void SelectPenPane(PenSizePane* pane);
  void BuildMenuBar();

  std::vector<ColorPane*> colorPanes{};
  std::vector<PenSizePane*> penPanes{};
  wxPanel* docPanel;
  const int penCount = 6;
  const std::string lightBackground = "#f4f3f3";
  const std::string darkBackground = "#2c2828";
  const std::vector<std::string> niceColors = {
    "#000000", "#ffffff", "#fd7f6f", "#7eb0d5", "#b2e061", "#bd7ebe",
    "#ffb55a", "#ffee65", "#beb9db", "#fdcce5", "#8bd3c7"
  };
};