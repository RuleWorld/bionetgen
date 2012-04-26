parser grammar BNGGrammar_Expression;

options {
  language = Java;
  output = template;

  
}
@members{
 public void getParentTemplate(){
 
  this.setTemplateLib(gParent.getTemplateLib());
 }
      @Override
  public String getErrorMessage(RecognitionException e,String[] tokenNames){
    return gParent.getErrorMessage(e,tokenNames);
  }
  
  public String getErrorMessage(Token s1,String error){
      return String.format("\%s line \%d:\%d \%s: \%s\n",input.getSourceName(),s1.getLine(),s1.getCharPositionInLine(),error,s1.getText());
  
  }
}
@init{
getParentTemplate();
}
/*
Expression rules for BioNetGen. These grammars include the expression rules (arithmetic operations using variables an integers) used in BioNetGen)

*/
expression [ Map<String,Register> memory] returns [Double value, Map reference]
scope{
  Map<String,Register> references;
  Map<String,Register> lmemory;
}
@init{
  $expression::references = new HashMap<String,Register>();
  $expression::lmemory = $memory;
}
:      
  expression2 {$value = $expression2.value;$reference = $expression::references;}
 ; 
 
expression2 returns [Double value]
:
  e1=relational_expression {$value = e1.value; } ((AND | OR) relational_expression)*
 ; 

 
relational_expression returns [Double value]: 
  a1=adding_expression {$value = a1.value;}((EQUALS|GT|GTE|LT|LTE) adding_expression)*
;

adding_expression returns [Double value]
: s1=multiplying_expression {$value = s1.value;} 
( PLUS s2=multiplying_expression {$value += s2.value;}
| MINUS s3=multiplying_expression {$value -= s3.value;})*
;

multiplying_expression returns [Double value]
: s1=power_expression {$value = s1.value;} 
( TIMES s2=power_expression {$value *= s2.value;}
| DIV s3=power_expression {$value /= s3.value;}
//| MOD s4=power_expression {$value \%= s4.value;}
)*
;

power_expression returns [Double value]
: 
(s1= sign_expression {$value = s1.value;}
(POWER s2=sign_expression {$value = Math.pow($value,s2.value);})*)


;

sign_expression returns [Double value]

: {$value = new Double(1);} ((MINUS {$value *= -1;}))* boolean_negation_expression {$value *= $boolean_negation_expression.value;}
;
//recover primite for functions
boolean_negation_expression returns [Double value]
: (n=NOT)? e=primitive_element {$value = e.value * (n == null ? 1 : -1);} 
;

primitive_element returns [Double value]
: 
        number {$value = $number.value;} 
        | (STRING LPAREN (expression2)? RPAREN) => function {$value = $function.value;}
        |  variable {$value = $variable.value;}
        | EXP LPAREN e1=expression2 RPAREN {$value = Math.exp($e1.value);} //TODO: not working
        | LPAREN e2=expression2 RPAREN {$value = $e2.value;}
        
        ;

number returns [Double value]: s1=INT {$value = Double.parseDouble($INT.text);}
      | s1=FLOAT { $value = Double.parseDouble($FLOAT.text);}
;
      
variable returns [Double value]: s1=STRING {
                  try{
                  
                  //if($value == null)
                    //throw [VariableNotFoundException]
                  if($expression::lmemory.containsKey($STRING.text)){
                    Register temp = $expression::lmemory.get($STRING.text);
                    $value = temp.getValue();
	                  if(!temp.getType().equals("parameter") && !temp.getType().equals("observable")){
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
                    String msg = getErrorMessage(s1,"variable not found");
                    System.err.println(msg);
                    
                  }
                  }
;

function returns [Double value]:
  s1=STRING LPAREN e1=
  
  (expression2)? RPAREN {
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
       else{
                    $value = 1.0;
                    //throw new RuntimeException("function not found: " + $STRING.text, new BNGSemanticException("function not found: " + $STRING.text,s1.getLine()));
                    String msg = getErrorMessage(s1,"function not found");
                    System.err.println(msg);
       }
       
        $expression::references.put($STRING.text,$expression::lmemory.get($STRING.text));
    }
    catch(NullPointerException e){
    
    }
  }
;
