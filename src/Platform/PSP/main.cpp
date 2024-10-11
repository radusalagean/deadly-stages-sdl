#include "../../Game.hpp"

// Reference: https://groups.csail.mit.edu/cag/raw/documents/R4400_Uman_book_Ed2.pdf (page 160 in book - 190 in PDF)
void disableFPUExceptions() {

    unsigned int fcsr_before, fcsr_after;

    // Read the initial FCSR value
    asm volatile("cfc1 %0, $31" : "=r"(fcsr_before));

    // Print the initial FCSR value
    printf("Initial FPU FCSR value: 0x%08X\n", fcsr_before);
    printf("Exception enable bits status:\n");
    printf("I (Inexact Operation): %s\n", (fcsr_before & (1 << 7)) ? "1" : "0");
    printf("U (Underflow): %s\n", (fcsr_before & (1 << 8)) ? "1" : "0");
    printf("O (Overflow): %s\n", (fcsr_before & (1 << 9)) ? "1" : "0");
    printf("Z (Division by Zero): %s\n", (fcsr_before & (1 << 10)) ? "1" : "0");
    printf("V (Invalid Operation): %s\n", (fcsr_before & (1 << 11)) ? "1" : "0");

    // Disable floating-point exceptions
    unsigned int fcsr = fcsr_before;
    fcsr &= ~(1 << 7);  // Clear the "I" (Inexact Operation) enable bit
    fcsr &= ~(1 << 8);  // Clear the "U" (Underflow) enable bit
    fcsr &= ~(1 << 9);  // Clear the "O" (Overflow) enable bit
    fcsr &= ~(1 << 10); // Clear the "Z" (Division by Zero) enable bit
    fcsr &= ~(1 << 11); // Clear the "V" (Invalid Operation) enable bit

    // Write the modified value back to FCSR
    asm volatile("ctc1 %0, $31" : : "r"(fcsr));

    // Read the FCSR value again to confirm changes
    asm volatile("cfc1 %0, $31" : "=r"(fcsr_after));

    printf("\n");

    // Print the final FCSR value
    printf("Final FPU FCSR value: 0x%08X\n", fcsr_after);
    printf("Exception enable bits status:\n");
    printf("I (Inexact Operation): %s\n", (fcsr_after & (1 << 7)) ? "1" : "0");
    printf("U (Underflow): %s\n", (fcsr_after & (1 << 8)) ? "1" : "0");
    printf("O (Overflow): %s\n", (fcsr_after & (1 << 9)) ? "1" : "0");
    printf("Z (Division by Zero): %s\n", (fcsr_after & (1 << 10)) ? "1" : "0");
    printf("V (Invalid Operation): %s\n", (fcsr_after & (1 << 11)) ? "1" : "0");

    printf("\n");
}

int main(int argc, char *argv[])
{
    disableFPUExceptions();
    Game::init();
    while (Game::isRunning)
    {
        Game::loop();
    }
    Game::dispose();

    return 0;
}