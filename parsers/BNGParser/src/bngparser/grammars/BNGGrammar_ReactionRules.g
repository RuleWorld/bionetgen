parser grammar BNGGrammar_ReactionRules;

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
  
 
  public String getErrorMessage2(Token s1,String error){
      return String.format("\%s line \%d:\%d \%s: \%s\n",input.getSourceName(),s1.getLine(),s1.getCharPositionInLine(),error,s1.getText());
  
  }
  

}


population_maps_block:
BEGIN POPULATION MAPS LB+
  (reaction_rule_def[1] LB+)* 
END POPULATION MAPS LB+
;





reaction_rules_block[List reactionRules]
scope{
  int reactionCounter;
  
 
}
@init{
getParentTemplate();
  $reaction_rules_block::reactionCounter = 1;
  gParent.paraphrases.push("in reaction rules block"); 
}
@after{
  gParent.paraphrases.pop();
}
        : BEGIN REACTION RULES LB+(reaction_rule_def[$reaction_rules_block::reactionCounter] 
                  {
                  reactionRules.add($reaction_rule_def.st);
                  StringTemplate sInvert = null;
                  //TODO: crashes when handling an error in a bidirectional reaction
                  if($reaction_rule_def.numReactions == 2)
                    sInvert = InvertBidirectional.invert($reaction_rule_def.st.toString(),$reaction_rules_block::reactionCounter+1);
                  reactionRules.add(sInvert);
                  $reaction_rules_block::reactionCounter+= $reaction_rule_def.numReactions;
                  
                  }LB+)* END REACTION RULES LB*
        ;



reactionLabel returns [String label]
@init{
  $label = "";
  }
:
(STRING {$label += $STRING.text + " ";}
          |INT{$label += $INT.text + " ";})+
        COLON 

;

reactionReference:
  STRING COLON COLON
;

reaction_rule_def[int reactionCounter] returns [int numReactions, String secondRate]
scope{
List patternsReactants;
List patternsProducts;
List<String> rateList;
ReactionAction reactionAction;
String name;
 String text;
 Map<String,Register> lmemory;
}
@init{
  $reaction_rule_def::patternsReactants = new ArrayList();
  $reaction_rule_def::patternsProducts = new ArrayList();
  $reaction_rule_def::rateList = new ArrayList<String>();
  $reaction_rule_def::reactionAction = new ReactionAction();
  $reaction_rule_def::name = "Rule" + reactionCounter;
  $reaction_rule_def::text = "";
  $reaction_rule_def::lmemory = new HashMap<String,Register>();
  
}
        :  
        (l1=reactionLabel{$reaction_rule_def::text += $l1.label + ":"; 
	                         $reaction_rule_def::name = $l1.label;
	                  })?

        reaction_def[$reaction_rule_def::patternsReactants,$reaction_rule_def::patternsProducts,"RR" + reactionCounter]{
          $reaction_rule_def::reactionAction.execute();
          if($reaction_def.bidirectional)
              $numReactions = 2;
           else
              $numReactions = 1;
           
          $reaction_rule_def::text += $reaction_def.text;
        }
        
        {
        //Whitespaces are normally skipped but they are still in the stream. In this case if this rule is valid
        //a valid WS would be located on the previous token
        //((ChangeableChannelTokenStream)input).seek(((ChangeableChannelTokenStream)input).index()-1);
        } 
        //WS
        
      //  (rate_function modif_command* DELETEMOLECULES? MOVECONNECTED? LB) => 
        bi=rate_list[$reaction_rule_def::rateList,$reaction_def.bidirectional] {
        //TODO: add a try catch exception to check that if a bidirectional reaction is required it asks for two reaction rates
            if($numReactions == 2 && $reaction_rule_def::rateList.size() > 1)
                $secondRate=$reaction_rule_def::rateList.get(1);
            else
                $secondRate="0";
            $reaction_rule_def::text += " " + $rate_list.text;
        }
        (modif_command)* (DELETEMOLECULES)? (MOVECONNECTED)?
        
        {
          $reaction_rule_def::text = $reaction_rule_def::text.replaceAll("<","&lt;");
          $reaction_rule_def::text = $reaction_rule_def::text.replaceAll(">","&gt;");
          $reaction_rule_def::text = $reaction_rule_def::text.replaceAll("\\$","&#36;");
        }

        -> reaction_block(id={"RR" + reactionCounter},reactant={$reaction_rule_def::patternsReactants},
        product={$reaction_rule_def::patternsProducts},name={$reaction_rule_def::name},
        rate={$reaction_rule_def::rateList},bidirectional={bi},
        //lawType={$rate_function.functionName},
        leftMap={$reaction_rule_def::reactionAction.getLeft()},
        rightMap={$reaction_rule_def::reactionAction.getRight()},operations={$reaction_rule_def::reactionAction.getOperations()},
        operator1={$reaction_rule_def::reactionAction.getOperator1()},
        operator2={$reaction_rule_def::reactionAction.getOperator2()},
        expression={$reaction_rule_def::text}
        )
        ;
        
match_attribute
        : LBRACKET MATCHONCE RBRACKET
        ;
        
reaction_def[List patternsReactants,List patternsProducts,String upperID] returns [boolean bidirectional, Map<String,List<ReactionRegister>> reactionStack,
Map<String,List<ReactionRegister>> productStack]
scope{
int reactantPatternCounter;
//Map<String,List<ReactionRegister>> reactionStack;
//Map<String,List<ReactionRegister>> productStack;
}
@init{
  $reaction_def::reactantPatternCounter =1;
  $reactionStack = new HashMap<String,List<ReactionRegister>>();
  $productStack = new HashMap<String,List<ReactionRegister>>();
}
:
  s1=rule_species_def[upperID+"_RP" + $reaction_def::reactantPatternCounter,$reaction_rule_def::reactionAction] 
           //Add as many chemicals as the stoichiometry tells us. We also have to modify some of the internal tags
           //(more specifically all the RP's and PP's
           {
            int counter = $reaction_def::reactantPatternCounter;
            for(int i=0;i<s1.stoichiometry;i++){
               StringTemplate correctedString = GenericMethods.replace(s1.st,"RP" + counter,"RP" + $reaction_def::reactantPatternCounter);
               patternsReactants.add(correctedString);
               ReactionRegister.mergeMaps($s1.map,$reactionStack);
              $reaction_def::reactantPatternCounter++;
            } 
           } 
  (PLUS 
  s2=rule_species_def[upperID+"_RP"+ $reaction_def::reactantPatternCounter,$reaction_rule_def::reactionAction]
            {
            int counter = $reaction_def::reactantPatternCounter;
            for(int i=0;i<s2.stoichiometry;i++){ 
              StringTemplate correctedString = GenericMethods.replace(s2.st,"RP" + counter,"RP" + $reaction_def::reactantPatternCounter);
               patternsReactants.add(correctedString);
               ReactionRegister.mergeMaps($s2.map,$reactionStack);
              $reaction_def::reactantPatternCounter++;
            }
            })* 
  (UNI_REACTION_SIGN {$bidirectional = false;} | BI_REACTION_SIGN {$bidirectional = true;})
  s3=rule_species_def[upperID+"_PP"+ 1,$reaction_rule_def::reactionAction] 
        {
        $reaction_def::reactantPatternCounter =1;
          int counter = $reaction_def::reactantPatternCounter;
          for(int i=0;i<s3.stoichiometry;i++){ 
            StringTemplate correctedString = GenericMethods.replace(s3.st,"PP" + counter,"PP" + $reaction_def::reactantPatternCounter);
            patternsProducts.add(correctedString);
            ReactionRegister.mergeMaps($s3.map,$productStack);
            $reaction_def::reactantPatternCounter++;
          }
        }
  (PLUS
  s4=rule_species_def[upperID+"_PP"+ $reaction_def::reactantPatternCounter,$reaction_rule_def::reactionAction] 
        {
            int counter = $reaction_def::reactantPatternCounter;
            for(int i=0;i<s4.stoichiometry;i++){ 
               StringTemplate correctedString = GenericMethods.replace(s4.st,"PP" + counter,"PP" + $reaction_def::reactantPatternCounter);
               patternsProducts.add(correctedString);
               $reaction_def::reactantPatternCounter++;
               ReactionRegister.mergeMaps($s4.map,$productStack);
            }
        })*
 ;
        
rule_species_def[String upperID,ReactionAction reactionAction] returns [int stoichiometry,Map <String,List<ReactionRegister>> map,boolean fixed] throws SemanticException
scope{
List reactants;
BondList bonds;
}
@init{
  $rule_species_def::reactants = new ArrayList();
  $rule_species_def::bonds = new BondList();
  $stoichiometry = 1;
  $fixed = false;
}
:
(
// (i1=INT {$stoichiometry = Integer.parseInt($i1.text);} TIMES)? // Stoichiometry (not yet supported in BNG)
 (match_attribute)?
 s1=species_def[$rule_species_def::reactants,$rule_species_def::bonds,upperID] {
       String trimmedName = $species_def.text;
       trimmedName = trimmedName.replaceAll("\\$","");
       reactionAction.addMolecule(upperID,trimmedName,$rule_species_def::bonds);
       $map = $species_def.listOfMolecules;
       $reaction_rule_def::lmemory.putAll($species_def.lmemory);
       if($s1.constant)
          $fixed = true;
  }
  {
       if(!$rule_species_def::bonds.validateBonds(0,0)){
          String err = String.format("\%s line \%d:\%d \%s\n",input.getSourceName(),s1.start.getLine(),s1.start.getCharPositionInLine(),"dangling bond");
  
          System.err.println(err);
        }
  }

  | i2=INT { // '0'
        $map  = new HashMap<String,List<ReactionRegister>>();
        if(!$i2.text.equals("0")){
          throw new RecognitionException();
        }
     }
)


    ->rule_seed_species_block(id={upperID},molecules={$rule_species_def::reactants},firstBonds={$rule_species_def::bonds.getLeft()},
      secondBonds={$rule_species_def::bonds.getRight()})
    ;

rate_primitive [Map<String,Register> memory] returns [String functionName]
@init{
  $functionName = "Ele";
  int numArguments = 0;
}:
    (function_keyword LPAREN) => f1=function_keyword {$functionName = $function_keyword.text;} 
    LPAREN expression[memory] {numArguments++;} (COMMA expression[memory] {numArguments++;})* RPAREN
    {
      if(numArguments != $function_keyword.numArguments){
	      String msg = getErrorMessage2(f1.tk,"Incorrect number of arguments (" + numArguments + "). Requires "
	                                          + $function_keyword.numArguments + ".");
	      System.err.println(msg);
      }
    } 
    |
    expression[memory]
    
;

function_keyword returns [int numArguments,Token tk]:
  SAT  {$numArguments = 2;$tk = $SAT;}  |
  MM   {$numArguments = 2;$tk = $MM;}   |
  HILL {$numArguments = 3;$tk = $HILL;} |
  ARRHENIUS {$numArguments = 2;$tk = $ARRHENIUS;}
;

rate_list[List<String> rateList,boolean bidirectional]
scope{
  Map<String,Register> memoryWithLocal;
  int numberRateLaws
}
@init{
  $rate_list::memoryWithLocal = new HashMap<String,Register>();
  $rate_list::memoryWithLocal.putAll(gParent.memory);
  $rate_list::memoryWithLocal.putAll($reaction_rule_def::lmemory);
  $rate_list::numberRateLaws = 1;
}
        : e1=rate_primitive[$rate_list::memoryWithLocal]
        {
          rateList.add($e1.text);
        }
        (COMMA e2=rate_primitive[$rate_list::memoryWithLocal] 
        {
          rateList.add($e2.text);
          $rate_list::numberRateLaws = 2;
        }
        )?
        {
          if ( $e1.functionName.equals("Arrhenius") && (!bidirectional || $rate_list::numberRateLaws == 2)
          || (!$e1.functionName.equals("Arrhenius") && ( bidirectional && $rate_list::numberRateLaws == 1)) 
          || (!$e1.functionName.equals("Arrhenius") && (!bidirectional && $rate_list::numberRateLaws == 2)))
          {
            String msg = getErrorMessage2((Token)e1.getStart(),"Incorrect number of rate laws (" 
                                                              + $rate_list::numberRateLaws + ")");
            System.err.println(msg);
          }
        }
        ;
        
modif_command
        : include_command
        | exclude_command
        ;
        
//are the patterns same in include and exclude?
include_command
        :  (INCLUDE_REACTANTS
          | INCLUDE_PRODUCTS)
          LPAREN INT COMMA pattern (COMMA pattern)* RPAREN
        ;
        
exclude_command
        : (EXCLUDE_REACTANTS
        | EXCLUDE_PRODUCTS)
        LPAREN INT COMMA pattern (COMMA pattern)* RPAREN
        ;



//pattern :   STRING|INT    ;
pattern :   STRING    ;






 