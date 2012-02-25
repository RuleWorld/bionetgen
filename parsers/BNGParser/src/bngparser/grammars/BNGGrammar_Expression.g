parser grammar BNGGrammar_Expression;

options {
  language = Java;
  output = template;

  
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
: s1=sign_expression {$value = s1.value;} 
( TIMES s2=sign_expression {$value *= s2.value;}
| DIV s3=sign_expression {$value /= s3.value;}
| MOD s4=sign_expression {$value \%= s4.value;})*
;

sign_expression returns [Double value]

: {$value = new Double(1);} ((MINUS {$value *= -1;}))* boolean_negation_expression {$value *= $boolean_negation_expression.value;}
;

boolean_negation_expression returns [Double value]
: (n=NOT)? e=primitive_element {$value = e.value * (n == null ? 1 : -1);} 
;

primitive_element returns [Double value]: 
        number {$value = $number.value;} 
        | variable {$value = $variable.value;}
        | LPAREN expression2 RPAREN {$value = $expression2.value;}
        ;

number returns [Double value]: s1=INT {$value = Double.parseDouble($INT.text);}
      | s1=FLOAT { $value = Double.parseDouble($FLOAT.text);}
;
      
variable returns [Double value] throws VariableNotFoundException: STRING {
            
                  Register temp = $expression::lmemory.get($STRING.text);
                  //if($value == null)
                    //throw [VariableNotFoundException]
                  $value = temp.getValue();
                  $expression::references.put($STRING.text,$expression::lmemory.get($STRING.text));
                  }
;
