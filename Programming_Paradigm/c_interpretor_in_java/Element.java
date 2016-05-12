public class Element {
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