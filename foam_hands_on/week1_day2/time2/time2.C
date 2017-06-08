#include "fvCFD.H"


/*
same as before but using #include files
*/

int main(int argc, char *argv[]){

#include "setRootCase.H"
/*Foam::argList args(argc, argv);
if (!args.checkRootCase()) {
Foam::FatalError.exit();
}*/

#include "createTime.H"
/*
Foam::Time runTime
(
Foam::Time::controlDictName,
args.rootPath(),
args.caseName()
);

*/
/// Create mesh and fields
/// ...
while( runTime.loop() ) {
Info << "Time : " << runTime.timeName() << "\n";
/// ... solver stuff
runTime.write(); /// The write trigger
}

// it's a list of labels initialized from a dictionary
labelIOList some = labelIOList
(
IOobject
(
"some",
"",
runTime,
IOobject::MUST_READ,
IOobject::NO_WRITE,
false /// Does not register with objectRegistry
)
);
/*
Similar to IOdictionary
Does not use keyword -> value style parsing
Array with I/O capability
Extensively used by mesh object
with list of faces or points
*/

Info << some[0] << endl;


	return 0;
}