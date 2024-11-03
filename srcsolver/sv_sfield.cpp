#include <iostream>
#include <ostream>
#include "sv_sfield.h"
#include "sv_guessescache.h"

namespace sv
{

bool scell::SetValue(const VALUE value) noexcept
{
    if (value_ != EMPTY_VALUE && value != value_)
        return false;

    for (COORDINATE i = 0; i < NOWHERE; ++i)
    {
        options[i] = EMPTY_VALUE;
    }
    options[value] = value_ = value;
    return true;
}


void scell::ClearOption(const VALUE value) noexcept
{
    options[value] = EMPTY_VALUE;
}


bool sfield::SetFieldCell(const COORDINATE x, const COORDINATE y, const VALUE value) noexcept
{
    const COORDINATE yFrom = NOWHERE * y;
    {// process the line at once && clear options
        const COORDINATE iTo = yFrom + NOWHERE;
        for (COORDINATE i = yFrom; i < iTo; ++i)
        {
            cells_[i].ClearOption(value); // clear line
        }
    }

    {// process the column at once: clear options
        for (COORDINATE i = x; i < CELLS_COUNT; i += NOWHERE)
        {
            cells_[i].ClearOption(value); // clear column
        }
    }

    {// square - clear options
        const COORDINATE xFrom = x - (x % 3);
        const COORDINATE xTo = xFrom + 3;
        const COORDINATE yPosFrom = (y - (y % 3)) * NOWHERE;
        const COORDINATE yPosTo = yPosFrom + 3 * NOWHERE;
        for (COORDINATE iY = yPosFrom; iY < yPosTo;  iY += NOWHERE)
        {
            for (COORDINATE iX = xFrom; iX < xTo; ++iX)
            {
                cells_[iY + iX].ClearOption(value); // clear square
            }
        }
    }

    auto& c = cells_[yFrom + x];
    if (c.value_ == EMPTY_VALUE) ++found_;
    return c.SetValue(value);;
}


COORDINATE sfield::CheckLine(const COORDINATE y, const VALUE value) const noexcept
{
    COORDINATE ret = NOWHERE;
    const COORDINATE xFrom = NOWHERE * y;
    const COORDINATE xTo = xFrom + NOWHERE;
    for (COORDINATE iX = xFrom; iX < xTo; ++iX)
    {
        const scell& c = cells_[iX];
        if (c.value_ == value) // value already in line
            return NOWHERE;

        if (c.options[value] == value)
        {
            if (ret == NOWHERE)
            {
                ret = iX - xFrom; // set first time
            }
            else
            {
                return NOWHERE; // 2 possible positions found
            }
        }
    }

    return ret;
}

COORDINATE sfield::CheckColumn(const COORDINATE x, const VALUE value) const noexcept
{
    COORDINATE ret = NOWHERE;
    for (COORDINATE iY = x; iY < CELLS_COUNT; iY += NOWHERE)
    {
        const scell& c = cells_[iY];
        if (c.value_ == value) // value already in column
            return NOWHERE;

        if (c.options[value] == value)
        {
            if (ret == NOWHERE)
            {
                ret = iY / NOWHERE; // set first time
            }
            else
            {
                return NOWHERE; // 2 possible positions found
            }
        }
    }

    return ret;
}

std::pair<COORDINATE, COORDINATE> sfield::CheckSquare(const COORDINATE sq, const VALUE value) const noexcept
{
    std::pair<COORDINATE, COORDINATE> ret = {NOWHERE, NOWHERE};

    const COORDINATE xFrom = 3 * (sq % 3); // 0,3,6 => 0;   1,4,7 => 3;   2,5,8 => 6  column pos
    const COORDINATE xTo = xFrom + 3;
    const COORDINATE yPosFrom = (sq / 3) * NOWHERE * 3;  // 0,1,2 => 0;   3,4,5 => 27;   6,7,8 => 54  row start from
    const COORDINATE yPosTo = yPosFrom + 3 * NOWHERE;
    for (COORDINATE iY = yPosFrom; iY < yPosTo; iY += NOWHERE)
    {
        for (COORDINATE iX = xFrom; iX < xTo; ++iX)
        {
            const COORDINATE offset = iY + iX;
            const scell& c = cells_[offset];
            if (c.value_ == value) // value already in square
                return {NOWHERE, NOWHERE};

            if (c.options[value] == value)
            {
                if (ret.first == NOWHERE)
                {
                    ret = {offset % NOWHERE, offset / NOWHERE}; // set first time
                }
                else
                {
                    return {NOWHERE, NOWHERE}; // 2 possible positions found
                }
            }
        }
    }

    return ret;
}

int sfield::Status() const noexcept
{
    // check is we have empty values so far
    for (const auto& cell : cells_)
    {
        if (cell.value_ == EMPTY_VALUE)
        {
            for (const auto& opt : cell.options)
            {
                if (EMPTY_VALUE != opt)
                    return 1; // not solved yet
            }
            return 3; // cell is empty and no more options for cell
        }
    }

    // no empty values remain
    // check lines validity
    {
        for (COORDINATE iY = 0; iY < CELLS_COUNT; iY += NOWHERE)
        {
            VALUE toControl[NOWHERE] = {EMPTY_VALUE};
            const COORDINATE xFrom = iY;
            const COORDINATE xTo = xFrom + NOWHERE;
            for (COORDINATE iX = xFrom; iX < xTo; ++iX)
            {
                const auto v = cells_[iX].value_;
                toControl[v] = v;
            }
            for (const VALUE& v : toControl)
            {
                if (EMPTY_VALUE == v) return 2; // duplicate in line
            }
        }
    }
    // check columns validity
    {
        // column iX
        for (COORDINATE iX = 0; iX < NOWHERE; ++iX)
        {
            VALUE toControl[NOWHERE] = {EMPTY_VALUE};
            for (COORDINATE iY = iX; iY < CELLS_COUNT; iY += NOWHERE)
            {
                const auto v = cells_[iY].value_;
                toControl[v] = v;
            }
            for (const VALUE& v : toControl)
            {
                if (EMPTY_VALUE == v) return 2; // duplicate in column
            }
        }
    }
    // check squares validity
    {
        // lines 0, 3, 6
        for (COORDINATE line = 0; line < CELLS_COUNT; line += 27)
        {
            for (COORDINATE squareLeftTop = line; squareLeftTop < NOWHERE; squareLeftTop += 3) // 3 in a row
            {
                VALUE toControl[NOWHERE] = {EMPTY_VALUE};
                const COORDINATE xTo = squareLeftTop + 3;
                for (COORDINATE x = squareLeftTop; x < xTo; ++x)
                {
                    const auto v0 = cells_[x].value_; // 3 in column
                    toControl[v0] = v0;
                    const auto v1 = cells_[x + 9].value_;
                    toControl[v1] = v1;
                    const auto v2 = cells_[x + 18].value_;
                    toControl[v2] = v2;
                }
                for (const VALUE& v : toControl)
                {
                    if (EMPTY_VALUE == v) return 2; // duplicate in square
                }
            }
        }
    }

    // checks passed no free positions remain
    return 0;
}


bool SolveSudoku(sfield& afield, std::unique_ptr<GuessesCache>& cache, sfield& solved)
{

    sfield* pField = cache->reset(afield);
    do
    {
        /// check lines, check columns, check squares for each number
        ///   if found something - place it
        /// if did not found for the whole cycle:
        ///    - check Status and do the guess


        VALUE loopCounter = 0; // we need to search 9 different values and then Guess
        do
        {
            for (VALUE testV = 0; testV < 9 && loopCounter < 9; ++testV, ++loopCounter)
            {
                // lines
                for (COORDINATE iY = 0; iY < NOWHERE; ++iY)
                {
                    if (const COORDINATE x = pField->CheckLine(iY, testV); x != NOWHERE)
                    {
                        loopCounter = 0;
                        pField->SetFieldCell(x, iY, testV);
                    };
                }
                // columns
                for (COORDINATE iX = 0; iX < NOWHERE; ++iX)
                {
                    if (const COORDINATE y = pField->CheckColumn(iX, testV); y != NOWHERE)
                    {
                        loopCounter = 0;
                        pField->SetFieldCell(iX, y, testV);
                    };
                }
                // squares
                for (COORDINATE sq = 0; sq < NOWHERE; ++sq)
                {
                    if (const auto pt = pField->CheckSquare(sq, testV); pt.first != NOWHERE)
                    {
                        loopCounter = 0;
                        pField->SetFieldCell(pt.first, pt.second, testV);
                    };
                }

            }
        } while (loopCounter < 9 && pField->found_ < CELLS_COUNT);

        const auto res = pField->Status();
        if (0 == res) // solved
        {
            solved = *pField;
            return true;
        }

        if (1 == res) // need to guess
        {
            if (const bool have_a_guess = cache->nextGuess(pField); !have_a_guess)
            {
                return false; // cannot guess, cannot solve
            }
            continue;
        }

        if (const bool have_a_guess = cache->wrongGuess(pField); !have_a_guess)
        {
            return false; // all guesses iterated, cannot solve
        }

    }while (true);

    return false;
}


} // namspace sv
