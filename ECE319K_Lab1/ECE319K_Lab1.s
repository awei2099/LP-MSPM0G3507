//****************** ECE319K_Lab1.s ***************
// Your solution to Lab 1 in assembly code
// Author: Andy Wei
// Last Modified: 6/2/25
// Spring 2025
        .data
        .align 2
// Declare global variables here if needed
// with the .space assembly directive

        .text
        .thumb
        .align 2
        .global EID
EID:    .string "AYW425" // replace ZZZ123 with your EID here

        .global Phase
        .align 2
Phase:  .long 10
// Phase= 0 will display your objective and some of the test cases, 
// Phase= 1 to 5 will run one test case (the ones you have been given)
// Phase= 6 to 7 will run one test case (the inputs you have not been given)
// Phase=10 will run the grader (all cases 1 to 7)
        .global Lab1
// Input: R0 points to the list
// Return: R0 as specified in Lab 1 assignment and terminal window
// According to AAPCS, you must save/restore R4-R7
// If your function calls another function, you must save/restore LR
Lab1: PUSH {R4-R7, LR}
        MOV R4, R0          // R4 = base address of the (ptr,score) list
        LDR R6, =EID        // R6 = address of our own EID string

Loop:
        LDR R1, [R4]        // R1 = pointer to current entry's EID
        CMP R1, #0
        BEQ NotFound        // if null pointer, we reached end, return -1

        MOV R2, R1          // R2 will walk through entry's EID
        MOV R3, R6          // R3 will walk through our EID

CompareLoop:
        LDRB R7, [R2]    // load byte from entry's EID; R2++
        LDRB R5, [R3]    // load byte from our EID; R3++
        CMP  R7, R5
        BNE  NotEqual        // bytes differ → not a match
        CMP  R7, #0
        BEQ  Found           // both bytes == 0 → end of string and matched
        ADDS R2, R2, #1
        ADDS R3, R3, #1

        B CompareLoop     // otherwise, continue comparing

NotEqual:
        ADDS R4, R4, #8      // move to next entry: each entry is 2 words (pointer, score)
        B Loop

Found:
        LDR R0, [R4, #4]    // load score for matching EID into R0
        POP {R4-R7, PC}     // restore and return

NotFound:
        MOVS R0, #0         // return -1 if no match
        SUBS R0, R0, #1
        POP {R4-R7, PC}     // restore and return



        .align 2
        .global myClass
myClass: .long pAB123  // pointer to EID
         .long 95      // Score
         .long pXYZ1   // pointer to EID
         .long 96      // Score
         .long pAB5549 // pointer to EID
         .long 94      // Score
         .long 0       // null pointer means end of list
         .long 0
pAB123:  .string "AB123"
pXYZ1:   .string "XYZ1"
pAB5549: .string "AB5549"
        .end
