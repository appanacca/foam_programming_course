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


surfaceScalarField flux
(
IOobject
(
"flux",
runTime.timeName(),
mesh, /// Just to get objectRegistry
IOobject::NO_READ, /// Read trigger
IOobject::AUTO_WRITE /// Write trigger
),
mesh, /// The mesh to which testFun is tied
dimensionedScalar( "dimless", dimless , 0.0 )
);

//Creating IOdictionary object
IOdictionary blockMesh
(
IOobject
(
"blockMeshDict", /// The dictionary file
"system", /// Relative path (from case root)
runTime, /// The Time object, defined above
IOobject::MUST_READ, /// Read for constructor
IOobject::NO_WRITE /// Foam::Time writeControl
)
);


double dx = readScalar(blockMesh.lookup("deltax"));

Info << dx << endl;

volScalarField laplacian
(
IOobject
(
"laplacian",
runTime.timeName(),
mesh, /// Just to get objectRegistry
IOobject::NO_READ, /// Read trigger
IOobject::AUTO_WRITE /// Write trigger
),
mesh, /// The mesh to which testFun is tied
dimensionedScalar( "dimless", dimless , 0.0 )
);

while (runTime.loop())
{


forAll(mesh.neighbour(), j){

	flux[j] = (p[mesh.owner()[j]] - p[mesh.neighbour()[j]])/dx;

	// from this line we can see that the boundaries are at the end of the list
	// with index 39 and 40 ( with a 20 cell mesh) the index are shown in polyMesh/boundary
	Info << j << ": " << mesh.C()[j] << ": " << flux[j] << endl;
}


flux.write();

// hot to get the boundary values from the field p (imposed boundary conditions)
// 1) first declare a temporary scalar field
const tmp<scalarField> sf;

// 2) this is for fixedValue -> Neumann
scalarField fixV( p.boundaryField()[1].valueBoundaryCoeffs(sf) );
Info << mesh.Cf()[39] << ": " << fixV << endl;

// 3) this if for fixedGradient -> Dirichlet
scalarField fixG( (p.boundaryField()[2].gradientBoundaryCoeffs() - p[mesh.owner()[19]]) /dx );
Info << mesh.Cf()[40] << ": " << fixV << endl;

// impose the two bc
flux.boundaryField()[1] = fixV;
Info << flux.boundaryField()[1][0] << endl;

flux.boundaryField()[2] = fixG;
Info << flux.boundaryField()[2][0] << endl;







forAll(mesh.neighbour(), j){
	laplacian[j] = flux[j] + flux[j+1]; 	
}
laplacian[0] = flux[0] + flux.boundaryField()[1][0];
laplacian[19] = flux[19] + flux.boundaryField()[2][0] ;


p = laplacian;

laplacian.write();

}
return 0;
}

