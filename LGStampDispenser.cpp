/*
    Stamp Dispenser.cpp
    
    By: Linus Gordon
        January 19, 2016

    Note: I added a few more assertions to test the program, as well as 
          a small destructor in order to avoid memory leaks. 
*/

#include <stdlib.h>
#include <assert.h>
#include <limits.h>

/// <summary>
/// Facilitates dispensing stamps for a postage stamp machine.
/// </summary>
class StampDispenser
{
public:
    /// <summary>
    /// Initializes a new instance of the <see cref="StampDispenser"/> class that will be 
    /// able to dispense the given types of stamps.
    /// </summary>
    /// <param name="stampDenominations">
    /// The values of the types of stamps that the machine has.  
    /// Should be sorted in descending order and contain at least a 1.
    /// </param>
    /// <param name="numStampDenominations">
    /// The number of types of stamps in the stampDenominations array. 
    /// </param>
    StampDispenser(const int* stampDenominations, size_t numStampDenominations);

    /// <summary>
    /// Removes the memory allocated for the StampDispenser class
    /// </summary>
    ~StampDispenser();

    /// <summary>
    /// Returns the minimum number of stamps that the machine can dispense to
    /// fill the given request.
    /// </summary>
    /// <param name="request">
    /// The total value of the stamps to be dispensed.
    /// </param>
    /// <returns>
    /// The minimum number of stamps needed to fill the given request.
    /// </returns>
    int CalcNumStampsToFillRequest(int request);

private: 
    int *stampDenoms;
    size_t numStampDenoms;

}; 

// Constructor
StampDispenser::StampDispenser(const int* stampDenominations, size_t numStampDenominations)
 {
    // Create Stamp Dispenser
    stampDenoms = new int[numStampDenominations];
    numStampDenoms = numStampDenominations;

    // Checks that stampDenominations is in descending order
    for(int i = 0; i < (int)numStampDenominations - 1; i++) {
            if(stampDenominations[i] < stampDenominations[i + 1]) {
                exit(1);
            }
            stampDenoms[i] = stampDenominations[i];
    }
    stampDenoms[numStampDenominations - 1] = stampDenominations[numStampDenominations - 1];

   // Checks that stampDenominations contains a 1 as its final value
   if(stampDenominations[(int)numStampDenominations - 1] != 1) {
       exit(1);
   }
 }

// Destructor
 StampDispenser::~StampDispenser() 
{
    delete [] stampDenoms; // frees memory allocated for stampDenoms array
}

// A classic dynamic programming problem that calculates the minimum number
// of stamps of various values to add up to the request
int StampDispenser::CalcNumStampsToFillRequest(int request) 
{

    if(request < 0) { // Handles an invalid request
        return -1;
    }

    // stamp_table[i] will store the minimum number of stamps to make the request. 
    // stamp_table[request] will store the result
    int stamp_table[request + 1];

    stamp_table[0] = 0;

    // initialize all values as max
    for(int i = 1; i <= request; i++) {
        stamp_table[i] = INT_MAX;
    }

    // Compute min stamps required for values 1 through request
    for(int i = 1; i <= request; i++) {
            // Iterate through values smaller than i
            for(int j = 0; j < (int)numStampDenoms; j++) {
                if(stampDenoms[j] <= i) {
                    int result = stamp_table[i - stampDenoms[j]];
                    if(result != -1 && result + 1 < stamp_table[i])
                        stamp_table[i] = result + 1;
            }
        }
    }
    return stamp_table[request];
}

int main()
{
    int stampDenominations[] = {90, 30, 24, 10, 6, 2, 1};
    StampDispenser stampDispenser(stampDenominations, 7);
    assert(stampDispenser.CalcNumStampsToFillRequest(18) == 3);

    // Additional Tests of invalid input
    assert(stampDispenser.CalcNumStampsToFillRequest(-8) == -1);
    assert(stampDispenser.CalcNumStampsToFillRequest(-1) == -1);
    assert(stampDispenser.CalcNumStampsToFillRequest(0) == 0);

    // Additional tests
    assert(stampDispenser.CalcNumStampsToFillRequest(1) == 1);
    assert(stampDispenser.CalcNumStampsToFillRequest(2) == 1);
    assert(stampDispenser.CalcNumStampsToFillRequest(20) == 2);
    assert(stampDispenser.CalcNumStampsToFillRequest(122) == 3);
    assert(stampDispenser.CalcNumStampsToFillRequest(19) == 4); 

    return 0;
}
