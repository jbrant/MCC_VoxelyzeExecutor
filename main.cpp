#include <iostream>
#include <cstring>
#include "VX_Object.h"
#include "VX_Environment.h"
#include "VX_Sim.h"
#include "VX_SimGA.h"


int main(int argc, char *argv[])
{
    char* InputFile;
    //create the main objects
    CVXC_Structure structure;
    CVX_Object Object;
    CVX_Environment Environment;
    CVX_SimGA Simulator;
    long int Step = 0;
    vfloat Time = 0.0; //in seconds
    bool print_scrn = false;

    //first, parse inputs. Use as: -f followed by the filename of the .vxa file that describes the simulation. Can also follow this with -p to cause console output to occur
    if (argc < 3)
    { // Check the value of argc. If not enough parameters have been passed, inform user and exit.
        std::cout << "\nInput file required. Quitting.\n";
        return(0);	//return, indicating via code (0) that we did not complete the simulation
    }
    else
    { // if we got enough parameters...
        for (int i = 1; i < argc; i++)
        {
            if (strcmp(argv[i],"-f") == 0)
            {
                InputFile = argv[i + 1];	// We know the next argument *should* be the filename:
            }
            else if (strcmp(argv[i],"-p") == 0)
            {
                print_scrn=true;	//decide if output to the console is desired
            }
        }
    }

    //setup main object
    Simulator.pEnv = &Environment;	//connect Simulation to environment
    Environment.pObj = &Object;		//connect environment to object

    //import the configuration file
    if (!Simulator.LoadVXAFile(InputFile)){
        if (print_scrn) std::cout << "\nProblem importing VXA file. Quitting\n";
        return(0);	//return, indicating via code (0) that we did not complete the simulation
    }
    std::string ReturnMessage;
    if (print_scrn) std::cout << "\nImporting Environment into simulator...\n";

    Simulator.Import(&Environment, 0, &ReturnMessage);
    if (print_scrn) std::cout << "Simulation import return message:\n" << ReturnMessage << "\n";

    // Simulator.pEnv->UpdateCurTemp(Time);	//set the starting temperature (nac: pointer removed for debugging)

    if (Simulator.WriteSimLogFile) Simulator.OpenSimLog(); // Open stream for verbose simulation logging

    while (not Simulator.StopConditionMet())
    {
        // do some reporting via the stdoutput if required:
        if (Step%100 == 0.0) //Only output every n time steps
        {
            if (print_scrn)
            {
                std::cout << "Time: " << Simulator.CurTime << std::endl;
                std::cout << "CM: " << Simulator.GetCM().Length() << std::endl;
                std::cout << "Temp: " << Simulator.pEnv->GetCurTemp() << std::endl;
                std::cout << "Max Disp: " << Simulator.SS.MaxTrialDisplacement << std::endl;
                std::cout << "Dist: " << Simulator.GetCurDistance() << std::endl;
                std::cout << "Total Dist: " << Simulator.SS.TotalDistanceTraversed << std::endl << std::endl;
            }

            if (Simulator.WriteSimLogFile)
            {
                Simulator.WriteSimLogEntry(Step);
            }
        }

        //do the actual simulation step
        Simulator.TimeStep(&ReturnMessage);
        Step += 1;	//increment the step counter
    }

    if (print_scrn) std::cout << "Ended at: " << Time << std::endl;

    // Close the verbose log file
    if (Simulator.WriteSimLogFile) Simulator.CloseSimLog(Simulator.SimLogFileName);

    Simulator.SaveResultFile(Simulator.FitnessFileName);

    return 1;	//code for successful completion  // could return fitness value if greater efficiency is desired
}
