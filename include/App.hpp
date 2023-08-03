#pragma once

#include <wx/wx.h>
#include "DrawingView.hpp"
#include "ToolSettings.hpp"

class Frame;

class App: public wxApp
{
public:
  virtual bool OnInit();

  static void SetupCanvasForView(DrawingView* view);
  static ToolSettings& GetToolSettings();

private:
  std::unique_ptr<wxDocManager> docManager;
  Frame* frame;

  ToolSettings toolSettings;
};