parser grammar BNGGrammar;

//bngl grammar. prog is the entry point for the grammar.
options{
  language = Java;
  output = template;
  tokenVocab = BNGLexer;
}
import BNGGrammar_Expression,
       BNGGrammar_Parameters,
       BNGGrammar_SeedSpecies,
       BNGGrammar_MoleculeDef,
       BNGGrammar_Observables,
       BNGGrammar_ReactionRules,
       BNGGrammar_Actions;
@header{
  package bngparser.grammars;
  import org.antlr.stringtemplate.*;
  import bngparser.dataType.BondList;
  import java.util.Map;
  import bngparser.methods.ReactionAction;
  import bngparser.dataType.Register;
  import bngparser.methods.InvertBidirectional;
  import bngparser.dataType.ChangeableChannelTokenStream;
  import bngparser.dataType.ReactionRegister;
  import bngparser.methods.GenericMethods;
  import java.util.Arrays;
  import org.apache.commons.lang3.StringEscapeUtils;
}
@members{
  public Stack<String> paraphrases = new Stack<String>();
  public Map<String,Register> memory = new HashMap<String,Register>();
  public boolean netGrammar = false;
  public List<String> moleculeName;
  public List<String> moleculeNumber;
  public List<String> variableName = new ArrayList<String>();
  public boolean conserveNewMolecules = true;
  public List<StringTemplate> molecules = new ArrayList<StringTemplate>();
  @Override
  public String getErrorMessage(RecognitionException e,String[] tokenNames){
    String msg = super.getErrorMessage(e,tokenNames);
    if(paraphrases.size(  )>0) {
      String paraphrase = paraphrases.peek();
      msg += " " + paraphrase;
    }
    return msg;
  }
}

//prog separates bngl into methods and actions
prog returns [StringTemplate actionsTemplate]
scope{
  List parameters;
  //List molecules;
  List seedSpecies;
  List observables;
  List reactionRules;
  List functions;
  List compartments;
  // Map<String,Register> memory;
  Stack elements;
}
@init{
  $prog::parameters = new ArrayList();
  $prog::seedSpecies = new ArrayList();
  $prog::observables = new ArrayList();
  $prog::reactionRules = new ArrayList();
  $prog::functions = new ArrayList();
  $prog::compartments = new ArrayList();
  // memory = new HashMap<String,Register>();
  $prog::elements = new Stack();
  paraphrases.push("in model");
}
@after{
  paraphrases.pop();
}
:
  LB*
  header_block*
  ((BEGIN MODEL LB+ (program_block)* END MODEL LB*) | (program_block)*)
  (a1=actions_block {$actionsTemplate = $a1.st;})? 
  EOF  
  -> prog2(parameters={$prog::parameters},
           molecules={molecules},
           species={$prog::seedSpecies},
           reactions={$prog::reactionRules},
           observables={$prog::observables},
           functions={$prog::functions}, 
           compartments={$prog::compartments})
;

header_block
: 
  version_def | 
  substance_def | 
  set_option |
  set_model_name
;

version_def
: 
//  VERSION LPAREN DBQUOTES VERSION_NUMBER (PLUS|MINUS)? STRING? DBQUOTES RPAREN (SEMI+)? LB+
  VERSION LPAREN DBQUOTES VERSION_NUMBER STRING? DBQUOTES RPAREN (SEMI+)? LB+
;

substance_def
: 
  SUBSTANCEUNITS LPAREN DBQUOTES STRING DBQUOTES RPAREN (SEMI+)? LB+
;

set_option
: 
  SET_OPTION LPAREN 
  DBQUOTES STRING DBQUOTES COMMA (DBQUOTES STRING DBQUOTES|INT|FLOAT)
  (COMMA DBQUOTES STRING DBQUOTES COMMA (DBQUOTES STRING DBQUOTES|INT|FLOAT))*
  RPAREN (SEMI+)? LB+
;

set_model_name
:
  SET_MODEL_NAME LPAREN DBQUOTES STRING DBQUOTES RPAREN (SEMI+)? LB+
;

// a list of the different sections a bngl file may have. Order is not enforced.
program_block
: 
  read_file |
  parameters_block[memory,$prog::parameters] | 
  molecule_types_block | 
  seed_species_block[$prog::seedSpecies] | 
  observables_block[$prog::observables] | 
  reaction_rules_block[$prog::reactionRules] | 
  functions_block | 
  population_maps_block | 
  compartments_block |
  energy_patterns_block
;

read_file
: 
  READFILE LPAREN LBRACKET 
    ((ATOMIZE ASSIGNS INT | BLOCKS ASSIGNS bng_blocks_list) COMMA)* 
    FILE ASSIGNS DBQUOTES filename DBQUOTES
    (COMMA (ATOMIZE ASSIGNS INT | BLOCKS ASSIGNS bng_blocks_list))*
  RBRACKET RPAREN SEMI?
  (LB+ | EOF)
;

filename
:
//  (STRING|DOT|DIV|MINUS|FLOAT)+
  (~DBQUOTES)+
;

bng_blocks_list
:
  LSBRACKET (DBQUOTES bng_blocks DBQUOTES (COMMA DBQUOTES bng_blocks DBQUOTES)*) RSBRACKET
;

bng_blocks
:
  PARAMETERS |
  COMPARTMENTS |
  MOLECULE TYPES |
  SPECIES | 
  SEED SPECIES |
  OBSERVABLES |
  FUNCTIONS |
  ENERGY PATTERNS |
  POPULATION TYPES |
  POPULATION MAPS |
  REACTION RULES |
  REACTIONS |
  GROUPS |
  ACTIONS
;

//Note: The grammars for all sections except functions and compartments are separated in their own files

functions_block
@init{
  paraphrases.push("in functions block");
}
@after{
  paraphrases.pop();
}
:
  BEGIN FUNCTIONS LB+
    (function_def {$prog::functions.add($function_def.st);} LB+)*     
  END FUNCTIONS LB*
;

function_def
scope{
  Map<String,Register> lmemory;
}
@init{
  $function_def::lmemory =  new HashMap<String,Register>();
  $function_def::lmemory.putAll(memory);
}
:
//  ((STRING | INT) COLON)?
  (STRING COLON)?
  s1=STRING LPAREN 
  parameter=(s2=STRING
  {
    $function_def::lmemory.put($s2.text,new Register(1.0,"ConstantExpression"));
  }  
  (COMMA s3=STRING)*
  {
    $function_def::lmemory.put($s3.text,new Register(1.0,"ConstantExpression"));
  }
  )? RPAREN (BECOMES)?  
  expression[$function_def::lmemory] 
  {
    memory.put($s1.text,new Register($expression.value,"function"));
  }
  -> functions_block(id={$s1.text},
                     referencesName={$expression.reference.keySet()},
                     referencesType={Register.getTypes($expression.reference)},
                     expression={StringEscapeUtils.escapeXml($expression.text)})
;

//http://bionetgen.org/index.php/Compartments_in_BNGL
compartments_block
:
  BEGIN COMPARTMENTS LB+
    (compartment LB+{$prog::compartments.add($compartment.st);})*
  END COMPARTMENTS LB+
;
  
compartment
:
  (STRING COLON)?
  s1=STRING INT s3=expression[memory] (s2=STRING)? 
  -> compartments_block(id={$s1.text},
                        dimensions={$INT.text},
                        size={$s3.value},
                        outside={$s2.text}) 
;

energy_patterns_block
scope{
  int patternCounter;
}
@init{
  $energy_patterns_block::patternCounter = 1;
}
:
  BEGIN ENERGY PATTERNS LB+
    (
    energy_species_def[$energy_patterns_block::patternCounter]
    {
      $energy_patterns_block::patternCounter++;
    } 
    expression[memory] 
    LB+)*
  END ENERGY PATTERNS LB+
;

energy_species_def[int counter]
scope
{
  List molecules;
  BondList bonds;
}
@init{
  $energy_species_def::molecules = new ArrayList();
  $energy_species_def::bonds = new BondList();
}
:
    (STRING COLON)? 
    species_def[$energy_species_def::molecules,
                $energy_species_def::bonds,
                "E" + counter]
;

label returns [String label]
:
//  MOD l1=(STRING | INT) {$label = $l1.text;}
  MOD STRING {$label = $STRING.text;}
;

dereference returns [String label]
:
  AMPERSAND STRING{$label = $STRING.text;}
;
