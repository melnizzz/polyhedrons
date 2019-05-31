# polyhedrons
Build the polyhedron

This programm can build the polyhedron using its nodes or equations of its faces; draw this polyhedron; find the intersection of 2 polyhedrons.

The programm works with input data in files.

HOW TO START THE PROGRAMM:
You should start the programm in your cmd with
1. the path to the programm
2. the path to the file
3.(optional) the path to the second file

Example: 
-> polyhedrons.exe a.txt
 or
-> polyhedrons.exe f1.txt f2.txt

1. To build the polyhedron using its nodes you should use this type of file:

  V            //special symbol
  4            //count of nodes
  0 0 0        //coordinates of nodes
  0 1 0
  1 0 0
  0 0 1
  
  See the example in file 'a.txt'.
  
2. To build the polyhedron using equations of its faces you should use this type of file:

  H            //special symbol
  4            //count of equations
  -1  0  0  0  //coefficients of equations
   0 -1  0  0
   0  0 -1  0
   1  1  1  1
   
  See the example in file 'b.txt'.
  
3. To find the intersection of 2 polyhedrons you should use this type of file for each of polyhedrons:

  4            //count of nodes
  0 0 0        //coordinates of nodes
  0 1 0
  1 0 0
  0 0 1
  
  See the example in files 'f1.txt' and 'f2.txt'.
