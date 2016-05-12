Using Java (or C), implement a software that, given in input a snippet of C code, would recognize the identifiers, the value, the data type and the scope. The input is supplied via text file "input.txt".

The software should print in output the list of identifiers along with the related information.


Example (the input will not contain the numbers near the lines)

1   int a = 1;

2   void fun() {

3      char c;

4      printf("%c", c);

5   }

6   int main() {

7      int a = 3;

8      fun();

9   }



The output should look like this (the order of the identifiers is not important):

a   ->  data type: integer, scope: global, value: 1

a   ->  data type: integer, scope: local to main, value: 3

fun -> data type: void function, scope: global

main -> data type: int function, scope: global

c -> data type: character, scope: local to fun, value: -



To get full points, the software, given the snippet of C code and a line number, should be able to reconstruct the Symbol Table in that specific point of execution.

After asking in input the line number, the output should look like:

Input line: 8

Output:

a   ->  data type: integer, scope: global, value: 1    -> data type: integer, scope: local to main, value: 3

fun -> data type: void function, scope: global

main-> data type: int function, scope: global

c -> data type: character, scope: local to fun, value: -



Input line: 4

Output:

a   ->  data type: integer, scope: global, value: 1

fun -> data type: void function, scope: global

c -> data type: character, scope: local to fun, value: -

identifiers: variables and functions
values: identifiers only, functions don’t have values

data type:
     char
     signed char
     unsigned char
     short
     short int
     signed short
     signed short int
     unsigned short
     unsigned short int
     int
     signed
     signed int
     unsigned
     unsigned int
     long
     long int
     signed long
     signed long int
     unsigned long
     unsigned long int
     long long
     long long int
     signed long long
     signed long long int
     unsigned long long
     unsigned long long int
     float
     double
     long double

Arrays?


Pointers?
Strings?
Structures?
Unions?

scrope: global, local (within a function)

=====================================

Declare varibles:
     identifier, datatype, value, scope
class Element {
     private String identifier;
     private String datatype;
     private String value;
     private String scope;

     public Element (String id, String d_type, String val, String sp){
          this.identifier = id;
          this.datatype = d_type;
          this.value = val;
          this.scope = sp;
     }

     public String getID() { return identifier; }
     public String getData() { return datatype; }
     public String getVal() { return value; }
     public String getSp() { return scope; }

     public void setID(String id) { this.identifier = id; }
     public void setData(String data) { this.datatype = data; }
     public void setVal(String val) { this.value = val; }
     public void setSp(String sp) { this.scope = sp; }
}

Read file with java.io.*

import java.io.*;

public class CopyFile {
   public static void main(String args[]) throws IOException
   {
      FileInputStream in = null;
      FileOutputStream out = null;

      try {
         in = new FileInputStream("input.txt");
         out = new FileOutputStream("output.txt");

         int c;
         while ((c = in.read()) != -1) {
            out.write(c);
         }
      }finally {
         if (in != null) {
            in.close();
         }
         if (out != null) {
            out.close();
         }
      }
   }
}

-create an Element array equal the the number of lines in the file
-after reading each line, it should have it’s own set of Element.
     -assign Element function
-after finish reading the file, all the Elements are read one after anothe to generate output.
     -output functionjava


Question on input.txt:

- will the code in input.txt be perfect syntax?

- will the code uses the same coding style as the example? (such as bracket placements, tabs, and one semicolon per line, etc.)

Question on variables:

- we only need to worry about int and char? (no other types necessary)

- we should consider function parameters and identify them as variables?

- if a = b and b = 1, the value of a should be 1 not b, right?

Question on functions:

- the return types are void and int in the example, that means if we have an int function with a return value, then that function will have an actual value instead of - in the output?

- do we have to worry about char functions?

- do we have to worry about function within functions? 

such as:

 int a (){

      int b (int c){

           return c;

      }

    return b ('0');

}