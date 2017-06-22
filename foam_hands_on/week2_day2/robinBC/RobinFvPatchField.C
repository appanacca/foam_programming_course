/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2014 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "RobinFvPatchField.H"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
RobinFvPatchField<Type>::RobinFvPatchField
(
    const fvPatch& p,   // patch
    const DimensionedField<Type, volMesh>& iF  //internal field
)
:
    fvPatchField<Type>(p, iF),
    gradPhiBoundary_( p.size(), pTraits<Type>::zero ),
    a_(), b_()
{}


template<class Type>
RobinFvPatchField<Type>::RobinFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict
)
:
    fvPatchField<Type>(p, iF, dict, true),
    gradPhiBoundary_( "gradient", dict, p.size() ),
    a_(dict.lookupOrDefault<scalar>( "a", scalar(1.0))),
    b_(dict.lookupOrDefault<scalar>( "b", scalar(0.0)))
{}


template<class Type>
RobinFvPatchField<Type>::RobinFvPatchField
(
    const RobinFvPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fvPatchField<Type>( ptf, p, iF, mapper ),
    gradPhiBoundary_( ptf.gradPhiBoundary_ ),
    a_( ptf.a_ ), b_( ptf.b_ )
{
    if (notNull(iF) && mapper.hasUnmapped())
    {
        WarningIn
        (
            "RobinFvPatchField<Type>::RobinFvPatchField\n"
            "(\n"
            "    const RobinFvPatchField<Type>&,\n"
            "    const fvPatch&,\n"
            "    const DimensionedField<Type, volMesh>&,\n"
            "    const fvPatchFieldMapper&\n"
            ")\n"
        )   << "On field " << iF.name() << " patch " << p.name()
            << " patchField " << this->type()
            << " : mapper does not map all values." << nl
            << "    To avoid this warning fully specify the mapping in derived"
            << " patch fields." << endl;
    }
}


template<class Type>
RobinFvPatchField<Type>::RobinFvPatchField
(
    const RobinFvPatchField<Type>& ptf
)
:
    fvPatchField<Type>( ptf ),
    gradPhiBoundary_( ptf.gradPhiBoundary_ ),
    a_( ptf.a_ ), b_( ptf.b_ )
{}


template<class Type>
RobinFvPatchField<Type>::RobinFvPatchField
(
    const RobinFvPatchField<Type>& ptf,
    const DimensionedField<Type, volMesh>& iF
)
:
    fvPatchField<Type>( ptf, iF ),
    gradPhiBoundary_( ptf.gradPhiBoundary_ ),
    a_( ptf.a_ ), b_( ptf.b_ )
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
tmp<Field<Type> > RobinFvPatchField<Type>::valueInternalCoeffs
(
    const tmp<scalarField>&
) const
{
    return tmp<Field<Type> >
    (
        new Field<Type>(this->size(), pTraits<Type>::one*a_)
    );
}


template<class Type>
tmp<Field<Type> > RobinFvPatchField<Type>::valueBoundaryCoeffs
(
    const tmp<scalarField>&
) const
{
    return (*this * a_ +gradPhiBoundary_*b_  * (this->patch().deltaCoeffs()));
}


template<class Type>
tmp<Field<Type> > RobinFvPatchField<Type>::gradientInternalCoeffs() const
{
    return (-pTraits<Type>::one*a_*this->patch().deltaCoeffs() + b_*gradPhiBoundary_);
}


template<class Type>
tmp<Field<Type> > RobinFvPatchField<Type>::gradientBoundaryCoeffs() const
{
    return this->patch().deltaCoeffs()*(*this);
}


template<class Type>
void RobinFvPatchField<Type>::write(Ostream& os) const
{
    fvPatchField<Type>::write(os);
    this->writeEntry("value", os);
    gradPhiBoundary_.writeEntry( "gradient", os);
    os.writeKeyword("a")
    << a_ << token::END_STATEMENT << nl;
    os.writeKeyword("b")
    << b_ << token::END_STATEMENT << nl;

    //token::END STATEMENT = ”;” (line delimiter)
}




// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
