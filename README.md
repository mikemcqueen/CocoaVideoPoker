This is a iPhone VideoPoker "trainer" program/game that I wrote in the 2010-2011 period. It's incomplete, but the basic functionality of deal/hold cards/draw is working. The purpose of this trainer was to be able to show the "best" cards to hold for any given hand, based on the payouts of the "pay schedule", which is the set of possible winning hands and the payout multipler per winning hand, per amount bet.

The secret sauce is stored in file SolverData/Jacks.dat, which is a datafile that contains all of the best holding cards for all possible hand combinations for a standard variant of "Jacks Or Better" five-card draw. This file was generated using "CardLib/SolverDataGenerator.*" on a PC. The CPU version of that code, if I remember correctly, took multiple 10s of days to execute for a single pay schedule on a 2009-era Core i7 CPU. So I CUDA-fied it, and using a 2009-era nVidia card the same computations were completed in a manner of minutes. The CUDA-fied code for the generator is not checked in here, and was a Windows project.

Some of the CardLib code was originally Windows code, and dates back to the early 2000s (or earlier) when I wrote a Windows-based Texas Holdem Poker playing program, which automatically folded bad starting  hands (and beeped on good hands) for an early internet real-money poker site. 

To run in simulator:

Load VideoPoker/xcode/VideoPoker.xcworkspace

You may then need to select "VideoPoker Debug Simulator" as the current project in Xcode.

Build & Run.
