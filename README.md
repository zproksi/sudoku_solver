# `sudoku_solver`

**PURPOSE:** Demonstrates different approaches to the same task - "sudoku solution" in learning purpuses
|Name|Description|
|:-|:-|
|**Single thread**|Usual solution logic without specific speedup technics|
|**Multithreaded**|Threads usage along with atomic types for synchronization; branchless programming|
|**CUDA NVIDIA**|Graphics processing units usage - NVIDIA card required|

**Table of Contents:**
* [`sudoku_solver`](#sudoku_solver)
  * [Description](#description)
  * [Entry Point](#entry-point)
  * [Building](#building)
  * [Unit Tests](#unit-tests)
  * [Architectural Diagrams](#architectural-diagrams)
  * [Contacts](#contacts)
  * [Copyright](#copyright)
  * [Reference](#reference)
## Description
* Sudoke field should be presented as std::string_view with length of 81. So first 9 characters will present first line of sudoku field. next 9 characters will present second line, etc.
* Every symbols '1','2','3','4','5','6','7','8','9' will be transfomwd inside the logic to the characters with code 0,1,2,3,4,5,6,7,8. All other characters will be transformed to the character defined as "NONE_CHARACTER"
* Every type of logic should provide next function for usage:

|Name|Description|
|:-|:-|
|SolveSudoku(std::string_view asudoku, class GuessesCache& acache)|function which solves sudoku|




## Entry point

## Building

## Unit Tests

## Architectural Diagrams

# Contacts

Feel free to use email zproksi@mail.ru along with the title:
   1. bpatch bug found
   1. bpatch improvement
   1. bpatch change request
   1. bpatch feature request
   1. bpatch support request
   1. bpatch collaboration proposal

## Copyright

[MIT License](https://opensource.org/license/mit/) Copyright <2024> [Alexey Zaytsev](https://www.linkedin.com/in/zaytsevalexey/)

## Reference

[`CMakeLists.txt`][cmakelists_txt]

[cmakelists_txt]:./CMakeLists.txt
