parser grammar NETGrammar;

options {
  language = Java;
  output = template;
  tokenVocab = BNGLexer;
  
}
import BNGGrammar_Expression,BNGGrammar_Parameters,BNGGrammar_SeedSpecies,BNGGrammar_MoleculeDef,BNGGrammar_Observables,BNGGrammar_ReactionRules;
@header{
  package bngparser.netGrammar;
  import bngparser.grammars.BNGLexer;
  import org.antlr.stringtemplate.*;
  import bngparser.dataType.BondList;
  import java.util.Map;
  import bngparser.methods.ReactionAction;
  import bngparser.dataType.Register;
  import bngparser.methods.InvertBidirectional;
  import bngparser.dataType.ChangeableChannelTokenStream;
  import bngparser.dataType.ReactionRegister;
  import bngparser.methods.GenericMethods;
}
@members{
  public Stack<String> paraphrases = new Stack<String>();
  public Map<String,Register> memory = new HashMap<String,Register>();
  
  private Map<String, Map<String,String>> options = new HashMap<String, Map<String,String>>();

  public boolean netGrammar = true;
 
  public List<StringTemplate> molecules = new ArrayList<StringTemplate>();
  public List<StringTemplate> reactions = new ArrayList<StringTemplate>();
  public List<StringTemplate> groups = new ArrayList<StringTemplate>();
  public Map<String,String> speciesMap = new HashMap<String,String>();
  
  @Override
  public String getErrorMessage(RecognitionException e,String[] tokenNames){
    String msg = super.getErrorMessage(e,tokenNames);
    if(paraphrases.size(  )>0) {
      String paraphrase = paraphrases.peek();
      msg += " " + paraphrase;
    }
    return msg;
  }
  
  public void addOption(String element, String option, String setting){
    if(!options.containsKey(element)){
      options.put(element,new HashMap<String,String>());
    }
    
    (options.get(element)).put(option,setting);
  }
  
  public void addOptions(String element, Map<String,String> option){
    if(!options.containsKey(element)){
      options.put(element,option);
    }
    else{
      options.get(element).putAll(option);
    }
  }
  
}



seed_species_def[int counter]
scope{
  List molecules;
  BondList bonds;
}
@init{
  $seed_species_def::molecules = new ArrayList();
  $seed_species_def::bonds = new BondList();
}

:
 pre_species_def[$seed_species_def::molecules,$seed_species_def::bonds,counter] {
 //There needs to need a space between species and the expression token, so we go back and make sure there was one
((ChangeableChannelTokenStream)input).seek(((ChangeableChannelTokenStream)input).index()-1)  ;
// System.out.println(input);
} 
      WS 
     expression[memory] 
     
     -> 
    seed_species_block(id={counter},concentration={$expression.text},name={$pre_species_def.text},molecules={$seed_species_def::molecules},
      firstBonds={$seed_species_def::bonds.getLeft()},secondBonds={$seed_species_def::bonds.getRight()});



pre_species_def[List molecules,BondList bonds, int speciesCounter] returns [String compartment]
scope{
  boolean constant;
  
}

:
INT  

 species_def[molecules,bonds,"S" + speciesCounter]
 {
    $pre_species_def::constant = $species_def.constant;
     speciesMap.put($species_def.text,$INT.text);
 }
 ;
  
  
        
groups_block:
BEGIN GROUPS
  group_line*
END GROUPS

;

group_line:
INT STRING group_list[$STRING.text]
;

group_list[String name]
scope{
  int counter;
}
@init{
  $group_list::counter = 0;
}:
  g1=group_element[$name,$group_list::counter] {groups.add($g1.st);$group_list::counter++;}
  (COMMA g2=group_element[$name,$group_list::counter] {groups.add($g2.st);$group_list::counter++;})*
  ;
  
group_element[String name,int counter]:
  i1=INT -> groups_block(id = {$i1.text},name={$name + "_" + counter}) 
  | (i2=INT TIMES i3=INT) -> groups_block(id = {$i3.text},name={$name + "_" + counter});
  


prog
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
SUBSTANCEUNITS LPAREN DBQUOTES STRING DBQUOTES RPAREN SEMI
(program_block)* EOF 


 -> prog2(parameters={$prog::parameters},molecules={speciesMap},species={$prog::seedSpecies},reactions={reactions},
                            observables={groups},functions={$prog::functions}, compartments={$prog::compartments});

version_def: VERSION LPAREN DBQUOTES VERSION_NUMBER DBQUOTES RPAREN SEMI;

//program_body:  ;

program_block
: parameters_block[memory,$prog::parameters]
| molecule_types_block
| seed_species_block[$prog::seedSpecies]
| observables_block[$prog::observables]
| reaction_rules_block[$prog::reactionRules]
| functions_block
| compartments_block
| reactions_block[reactions]
| groups_block
//| set_option
;


functions_block
@init{
  paraphrases.push("in function block");
}
@after{
  paraphrases.pop();
}
:
      BEGIN FUNCTIONS 
       (function_def {$prog::functions.add($function_def.st);})*     
      END FUNCTIONS
;

function_def:
    s1=STRING LPAREN (STRING)? RPAREN (BECOMES)?  expression[memory] ->  functions_block(id={$s1.text},
                            referencesName={$expression.reference.keySet()},referencesType={Register.getTypes($expression.reference)},expression={$expression.text})
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
  
  

 reactions_block[List<StringTemplate> templates]:
BEGIN REACTIONS
  (INT reaction
  {
    $templates.add($reaction.st);
  })*
END REACTIONS
;

reaction
scope{
  List<String> reactant;
  List<String> product;
}
@init{
  $reaction::reactant = new ArrayList<String>();
  $reaction::product = new ArrayList<String>();
}
:
 reactant[$reaction::reactant] reactant[$reaction::product] expression[memory]
 -> reactions(reactant={$reaction::reactant},product={$reaction::product}, rate={$expression.text})
;

reactant[List<String> elements]:

  i1=INT
  {
    $elements.add($i1.text);
  }
   
  (COMMA i2=INT
  {
    $elements.add($i2.text);
  }
  )*
;

