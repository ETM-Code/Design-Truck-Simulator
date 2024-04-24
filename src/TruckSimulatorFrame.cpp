#include "TruckSimulatorFrame.h"
#include <wx/string.h>
#include <wx/textctrl.h>
#include <wx/stattext.h>
#include <cmath> // For std::sqrt

int autosim = 0;

TruckSimulatorFrame::TruckSimulatorFrame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(400, 600)) {
    // Panel to hold all controls
    wxPanel* panel = new wxPanel(this, -1);

    // Sizers for layout
    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
    panel->SetSizer(vbox);

    // Adding input fields
    vbox->Add(new wxStaticText(panel, wxID_ANY, wxT("Truck Weight (kg):")), 0, wxEXPAND | wxALL, 5);
    truckWeightEdit = new wxTextCtrl(panel, wxID_ANY);
    vbox->Add(truckWeightEdit, 0, wxEXPAND | wxALL, 5);
    truckWeightEdit->Bind(wxEVT_TEXT, &TruckSimulatorFrame::OnVariableChanged, this);

    // Input field for Load Weight
    vbox->Add(new wxStaticText(panel, wxID_ANY, wxT("Load Weight (kg):")), 0, wxEXPAND | wxALL, 5);
    loadWeightEdit = new wxTextCtrl(panel, wxID_ANY);
    vbox->Add(loadWeightEdit, 0, wxEXPAND | wxALL, 5);
    loadWeightEdit->Bind(wxEVT_TEXT, &TruckSimulatorFrame::OnVariableChanged, this);

    vbox->Add(new wxStaticText(panel, wxID_ANY, wxT("Gear Ratio:")), 0, wxEXPAND | wxALL, 5);
    gearRatioEdit = new wxTextCtrl(panel, wxID_ANY);
    vbox->Add(gearRatioEdit, 0, wxEXPAND | wxALL, 5);
    gearRatioEdit->Bind(wxEVT_TEXT, &TruckSimulatorFrame::OnVariableChanged, this);

    vbox->Add(new wxStaticText(panel, wxID_ANY, wxT("Wheel Diameter (m):")), 0, wxEXPAND | wxALL, 5);
    wheelDiameterEdit = new wxTextCtrl(panel, wxID_ANY);
    vbox->Add(wheelDiameterEdit, 0, wxEXPAND | wxALL, 5);
    wheelDiameterEdit->Bind(wxEVT_TEXT, &TruckSimulatorFrame::OnVariableChanged, this);

    /*vbox->Add(new wxStaticText(panel, wxID_ANY, wxT("Traction Coefficient:")), 0, wxEXPAND | wxALL, 5);
    tractionEdit = new wxTextCtrl(panel, wxID_ANY);
    vbox->Add(tractionEdit, 0, wxEXPAND | wxALL, 5);*/

    // Simulate button
    wxButton* simulateButton = new wxButton(panel, wxID_ANY, wxT("Simulate"));
    vbox->Add(simulateButton, 0, wxALIGN_CENTER | wxALL, 5);

    // Result display
    resultLabel = new wxStaticText(panel, wxID_ANY, wxT("Result: "));
    vbox->Add(resultLabel, 0, wxEXPAND | wxALL, 5);

    TorqueLabel = new wxStaticText(panel, wxID_ANY, wxT("Torque: "));
    vbox->Add(TorqueLabel, 0, wxEXPAND | wxALL, 5);

    AppliedForceLabel = new wxStaticText(panel, wxID_ANY, wxT("Force at ground: "));
    vbox->Add(AppliedForceLabel, 0, wxEXPAND | wxALL, 5);

    accelerationLabel = new wxStaticText(panel, wxID_ANY, wxT("Acceleration: "));
    vbox->Add(accelerationLabel, 0, wxEXPAND | wxALL, 5);

    ScoreLabel = new wxStaticText(panel, wxID_ANY, wxT("Score: "));
    vbox->Add(ScoreLabel, 0, wxEXPAND | wxALL, 5);



    // Event binding
    simulateButton->Bind(wxEVT_BUTTON, &TruckSimulatorFrame::OnSimulate, this);
}

double CalculateMotorTorque(double rpm) {
    double Torque = 1.242-0.0096796*rpm;
    if (Torque<0) {
        return -1;
    }
    return Torque;
}

double travelSim(double a, double vMax) {
    double step = 0.001; // Simulation time step
    double u = 0;        // Initial speed
    double s = 0;        // Distance traveled
    double time = 0;     // Elapsed time

    while (s < 3) {      // Continue until 3 meters are traveled
        if (u < vMax) {
            u += a * step;
            s+=u*step;
            time+=step;
             }           
        else {
            s += u * step;                 // Increment distance by speed over one time step
            time += step;
        }                  // Increment time by one time step
    }
    //printf("%f",s);
    return time; // Return the total time
}


void TruckSimulatorFrame::OnSimulate(wxCommandEvent& event) {
    autosim=1;
    resultLabel->Refresh();
resultLabel->Update();
  TorqueLabel->Refresh();
TorqueLabel->Update();
  AppliedForceLabel->Refresh();
AppliedForceLabel->Update();
accelerationLabel->Refresh();
accelerationLabel->Update();
ScoreLabel->Refresh();
ScoreLabel->Update();

    double truckWeight = 0.0;
    double loadWeight = 0.0;
    double gearRatio = 0.0;
    double wheelDiameter = 0.0;
    double inputTractionCoefficient = 0.55; 

    // Use wxString::ToDouble to convert text to double
    truckWeightEdit->GetValue().ToDouble(&truckWeight);
    loadWeightEdit->GetValue().ToDouble(&loadWeight);
    gearRatioEdit->GetValue().ToDouble(&gearRatio);
    wheelDiameterEdit->GetValue().ToDouble(&wheelDiameter);

    double motorRPM = 60/gearRatio;
    //motorSpeedEdit->GetValue().ToDouble(&motorRPM);

    // Motor torque curve function or lookup here
    double motorTorque = CalculateMotorTorque(motorRPM);
    if (motorTorque == -1) {
          resultLabel->SetLabel(wxString::Format(wxT("Motor Overloaded")));
        return;
    }
    double totalWeight = truckWeight+loadWeight;
    double wheelRadius = wheelDiameter / 2.0;
    double forceAtWheel = motorTorque / wheelRadius;

    double breakingForce = (inputTractionCoefficient * totalWeight * 9.81)*wheelDiameter;
    if (breakingForce>1.2*gearRatio) {
        resultLabel->SetLabel(wxString::Format(wxT("Motor Overloaded")));
        return;
    }

    //double appliedForce = std::min(tractionForce, forceAtWheel);
    //printf("%f\n", forceAtWheel);

    double acceleration = forceAtWheel / (totalWeight+0.3); //testing suggests ammendments to how acceleration works in the simulator, due to motor variability
    //printf("%f\n", acceleration);

    double maxVelocity = ((motorRPM/60)*wheelDiameter*3.1416*1.7)/(totalWeight/2.1+0.3); //testing suggests a relation between total weight and maximum velocity. This was ammended iteratively
    double timeToTravel3m = travelSim(acceleration, maxVelocity)/1.5;
    double score = (loadWeight*1000)/timeToTravel3m;

    resultLabel->SetLabel(wxString::Format(wxT("Time to travel 3m: %.2f seconds"), timeToTravel3m));
    ScoreLabel->SetLabel(wxString::Format(wxT("Score: %.2f"), score));
    TorqueLabel->SetLabel(wxString::Format(wxT("Torque: %.2f"), motorTorque));
    AppliedForceLabel->SetLabel(wxString::Format(wxT("Force at the ground: %.2f"), forceAtWheel));
    accelerationLabel->SetLabel(wxString::Format(wxT("Acceleration: %.2f"), acceleration));
}


void TruckSimulatorFrame::OnVariableChanged(wxCommandEvent& event) {
if (autosim==1) {
        resultLabel->Refresh();
resultLabel->Update();

    double truckWeight = 0.0;
    double loadWeight = 0.0;
    double gearRatio = 0.0;
    double wheelDiameter = 0.0;
    double inputTractionCoefficient = 0.55; 

    // Use wxString::ToDouble to convert text to double
    truckWeightEdit->GetValue().ToDouble(&truckWeight);
    loadWeightEdit->GetValue().ToDouble(&loadWeight);
    gearRatioEdit->GetValue().ToDouble(&gearRatio);
    wheelDiameterEdit->GetValue().ToDouble(&wheelDiameter);
if (wheelDiameter>0) {
    double motorRPM = 60/gearRatio;
    //motorSpeedEdit->GetValue().ToDouble(&motorRPM);

    // Motor torque curve function or lookup here
    double motorTorque = CalculateMotorTorque(motorRPM);
    if (motorTorque == -1) {
          resultLabel->SetLabel(wxString::Format(wxT("Motor Overloaded")));
        return;
    }
    double totalWeight = truckWeight+loadWeight;
    double wheelRadius = wheelDiameter / 2.0;
    double forceAtWheel = motorTorque / wheelRadius;

    double breakingForce = (inputTractionCoefficient * totalWeight * 9.81)*wheelDiameter;
    if (breakingForce>1.2*gearRatio) {
        resultLabel->SetLabel(wxString::Format(wxT("Motor Overloaded")));
        return;
    }

    //double appliedForce = std::min(tractionForce, forceAtWheel);
    //printf("%f\n", forceAtWheel);

    double acceleration = forceAtWheel / (totalWeight+0.3); //testing suggests ammendments to how acceleration works in the simulator, due to motor variability
    //printf("%f\n", acceleration);

    double maxVelocity = ((motorRPM/60)*wheelDiameter*3.1416*1.7)/(totalWeight/2.1+0.3); //testing suggests a relation between total weight and maximum velocity. This was ammended iteratively
    double timeToTravel3m = travelSim(acceleration, maxVelocity)/1.5;
    double score = (loadWeight*1000)/timeToTravel3m;

    resultLabel->SetLabel(wxString::Format(wxT("Time to travel 3m: %.2f seconds"), timeToTravel3m));
    ScoreLabel->SetLabel(wxString::Format(wxT("Score: %.2f"), score));
    TorqueLabel->SetLabel(wxString::Format(wxT("Torque: %.2f"), motorTorque));
    AppliedForceLabel->SetLabel(wxString::Format(wxT("Force at the ground: %.2f"), forceAtWheel));
    accelerationLabel->SetLabel(wxString::Format(wxT("Acceleration: %.2f"), acceleration));
}
}}



class TruckSimulatorApp : public wxApp {
public:
    virtual bool OnInit() {
        TruckSimulatorFrame* frame = new TruckSimulatorFrame(wxT("Electric Truck Simulator"));
        frame->Show(true);
        return true;
    }
};


