parser grammar BNGGrammar_Actions;

options{
  language = Java;
  output = template;
}
@members{
  public void getParentTemplate(){
    this.setTemplateLib(gParent.getTemplateLib());
  }
  List<StringTemplate> actions = new ArrayList<StringTemplate>();
  @Override
  public String getErrorMessage(RecognitionException e,String[] tokenNames){
    return gParent.getErrorMessage(e,tokenNames);
  }
}

actions_prog
:
  .*
  actions_block
  EOF 
  -> actions(actions = {actions})
;

actions_block
@init{
  getParentTemplate();
  gParent.paraphrases.push("in the actions block");
}
@after{
  gParent.paraphrases.pop();
}
: 
  (BEGIN ACTIONS LB+)?
    (action LB*)+
  (END ACTIONS LB*)?
;

action  
: 
  generate_network {actions.add($generate_network.st);} | 
  generate_hybrid_model {actions.add($generate_hybrid_model.st);} |
  simulate_method {actions.add($simulate_method.st);} | 
  read_file {actions.add($read_file.st);} | 
//  write_file {actions.add($write_file.st);} | 
  write_model {actions.add($write_model.st);} | 
  write_xml {actions.add($write_xml.st);} | 
  write_network {actions.add($write_network.st);} |
  write_sbml {actions.add($write_sbml.st);} |
  write_mfile {actions.add($write_mfile.st);} | 
  write_mexfile {actions.add($write_mexfile.st);} | 
  write_mdl {actions.add($write_mdl.st);} |
  set_concentration {actions.add($set_concentration.st);} | 
  add_concentration {actions.add($add_concentration.st);} |
  save_concentrations {actions.add($save_concentrations.st);} | 
  reset_concentrations {actions.add($reset_concentrations.st);} | 
  set_parameter {actions.add($set_parameter.st);}  |
  save_parameters {actions.add($save_parameters.st);} | 
  reset_parameters {actions.add($reset_parameters.st);} |
  quit {actions.add($quit.st);}
;

generate_network
scope{
  Map<String,String> actions;
}
@init{
  $generate_network::actions = new HashMap<String,String>();
}
: 
  GENERATENETWORK LPAREN (LBRACKET
  ((gn_action_par_def[$generate_network::actions]) 
  (COMMA gn_action_par_def[$generate_network::actions])*)?
  RBRACKET)? RPAREN SEMI? 
  -> action(id={$GENERATENETWORK.text},optionMap={$generate_network::actions})
;
        
gn_action_par_def[Map<String,String> map]
: 
  ps_par_def |
  (VERBOSE ASSIGNS i1=INT {map.put($VERBOSE.text,$i1.text);}) | 
  (OVERWRITE ASSIGNS i2=INT {map.put($OVERWRITE.text,$i2.text);}) | 
  (PRINT_ITER ASSIGNS i3=INT {map.put($PRINT_ITER.text,$i3.text);}) | 
  (MAX_AGG ASSIGNS i4=INT {map.put($MAX_AGG.text,$i4.text);}) | 
  (MAX_ITER ASSIGNS i5=INT {map.put($MAX_ITER.text,$i5.text);}) | 
  (MAX_STOICH ASSIGNS hash_value) | 
  (TEXTSPECIES ASSIGNS i6=INT {map.put($TEXTSPECIES.text,$i6.text);})  |
  (TEXTREACTION ASSIGNS i7=INT {map.put($TEXTREACTION.text,$i7.text);})
;

generate_hybrid_model
scope{
  Map<String,String> actions;
}
@init{
  $generate_hybrid_model::actions = new HashMap<String,String>();
}
: 
  GENERATEHYBRIDMODEL LPAREN (LBRACKET
  ((gnhy_action_par_def[$generate_hybrid_model::actions]) 
  (COMMA gnhy_action_par_def[$generate_hybrid_model::actions])*)?
  RBRACKET)? RPAREN SEMI? 
  -> action(id={$GENERATEHYBRIDMODEL.text},optionMap={$generate_hybrid_model::actions})
;
        
gnhy_action_par_def[Map<String,String> map]
: 
  ps_par_def |
  (VERBOSE ASSIGNS i1=INT {map.put($VERBOSE.text,$i1.text);}) | 
  (OVERWRITE ASSIGNS i2=INT {map.put($OVERWRITE.text,$i2.text);}) | 
  (EXECUTE ASSIGNS i3=INT {map.put($EXECUTE.text,$i3.text);}) | 
  (ACTIONS ASSIGNS LSBRACKET a1=((~RSBRACKET)*) RSBRACKET {map.put($ACTIONS.text,$a1.text);}) | 
  (SAFE ASSIGNS i4=INT {map.put($SAFE.text,$i4.text);})
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
: 
  simulate[$simulate_method::actions] {$simulate_method::method = "simulate";} |
  simulate_ode[$simulate_method::actions] {$simulate_method::method = "simulate_ode";} | 
  simulate_ssa[$simulate_method::actions] {$simulate_method::method = "simulate_ssa";} | 
  simulate_pla[$simulate_method::actions] {$simulate_method::method = "simulate_pla";} | 
  simulate_nf[$simulate_method::actions] {$simulate_method::method = "simulate_nf";} | 
//  write_mfile[$simulate_method::actions] {$simulate_method::method = "writeMfile";} | 
//  write_mexfile[$simulate_method::actions] {$simulate_method::method = "writeMexfile";} | 
//  write_network[$simulate_method::actions] {$simulate_method::method = "writeNetwork";} |
  parameter_scan[$simulate_method::actions] {$simulate_method::method = "parameter_scan";}
  -> action(id={$simulate_method::method},optionMap={$simulate_method::actions})
;
        
simulate[Map<String,String> map]
: 
  SIMULATE LPAREN (LBRACKET 
  ((ps_par_def|simulate_par_def[map]|simulate_ode_par_def[map]|simulate_pla_par_def[map])
  (COMMA (ps_par_def|simulate_par_def[map]|simulate_ode_par_def[map]|simulate_pla_par_def[map]))*)? 
  RBRACKET)? RPAREN SEMI?
;

simulate_ode[Map<String,String> map]
:
  SIMULATE_ODE LPAREN (LBRACKET
  ((ps_par_def|simulate_par_def[map]|simulate_ode_par_def[map])
  (COMMA (ps_par_def|simulate_par_def[map]|simulate_ode_par_def[map]))*)? 
  RBRACKET)? RPAREN SEMI?
;

simulate_ssa[Map<String,String> map]
: 
  SIMULATE_SSA LPAREN (LBRACKET
  ((ps_par_def|simulate_par_def[map]) 
  (COMMA (ps_par_def|simulate_par_def[map]))*)?
  RBRACKET)? RPAREN SEMI?
;

simulate_pla[Map<String,String> map]
:
  SIMULATE_PLA LPAREN (LBRACKET
  ((ps_par_def|simulate_par_def[map]|simulate_pla_par_def[map])
  (COMMA (ps_par_def|simulate_par_def[map]|simulate_pla_par_def[map]))*)? 
  RBRACKET)? RPAREN SEMI?
;

simulate_nf[Map<String,String> map]
:
  SIMULATE_NF LPAREN (LBRACKET
  ((ps_par_def|simulate_par_def[map]|simulate_nf_par_def[map])
  (COMMA (ps_par_def|simulate_par_def[map]|simulate_nf_par_def[map]))*)? 
  RBRACKET)? RPAREN SEMI?
;

parameter_scan[Map<String,String> map]
:
  PARAMETER_SCAN LPAREN (LBRACKET 
  ((ps_par_def|simulate_par_def[map]|simulate_ode_par_def[map]|simulate_pla_par_def[map]|pscan_par_def[map])
  (COMMA (ps_par_def|simulate_par_def[map]|simulate_ode_par_def[map]|simulate_pla_par_def[map]|pscan_par_def[map]))*)? 
  RBRACKET)? RPAREN SEMI?
;

read_file
scope{
  Map<String,String> actions;
}
@init{
  $read_file::actions = new HashMap<String,String>();
}
: 
  READFILE LPAREN 
  (LBRACKET 
  (FILE ASSIGNS DBQUOTES s1=(filename) DBQUOTES {$read_file::actions.put($FILE.text,$s1.text);}) 
  RBRACKET) 
  RPAREN SEMI? 
  -> action(id={$READFILE.text},optionMap={$read_file::actions})
;

//write_file
//scope{
//  Map<String,String> actions;
//}
//@init{
//  $write_file::actions = new HashMap<String,String>();
//}
//: 
//  write_type
//  LPAREN (LBRACKET (write_par_def (COMMA write_par_def)*)? RBRACKET)?
//  RPAREN SEMI? 
//  -> action(id={$write_type.text})
//;

//write_type
//: 
//  WRITEFILE  |
//  WRITEMODEL |
//  WRITEXML   
//  WRITENET   | 
//  WRITESBML  | 
//;

write_model
scope{
  Map<String,String> actions;
}
@init{
  $write_model::actions = new HashMap<String,String>();
}
: 
  WRITEMODEL
  LPAREN 
  (LBRACKET 
  (write_model_args (COMMA write_model_args)*)? 
  RBRACKET)?
  RPAREN SEMI? 
  -> action(id={$WRITEMODEL.text})
;

write_model_args
: 
  ps_par_def |
  OVERWRITE ASSIGNS INT |
  PRETTY_FORMATTING ASSIGNS INT |
  EVALUATE_EXPRESSIONS ASSIGNS INT
;

write_xml
scope{
  Map<String,String> actions;
}
@init{
  $write_xml::actions = new HashMap<String,String>();
}
: 
  WRITEXML
  LPAREN 
  (LBRACKET 
  (write_xml_args (COMMA write_xml_args)*)? 
  RBRACKET)?
  RPAREN SEMI? 
  -> action(id={$WRITEXML.text})
;

write_xml_args
: 
  ps_par_def |
  OVERWRITE ASSIGNS INT
;

write_network
scope{
  Map<String,String> actions;
}
@init{
  $write_network::actions = new HashMap<String,String>();
}
: 
  WRITENETWORK
  LPAREN 
  (LBRACKET 
  (write_network_args (COMMA write_network_args)*)? 
  RBRACKET)?
  RPAREN SEMI? 
  -> action(id={$WRITENETWORK.text})
;

write_network_args
: 
  ps_par_def |
  OVERWRITE ASSIGNS INT |
  INCLUDE_MODEL ASSIGNS INT |
  PRETTY_FORMATTING ASSIGNS INT |
  EVALUATE_EXPRESSIONS ASSIGNS INT |
  TEXTREACTION ASSIGNS INT |
  TEXTSPECIES ASSIGNS INT
;

write_sbml
scope{
  Map<String,String> actions;
}
@init{
  $write_sbml::actions = new HashMap<String,String>();
}
: 
  WRITESBML
  LPAREN 
  (LBRACKET 
  (write_sbml_args (COMMA write_sbml_args)*)?
  RBRACKET)?
  RPAREN SEMI? 
  -> action(id={$WRITESBML.text})
;

write_sbml_args
:
  ps_par_def
;

write_mfile
scope{
  Map<String,String> actions;
}
@init{
  $write_mfile::actions = new HashMap<String,String>();
}
: 
  WRITEMFILE
  LPAREN 
  (LBRACKET 
  (write_mfile_args (COMMA write_mfile_args)*)? 
  RBRACKET)?
  RPAREN SEMI? 
  -> action(id={$WRITEMFILE.text})
;

write_mfile_args
:
  ps_par_def |
  T_START ASSIGNS (INT|FLOAT) |
  T_END ASSIGNS (INT|FLOAT) |
  N_STEPS ASSIGNS (INT|FLOAT) | 
  ATOL ASSIGNS FLOAT | 
  RTOL ASSIGNS FLOAT | 
  MAX_STEP ASSIGNS (INT|FLOAT) |
  BDF ASSIGNS INT |
  MAXORDER ASSIGNS INT |
  STATS ASSIGNS INT
;

write_mexfile
scope{
  Map<String,String> actions;
}
@init{
  $write_mexfile::actions = new HashMap<String,String>();
}
: 
  WRITEMEXFILE
  LPAREN 
  (LBRACKET 
  (write_mexfile_args (COMMA write_mexfile_args)*)? 
  RBRACKET)?
  RPAREN SEMI? 
  -> action(id={$WRITEMEXFILE.text})
;

write_mexfile_args
:
  ps_par_def |
  T_START ASSIGNS (INT|FLOAT) |
  T_END ASSIGNS (INT|FLOAT) |
  N_STEPS ASSIGNS (INT|FLOAT) | 
  ATOL ASSIGNS FLOAT | 
  RTOL ASSIGNS FLOAT | 
  MAX_STEP ASSIGNS (INT|FLOAT) |
  MAX_NUM_STEPS ASSIGNS (INT|FLOAT) |
  MAX_ERR_TEST_FAILS ASSIGNS INT |
  MAX_CONV_FAILS ASSIGNS INT |
  STIFF ASSIGNS INT |
  SPARSE ASSIGNS INT
;

write_mdl
scope{
  Map<String,String> actions;
}
@init{
  $write_mdl::actions = new HashMap<String,String>();
}
: 
  WRITEMDL
  LPAREN 
  (LBRACKET 
  (write_mdl_args (COMMA write_mdl_args)*)?
  RBRACKET)?
  RPAREN SEMI? 
  -> action(id={$WRITEMDL.text})
;

write_mdl_args
:
  ps_par_def
;

//write_m_file[Map<String,String> map]
//: 
//  WRITEMFILE LPAREN (LBRACKET
//  ((write_m_par_def[map]) 
//  (COMMA write_m_par_def[map])*)?
//  RBRACKET)? RPAREN SEMI?
//;
//
//write_mex_file[Map<String,String> map]
//: 
//  WRITEMEXFILE LPAREN LBRACKET
//  ((ps_par_def|write_m_par_def[map]) 
//  (COMMA ps_par_def|write_m_par_def[map])*)?
//  RBRACKET RPAREN SEMI?
//;
//
//write_m_par_def[Map<String,String> map]
//: 
//  ATOL ASSIGNS f1=FLOAT {map.put($ATOL.text,$f1.text);} | 
//  RTOL ASSIGNS f2=FLOAT {map.put($RTOL.text,$f2.text);} | 
//  T_END ASSIGNS (i1=INT {map.put($T_END.text,$i1.text);}|f1=FLOAT {map.put($T_END.text,$f1.text);})  | 
//  T_START ASSIGNS (i2=INT {map.put($T_START.text,$i2.text);}|f2=FLOAT {map.put($T_START.text,$f2.text);}) | 
//  N_STEPS ASSIGNS i3=INT {map.put($N_STEPS.text,$i3.text);} | 
//  SPARSE ASSIGNS i4=INT {map.put($SPARSE.text,$i4.text);} | 
//  BDF ASSIGNS i5=INT {map.put($BDF.text,$i5.text);}
//;

//write_network[Map<String,String> map]
//: 
//  WRITENETWORK LPAREN (LBRACKET 
//  (
//  (ps_par_def|simulate_par_def[map] |
//    s1=TEXTREACTION ASSIGNS i1=INT {map.put($s1.text,$i1.text);} |
//    s2=TEXTSPECIES ASSIGNS  i2=INT {map.put($s2.text, $i2.text);})
//  (COMMA (ps_par_def|simulate_par_def[map] |
//    s3=TEXTREACTION ASSIGNS i3=INT {map.put($s3.text,$i3.text);} |
//    s4=TEXTSPECIES ASSIGNS  i4=INT {map.put($s4.text, $i4.text);}))
//  *)?
//  RBRACKET)? RPAREN SEMI?
//;

set_concentration 
scope{
  Map<String,String> options;
}
@init{
  $set_concentration::options = new HashMap<String,String>();
}
: 
  SETCONCENTRATION LPAREN 
  variable_definition 
  {
    $set_concentration::options.put($variable_definition.variableName,$variable_definition.variableValue);
  }
  RPAREN SEMI? 
  -> action(id={$SETCONCENTRATION.text},optionMap={$set_concentration::options})
; 
        
add_concentration 
scope{
  Map<String,String> options;
}
@init{
  $add_concentration::options = new HashMap<String,String>();
}
: 
  ADDCONCENTRATION LPAREN 
  variable_definition 
  {
    $add_concentration::options.put($variable_definition.variableName,$variable_definition.variableValue);
  }
  RPAREN SEMI? 
  -> action(id={$ADDCONCENTRATION.text},optionMap={$add_concentration::options})
; 
        
save_concentrations
: 
  SAVECONCENTRATIONS LPAREN (DBQUOTES STRING DBQUOTES)? RPAREN SEMI? 
  -> action(id={$SAVECONCENTRATIONS.text})
;

reset_concentrations
: 
  RESETCONCENTRATIONS LPAREN (DBQUOTES STRING DBQUOTES)? RPAREN SEMI? 
  -> action(id={$RESETCONCENTRATIONS.text})
;
        
set_parameter
scope{
  Map<String,String> options;
}
@init{
  $set_parameter::options = new HashMap<String,String>();
}
:
  SETPARAMETER LPAREN 
  parameter_definition 
  {
    $set_parameter::options.put($parameter_definition.parameterName,$parameter_definition.parameterValue);
  }
  RPAREN SEMI? 
  -> action(id={$SETPARAMETER.text},optionMap={$set_parameter::options})
;

save_parameters
: 
  SAVEPARAMETERS LPAREN (DBQUOTES STRING DBQUOTES)? RPAREN SEMI? 
  -> action(id={$SAVEPARAMETERS.text})
;

reset_parameters
: 
  RESETPARAMETERS LPAREN (DBQUOTES STRING DBQUOTES)? RPAREN SEMI? 
  -> action(id={$RESETPARAMETERS.text})
;

quit
:
  QUIT LPAREN RPAREN SEMI?
  -> action(id={$QUIT.text})
;

parameter_definition returns [String parameterName,String parameterValue]
:
  DBQUOTES STRING {$parameterName = $STRING.text;} DBQUOTES 
  COMMA 
  (e1=expression[gParent.memory] {$parameterValue = $e1.text;} | 
  (DBQUOTES e2=expression[gParent.memory] DBQUOTES {$parameterValue = $e2.text;}))
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
  species_def[$variable_definition::reactants,$variable_definition::bonds,""] 
  {
    $variableName = $species_def.text;
  } 
  DBQUOTES COMMA 
  (e1=expression[gParent.memory] {$variableValue = $e1.text; } | 
  (DBQUOTES e2=expression[gParent.memory] DBQUOTES {$variableValue = $e2.text; }))
;

hash_value
//: STRING ASSIGNS LBRACKET assignment_list RBRACKET
: 
  LBRACKET assignment_list RBRACKET
;

assignment_list
: 
  STRING ASSIGNS value (COMMA STRING ASSIGNS value)
;

value   
: 
  INT | FLOAT | STRING
;

//ps_par_def [Map<String,String> map]
ps_par_def
: 
  PREFIX ASSIGNS ((DBQUOTES  ~(DBQUOTES )* DBQUOTES)) | 
  SUFFIX ASSIGNS ((DBQUOTES  ~(DBQUOTES )* DBQUOTES))
;

simulate_ode_par_def[Map<String,String> map]
: 
  ATOL ASSIGNS f1=FLOAT {map.put($ATOL.text,$f1.text);} | 
  RTOL ASSIGNS f2=FLOAT {map.put($RTOL.text,$f2.text);} | 
  STEADY_STATE ASSIGNS i1=INT {map.put($STEADY_STATE.text,$i1.text);} | 
  SPARSE ASSIGNS i2=INT {map.put($SPARSE.text,$i2.text);}
;
        
simulate_pla_par_def[Map<String,String> map]
scope{
  String temp;
}
@init{
  $simulate_pla_par_def::temp = "";
}
: 
  PLA_CONFIG ASSIGNS 
  DBQUOTES (s1=~(DBQUOTES ){$simulate_pla_par_def::temp += $s1.text;})* 
  DBQUOTES {map.put($PLA_CONFIG.text,$simulate_pla_par_def::temp);} |
  PLA_OUTPUT ASSIGNS i1=INT {map.put($PLA_OUTPUT.text,$i1.text);}
;
 
simulate_nf_par_def[Map<String,String> map]
: 
  PARAM ASSIGNS DBQUOTES (MINUS s1=(STRING|NOCSLF|NOTF|UTL|GML|SEED) s2=(STRING|INT|DOUBLE)?)* DBQUOTES {map.put($PARAM.text,"-" + $s1.text + " " + $s2.text);} | 
  COMPLEX ASSIGNS i1=INT {map.put($COMPLEX.text,$i1.text);} | 
  GET_FINAL_STATE ASSIGNS i2=INT {map.put($GET_FINAL_STATE.text,$i2.text);} | 
  GML ASSIGNS i3=INT {$map.put($GML.text,$i3.text);} | 
  NOCSLF ASSIGNS i4=INT {$map.put($NOCSLF.text,$i4.text);} | 
  NOTF ASSIGNS i5=INT {$map.put($NOTF.text,$i5.text);} | 
  BINARY_OUTPUT ASSIGNS i6=INT {$map.put($BINARY_OUTPUT.text,$i6.text);} | 
  UTL ASSIGNS i7=INT {$map.put($UTL.text,$i7.text);} | 
  EQUIL ASSIGNS i8=(FLOAT|INT) {$map.put($EQUIL.text,$i8.text);}
;    
        
//write_par_def
//:
//  SUFFIX ASSIGNS DBQUOTES ~(DBQUOTES )* DBQUOTES
//;

//TODO: error or warning?????
simulate_par_def[Map<String,String> map]
: 
  METHOD ASSIGNS DBQUOTES method_definition DBQUOTES {map.put($METHOD.text,$method_definition.text);} |
  T_END ASSIGNS (i1=INT {map.put($T_END.text,$i1.text);}|f1=FLOAT {map.put($T_END.text,$f1.text);})  | 
  T_START ASSIGNS (i2=INT {map.put($T_START.text,$i2.text);}|f2=FLOAT {map.put($T_START.text,$f2.text);}) | 
  N_STEPS ASSIGNS i3=(INT|FLOAT) {map.put($N_STEPS.text,$i3.text);} | 
  N_OUTPUT_STEPS ASSIGNS multiple_definition {map.put($N_OUTPUT_STEPS.text,$multiple_definition.value);} | 
  SAMPLE_TIMES ASSIGNS i4=array_value | 
  VERBOSE ASSIGNS i5=INT {map.put($VERBOSE.text,$i5.text);} | 
  PRINT_CDAT ASSIGNS i6=INT {map.put($PRINT_CDAT.text,$i6.text);} | 
  PRINT_FUNCTIONS ASSIGNS i7=INT {map.put($PRINT_FUNCTIONS.text,$i7.text);} | 
  MAX_SIM_STEPS ASSIGNS multiple_definition {map.put($MAX_SIM_STEPS.text,$multiple_definition.value);} | 
  ARGFILE ASSIGNS DBQUOTES s3=filename DBQUOTES {map.put($ARGFILE.text,$s3.text);} | 
  SAVE_PROGRESS ASSIGNS i10=INT {map.put($SAVE_PROGRESS.text,$i10.text);} | 
  PRINT_NET ASSIGNS i11=INT {map.put($PRINT_NET.text,$i11.text);} | 
  PRINT_END ASSIGNS i12=INT {map.put($PRINT_END.text,$i12.text);} | 
  NETFILE ASSIGNS DBQUOTES s4=filename DBQUOTES {map.put($NETFILE.text,$s4.text);} | 
  CONTINUE ASSIGNS i13=INT {map.put($CONTINUE.text,$i13.text);} | 
  EVALUATE_EXPRESSIONS ASSIGNS i14=INT {map.put($EVALUATE_EXPRESSIONS.text,$i14.text);} | 
  OVERWRITE ASSIGNS i15=INT {map.put($OVERWRITE.text,$i15.text);} | 
  SEED ASSIGNS i16=INT {map.put($SEED.text,$i16.text);} |
  STOP_IF ASSIGNS DBQUOTES e1=expression[gParent.memory] DBQUOTES {map.put($STOP_IF.text,$e1.text);} |
  PRINT_ON_STOP ASSIGNS i17=INT {map.put($PRINT_ON_STOP.text,$i17.text);}
;

pscan_par_def[Map<String,String> map]
:
  PARAMETER ASSIGNS DBQUOTES STRING DBQUOTES {map.put($PARAMETER.text,$STRING.text);} |
  PAR_MIN ASSIGNS i1=(INT|FLOAT) {map.put($PAR_MIN.text,$i1.text);} |
  PAR_MAX ASSIGNS i2=(INT|FLOAT) {map.put($PAR_MAX.text,$i2.text);} |
  N_SCAN_PTS ASSIGNS i3=(INT|FLOAT) {map.put($N_SCAN_PTS.text,$i3.text);} |
  LOG_SCALE ASSIGNS i4=INT {map.put($LOG_SCALE.text,$i4.text);}
;

multiple_definition returns [String value]
:
  INT {$value=$INT.text;} | 
  FLOAT {$value=$FLOAT.text;} | 
  STRING 
  {
    if($STRING.text.equals("INT_MAX") || $STRING.text.equals("FLOAT_MAX"))
      $value=$STRING.text;
    else{
      String err = String.format("\%s line \%d:\%d \%s\n",input.getSourceName(),$STRING.getLine(),$STRING.getCharPositionInLine(),"Input should be an INT,FLOAT, 'FLOAT_MAX' or 'INT_MAX'");
    } 
  }
;
     
method_definition 
:
  ODE|SSA|PLA
;

array_value
: 
  LSBRACKET STRING (COMMA STRING)* RSBRACKET
;

filename
:
  (STRING|DOT|DIV|MINUS|FLOAT)+
;
