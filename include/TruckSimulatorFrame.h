#ifndef TRUCKSIMULATORFRAME_H
#define TRUCKSIMULATORFRAME_H

#include <wx/wx.h>

class TruckSimulatorFrame : public wxFrame {
public:
    TruckSimulatorFrame(const wxString& title);

private:
    void OnSimulate(wxCommandEvent& event);
    void OnVariableChanged(wxCommandEvent& event);
    // Declare pointers to your controls here so they can be accessed in the OnSimulate method.
    wxTextCtrl* truckWeightEdit;
    wxTextCtrl* loadWeightEdit;
    wxTextCtrl* gearRatioEdit;
    wxTextCtrl* wheelDiameterEdit;
    wxTextCtrl* tractionEdit;
    wxTextCtrl* motorSpeedEdit;
    wxStaticText* resultLabel;
    wxStaticText* accelerationLabel;
    wxStaticText* TorqueLabel;
    wxStaticText* AppliedForceLabel;
    wxStaticText* ScoreLabel;
};

#endif // TRUCKSIMULATORFRAME_H
