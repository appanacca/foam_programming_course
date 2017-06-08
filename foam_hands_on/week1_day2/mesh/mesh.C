#include "fvCFD.H"


/*

*/

int main(int argc, char *argv[]){


#include "setRootCase.H"
#include "createTime.H"

#include "createMesh.H"
// it do this
/*
Foam::fvMesh mesh
(
Foam::IOobject
(
Foam::fvMesh::defaultRegion,
runTime.timeName(),
runTime,
Foam::IOobject::MUST_READ
)
);
*/

Info << mesh.C()[0] << endl;


/* Divide the mesh.Sf() and mesh.magSf() to obtain the unit
normals at each face, the resulting unit normal is a
surfaceVectorField */
surfaceVectorField unitFaceVec = mesh.Sf()/mesh.magSf();

// how to loop throught an object list
/*forAll(unitFaceVec, i){
	Info << unitFaceVec[i] << endl;
}*/

volScalarField testFun
(
IOobject
(
"testFun",
runTime.timeName(),
mesh, /// Just to get objectRegistry
IOobject::NO_READ, /// Read trigger
IOobject::AUTO_WRITE /// Write trigger
),
mesh, /// The mesh to which testFun is tied
dimensionedScalar( "testFun", dimless , 0.0 )
);

// use the dot product to generate a scalar field
forAll( testFun, j){
	testFun[j] = mesh.C()[j] & mesh.C()[j];
}

// write the field
testFun.write();

	return 0;
}