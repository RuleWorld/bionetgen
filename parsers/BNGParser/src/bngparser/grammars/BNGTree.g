tree grammar BNGTree;

options{
  tokenVocab = BNGGrammar;
  ASTLabelType=CommonTree;
  language = Java;
}

@header{
  package bngparser.grammars;
  import java.util.HashMap;
  import java.util.Stack;
  import bngparser.dataType.SingleMoleculeType;
  import bngparser.dataType.MoleculeType;
  
}

@members{
  HashMap memory = new HashMap();
  Stack<String> stack = new Stack<String>();
}

prog: {
System.out.println("Ready to rock");
} program_body;

program_body:  (program_block)*;

program_block
: parameters_block
| molecule_types_block
| seed_species_block
//| observables_block
//| reaction_rules_block
//| set_option
;

parameters_block: 
  PARAMETERS parameters_def*;

parameters_def: ^(BECOMES STRING expr){memory.put($STRING.text,new Float($expr.value));
System.out.println("parameter: " + $STRING.text + " = " + $expr.value);};

expr returns [float value]:
   ^(PLUS a=expr b=expr) {$value = a+b;}
|  ^(MINUS a=expr b=expr) {$value = a-b;}
|  ^(TIMES a=expr b=expr) {$value = a*b;}
|  ^(DIV a=expr b=expr) {$value = a/b;}
|  ^(MOD a=expr b=expr) {$value = a\%b;}
|  FLOAT {$value = Float.parseFloat($FLOAT.text);}
| STRING {
    Float v = (Float)memory.get($STRING.text);
    if(v!=null) $value = v.floatValue();
   }
;

molecule_types_block: MOLECULE TYPES molecule_def*;

molecule_def: ^(STRING site_def*){
  SingleMoleculeType moleculeTypes = new SingleMoleculeType($STRING.text);
  while(!stack.empty()){
    String pointer = stack.pop();
    String[] temp = pointer.split(",");
    if(temp[0].equals("siteDef"))
      moleculeTypes.addNewSiteElement(temp[1]);
    else
      moleculeTypes.addElement(temp[1]);
  }
  stack.clear();
  moleculeTypes.reverse();
  memory.put($STRING.text, moleculeTypes);
  
  System.out.println(memory.get($STRING.text));
  };
  
site_def: ^(STRING state_label*){
  stack.push(new String("siteDef," + $STRING.text));
  };

state_label: STRING {
  stack.push(new String("stateLabel," + $STRING.text));
  
  };

seed_species_block: SEED SPECIES seed_species_def*;

seed_species_def: constant_species_def expr{
                    memory.put($constant_species_def.value, $expr.value);
                  }
                  | (species_def expr){
                      MoleculeType moleculeType = new MoleculeType();
                      while(!stack.empty()){
                          String pointer = stack.pop();
                          String[] temp = pointer.split(",");
                          if(temp[0].equals("bondLabelInt")){
                            System.out.println("bondLabel " +temp[1]);
                          }
                          else if(temp[0].equals("bondLabelString")){
                            System.out.println("bondLabelString "+  temp[1]);
                          
                          }
                          
                          if(temp[0].equals("stateLabel")){
                            System.out.println("stateLabel " + temp[1]);
                          
                          }
                          
                          else if(temp[0].equals("siteLabel")){
                          
                            System.out.println("siteLabel " + temp[1]);
                          }
                          
                          else if(temp[0].equals("speciesLabel")){
                            SingleMoleculeType v = (SingleMoleculeType)memory.get(temp[1]);
                            moleculeType.addMolecules(v);
                            System.out.println("speciesLabel " + temp[1]);
                            //memory.put($STRING.text, moleculeTypes);
                            
                          }
                          else{
                          
                          }   
                       
                      }
                      
                      stack.clear();
                  };

constant_species_def returns [String value]: DOLLAR STRING
      {
        $value = $STRING.text;
      };

species_def: species_element (DOT species_element)* ;


species_element: ^(STRING site_list){
    stack.push(new String("speciesLabel" + $STRING.text));
   
    
};


site_list:   site_obs (COMMA site_obs)*
  | //nothing;
  ;

site_obs: STRING {
    stack.push(new String("siteLabel," + $STRING.text));
  }
(TILDE state_label)? (EMARK (PLUS|QMARK|bond_name))?;

bond_name: INT
{
  stack.push(new String("bondLabelInt," + $INT.text));
}
          |STRING{
  stack.push(new String("bondLabelString," + $STRING.text));
};

reaction_rules_block: REACTION reaction_rule_def+;
    
reaction_rule_def: BEGIN (modif_command)*;

modif_command:  END
          /*include_command
        | exclude_command*/
        ;


        
/*rel_expr returns [boolean value]:
 ^(EQUALS a=rel_expr b=rel_expr) {$value = a==b;}
  ^(GT a=rel_expr b=rel_expr) {$value = a>b;}
  ^(GTE a=rel_expr b=rel_expr) {$value = a>=b;}
  ^(LT a=rel_expr b=rel_expr) {$value = a<=b;}
  ^(LTE a=rel_expr b=rel_expr) {$value = a<b;}
;*/

primitive_element: number | variable;
number : INT | FLOAT;
variable : STRING;



  actions_block: END;