#include "sv_guessescache.h"
#include "sv_types.h"

namespace sv
{

sfield* GuessesCache::reset(const sfield& fld) noexcept
{
    guessAt_ = 1;
    GuessState& state = guesses_[0];
    state.field_ = fld;
    state.posGuess = NOWHERE; // we are not doing guesses on the first field
    return &state.field_;
}

bool GuessesCache::nextGuess(sfield*& pfield) noexcept
{
    const GuessState& prev = guesses_[guessAt_ - 1]; // previous field
    GuessState& state = guesses_[guessAt_++]; // field for a guess
    sfield& field = state.field_; // field to work with
    field = prev.field_; // initialize it

    scell* pCell = nullptr;
    for (COORDINATE i = 0; i < CELLS_COUNT; ++i)
    {
        if (field.cells_[i].value_ == EMPTY_VALUE)
        {
            pCell = field.cells_ + i;            // candidate
            state.yGuess = i / NOWHERE;          // y coordinate
            state.xGuess = i % NOWHERE;          // x coordinate
            break;
        }
    }

    if (pCell) //state.posGuess
        for (state.posGuess = 0; state.posGuess < NOWHERE; ++state.posGuess) // iterate through options
        {
            if (auto valueOption = pCell->options[state.posGuess]; valueOption != EMPTY_VALUE)
            {
                field.SetFieldCell(state.xGuess, state.yGuess, valueOption);
                pfield = &field; // set field to process further
                ++state.posGuess; // change pos to next guess
                return true;
            }
        }

    return false;
}

bool GuessesCache::wrongGuess(sfield*& pfield) noexcept
{
    while (guessAt_ > 1)
    {
        GuessState* pState = &guesses_[guessAt_ - 1]; // current field field of the guess
        GuessState& sourceState = guesses_[guessAt_ - 2]; // current field field of the guess
        pState->field_ = sourceState.field_; // set field again
        sfield& field= pState->field_;
        for (; pState->posGuess < NOWHERE; ++pState->posGuess) // iterate through options - find next option
        {
            scell& cell = field.cells_[pState->yGuess * NOWHERE + pState->xGuess];
            if (auto valueOption = cell.options[pState->posGuess]; valueOption != EMPTY_VALUE)
            {
                field.SetFieldCell(pState->xGuess, pState->yGuess, valueOption);
                pfield = &field; // set field to process further
                ++pState->posGuess; // change pos to next guess
                return true;
            }
        }

        --guessAt_; // move to previous guess
    }


    return false;
}

} // namespace sv
