#include "fvCFD.H"

int main(int argc, char* argv[]){

dimensionedScalar inputPressure =
dimensionedScalar
(
"pressure", /// A name field
dimensionSet( 1, -1, -2, 0, 0, 0, 0 ),
1.0 /// Value to initialize
);

Info << inputPressure << "\n";

dimensionedScalar Mach =
dimensionedScalar
(
"dimless", /// A name field
dimless,
1.0 /// Value to initialize
);
Info << Mach << "\n";

const dimensionSet dimMass(1, 0, 0, 0, 0, 0, 0);

Info << dimMass << "\n";

inputPressure.value() = Mach.value();
// hack to set different dimension object with the same value

	return 0;
}