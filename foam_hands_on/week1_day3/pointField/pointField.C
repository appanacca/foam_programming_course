#include "fvCFD.H"

/*

*/

typedef Field<vector> vectorField;

namespace Foam
{
	typedef vectorField pointField;
}

int main(int argc, char *argv[]){


#include "setRootCase.H"
#include "createTime.H"
#include "createMesh.H"


//construct the pointField
pointField pField( mesh.points().size() );

IOdictionary fieldDict
(
IOobject
(
"field",
runTime.timeName(),
runTime,
IOobject::MUST_READ,
IOobject::AUTO_WRITE
)
);

//vectorList initialization reading from a dictionary
List<vector> fieldList = List<vector>( fieldDict.lookup("field") );

// here the list has its own length
Info << fieldList.size() << endl;

// here we transfer the data of the previous list in a pointField object
pointField field( fieldList.xfer() );

// so we can see here that after the tranfer the previous list is now empty
Info << fieldList.size() << endl;

	return 0;
}

