parser grammar BNGGrammar;

//bngl grammar. prog is the entry point for the grammar.
options {
  language = Java;
  output = template;
  tokenVocab = BNGLexer;
  
}

import BNGGrammar_Expression,BNGGrammar_Parameters,BNGGrammar_SeedSpecies,BNGGrammar_MoleculeDef,BNGGrammar_Observables,BNGGrammar_ReactionRules,BNGGrammar_Actions;
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
  import bngparser.exceptions.VariableNotFoundException;
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
scope {
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
@init {
  $prog::parameters = new ArrayList();
  $prog::seedSpecies = new ArrayList();
  $prog::reactionRules = new ArrayList();
  $prog::observables = new ArrayList();
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
((BEGIN MODEL LB+ (program_block)* END MODEL LB+) | (program_block)*)
(a1=actions_block {$actionsTemplate = $a1.st;}| (BEGIN ACTIONS a2=actions_block END ACTIONS {$actionsTemplate = $a2.st;;}) )? 
EOF  -> prog2(parameters={$prog::parameters},molecules={molecules},species={$prog::seedSpecies},reactions={$prog::reactionRules},
                            observables={$prog::observables},functions={$prog::functions}, compartments={$prog::compartments});

version_def: VERSION LPAREN DBQUOTES VERSION_NUMBER DBQUOTES RPAREN SEMI;

// a list of the different sections a bngl file may have. Order is not enforced.
program_block
: parameters_block[memory,$prog::parameters]
| molecule_types_block
| seed_species_block[$prog::seedSpecies]
| observables_block[$prog::observables]
| reaction_rules_block[$prog::reactionRules]
| functions_block
| compartments_block
//| set_option
;

//Note: The grammars for all sections except functions and compartments are separated in their own files

functions_block
@init{
  paraphrases.push("in function block");
}
@after{
  paraphrases.pop();
}
:
      BEGIN FUNCTIONS LB+
       (function_def {$prog::functions.add($function_def.st);} LB+)*     
      END FUNCTIONS LB+
;

function_def:
    s1=STRING LPAREN 
    parameter=(s2=STRING
    {
      memory.put($s2.text,new Register(1.0,"parameter"));
    }
    )? RPAREN (BECOMES)?  

    (expression[memory] 
    {
     memory.put($s1.text,new Register($expression.value,"function"));
    }
    ->  functions_block(id={$s1.text},
                            referencesName={$expression.reference.keySet()},referencesType={Register.getTypes($expression.reference)},
                            expression={$expression.text})
    | if_expression
    {
      memory.put($s1.text,new Register($if_expression.value,"function"));
    }
    )
    {
      if(memory.containsKey($s2.text)){
        memory.remove($s2.text);
        
      }
    }
    
;

if_expression  returns [Double value]
scope{
  Map<String,Register> references;
  Map<String,Register> lmemory;
}
@init{
  $if_expression::references = new HashMap<String,Register>();
  $if_expression::lmemory = memory;
}:
  IF LPAREN STRING 
  
  (EQUALS|GT|GTE|LT|LTE) (STRING|INT) COMMA e1=expression[memory] COMMA expression[memory] RPAREN
  {
    $value = $e1.value;
  }
;

//http://bionetgen.org/index.php/Compartments_in_BNGL
compartments_block:

  BEGIN COMPARTMENTS
      (compartment {$prog::compartments.add($compartment.st);})*
  END COMPARTMENTS;
  
compartment:
   s1=STRING INT s3=expression[memory] (s2=STRING)? -> compartments_block(id={$s1.text},dimensions={$INT.text},size={$s3.value},outside={$s2.text}) 
;
  
 
label returns [String label]:
  MOD STRING{$label = $STRING.text;};
  
dereference returns [String label]:
  AMPERSAND STRING{$label = $STRING.text;};  
  
  
