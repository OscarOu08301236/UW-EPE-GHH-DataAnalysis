// Stand-alone program based on Analyze.C
// To compile this:
//   g++ `root-config --cflags --libs` Analyze.cpp -o Analyze

#include <TSelector.h>
#include <TFile.h>
#include <TTree.h>

// Main program.
int main(int argc, char *argv[])
{
  // Open the file that the TSelector class will process.
  TFile* inputfile = new TFile("experiment.root");
  // Read tree1 from the input file.
  TTree* tree1 = (TTree*) inputfile->Get("tree1");
  // Did we read in tree1?
  if  (tree1 != NULL)
    {
      // Tell tree1 to process the class created by MakeSelector.
      tree1->Process("Analyze.C");
    }
    return 0;
}
