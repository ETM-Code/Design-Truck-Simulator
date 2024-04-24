#include <wx/wx.h>
#include "TruckSimulatorApp.h"

wxIMPLEMENT_APP(TruckSimulatorApp);
/*Run with this command: "
clang++ -std=c++11 -stdlib=libc++ $(wx-config --cxxflags) -Iinclude -o YourApp src/main.cpp src/TruckSimulatorApp.cpp src/TruckSimulatorFrame.cpp $(wx-config --libs)
"*/