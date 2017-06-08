#include "fvCFD.H"

/*
Parse command line inputs and options

> ./argList -speed '(1 3 4)' -mach 4

*/

int main(int argc, char* argv[]){

//Register the command line option using validOptions
Foam::argList::validOptions.set("mach", "sclar");
Foam::argList::validOptions.set("speed", "vector");


// Create an object instance of argList
Foam::argList args(argc, argv);

//Suppress printing OpenFOAM banner when using Info <<
Foam::argList::noBanner();

//Check for argument presence
args.optionFound("mach");

//Read option from command line and print it
scalar M;
args.optionReadIfPresent("mach", M);

Info << "mach: " << M << endl; 

vector S;
args.optionReadIfPresent("speed", S);
Info << "speed: " << S << endl; 

	return 0;
}