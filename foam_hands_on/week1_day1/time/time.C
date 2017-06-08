#include "fvCFD.H"

//#include "Time.H"
//#include "argList.H"

/*
Class Time:
Solver time and iteration control
Controls all other allied operations tied to the above
Necessary to create FOAM objectRegistry
Writing variable values with iteration
Reading variable values with iteration
Necessary for almost all derived classes (mesh, fields, etc)
Constructor requires an input file called control dictionary

Dictionary (input) files are read/written using IOdictionary
objects
All FOAM applications use the string:
Foam::Time::controlDictName = "controlDict";

Class objectRegistry:
Hierarchical database that FOAM uses to organize its
model-related data
Complemented by IOobject, and regIOobject
IOobject provides standardized input / output support
also gives access to Foam::Time, which is at the root of
objectRegistry
regIOobject automatically manages the registration and
deregistration of objects to the objectRegistry
*/

int main(int argc, char* argv[]){

Foam::Time::controlDictName = "controlDict";


/// Init the args object
Foam::argList args(argc, argv);

Foam::Time runTime
(
Foam::Time::controlDictName, /// Dictionary file
args.rootPath(), /// Case root
args.caseName() /// Case Name (cavity, etc)
);


//Creating IOdictionary object
IOdictionary ioDictObj
(
IOobject
(
"myDictFile", /// The dictionary file
"", /// Relative path (from case root)
runTime, /// The Time object, defined above
IOobject::MUST_READ, /// Read for constructor
IOobject::NO_WRITE /// Foam::Time writeControl
)
);

//parsing vector data type
vector vec_in = vector( ioDictObj.lookup("vec_in") );

// parsing a scalar, SIMPLE SCALAR IS NOT IMPLEMENTED I HAVE TO USE THE DIMLESS CONSTRUCTOR
dimensionedScalar scalar_in = dimensionedScalar( ioDictObj.lookup("scalar_in"));

// possible subdictionary
word name = word( ioDictObj.subDict("subDict").lookup("example") );


// List is a template class that acts like a vector of objects
List<vector> myList = List<vector>( ioDictObj.lookup("myList") );

Info << vec_in << nl << scalar_in << nl << name << nl << myList << endl;

Info << myList[0].x() << endl;

	return 0;
}
