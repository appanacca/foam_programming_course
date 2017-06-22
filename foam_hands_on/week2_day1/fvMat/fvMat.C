#include "fvCFD.H"

// example of function for
// Negated row-wise sum of non-zero off diagonal coeffs
// it sums the scalar coefficient presents in upper() to their 
// correspondent diag() cell (wit a minus sign)
// at the end of the day summ up all the flux coefficients to the diagonal
// it's actually a definition of a function inside the lduMatrix class

void Foam::lduMatrix::negSumDiag()
{
    const scalarField& Lower = const_cast<const lduMatrix &>(*this).lower();
    const scalarField& Upper = const_cast<const lduMatrix &>(*this).upper();
    scalarField& Diag = diag();
    const labelUList& l = lduAddr().lowerAddr();
    const labelUList& u = lduAddr().upperAddr();
    for (register label face=0; face<l.size(); face++){
        Diag[l[face]] -= Lower[face];
        Diag[u[face]] -= Upper[face];
    } 
};

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




fvScalarMatrix A( p, p.dimensions() );

// number of internal faces is equal to :
Info << A.lower().size() << endl;


// number of cells is equal to :
Info << A.diag().size() << endl;



A.upper() = 1;

Info << A.lduAddr().lowerAddr()[17] << endl;
// since it is defined as a class member I have to call it from an object
A.Foam::lduMatrix::negSumDiag();


//Info << A.symmetric() << endl;

Info << A.upper() << endl;

Info << A.diag() << endl;

A.lower() = 1;

Info << A.lower() << endl;


//here it tell that the matrix is symmetric
// because the symmetric() memeber control only if the 
// upperPtr_ and the lowerPtr_ are defined
// it doesn't check if the coefficients are equal
Info << A.symmetric() << endl;

// in fact..
Info << (A.upper()[0] == A.lower()[0]) << endl;



// the function psi() return a scalarField 
// and is used int the tempophisical problem
//in the case of perfect gas  psi = R*T
forAll( A.psi().boundaryField(), ipatch ) {
    if
    (A.psi().boundaryField()[ipatch].type() == "fixedValue"){
            const tmp<scalarField> sf;
            scalarField value
            (
                A.psi().boundaryField()[ipatch].valueBoundaryCoeffs(sf)
            );
            A.boundaryCoeffs()[ipatch] = -1.0 * value;
            A.internalCoeffs()[ipatch] = -1.0;
    }
}



Info << A.boundaryCoeffs() << ": " << endl;


A.source() = 8;


// define a simple laplacian problem and solve it
// should define the solver and the schema in the dictionaries
fvScalarMatrix B( fvm::laplacian(p) );
B.source() = 0.0;
B.solve();
p.write();

return 0;
}
