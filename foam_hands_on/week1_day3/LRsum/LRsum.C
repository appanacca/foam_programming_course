#include "fvCFD.H"

/*

*/

int main(int argc, char *argv[]){


#include "setRootCase.H"
#include "createTime.H"
#include "createMesh.H"

volScalarField p
(
IOobject
(
"p",
runTime.timeName(),
mesh, /// Just to get objectRegistry
IOobject::MUST_READ, /// Read trigger
IOobject::AUTO_WRITE /// Write trigger
),
mesh /// The mesh to which testFun is tied
// Missing dimensionedScalar argument
// it throws an error if file "p" nto found
);



surfaceScalarField faceLRsum
(
IOobject
(
"faceLRsum",
runTime.timeName(),
mesh, /// Just to get objectRegistry
IOobject::NO_READ, /// Read trigger
IOobject::AUTO_WRITE /// Write trigger
),
mesh, /// The mesh to which testFun is tied
dimensionedScalar( "dimless", dimless , 0.0 )
);

Info << p[0] << nl << p.dimensions() << endl;

Info << p[mesh.owner()[0]] << endl;

// I do the loop over the neighbour because I'm sure that all the 
// left and rigth cell centers for whcich I compute p they will be present
// in fact at the end the number of elements of faceLRsum are the exact 
// internal faces
forAll( mesh.neighbour(), j){
	faceLRsum[j] = p[mesh.owner()[j]] + p[mesh.neighbour()[j]];
}

// write the field
//faceLRsum.write();
Info << faceLRsum.size() << endl;
Info << faceLRsum.boundaryField()[0].size() << endl;

/// First loop over all patches
forAll( p.boundaryField(), ipatch ) {

	//get the type of each boundary patch
	Info << p.boundaryField()[ipatch].type() << endl; 
	
	/// each face in the the patch
	forAll( p.boundaryField()[ipatch], iface ) {

		//acces the ipatch and iface face of the boundary
		//p.boundaryField()[ipatch][iface] = 0;
		
		//access the internal field p (the value of p at the center of the cell) for that specific patch
		// so it will return a list of values long as the number of bourndary faces for that specific patch
		//Info << iface <<  " : " <<p.boundaryField()[ipatch].patchInternalField() << endl;
	}
}

/// First loop over all patches
forAll( faceLRsum.boundaryField(), ipatch ) {
	// in this manner I assign the nex-to-boundary values of p to the boundary values (faces) of faceLRsum
	faceLRsum.boundaryField()[ipatch] = p.boundaryField()[ipatch].patchInternalField();
	//Info << faceLRsum.boundaryField()[ipatch] << endl;
	/// each face in the the patch
	forAll( faceLRsum.boundaryField()[ipatch], iface ) {
		//vector mean += faceLRsum.boundaryField()[ipatch].patchInternalField();
		//faceLRsum.boundaryField()[ipatch] = p.boundaryField()[ipatch].patchInternalField();
	}
}



faceLRsum.write();
Info << faceLRsum.size() << endl;
Info << faceLRsum.boundaryField()[0].size() << endl;
// the size rest the same as before but now they have a value

Info << mesh.boundaryMesh()[0].size() << endl;
// this is the same method as above but for the mesh and not the field

	return 0;
}


