#pragma once

#include "sv_sfield.h"

namespace sv
{

class GuessesCache final
{
    /// <summary>
    ///   describes the guess status for a cell
    /// </summary>
    struct GuessState
    {
        sfield field_;            /// the field to make a guess
        COORDINATE xGuess = 0;    /// the cell x coordinate for a guess
        COORDINATE yGuess = 0;    /// the cell y coordinate for a guess
        COORDINATE posGuess = 0;  /// the cell item for a guess [0..8]
    };

    GuessState guesses_[50]; // field at 0 index is very basic // 48 was requested for the empty field
    size_t guessAt_ = 0;     // active guess


public:
    /// <summary>
    ///   restart using this Cache
    /// whole previous GuessesCache will be overwritten
    /// </summary>
    /// <param name="fld">this field copied inside</param>
    /// <returns>the field to work with</returns>
    sfield* reset(const sfield& fld) noexcept;

    /// <summary>
    ///    generates the new guess field and place a value in the field
    /// </summary>
    /// <param name="pfield">field to solve - returned via this parameter</param>
    /// <returns>true - if it is possible to make a new guess</returns>
    bool nextGuess(sfield*& pfield) noexcept;

    /// <summary>
    ///    Informing that last guess leads to the mistake in the solving
    /// So the logic should generate the next field with next guess
    /// </summary>
    /// <param name="pfield">field to solve - returned via this parameter</param>
    /// <returns>true - if it is possible to make a new guess</returns>
    bool wrongGuess(sfield*& pfield) noexcept;
};

} // namspace sv
