parser grammar BNGGrammar_Actions;

options {
  language = Java;
  output = template;
}


@members{
 public void getParentTemplate(){
 
  this.setTemplateLib(gParent.getTemplateLib());
 }
 List<StringTemplate> actions = new ArrayList<StringTemplate>();
}

actions_prog:
.*
(actions_block  | (BEGIN ACTIONS actions_block END ACTIONS))? 
EOF -> actions(actions = {actions})
;
actions_block

@init{
  getParentTemplate();
  gParent.paraphrases.push("in the actions block");
}
@after{
  gParent.paraphrases.pop();
}
        : (action)+ 
;
action  : generate_network {actions.add($generate_network.st);}
        | simulate_method {actions.add($simulate_method.st);}
        | read_file {actions.add($read_file.st);}
        | write_file {actions.add($write_file.st);}
        | set_concentration {actions.add($set_concentration.st);}
        | set_parameter {actions.add($set_parameter.st);}
        | save_concentrations {actions.add($save_concentrations.st);}
        | reset_concentrations {actions.add($reset_concentrations.st);}
        ;
generate_network
scope{
  Map<String,String> actions;
}
@init{
  $generate_network::actions = new HashMap<String,String>();
}
        : GENERATENETWORK LPAREN (LBRACKET
        ((gn_action_par_def[$generate_network::actions]) 
        (COMMA gn_action_par_def[$generate_network::actions])*)?
        RBRACKET)? RPAREN SEMI -> action(id={$GENERATENETWORK.text},optionMap={$generate_network::actions})
        ;
        
gn_action_par_def[Map<String,String> map]
        : (MAX_AGG ASSIGNS i1=INT {map.put($MAX_AGG.text,$i1.text);})
        | (MAX_ITER ASSIGNS i2=INT {map.put($MAX_ITER.text,$i2.text);})
        | (MAX_STOICH ASSIGNS hash_value)
        | (OVERWRITE ASSIGNS i3=INT {map.put($OVERWRITE.text,$i3.text);})
        | (PRINT_ITER ASSIGNS i4=INT {map.put($PRINT_ITER.text,$i4.text);})
        | ps_par_def[map]
        ;
        
simulate_method
scope{
  Map<String,String> actions;
  String method;
}
@init{
  $simulate_method::method = "";
  $simulate_method::actions = new HashMap<String,String>();
}

        : (simulate_ode[$simulate_method::actions] {$simulate_method::method = "simulate_ode";}
        | simulate_ssa[$simulate_method::actions] {$simulate_method::method = "simulate_ssa";})
        -> action(id={$simulate_method::method},optionMap={$simulate_method::actions})
        ;
simulate_ode[Map<String,String> map]
        : SIMULATE_ODE LPAREN LBRACKET
         ((ps_par_def[map]|simulate_par_def[map]|simulate_ode_par_def[map])
          (COMMA (ps_par_def[map]|simulate_par_def[map]|simulate_ode_par_def[map]))*)? 
          RBRACKET RPAREN SEMI
         ;
simulate_ssa[Map<String,String> map]
        : SIMULATE_SSA LPAREN LBRACKET
          ((ps_par_def[map]|simulate_par_def[map]) (COMMA (ps_par_def[map]|simulate_par_def[map]))*)?
          RBRACKET RPAREN SEMI ;
read_file
scope{
  Map<String,String> actions;
}
@init{
  $read_file::actions = new HashMap<String,String>();
}
        : READFILE LPAREN 
        (LBRACKET (FILE ASSIGNS STRING {$read_file::actions.put($FILE.text,$STRING.text);})? RBRACKET)? 
        RPAREN SEMI -> action(id={$READFILE.text},optionMap={$read_file::actions})
        ;
write_file
scope{
  Map<String,String> actions;
}
@init{
  $write_file::actions = new HashMap<String,String>();
}
        : write_type
          LPAREN (LBRACKET RBRACKET)?
           RPAREN SEMI -> action(id={$write_type.text})
        ;
write_type
        : WRITENET
        | WRITESBML
        | WRITEMFILE
        | WRITEXML
        ;
set_concentration 
scope{
  Map<String,String> options;
}
@init{
  $set_concentration::options = new HashMap<String,String>();
}
        : SETCONCENTRATION LPAREN variable_definition 
        {$set_concentration::options.put($variable_definition.variableName,$variable_definition.variableValue);}
        RPAREN SEMI -> action(id={$SETCONCENTRATION.text},optionMap={$set_concentration::options})
        ; 
set_parameter
scope{
  Map<String,String> options;
}
@init{
  $set_parameter::options = new HashMap<String,String>();
}
        :  SETPARAMETER LPAREN parameter_definition 
          {$set_parameter::options.put($parameter_definition.parameterName,$parameter_definition.parameterValue);}
        RPAREN SEMI -> action(id={$SETPARAMETER.text},optionMap={$set_parameter::options})
        ;

parameter_definition returns [String parameterName,String parameterValue]
:
DBQUOTES 
STRING {$parameterName = $STRING.text;} 
DBQUOTES COMMA 
(e1=expression[gParent.memory] {$parameterValue = $e1.text; }| 
(DBQUOTES e2=expression[gParent.memory] DBQUOTES {$parameterValue = $e2.text; }))
;
        
variable_definition returns [String variableName, String variableValue]
scope{
List reactants;
BondList bonds;

}
@init{
  $variable_definition::reactants = new ArrayList();
  $variable_definition::bonds = new BondList();
}
:
DBQUOTES 
species_def[$variable_definition::reactants,$variable_definition::bonds,""] {$variableName = $species_def.text;} 
DBQUOTES COMMA 
(e1=expression[gParent.memory] {$variableValue = $e1.text; }| 
(DBQUOTES e2=expression[gParent.memory] DBQUOTES {$variableValue = $e2.text; }))
          
           
            ;


save_concentrations
        : SAVECONCENTRATIONS LPAREN RPAREN SEMI -> action(id={$SAVECONCENTRATIONS.text})
        ;
reset_concentrations
        : RESETCONCENTRATIONS LPAREN RPAREN SEMI -> action(id={$RESETCONCENTRATIONS.text})
        ;
hash_value
        : STRING ASSIGNS LBRACKET assignment_list RBRACKET
        ;
assignment_list
        : STRING ASSIGNS value (COMMA STRING ASSIGNS value)
        ;
value   : INT | FLOAT | STRING
        ;
ps_par_def[Map<String,String> map]
        : PREFFIX ASSIGNS ((DBQUOTES  STRING DBQUOTES) | STRING)
        | SUFFIX ASSIGNS ((DBQUOTES  STRING DBQUOTES) | STRING)
        ;
simulate_ode_par_def[Map<String,String> map]
        : ATOL ASSIGNS f1=FLOAT {map.put($ATOL.text,$f1.text);}
        | RTOL ASSIGNS f2=FLOAT {map.put($RTOL.text,$f2.text);}
        | STEADY_STATE ASSIGNS i1=INT {map.put($STEADY_STATE.text,$i1.text);}
        | SPARSE ASSIGNS i2=INT {map.put($SPARSE.text,$i2.text);}
        ;
simulate_par_def[Map<String,String> map]
        : T_END ASSIGNS (i1=INT {map.put($T_END.text,$i1.text);}|f1=FLOAT {map.put($T_END.text,$f1.text);}) 
        | T_START ASSIGNS (i2=INT {map.put($T_START.text,$i2.text);}|f2=FLOAT {map.put($T_START.text,$f2.text);})
        | N_STEPS ASSIGNS i3=INT {map.put($N_STEPS.text,$i3.text);}
        | SAMPLE_TIMES ASSIGNS i4=array_value
        ;
        
array_value
        : LSBRACKET STRING (COMMA STRING)* RSBRACKET
        ;

