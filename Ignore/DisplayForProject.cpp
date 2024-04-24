#include "TruckSimulatorFrame.h"
#include <wx/string.h>
#include <wx/textctrl.h>
#include <wx/stattext.h>
#include <cmath>

int autosim = 0;

TruckSimulatorFrame::TruckSimulatorFrame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(400, 600)) {
    //Code to set-up and display UI omitted for brevity
}

double CalculateMotorTorque(double rpm) {
    double Torque = 1.242-0.0096796*rpm; //This is the linear relationship suggested by our testing
    if (Torque<0) {
        return -1; //Returns an error if torque is negative
    }
    return Torque;
}

double travelSim(double a, double vMax) {
    //Simulates the truck's movement based off inputs from the main function
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
            s += u * step;
            time += step;
        }
    }
    return time; // Return the total time
}

void TruckSimulatorFrame::OnSimulate(wxCommandEvent& event) {
    autosim=1; //Sets up a variable to allow real time recalculation

//Code gathering UI elements omitted for brevity

    double truckWeight = 0.0;
    double loadWeight = 0.0;
    double gearRatio = 0.0;
    double wheelDiameter = 0.0;
    double inputTractionCoefficient = 0.55; 

    //This gets the input values from the UI
    truckWeightEdit->GetValue().ToDouble(&truckWeight);
    loadWeightEdit->GetValue().ToDouble(&loadWeight);
    gearRatioEdit->GetValue().ToDouble(&gearRatio);
    wheelDiameterEdit->GetValue().ToDouble(&wheelDiameter);

    double motorRPM = 60/gearRatio;

    double motorTorque = CalculateMotorTorque(motorRPM);
    if (motorTorque == -1) {
          resultLabel->SetLabel(wxString::Format(wxT("Motor Overloaded")));
        return;
    }
    double totalWeight = truckWeight+loadWeight;
    double wheelRadius = wheelDiameter / 2.0; //It's easier to measure the diameter
    double forceAtWheel = motorTorque / wheelRadius;
    double breakingForce = (inputTractionCoefficient * totalWeight * 9.81)*wheelDiameter;
    if (breakingForce>0.5 /*Testing suggested motor max force was 0.7N, 
    but these were optimal conditions and the motor began having issues at 0.5N*/) {
        resultLabel->SetLabel(wxString::Format(wxT("Motor Overloaded")));
        return;
    }

    double acceleration = forceAtWheel / (totalWeight+0.3); //The +0.3 is to account for misc forces e.g. friction
    double maxVelocity = ((motorRPM/60)*wheelDiameter*3.1416*1.7)/(totalWeight/2.1+0.8); 
    //Testing made it clear that weight had an impact on the maximum velocity, and this equation seems to line up well with reality
    double timeToTravel3m = travelSim(acceleration, maxVelocity)/1.5;
    double score = (loadWeight*1000)/timeToTravel3m;

    //Displays results for ease of iteration
    resultLabel->SetLabel(wxString::Format(wxT("Time to travel 3m: %.2f seconds"), timeToTravel3m));
    ScoreLabel->SetLabel(wxString::Format(wxT("Score: %.2f"), score));
    TorqueLabel->SetLabel(wxString::Format(wxT("Torque: %.2f"), motorTorque));
    AppliedForceLabel->SetLabel(wxString::Format(wxT("Force at the ground: %.2f"), forceAtWheel));
    accelerationLabel->SetLabel(wxString::Format(wxT("Acceleration: %.2f"), acceleration));
}

void TruckSimulatorFrame::OnVariableChanged(wxCommandEvent& event) {
if (autosim==1) {
//Function containing the same logic, activated for real time simulation editing once the simulation has begun
}
}

//Further UI logic omitted for brevity