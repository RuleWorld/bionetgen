parser grammar BNGGrammar_Expression;

options{
  language = Java;
  output = template;
}
@members{
  public void getParentTemplate(){
    this.setTemplateLib(gParent.getTemplateLib());
  }
  String[] list = {"sin",
                   "cos",
                   "tan",
                   "exp",
                   "ln",
                   "log10",
                   "log2",
                   "abs",
                   "floor",
                   "ceil",
                   "sqrt",
                   "asin",
                   "acos",
                   "atan",
                   "sinh",
                   "cosh",
                   "tanh",
                   "asinh",
                   "acosh",
                   "atanh"};
  @Override
  public String getErrorMessage(RecognitionException e,String[] tokenNames){
    return gParent.getErrorMessage(e,tokenNames);
  }
  public String getErrorMessage(Token s1,String error){
    return String.format("\%s line \%d:\%d \%s: \%s",
                         input.getSourceName(),
                         s1.getLine(),
                         s1.getCharPositionInLine(),
                         error,
                         s1.getText());
  }
  public boolean isSpecialFunction(String text){
    //if(Arrays.asList(list).contains(text))
    //  return true;
     return false;
  }
}
@init{
  getParentTemplate();
}

/*
Expression rules for BioNetGen. These grammars include the expression rules (arithmetic operations using variables and integers) used in BioNetGen
*/
expression [Map<String,Register> memory] returns [Double value, Map reference]
scope{
  Map<String,Register> references;
  Map<String,Register> lmemory;
}
@init{
  $expression::references = new HashMap<String,Register>();
  $expression::lmemory = $memory;
}
:      
  expression2 {$value = $expression2.value; $reference = $expression::references;}
; 
 
expression2 returns [Double value]
:
  e1=relational_expression {$value = e1.value;} 
  (((AMPERSAND AMPERSAND)| (PIPE PIPE)) relational_expression)*
; 
 
relational_expression returns [Double value]
: 
  a1=adding_expression {$value = a1.value;}
  ((EQUALS|GT|GTE|LT|LTE) adding_expression)*
;

adding_expression returns [Double value]
: 
  s1=multiplying_expression {$value = s1.value;} 
  (PLUS s2=multiplying_expression {$value += s2.value;} | 
  MINUS s3=multiplying_expression {$value -= s3.value;})*
;

multiplying_expression returns [Double value]
: 
  s1=power_expression {$value = s1.value;} 
  (TIMES s2=power_expression {$value *= s2.value;} | 
  DIV s3=power_expression {$value /= s3.value;}
  //| MOD s4=power_expression {$value \%= s4.value;}
  )*
;

power_expression returns [Double value]
: 
  (s1= sign_expression {$value = s1.value;}
  (POWER s2=sign_expression {$value = Math.pow($value,s2.value);})*)
;

sign_expression returns [Double value]
: 
  {$value = new Double(1);} 
  ((MINUS {$value *= -1;}))* 
  boolean_negation_expression {$value *= $boolean_negation_expression.value;}
;

//recover primitive for functions
boolean_negation_expression returns [Double value]
: 
// (n=NOT)? e=primitive_element {$value = e.value * (n == null ? 1 : -1);} // BNG doesn't support unary 'not'
  e=primitive_element {$value = e.value;} 
;

primitive_element returns [Double value]
: 
  number {$value = $number.value;} | 
  (IF LPAREN (~RPAREN)+ RPAREN) => if_expression {$value = $if_expression.value;} | 
  (STRING LPAREN (expression2 (COMMA expression2)*)? RPAREN) => function {$value = $function.value;} | 
  variable {$value = $variable.value;} | 
  (EXP | LN | LOG10 | LOG2 | SQRT | RINT | ABS | SIN | COS | TAN | ASIN | ACOS | ATAN | SINH | COSH | TANH | ASINH | ACOSH | ATANH) LPAREN e1=expression2 RPAREN {$value = Math.exp($e1.value);} | 
  LPAREN e2=expression2 RPAREN {$value = $e2.value;} | 
  (PI | EULERIAN | TIME) LPAREN RPAREN {$value = Math.PI;} | 
  (MIN | MAX | SUM | AVG) LPAREN (e3=expression2 (COMMA expression2)*) RPAREN {$value = $e3.value;}
;

number returns [Double value]
: 
  s1=INT {$value = Double.parseDouble($INT.text);} | 
  s1=FLOAT { $value = Double.parseDouble($FLOAT.text);}
;
    
variable returns [Double value]
: 
  s1=STRING 
  {
    try{
      //if($value == null)
      //throw [VariableNotFoundException]
      if($expression::lmemory.containsKey($STRING.text)){
        Register temp = $expression::lmemory.get($STRING.text);
        $value = temp.getValue();
	      if(!temp.getType().equals("ConstantExpression") && !temp.getType().equals("observable")){
	        String msg = getErrorMessage(s1,"the following token is in memory but is not a variable or an observable");
	        System.err.println(msg);
	      }
      }
      else{
        $value = 1.0;
        String msg = getErrorMessage(s1,"variable not found");
        System.err.println(msg);
      }
      $expression::references.put($STRING.text,$expression::lmemory.get($STRING.text));
    }
    catch(NullPointerException e){
      $value = 1.0;
      String msg = getErrorMessage(s1,"variable not found");
      System.err.println(msg);
    }
  }
;

if_expression returns [Double value]
:
  IF LPAREN expression2 COMMA e1=expression2 COMMA expression2 RPAREN {$value = $e1.value;}
;

function returns [Double value]
:
  s1=STRING LPAREN 
  e1=(expression2 (COMMA expression2)*)? RPAREN 
  {
    try{
      if($expression::lmemory.containsKey($STRING.text)){
        Register temp = $expression::lmemory.get($STRING.text);
        $value = temp.getValue();
        if(!temp.getType().equals("function") && !temp.getType().equals("observable")){
          //throw new RuntimeException($STRING.text + "is in memory but it is not a function. Check syntax", new BNGSemanticException($STRING.text + " is in memory but it is not a function. Check syntax",s1.getLine()));
          $value = 1.0;
          String msg = getErrorMessage(s1,"the following token is in memory but is not a function or an observable");
          System.err.println(msg);
        }
      }
      else if(isSpecialFunction($STRING.text)){
      //add xml code in here
        $value=1.0;
      }
//    else if($STRING.text.equals("MM")){
//      $value=1.0;
//    }
      else{
        $value = 1.0;
        //throw new RuntimeException("function not found: " + $STRING.text, new BNGSemanticException("function not found: " + $STRING.text,s1.getLine()));
        String msg = getErrorMessage(s1,"function not found");
        System.err.println(msg);
      }
      $expression::references.put($STRING.text,$expression::lmemory.get($STRING.text));
    }
    catch(NullPointerException e){}
  }  
;
