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



// define a simple laplacian problem and solve it
// should define the solver and the schema in the dictionaries
fvScalarMatrix B( fvm::laplacian(p) );
B.source() = 0.0;
B.solve();
p.write();

    //Info << p.boundaryField()[1];  // 1 is "left" patch

// surfaceInterpolation.C : lines (234 - 242)
// deltaCoeffs() is the reciprocal distance between owner and neighbour
// cell centroids of an internal face
const volVectorField& C = mesh_.C();
const labelUList& owner = mesh_.owner();
const labelUList& neighbour = mesh_.neighbour();
forAll(owner, facei)
{
    DeltaCoeffs[facei] = 1.0/mag(C[neighbour[facei]] -C[owner[facei]]);
}

//between owner and boundary face centroid of patch face
//surfaceInterpolation.C : lines (244 - 248)
forAll(DeltaCoeffs.boundaryField(), patchi)
{
    DeltaCoeffs.boundaryField()[patchi] = 1.0/mag(mesh_.boundary()[patchi].delta());
}

//fvPatch.C : lines (141 - 143)
// this is the above function delta()
Foam::tmp<Foam::vectorField> Foam::fvPatch::delta()
const
{
    return Cf() - Cn();

}





    return 0;
}
