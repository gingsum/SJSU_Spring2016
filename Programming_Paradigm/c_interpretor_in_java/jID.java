import java.io.FileInputStream;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.IOException;
import java.lang.NumberFormatException;
import java.util.ArrayList;
import java.lang.System;

public class jID {
  static ArrayList<Element> eList = new ArrayList<Element>();

  private static void setElement (String type, String var, String val, String scope){
      Element a = new Element(type,var,val,scope);
      eList.add(a);
  } 
  private static void createTable()  {
    ArrayList<String> file_content = new ArrayList<String>();
    FileInputStream fin = null;
    BufferedReader br = null;
    String curr_type = null;
    String curr_var = null;
    String curr_val = null;
    String curr_scope = "global";
    String future_scope = null;
    String line = null;
    int track_bracket = 0;

    // read file and store in an ArrayList
    try {
       fin = new FileInputStream("input.txt");
       br = new BufferedReader(new InputStreamReader(fin));
       while ((line = br.readLine()) != null) {
         if(!(line.equals("\n") || line.equals("\t") || line.equals(" ") || line.equals("")))
            file_content.add(line);
       }
       fin.close();
    } catch(IOException ex) {
      System.out.println("Invalid file, program terminated");   
      System.exit(1);
    }

    // start here
    for(int i = 0; i < file_content.size(); i++){
      curr_type = null;
      curr_var = null;
      curr_val = null;
      future_scope = null;
      //System.out.println(i + file_content.get(i));

      String[]words = file_content.get(i).split(" +");
      
      // separate the tokens in each line
      for(int j = 0; j < words.length; j++){
        words[j] = words[j].replaceAll(" ", "");
        words[j] = words[j].replaceAll("\t", "");
        words[j] = words[j].replaceAll("\n", "");

        if(words[j].contains("{"))
          track_bracket++;
        // check for int, char, void first
        if(curr_type == null && (words[j].equals("int") || words[j].equals("char") || words[j].equals("void")))
          curr_type = words[j];
        else if(curr_type != null && curr_var == null){
          if(words[j].contains("(")){
            curr_type = curr_type + " function";
            curr_var = words[j].substring(0,words[j].indexOf("("));
            future_scope = curr_var;
          } else { 
            if (words[j].contains(";"))
              curr_var = words[j].substring(0,words[j].indexOf(";"));
            else
              curr_var = words[j];
          }
        } else if(words[j].equals("=")){
          curr_val = "0";
        } else if(curr_val != null && curr_val.equals("0")){
          if(curr_var != null)
            if(words[j].contains(";"))
              curr_val = words[j].substring(0,words[j].indexOf(";"));
            else
              curr_val = words[j];
        } else if(words[j].equals("}") && track_bracket == 1){
            curr_scope = "global";
        }
        if (curr_var == null && curr_val != null){
          curr_var = words[j - 1];
          if(words[j+1].contains(";"))
            curr_val = words[j+1].substring(0,words[j+1].indexOf(";"));
          else
            curr_val = words[j+1];
        }
        if(words[j].contains("}"))
          track_bracket--;
        // end of for down here

      }
      // set element 
      setElement(curr_type, curr_var, curr_val, curr_scope);
      if(future_scope != null)
        curr_scope = future_scope;
    }
  }

  public static void main(String args[]) {
      int line_input;
      createTable();
/*
      for(int j = 0; j < eList.size(); j++){
        //System.out.println("eList: " + eList.size());  
        System.out.println(eList.get(j).getID() + " " +
                           eList.get(j).getData() + " " +
                           eList.get(j).getVal() + " " +
                           eList.get(j).getSp());
      }
  */    
      while (true){
        System.out.print("Input line: ");
        String input = System.console().readLine();

        try{
         int i = Integer.parseInt(input);
         line_input = Integer.parseInt(input);
        }catch(NumberFormatException ex){ 
          System.out.println("Invalid input, please try again.");           
          continue;
        }

        for(int k = 0; k < eList.size(); k++){
          String t = eList.get(k).getID();
          String r = eList.get(k).getData();
          String l = eList.get(k).getVal();
          String s = eList.get(k).getSp();

          if(t == null && r != null && l != null && s != null){
            for (int m = 0; m < eList.size(); m++){
              String t1 = eList.get(m).getID();
              String r1 = eList.get(m).getData();
              String l1 = eList.get(m).getVal();
              String s1 = eList.get(m).getSp();

              if(t1 != null && r1 != null && s1 != null)
                if(r1.equals(r) && s1.equals(s)){
                  Element temp_element = new Element(t1,r1,l,s1);
                  eList.set(m,temp_element);
                }
            }
          }
        }

        // read output 
        for(int j = 0; j < line_input; j++){
          String type = eList.get(j).getID();
          String var = eList.get(j).getData();
          String val = eList.get(j).getVal();
          String scope = eList.get(j).getSp();
          
          String temp = var + " -> data type: " + type + ", scope: " + 
                        scope + ", value: " + val;
          if(type != null)
            System.out.println(temp);
        }
     
        System.out.print("Would you like to try again? (y/n)");
        input = System.console().readLine();
        while(true)
          if(input.equals("y") || input.equals("Y")){
            break;
          }
          else if (input.equals("N") || input.equals("n"))
            return;
          else{
            System.out.println("Invalid input, please try again.");
            System.out.print("Would you like to try again? (y/n)");
            input = System.console().readLine();
          }
          continue;
      }
  }
}