#include "TruckSimulatorApp.h"

bool TruckSimulatorApp::OnInit() {
    if (!wxApp::OnInit())
        return false;

    TruckSimulatorFrame* frame = new TruckSimulatorFrame(wxT("Electric Truck Simulator"));
    frame->Show(true);
    return true;
}
